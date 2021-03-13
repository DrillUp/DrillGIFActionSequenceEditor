#include "stdafx.h"

#include "s_ProjectManager.h"
#include "s_ProjectWindowManager.h"

#include "data/c_ProjectData.h"
#include "data/w_ProjectCreate.h"
#include "storage/s_StorageManager.h"
#include "storageGlobal/s_IniManager.h"
#include "file/s_TempFileManager.h"

#include "DrillGIFActionSequenceEditor.h"
/*
-----==========================================================-----
		类：		项目管理.cpp
		版本：		v1.14
		所属模块：	项目管理模块
		功能：		项目管理相关数据、相关操作都在这里主要控制。
					> 基本功能
						> 项目文件夹：
							保存项目时，默认建立一个项目文件夹 项目名Files，存储其他相关文件。
							工程文件 项目名.xxx 存储根目录，与项目文件夹同级。
							项目名支持"xxx.xx.xx.xx"的命名，不会出错。
						> 项目改动监听：
							如果项目中出现了任何值的改动提交，就会被记录下来，标题出现"*"号。
							关闭文件时根据是否改动，提示"当前有未保存的修改，是否保存？"。
						> 非模态锁定：
							创建非模态框时，执行项目的锁定方法，可以使得窗口在编辑期间，不能执行新建/打开/保存/另存为，也不能关闭窗口。
						> 最近打开的文件：（history）
							项目管理将记录最近打开的项目。
					> temp文件夹
						> 转移文件：
							直接在exe文件目录下建立temp文件夹，对文件作转移、临时处理。
							保存项目后，文件夹内的所有相关文件都会保存到项目文件夹中。
						> 唯一文件：
							将某个文件导入后，命名为uuid唯一文件名，确保可以导入重复文件，并且修改项目中的文件名。
						> log文件：
							提供快速生成debug用的文本文件，放在temp文件夹中。
					> 存储文件管理
						> json存储： 
							能够将基于模板的所有数据都存储在"PROJECT_SUFFIX"的后缀文件中。
							要求含有存储功能的类必须继承实现存储模板的4个方法。
							数据结构类需要实现"getJsonObject"方法和"setJsonObject"方法，确保所有数据类都能够转成json数据。
			
		注意事项：	1.该管理器需要修改窗口名字，会发送信号。
					2.PROJECT_SUFFIX 在c_ProjectData类中定义。
-----==========================================================-----
*/

S_ProjectManager::S_ProjectManager(){
	S_TempFileManager::getInstance()->setSkipSuffix(S_TempFileManager::getInstance()->getSkipSuffix() << PROJECT_SUFFIX << "cbf");

	// > 参数初始化
	this->m_isDirty = false;
	this->m_lockingWidgets.clear();

	// > 历史记录初始化
	this->m_historyProjectTank.clear();
	this->m_historyDirTank.clear();
	this->m_historyProjectMax = 10;
	this->m_historyDirMax = 6;
	this->loadHistory();
}
S_ProjectManager::~S_ProjectManager() {
}

/* --------------------------------------------------------------
----------ProjectManager 单例
*/
S_ProjectManager* S_ProjectManager::project_manager = NULL;
S_ProjectManager* S_ProjectManager::getInstance() {
	if (project_manager == NULL) {
		project_manager = new S_ProjectManager();
	}
	return project_manager;
}


/* --------------------------------------------------------------
		全局 - 设置数据已修改
*/
void S_ProjectManager::setDirty() {
	if (this->m_isDirty == false) {
		this->m_isDirty = true;
		this->changeTitle();
	}
}
/* --------------------------------------------------------------
		全局 - 锁定项目
*/
void S_ProjectManager::addLock(QWidget* widget) {
	if (this->m_lockingWidgets.size() == 0) {
		emit lockChanged(true);
	}
	this->m_lockingWidgets.push_back(widget);
}
/* --------------------------------------------------------------
		全局 - 解锁项目
*/
void S_ProjectManager::removeLock(QWidget* widget) {
	this->m_lockingWidgets.removeOne(widget);
	if (this->m_lockingWidgets.size() == 0) {
		emit lockChanged(false);
	}
}
/* --------------------------------------------------------------
		#全局 - 修改名称
*/
void S_ProjectManager::changeTitle(){
	QString title = "";
	if (this->data_ProjectData.isNull()) {
		title = this->data_ProjectData.getSoftname() + " - newProject";
	}else {
		title = this->data_ProjectData.getSoftname() + " - " + this->data_ProjectData.getName();
	}
	if (this->m_isDirty) {
		title += "*";
	}
	emit changeWindowTitle(title);
}
/* --------------------------------------------------------------
		全局 - 判断锁定
*/
bool S_ProjectManager::isLocked() {
	return this->m_lockingWidgets.size() > 0;
}
/* --------------------------------------------------------------
		全局 - 判断修改
*/
bool S_ProjectManager::isDirty() {
	return this->m_isDirty;
}
/* --------------------------------------------------------------
		全局 - 保存修改部分提示
*/
bool S_ProjectManager::dirtyTip() {

	if (this->m_isDirty) {
		switch (QMessageBox::question(PROJECT_INSTANCE, "提示", "当前有未保存的修改，是否保存？", "保存", "不保存", "取消", 0)) {
		case 0:
			return this->saveProject();
		case 1:
			return true;
		case 2:
		default:
			return false;
		}
	}
	else {
		return true;
	}

}

/* --------------------------------------------------------------
		项目 - 新建（对话框）
*/
bool S_ProjectManager::newProject() {

	// > 修改提示（取消则返回false）
	if (this->dirtyTip() == false) { return false; }

	this->clearProject();					//清理项目
	this->changeTitle();					//修改标题
	emit newProjectFinished();				//项目新建清空后（信号）

	return true;
}


/* --------------------------------------------------------------
		项目 - 打开（对话框）
*/
bool S_ProjectManager::openProject() {

	// > 修改提示（取消则返回false）
	if (this->dirtyTip() == false) { return false; }

	// > 用户选择项目文件
	QString file_path;
	QFileDialog fd;
	fd.setWindowTitle("打开项目");
	fd.setAcceptMode(QFileDialog::AcceptOpen);
	fd.setDirectory(this->data_ProjectData.getParentPath());	
	fd.setNameFilters(QStringList()	<< QString("项目文件(*.") + PROJECT_SUFFIX + ")");
	fd.setViewMode(QFileDialog::Detail);
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()) {
			//（没有选择文件进入的情况）
			return false;
		}
		file_path = fd.selectedFiles().at(0);
	}else {
		//（点击关闭或者取消操作的情况）
		return false;
	}

	// > 必须指定文件打开
	this->openProjectDirectly(file_path);
	return true;
}
/* --------------------------------------------------------------
		项目 - 打开（输入值）
*/
bool S_ProjectManager::openProjectByArgs(int argc, char *argv[]) {
	if (argc == 2) {
		//qDebug() << argv[0];	//exe的路径
		//qDebug() << argv[1];	//输入文件的路径
		QString exe_path = argv[0];
		QString file_path = argv[1];
		if (QFileInfo(file_path).suffix() == PROJECT_SUFFIX) {
			this->openProjectDirectly(file_path);		//根据文件打开
			return true;
		}
	}
	return false;
}
/* --------------------------------------------------------------
		项目 - 打开（拖拽文件）
*/
bool S_ProjectManager::openProjectByDropFile(QDropEvent *event) {

	// > 修改提示（取消则返回false）
	if (this->dirtyTip() == false) { return false; }

	// > 用户选择项目文件夹
	QList<QUrl> urls = event->mimeData()->urls();		//只要有一个满足"drill"读取文件，则打开
	for (int i = 0; i < urls.count(); i++) {
		QString file_path = urls.at(i).path();
		if (file_path.at(0) == '/') {					//多了不明的特殊字符，去掉
			file_path = file_path.mid(1);
		}
		if (QFileInfo(file_path).suffix() == PROJECT_SUFFIX) {
			this->openProjectDirectly(file_path);		//根据文件打开
			return true;
		}
	}

	return false;
}
/* --------------------------------------------------------------
		项目 - 保存（对话框）
*/
bool S_ProjectManager::saveProject() {

	// > 项目未打开情况
	if (this->data_ProjectData.isNull()) {
		W_ProjectCreate d(PROJECT_INSTANCE);
		d.setDataInModifyMode(this->data_ProjectData);
		bool accepted = d.exec();
		if (accepted == QDialog::Accepted) {
			this->data_ProjectData = d.getData();	//刷新项目数据
			this->changeTitle();					//修改标题

			this->saveAll(this->data_ProjectData.getProjectRootPath());
		}
		return accepted;
	}
	// > 项目已打开情况
	else{
		this->saveAll(this->data_ProjectData.getProjectRootPath());
		return true;
	}

}

/* --------------------------------------------------------------
		项目 - 另存为（对话框）
*/
bool S_ProjectManager::saveAs() {

	W_ProjectCreate d(PROJECT_INSTANCE);
	d.setDataInModifyMode(this->data_ProjectData);
	d.setWindowTitle("项目另存为");
	bool accepted = d.exec();
	if (accepted == QDialog::Accepted) {
		this->data_ProjectData = d.getData();	//刷新项目数据
		this->changeTitle();					//修改标题

		this->saveAll(this->data_ProjectData.getProjectRootPath());
	}
	return accepted;

}
/* --------------------------------------------------------------
		项目 - 强制保存
*/
bool S_ProjectManager::saveInForce() {

	// > 项目未打开情况
	if (this->data_ProjectData.isNull()) { return false; }

	// > 项目已打开情况
	this->saveAll(this->data_ProjectData.getProjectRootPath());
	return true;
}

/* --------------------------------------------------------------
		#项目 - 清除项目数据
*/
void S_ProjectManager::clearProject() {

	// > 项目所有参数初始化
	S_StorageManager::getInstance()->clearAllApplicationData();		//清理存储数据
	this->data_ProjectData = C_ProjectData();						//清理当前项目数据
	this->m_isDirty = false;										//清理项目修改痕迹

	// > 清除缓存数据
	S_TempFileManager::getInstance()->removeAllTempFile();

	// > 清理输出窗口
	//S_ConsoleManager::getInstance()->clearContents();
	//S_ConsoleManager::getInstance()->append("----开始新工程----");
}

/* --------------------------------------------------------------
		#项目 - 打开（根据文件名）
*/
void S_ProjectManager::openProjectDirectly(QString open_path) {

	// > 清理项目
	this->clearProject();

	// > 刷新项目路径
	QFileInfo info(open_path);
	this->data_ProjectData.setPath(info.absolutePath());
	this->data_ProjectData.setName(info.completeBaseName());
	this->addHistoryProject(info.absoluteFilePath());
	this->addHistoryDir(info.absolutePath());
	this->saveHistory();

	// > 复制文件到temp中
	bool success = S_TempFileManager::getInstance()->copyResourceToTemp_Dir(this->data_ProjectData.getProjectFilePath());
	if (success == false){
		QMessageBox message(QMessageBox::Information, "提示", "当前项目中包含非法文件路径，打开失败。\n请将工程文件移动至一个空的文件夹中，再打开工程。");
		message.exec();
		return;
	}

	// > 读取存储文件
	this->readSaveFile();

	// > 修改窗口项目名
	emit changeWindowTitle(this->data_ProjectData.getSoftname() + " - " + this->data_ProjectData.getName());

	// > 刷新设置树
	emit S_ProjectWindowManager::getInstance()->reflashAllTree();

	// > 项目打开后（信号）
	emit openProjectFinished();				
}
/* --------------------------------------------------------------
		#项目 - 保存（目标文件夹）
*/
void S_ProjectManager::saveAll(QString url) {

	// > 创建文件夹
	if (url.at(url.size() - 1) != '/') { url += "/"; }
	QDir temp_dir;
	temp_dir.mkpath(url);

	// > 生成存储文件
	this->createSaveFile();

	// > 清理痕迹
	this->m_isDirty = false;
	this->changeTitle();

	// > 项目保存后（信号）
	emit saveProjectFinished();
}
/* --------------------------------------------------------------
		#项目 - 保存（文件）
*/
void S_ProjectManager::createSaveFile() {
	// > 存储管理器
	S_StorageManager::getInstance()->createSaveFile(this->data_ProjectData.getProjectFile());
	this->addHistoryProject(this->data_ProjectData.getProjectFile());
	this->addHistoryDir(this->data_ProjectData.getProjectRootPath());
	this->saveHistory();

	// > 复制 temp 到 src
	S_TempFileManager::getInstance()->copyTempToTarget_DirWithAllSubfolders(this->data_ProjectData.getProjectFilePath());

}

/* --------------------------------------------------------------
		#项目 - 读取（文件）
*/
void S_ProjectManager::readSaveFile() {
	// > 确保复制文件时，不出现覆盖问题
	S_TempFileManager::getInstance()->removeInTemp_File(this->data_ProjectData.getName() + "." + this->data_ProjectData.getSuffix());

	// > 存储管理器
	S_StorageManager::getInstance()->readSaveFile(this->data_ProjectData.getProjectFile());

	// > 复制 src 到 temp
	S_TempFileManager::getInstance()->copyResourceToTemp_DirWithAllSubfolders(this->data_ProjectData.getProjectFilePath());

}


/* --------------------------------------------------------------
		#项目 - 获取文件（根据后缀，F:/aaa/bbb.suf完整路径）
*/
QString S_ProjectManager::getOneFileBySuffix(QString url, QString suffix) {
	suffix = suffix.replace(".", "");
	QDir dir(url);
	QString result;
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); i++) {
		QString file_sufx = list.at(i).suffix();
		if ( file_sufx == suffix ) {
			result = list.at(i).absoluteFilePath();		//完整路径
			break;
		}
	}
	return result;
}

/*-----------------------------------
		数据 - 获取存储的名称
*/
QString S_ProjectManager::getSaveName() {
	return "S_ProjectManager";
}
/*-----------------------------------
		数据 - 清除当前管理器数据
*/
void S_ProjectManager::clearAllData() {
	
	this->data_ProjectData = C_ProjectData();
	
}

/*-----------------------------------
		数据 - 全部求解需求数据 -> QJsonObject
*/
QJsonObject S_ProjectManager::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	QJsonObject obj_ProjectData = QJsonObject();			//项目管理数据
	obj_ProjectData.insert("type", "ProjectData");
	obj_ProjectData.insert("data", this->data_ProjectData.getJsonObject());

	obj_all.insert("ProjectData", obj_ProjectData);
	return obj_all;
}

/*-----------------------------------
		数据 - QJsonObject -> 全部求解需求数据
*/
void S_ProjectManager::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	QJsonObject obj_ProjectData = obj_all.value("ProjectData").toObject();	//项目管理数据
	this->data_ProjectData.setJsonObject(obj_ProjectData.value("data").toObject());

	/*----------------项目路径需要重新刷当前的新路径----------------*/
	this->data_ProjectData.setPath( this->m_storage_fileInfo.absolutePath());
}


/* --------------------------------------------------------------
		历史记录 - 获取项目
*/
QList<QFileInfo> S_ProjectManager::getHistoryProjectTank(){
	return this->m_historyProjectTank;
}
/* --------------------------------------------------------------
		历史记录 - 添加项目
*/
void S_ProjectManager::addHistoryProject(QFileInfo info){
	
	// > 已存在时，前置
	if (this->m_historyProjectTank.contains(info)){
		this->m_historyProjectTank.removeOne(info);
		this->m_historyProjectTank.push_front(info);
		return; 
	}

	// > 不存在时，添加
	this->m_historyProjectTank.insert(0, info);
	
	if (this->m_historyProjectTank.count() > this->m_historyProjectMax){
		this->m_historyProjectTank.removeLast();
	}
}
/* --------------------------------------------------------------
		历史记录 - 去除项目
*/
void S_ProjectManager::removeHistoryProject(int index){
	if (index < 0){ return; }
	if (index >= this->m_historyProjectTank.count()){ return; }
	this->m_historyProjectTank.removeAt(index);
}
/* --------------------------------------------------------------
		历史记录 - 获取根目录
*/
QList<QDir> S_ProjectManager::getHistoryDirTank(){
	return this->m_historyDirTank;
}
/* --------------------------------------------------------------
		历史记录 - 添加根目录
*/
void S_ProjectManager::addHistoryDir(QDir dir){

	// > 已存在时，前置
	if (this->m_historyDirTank.contains(dir)){
		this->m_historyDirTank.removeOne(dir);
		this->m_historyDirTank.push_front(dir);
		return;
	}

	// > 不存在时，添加
	this->m_historyDirTank.insert(0, dir);

	if (this->m_historyDirTank.count() > this->m_historyDirMax){
		this->m_historyDirTank.removeLast();
	}
}
/* --------------------------------------------------------------
		历史记录 - 去除根目录
*/
void S_ProjectManager::removeHistoryDir(int index){
	if (index < 0){ return; }
	if (index >= this->m_historyDirTank.count()){ return; }
	this->m_historyDirTank.removeAt(index);
}
/* --------------------------------------------------------------
		历史记录 - 保存记录（私有）
*/
void S_ProjectManager::saveHistory(){

	QStringList project_list = QStringList();
	for (int i = 0; i < this->m_historyProjectTank.count(); i++){
		QFileInfo info = this->m_historyProjectTank.at(i);
		project_list.append(info.absoluteFilePath());
	}
	QStringList dir_list = QStringList();
	for (int i = 0; i < this->m_historyDirTank.count(); i++){
		QDir dir = this->m_historyDirTank.at(i);
		dir_list.append(dir.absolutePath());
	}

	S_IniManager::getInstance()->setConfig("COAS_historyProject", project_list.join("|"));
	S_IniManager::getInstance()->setConfig("COAS_historyDir", dir_list.join("|"));
}
/* --------------------------------------------------------------
		历史记录 - 读取记录（私有）
*/
void S_ProjectManager::loadHistory(){
	QStringList project_list = S_IniManager::getInstance()->getConfig("COAS_historyProject").split("|");	//（用竖线划分）
	QStringList dir_list = S_IniManager::getInstance()->getConfig("COAS_historyDir").split("|");	

	this->m_historyProjectTank.clear();
	for (int i = 0; i < project_list.count(); i++){
		QString path = project_list.at(i);
		QFileInfo info(path);
		if (info.exists() == false){ continue; }
		this->m_historyProjectTank.append(info);
	}

	this->m_historyDirTank.clear();
	for (int i = 0; i < dir_list.count(); i++){
		QString path = dir_list.at(i);
		QDir dir(path);
		if (dir.exists() == false){ continue; }
		this->m_historyDirTank.append(dir);
	}
}

/* --------------------------------------------------------------
		其他 - 打开项目路径文件夹
*/
bool S_ProjectManager::openDesktopProjectUrl() {
	QString buffer_path = this->data_ProjectData.getProjectRootPath();
	return QDesktopServices::openUrl(QUrl("file:/" + buffer_path + "/"));
}

/* --------------------------------------------------------------
		其他 - 打开项目路径中指定文件
*/
bool S_ProjectManager::openDesktopProjectFile(QString filename) {
	QString buffer_path = this->data_ProjectData.getProjectRootPath();
	return QDesktopServices::openUrl(QUrl("file:/" + buffer_path + "/" + filename));
}