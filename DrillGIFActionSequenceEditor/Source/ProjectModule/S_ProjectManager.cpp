#include "stdafx.h"
#include "S_ProjectManager.h"

#include "Data/C_ProjectData.h"
#include "Data/W_ProjectCreate.h"
#include "Storage/S_StorageManager.h"
#include "StorageGlobal/S_IniManager.h"
#include "File/S_TempFileManager.h"
#include PROJECT_INCLUDE

#ifdef PROJECT_USE_S_UniqueTempFileFactory
#include "File/UniqueTempFile/S_UniqueTempFileFactory.h"
#endif


/*
-----==========================================================-----
		类：		项目管理.cpp
		版本：		v1.22
		作者：		drill_up
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
			
		注意事项：	1. 该管理器需要修改窗口名字，会发送信号。
					2. 自定义部分 全部转移到 S_ProjectManager_Custom 中执行。
-----==========================================================-----
*/

S_ProjectManager::S_ProjectManager(){
	
	// > 参数初始化
	this->m_fixTitle.clear();
	this->m_isSampleProject = false;
	this->m_isDirty = false;
	this->m_lockingWidgets.clear();
	
	// > 历史记录初始化
	this->m_historyProjectTank.clear();
	this->m_historyDirTank.clear();
	this->m_historyProjectMax = 10;
	this->m_historyDirMax = 6;
	this->loadHistory();

	// > 唯一文件初始化
	#ifdef PROJECT_USE_S_UniqueTempFileFactory
		S_UniqueTempFileFactory::getInstance();
	#endif
}
S_ProjectManager::~S_ProjectManager() {
}

/* --------------------------------------------------------------
----------单例
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
		emit signal_dirtyChanged(true);
		this->refreshTitle();
	}
}
/* --------------------------------------------------------------
		全局 - 锁定项目
*/
void S_ProjectManager::addLock(QWidget* widget) {
	if (this->m_lockingWidgets.size() == 0) {
		emit signal_lockChanged(true);
	}
	this->m_lockingWidgets.push_back(widget);
}
/* --------------------------------------------------------------
		全局 - 解锁项目
*/
void S_ProjectManager::removeLock(QWidget* widget) {
	this->m_lockingWidgets.removeOne(widget);
	if (this->m_lockingWidgets.size() == 0) {
		emit signal_lockChanged(false);
	}
}
/* --------------------------------------------------------------
		全局 - 修改名称
*/
void S_ProjectManager::refreshTitle(){

	// > 自定义部分
	S_ProjectManager_Custom::getInstance()->changeTitle_Before();

	// > 名称设置
	QString new_title;
	if (this->data_ProjectData.isNull()) {
		new_title.append(this->data_ProjectData.getSoftname());
		new_title.append(" - ");
		new_title.append("newProject");
	}else {
		new_title.append(this->data_ProjectData.getSoftname());
		new_title.append(" - ");
		new_title.append(this->data_ProjectData.getName());
	}
	if (this->m_isDirty) {
		new_title.append("*");
	}

	// > 固定名称
	if (this->m_fixTitle != ""){
		new_title = this->m_fixTitle;
	}

	// > 自定义部分
	S_ProjectManager_Custom::getInstance()->changeTitle_After(new_title);
	emit signal_titleChanged(new_title);
}
/* --------------------------------------------------------------
		全局 - 使用固定名称
*/
void S_ProjectManager::setFixTitle(QString fix_title){
	this->m_fixTitle = fix_title;
}
void S_ProjectManager::clearFixTitle(){
	this->m_fixTitle = "";
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
		switch (QMessageBox::question(PROJECT_INSTANCE, tr("提示"), tr("当前有未保存的修改，是否保存？"), tr("保存"), tr("不保存"), tr("取消"), 0)) {
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
	this->refreshTitle();					//修改标题
	emit signal_newProjectFinished();		//项目新建清空后（信号）

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
	fd.setWindowTitle(tr("打开项目"));
	fd.setAcceptMode(QFileDialog::AcceptOpen);
	fd.setDirectory(this->data_ProjectData.getParentPath());
	fd.setNameFilters(QStringList() << this->data_ProjectData.getProjectFileFilterWithOld() << QString(tr("所有文件(*.*)")));
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
		QString exe_path = QString::fromLocal8Bit(argv[0]);		//（需要转换一下Unicode字符，确保识别中文路径，不能用Latin1）
		QString file_path = QString::fromLocal8Bit(argv[1]);
		if (QFileInfo(file_path).suffix() == PROJECT_SUFFIX) {
			this->openProjectDirectly(file_path);				//根据文件打开
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

		// > 新建项目（窗口）
		if (PROJECT_USE_W_ProjectCreate){
			W_ProjectCreate d(PROJECT_INSTANCE);
			d.setDataInModifyMode(this->data_ProjectData);
			bool accepted = d.exec();
			if (accepted == QDialog::Accepted) {
				this->data_ProjectData = d.getData();	//刷新项目数据
				this->refreshTitle();					//修改标题

				this->saveAll(this->data_ProjectData.getProjectFilePath(), "");
			}
			return accepted;

		// > 新建项目（原生选择框）
		}else{
			C_ProjectData data = C_ProjectData::openQDialogForProjectCreate();
			if (data.isNull()){ return false; }

			this->data_ProjectData = data;			//刷新项目数据
			this->refreshTitle();					//修改标题

			this->saveAll(this->data_ProjectData.getProjectFilePath(), "");
			return true;
		}

	// > 项目已打开情况
	}else{

		// > 样例工程标记
		if (this->m_isSampleProject == true){
			return this->saveAs();
		}

		this->saveAll(this->data_ProjectData.getProjectFilePath(), "");
		return true;
	}

}

/* --------------------------------------------------------------
		项目 - 另存为（对话框）
*/
bool S_ProjectManager::saveAs() {

	// > 新建项目（窗口）
	if (PROJECT_USE_W_ProjectCreate){
		W_ProjectCreate d(PROJECT_INSTANCE);
		d.setDataInModifyMode(this->data_ProjectData);
		d.setWindowTitle(tr("项目另存为"));
		bool accepted = d.exec();
		if (accepted == QDialog::Accepted) {
			QString saveAs_lastDir = "";
			if (this->data_ProjectData.isNull() == false) {		//（这里要避免新建工程时直接点击另存为的情况）
				saveAs_lastDir = this->data_ProjectData.getProjectFilePath();
			}

			this->data_ProjectData = d.getData();	//刷新项目数据
			this->refreshTitle();					//修改标题

			this->saveAll(this->data_ProjectData.getProjectFilePath(), saveAs_lastDir);
		}
		return accepted;

	// > 新建项目（原生选择框）
	}else{
		C_ProjectData data = C_ProjectData::openQDialogForProjectCreate();
		if (data.isNull()) { return false; }
		QString saveAs_lastDir = "";
		if (this->data_ProjectData.isNull() == false) {			//（这里要避免新建工程时直接点击另存为的情况）
			saveAs_lastDir = this->data_ProjectData.getProjectFilePath();
		}

		this->data_ProjectData = data;				//刷新项目数据
		this->refreshTitle();						//修改标题

		this->saveAll(this->data_ProjectData.getProjectFilePath(), saveAs_lastDir);
		return true;
	}
}
/* --------------------------------------------------------------
		项目 - 强制保存
*/
bool S_ProjectManager::saveInForce() {

	// > 项目未打开情况
	if (this->data_ProjectData.isNull()) { return false; }

	// > 项目已打开情况
	this->saveAll(this->data_ProjectData.getProjectFilePath(), "");
	return true;
}

/* --------------------------------------------------------------
		项目 - 清除项目数据
*/
void S_ProjectManager::clearProject() {

	// > 自定义部分
	S_ProjectManager_Custom::getInstance()->clearProject_Before();


	// > 项目所有参数初始化
	S_StorageManager::getInstance()->clearAllApplicationData();		//清理存储数据
	this->data_ProjectData.clear();									//清理当前项目数据
	this->m_isSampleProject = false;								//清理样例工程标记
	this->m_isDirty = false;										//清理项目修改痕迹
	emit signal_dirtyChanged(false);
	
	#ifdef PROJECT_USE_S_UniqueTempFileFactory
		
		// > 唯一文件工厂 - 清空缓存文件夹
		S_UniqueTempFileFactory::getInstance()->removeAllTempFile();
	#else
		// > 只缓存文件夹 - 清空缓存文件夹
		S_TempFileManager::getInstance()->removeAllTempFile();
	#endif


	// > 自定义部分
	S_ProjectManager_Custom::getInstance()->clearProject_After();
}

/* --------------------------------------------------------------
		项目 - 打开（根据文件名）
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

	// > 读取存储文件
	this->readSaveFile();

	// > 修改窗口项目名
	emit signal_titleChanged(this->data_ProjectData.getSoftname() + " - " + this->data_ProjectData.getName());

	// > 项目打开后（信号）
	emit signal_openProjectFinished();
}
/* --------------------------------------------------------------
		项目 - 保存（目标文件夹）
*/
void S_ProjectManager::saveAll(QString tar_fileDir, QString saveAs_last_fileDir) {

	// > 创建文件夹
	QDir temp_dir(tar_fileDir);
	temp_dir.mkpath(tar_fileDir);

	// > 保存记录
	this->data_ProjectData.lastSaveDate = QDateTime::currentDateTime();

	// > 生成存储文件
	//		（注意，生成存储文件时，样例工程的标记仍然存在）
	this->createSaveFile(tar_fileDir, saveAs_last_fileDir);

	// > 清理痕迹
	this->m_isSampleProject = false;
	this->m_isDirty = false;
	emit signal_dirtyChanged(false);
	this->refreshTitle();

	// > 项目保存后（信号）
	emit signal_saveProjectFinished();
}
/* --------------------------------------------------------------
		项目 - 保存（文件）
*/
void S_ProjectManager::createSaveFile(QString tar_fileDir, QString saveAs_last_fileDir) {

	// > 自定义部分
	S_ProjectManager_Custom::getInstance()->createSaveFile_Before(tar_fileDir, saveAs_last_fileDir);

	// > 存储
	S_StorageManager::getInstance()->createSaveFile(this->data_ProjectData.getProjectFile());
	this->addHistoryProject(this->data_ProjectData.getProjectFile());
	this->addHistoryDir(this->data_ProjectData.getProjectRootPath());	//（tar_dir的值 就是 this->data_ProjectData.getProjectRootPath() )
	this->saveHistory();

	#ifdef PROJECT_USE_S_UniqueTempFileFactory
		
		// > 唯一文件工厂 - 执行保存（相同的文件会被覆盖）
		if (saveAs_last_fileDir == "") {
			S_UniqueTempFileFactory::getInstance()->doSave(QDir(tar_fileDir));
		
		// > 唯一文件工厂 - 执行复制
		}else{
			S_UniqueTempFileFactory::getInstance()->doSaveAs(QDir(tar_fileDir), QDir(saveAs_last_fileDir));
		}
	#else
		// > 只缓存文件夹 - 确保复制文件时，不出现工程文件覆盖问题
		S_TempFileManager::getInstance()->removeInTemp_File(this->data_ProjectData.getName() + "." + this->data_ProjectData.getSuffix());
		
		// > 只缓存文件夹 - 复制 unique文件 到 src
		S_TempFileManager::getInstance()->copy_DirWithAllSubfolders(QDir(S_TempFileManager::getInstance()->getTempFileUrl() + "/unique/"), QDir(this->data_ProjectData.getProjectFilePath() + "unique/"));
	#endif

	// > 自定义部分
	S_ProjectManager_Custom::getInstance()->createSaveFile_After(tar_fileDir, saveAs_last_fileDir);
}

/* --------------------------------------------------------------
		项目 - 读取（文件）
*/
void S_ProjectManager::readSaveFile() {

	// > 自定义部分
	S_ProjectManager_Custom::getInstance()->readSaveFile_Before();
	
	#ifdef PROJECT_USE_S_UniqueTempFileFactory
		
		// > 唯一文件工厂 - 提前读取
		S_UniqueTempFileFactory::getInstance()->preReadFile(QFileInfo(this->data_ProjectData.getProjectFile()));

		// > 唯一文件工厂 - 执行打开
		S_UniqueTempFileFactory::getInstance()->doOpen(QDir(this->data_ProjectData.getProjectFilePath()));
	#else
		// > 只缓存文件夹 - 复制 src 到 temp【先复制文件，再读取数据，因为读取数据会影响到所有DataContainer，包括一些读取文件数据的操作】
		bool success = S_TempFileManager::getInstance()->copyResourceToTemp_DirWithAllSubfolders(this->data_ProjectData.getProjectFilePath());
		if (success == false){
			QMessageBox::warning(nullptr, tr("提示"), tr("当前项目中包含非法文件路径，打开失败。\n请将工程文件移动至一个空的文件夹中，再打开工程。"));
			return;
		}
	#endif

	// > 读取
	S_StorageManager::getInstance()->readSaveFile(this->data_ProjectData.getProjectFile());


	// > 自定义部分
	S_ProjectManager_Custom::getInstance()->readSaveFile_After();
}


/* --------------------------------------------------------------
		项目 - 获取文件（根据后缀，F:/aaa/bbb.suf完整路径）
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
		样例工程 - 设置
*/
void S_ProjectManager::setSampleProject(bool enabled){
	this->m_isSampleProject = enabled;
}
bool S_ProjectManager::isSampleProject(){
	return this->m_isSampleProject;
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
	this->data_ProjectData.clear();
}
/*-----------------------------------
		数据 - 全部求解需求数据 -> QJsonObject
*/
QJsonObject S_ProjectManager::getAllDataOfJsonObject(){
	QJsonObject obj_all;

	QJsonObject obj_ProjectData;			//项目管理数据
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

	QStringList project_list;
	for (int i = 0; i < this->m_historyProjectTank.count(); i++){
		QFileInfo info = this->m_historyProjectTank.at(i);
		project_list.append(info.absoluteFilePath());
	}
	QStringList dir_list;
	for (int i = 0; i < this->m_historyDirTank.count(); i++){
		QDir dir = this->m_historyDirTank.at(i);
		dir_list.append(dir.absolutePath());
	}

	S_IniManager::getInstance()->setConfig("historyProject", project_list.join("|"));
	S_IniManager::getInstance()->setConfig("historyDir", dir_list.join("|"));
}
/* --------------------------------------------------------------
		历史记录 - 读取记录（私有）
*/
void S_ProjectManager::loadHistory(){
	QStringList project_list = S_IniManager::getInstance()->getConfig("historyProject").split("|");	//（用竖线划分）
	QStringList dir_list = S_IniManager::getInstance()->getConfig("historyDir").split("|");	

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