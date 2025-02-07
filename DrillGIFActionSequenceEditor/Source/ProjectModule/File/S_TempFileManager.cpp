#include "stdafx.h"
#include "S_TempFileManager.h"


/*
-----==========================================================-----
		类：		缓存文件夹管理器.cpp
		版本：		v1.30
		作者：		drill_up
		所属模块：	项目管理模块
		功能：		该管理器本质上是一个文件操作器。
					默认链接exe路径下的temp文件夹，也可以自定义父路径并建立temp文件夹。
					提供文件生成、复制、移动、删除、查找等基本功能。
					【该管理器只管temp文件夹，不管下面的子文件，只提供子文件的操作函数】
					
		说明：		> 单例无法在销毁时清空文件夹，需要你手动 destroyInstance() 。
					> 该temp文件夹与QDir::tempPath()文件夹完全不同，用于项目临时或长期存储的目录。
					> 不管文件还是文件夹，
						absolutePath     - 上一级目录
						absoluteDir      - 上一级目录
						absoluteFilePath - 文件或文件夹的全名
					> entryInfoList包含全部父级文件夹"./"和"../"，需要跳出。

		调用方法：	> 获取单例
						S_TempFileManager::getInstance()->clearAllTempFile();
						S_TempFileManager::getInstance()->copyResourceToTempFile("D:/aaa");
					> 获取文件（无法传递QFile，因此需要手动配字符串）
						QFile file( S_TempFileManager::getInstance()->getTempFileUrl() + "/" + filename);
					> 手动销毁单例
						S_TempFileManager::getInstance()->destroyInstance();
-----==========================================================-----
*/

S_TempFileManager::S_TempFileManager() :P_FileOperater(){
	this->init();
}
S_TempFileManager::~S_TempFileManager(){
}
/* -------------单例------------ */
S_TempFileManager* S_TempFileManager::cur_instance = NULL;
S_TempFileManager* S_TempFileManager::getInstance() {
	if (cur_instance == NULL) {
		cur_instance = new S_TempFileManager();
	}
	return cur_instance;
}
/* -------------销毁------------ */
void S_TempFileManager::destroyInstance() {
	if (S_TempFileManager::cur_instance == NULL) { return; }
	S_TempFileManager::cur_instance = NULL;

	// > 清空文件
	this->removeAllTempFile();

	// > 去除文件夹
	QDir dir(this->getTempFileUrl());
	dir.removeRecursively();

	// > 销毁单例
	delete cur_instance;
}
/* -------------初始化------------ */
void S_TempFileManager::init() {

	// > 父路径（当前）
	this->m_dir_isUseCustom = false;

	// > 父路径（默认）
	this->m_dir_defaultTemp = QCoreApplication::applicationDirPath() + "/temp";
	this->m_dir_defaultWorkspace = this->getEmptyWorkspacePath(this->m_dir_defaultTemp);

	// > 父路径（默认） - 创建路径
	QDir dir(this->m_dir_defaultTemp);
	dir.mkdir(this->m_dir_defaultTemp);
	QDir dir2(this->m_dir_defaultWorkspace);
	dir2.mkdir(this->m_dir_defaultWorkspace);

	// > 父路径（自定义）
	this->m_dir_customTemp.clear();
	this->m_dir_customWorkspace.clear();

	// > 后缀管理
	this->m_suffix_defaultSkip = QStringList();								//忽略后缀
	this->m_suffix_defaultForbidden = QStringList() << "exe" << "lnk";		//禁止后缀
	this->m_suffix_skip = this->m_suffix_defaultSkip;
	this->m_suffix_forbidden = this->m_suffix_defaultForbidden;
}


/*-------------------------------------------------
		父路径 - 当前主路径
*/
QString S_TempFileManager::getMainUrl() {
	if (this->m_dir_isUseCustom == true){ return this->getMainUrl_Custom(); }
	return this->getMainUrl_Default();
}
/*-------------------------------------------------
		父路径 - 当前工作区路径
*/
QString S_TempFileManager::getTempFileUrl() {
	if (this->m_dir_isUseCustom == true){ return this->getTempFileUrl_Custom(); }
	return this->getTempFileUrl_Default();
}
/*-------------------------------------------------
		父路径 - 获取工作区ID
*/
int S_TempFileManager::findEmptyWorkspaceId(QString parent_path) {
	int workspace_id = 1;		//命名序号修改
	while (true){
		QString cur_path = parent_path;
		cur_path.append("/");
		cur_path.append("workspace_");
		cur_path.append(QString::number(workspace_id));

		// > 文件夹已存在，继续循环
		if (QFileInfo(cur_path).exists()){
			workspace_id++;
			continue;

		// > 文件夹不存在，则跳出
		}else{
			break;
		}
	}
	return workspace_id;
}
/*-------------------------------------------------
		父路径 - 获取可用工作区路径
*/
QString S_TempFileManager::getEmptyWorkspacePath(QString parent_path){
	int workspace_id = this->findEmptyWorkspaceId(parent_path);
	QString cur_path = parent_path;
	cur_path.append("/");
	cur_path.append("workspace_");
	cur_path.append(QString::number(workspace_id));
	return cur_path;
}


/*-------------------------------------------------
		父路径 - 默认主路径
*/
QString S_TempFileManager::getMainUrl_Default(){
	return this->m_dir_defaultTemp;
}
/*-------------------------------------------------
		父路径 - 默认工作区路径
*/
QString S_TempFileManager::getTempFileUrl_Default(){
	return this->m_dir_defaultWorkspace;
}


/*-------------------------------------------------
		父路径 - 自定义主路径
*/
QString S_TempFileManager::getMainUrl_Custom(){
	return this->m_dir_customTemp;
}
/*-------------------------------------------------
		父路径 - 自定义工作区路径
*/
QString S_TempFileManager::getTempFileUrl_Custom(){
	return this->m_dir_customWorkspace;
}
/*-------------------------------------------------
		父路径 - 转移缓存工作区路径
*/
bool S_TempFileManager::setCustomMainUrl(QString parent_path){

	QString org_dir_temp = this->getMainUrl();
	QString org_dir_workspace = this->getTempFileUrl();
	QString new_dir_temp = QFileInfo(parent_path).absoluteFilePath() + "/temp";
	QString new_dir_workspace = this->getEmptyWorkspacePath(new_dir_temp);

	this->m_dir_isUseCustom = true;
	this->m_dir_customTemp = new_dir_temp;
	this->m_dir_customWorkspace = new_dir_workspace;

	// > 创建路径
	QDir dir(new_dir_temp);
	dir.mkdir(new_dir_temp);

	// > 移动 - 完全替换
	bool success = true;
	if (QFileInfo(new_dir_workspace).exists() == false){
		success = this->move_DirInReplace(QDir(org_dir_workspace), QDir(new_dir_workspace));

	// > 移动 - 合并
	}else{
		success = this->move_DirWithAllSubfolders(QDir(org_dir_workspace), QDir(new_dir_workspace));
	}
	return success;
}



/* ----------------------------------------------------------------------------------------------------------------------------
------生成 - 生成一个文件，写入数据（文件已存在，则不操作）
*/
void S_TempFileManager::generateTempFile(QString filename, QString filedata, QString code) {
	if (this->hasTempFile(filename)) { return; }
	this->generateTempFileStrict(filename, filedata, code);
}
/* ----------------------------------------------------------------------------------------------------------------------------
------生成 - 生成一个文件，写入数据（文件已存在，则覆盖）
*/
void S_TempFileManager::generateTempFileStrict(QString filename, QString filedata, QString code) {
	QString file_path = this->getTempFileUrl() + "/" + filename;
	QString file_dir = QFileInfo(file_path).absolutePath();

	// > 路径生成
	QDir dir_to(file_dir);
	if (dir_to.exists() == false){
		dir_to.mkdir(file_dir);
	}

	// > 覆盖文件
	QFile file_to(file_path);
	if (file_to.exists()) { file_to.remove(); }

	// > 写入文件
	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return; }
	QByteArray ba;
	if (code == "toLocal8Bit"){			//windows本地字符
		ba = filedata.toLocal8Bit();
	}
	else if (code == "toLatin1"){		//ISO-8859-1编码，只有ASCII，不支持汉字
		ba = filedata.toLatin1();
	}
	else{
		ba = filedata.toUtf8();
	}
	file_to.write(ba);
	file_to.close();

	/*else if (code == "toUcs4"){		//4字节的宽字符编码
		ba = filedata.toUcs4();
	}*/
}



/* ----------------------------------------------------------------------------------------------------------------------------
------复制 - 多个文件 -> temp文件夹（前者目标文件夹 F:/aaa/bbb，后者目标文件列表：["aa.txt","bb.vcut"…]）
*/
void S_TempFileManager::copyResourceToTemp_SeveralFile(QString src_url, QStringList file_names) {
	QFileInfo info(src_url);
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return; }	//（temp文件夹内部不能复制）
	if (src_url.at(src_url.size()-1) == '/'){
		src_url = src_url.mid(0, src_url.size() - 1);
	}
	for (int i = 0; i < file_names.size(); i++){
		QString file_from_name = src_url + "/" + file_names.at(i);
		if (QFile(file_from_name).exists()){
			QString file_to_name = this->getTempFileUrl() + "/" + file_names.at(i);
			this->copyFilePrivate(file_from_name, file_to_name);
		}
	}
}
/* ----------------------------------------------------------------------------------------------------------------------------
------复制 - 多个temp文件 -> 指定文件夹 （前者目标文件夹 F:/aaa/bbb，后者temp文件列表：["aa.txt","bb.vcut"…]）
*/
void S_TempFileManager::copyTempToTarget_SeveralFile(QString tar_url, QStringList file_names) {
	if (tar_url.at(tar_url.size() - 1) == '/'){
		tar_url = tar_url.mid(0, tar_url.size() - 1);
	}
	for (int i = 0; i < file_names.size(); i++){
		QString file_from_name = this->getTempFileUrl() + "/" + file_names.at(i);
		if (QFile(file_from_name).exists()){
			QString file_to_name = tar_url + "/" + file_names.at(i);
			this->copyFilePrivate(file_from_name, file_to_name);
		}
	}
}
/* ----------------------------------------------------------------------------------------------------------------------------
------复制 - 指定文件/文件夹 -> temp文件夹 （文件或文件夹都可以，F:/aaa/bbb 完整路径）
*/
bool S_TempFileManager::copyResourceToTemp_File(QString src_url) {
	QFileInfo info(src_url);
	if (!info.isFile()){ return false; }
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return false; }	//（temp文件夹内部不能复制）
	return this->copyFilePrivate(info.absoluteFilePath(), this->getTempFileUrl() + "/" + info.fileName());
}
bool S_TempFileManager::copyResourceToTemp_Dir(QString src_url) {
	// > 递归复制（深度0）
	QFileInfo info(src_url);
	if (!info.isDir()){ return false; }
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return false; }	//（temp文件夹内部不能复制）
	QString dir_from = src_url;
	QString dir_to = this->getTempFileUrl();
	return this->copyDirPrivate_recursion(dir_to, 0, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyResourceToTemp_DirWithDepth(QString src_url, int depth) {
	// > 递归复制（深度自定义）
	QFileInfo info(src_url);
	if (!info.isDir()){ return false; }
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return false; }	//（temp文件夹内部不能复制）
	QString dir_from = src_url;
	QString dir_to = this->getTempFileUrl();
	return this->copyDirPrivate_recursion(dir_to, depth, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyResourceToTemp_DirWithAllSubfolders(QString src_url) {
	// > 递归复制（深度-1）
	QFileInfo info(src_url);
	if (!info.isDir()){ return false; }
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return false; }	//（temp文件夹内部不能复制）
	QString dir_from = src_url;
	QString dir_to = this->getTempFileUrl();
	return this->copyDirPrivate_recursion(dir_to, -1, dir_from, dir_from, 0);
}

/* ----------------------------------------------------------------------------------------------------------------------------
------复制 - temp文件夹 -> 指定文件夹 （F:/aaa/bbb 完整路径）
*/
bool S_TempFileManager::copyTempToTarget_Dir(QString tar_url) {
	// > 递归复制（深度0）
	QString dir_from = this->getTempFileUrl();
	QString dir_to = tar_url;
	return this->copyDirPrivate_recursion(dir_to, 0, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTarget_DirWithDepth(QString tar_url, int depth) {
	// > 递归复制（深度自定义）
	QString dir_from = this->getTempFileUrl();
	QString dir_to = tar_url;
	return this->copyDirPrivate_recursion(dir_to, depth, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTarget_DirWithAllSubfolders(QString tar_url) {
	// > 递归复制（深度-1）
	QString dir_from = this->getTempFileUrl();
	QString dir_to = tar_url;
	return this->copyDirPrivate_recursion(dir_to, -1, dir_from, dir_from, 0);
}



/* ----------------------------------------------------------------------------------------------------------------------------
------复制 - temp文件夹 的文件，并命名新的名字
*/
void S_TempFileManager::copyTempToTemp_FileWithNewName(QString filename, QString file_new_name){
	if (filename == ""){ return; }
	if (file_new_name == ""){ return; }
	filename = filename.replace(this->getTempFileUrl() + "/", "");
	filename = filename.replace(this->getTempFileUrl(), "");
	file_new_name = file_new_name.replace(this->getTempFileUrl() + "/", "");
	file_new_name = file_new_name.replace(this->getTempFileUrl(), "");
	QString urlA = this->getTempFileUrl() + "/" + filename;
	QString urlB = this->getTempFileUrl() + "/" + file_new_name;
	this->copyFilePrivate(urlA, urlB);
}
/* ----------------------------------------------------------------------------------------------------------------------------
------复制 - temp文件夹 的文件夹，并命名新的名字
*/
bool S_TempFileManager::copyTempToTemp_Dir(QString dir_name, QString dir_new_name){
	QString dir_from = this->getTempFileUrl() + "/" + dir_name;
	QString dir_to = this->getTempFileUrl() + "/" + dir_new_name;
	return this->copyDirPrivate_recursion(dir_to, 0, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTemp_DirWithDepth(QString dir_name, QString dir_new_name, int depth){
	QString dir_from = this->getTempFileUrl() + "/" + dir_name;
	QString dir_to = this->getTempFileUrl() + "/" + dir_new_name;
	return this->copyDirPrivate_recursion(dir_to, depth, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTemp_DirWithAllSubfolders(QString dir_name, QString dir_new_name){
	QString dir_from = this->getTempFileUrl() + "/" + dir_name;
	QString dir_to = this->getTempFileUrl() + "/" + dir_new_name;
	return this->copyDirPrivate_recursion(dir_to, -1, dir_from, dir_from, 0);
}



/* ----------------------------------------------------------------------------------------------------------------------------
------删除 - 删除文件（根据后缀）
*/
void S_TempFileManager::removeInTemp_Dir(QString dirname) {
	if (dirname == ""){ Q_ASSERT(false); return; }	//（空数据跳出，防止乱删）
	QFileInfo info = QFileInfo(this->getTempFileUrl() + "/" + dirname);
	if (info.isDir()) {
		QDir dir(info.absoluteFilePath());
		dir.removeRecursively();
	}
}
void S_TempFileManager::removeInTemp_File(QString filename) {
	if (filename == ""){ Q_ASSERT(false); return; }	//（空数据跳出，防止乱删）
	QFileInfo info = QFileInfo(this->getTempFileUrl() + "/" + filename);
	if (info.isFile()) {
		QFile::remove(info.absoluteFilePath());
	}
}
void S_TempFileManager::removeInTemp_FileBySuffix(QString suffix) {
	if (suffix == ""){ Q_ASSERT(false); return; }	//（空数据跳出，防止乱删）
	// > 递归删除（深度0）
	this->removeFilePrivate_recursion("*." + suffix, 0, this->getTempFileUrl(), 0);
}
void S_TempFileManager::removeInTemp_FileBySuffix_WithAllSubfolders(QString suffix) {
	if (suffix == ""){ Q_ASSERT(false); return; }	//（空数据跳出，防止乱删）
	// > 递归删除（深度-1）
	this->removeFilePrivate_recursion("*." + suffix, -1, this->getTempFileUrl(), 0);
}
void S_TempFileManager::removeInTemp_FileByNameNoSuffix(QString onlyname) {
	if (onlyname == ""){ Q_ASSERT(false); return; }	//（空数据跳出，防止乱删）
	// > 递归删除（深度0）
	this->removeFilePrivate_recursion(onlyname + ".*", 0, this->getTempFileUrl(), 0);
}
void S_TempFileManager::removeInTemp_FileByNameNoSuffix_WithAllSubfolders(QString onlyname) {
	if (onlyname == ""){ Q_ASSERT(false); return; }	//（空数据跳出，防止乱删）
	// > 递归删除（深度-1）
	this->removeFilePrivate_recursion(onlyname + ".*", -1, this->getTempFileUrl(), 0);
}
/* ----------------------------------------------------------------------------------------------------------------------------
------删除 - 清空所有文件（包括文件夹）
*/
void S_TempFileManager::removeAllTempFile() {
	QString dir_path = this->getTempFileUrl();
	if (dir_path == ""){ Q_ASSERT(false); return; }	//（空数据跳出，防止乱删）
	QDir dir(dir_path);
	if (!dir.exists()) { return; }

	QFileInfoList fileList;
	QFileInfo curfile;
	fileList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::Readable | QDir::Writable | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);

	int infoNum = fileList.size();
	for (int i = infoNum - 1; i >= 0; i--) {	//倒序删除（这样就不需要迭代器了）
		curfile = fileList[i];
		if (curfile.isFile()) {
			QFile file_temp(curfile.filePath());
			file_temp.remove();
			fileList.removeAt(i);
		}
		if (curfile.isDir()) {
			QDir dir2(curfile.filePath());		//删除文件夹
			dir2.removeRecursively();
			fileList.removeAt(i);
		}
	}
}



/* ----------------------------------------------------------------------------------------------------------------------------
------查找 - 是否存在指定文件
*/
bool S_TempFileManager::hasTempFile(QString filename) {
	return QFile(this->getTempFileUrl() + "/" + filename).exists();
}
/* ----------------------------------------------------------------------------------------------------------------------------
------查找 - 是否存在指定文件名
*/
bool S_TempFileManager::hasTempFileBaseName(QString basename) {
	return this->getTempFile_ByBaseName(basename).count() > 0;
}
bool S_TempFileManager::hasTempFileBaseName_WithAllSubfolders(QString basename){
	return this->getTempFile_ByBaseName_WithAllSubfolders(basename).count() > 0;
}
/* ----------------------------------------------------------------------------------------------------------------------------
------查找 - 是否存在指定后缀
*/
bool S_TempFileManager::hasTempFileSuffix(QString basename) {
	return this->getTempFile_BySuffix(basename).count() > 0;
}
bool S_TempFileManager::hasTempFileSuffix_WithAllSubfolders(QString basename){
	return this->getTempFile_BySuffix_WithAllSubfolders(basename).count() > 0;
}
/* ----------------------------------------------------------------------------------------------------------------------------
------查找 - 指定文件是否在temp文件夹中
*/
bool S_TempFileManager::isInCurTempFile(QString filename) {
	return QFileInfo(filename).absoluteFilePath().contains(QFileInfo(this->getTempFileUrl()).absoluteFilePath());
}
/* ----------------------------------------------------------------------------------------------------------------------------
------查找 - 获取指定文件名的文件
*/
QList<QFileInfo> S_TempFileManager::getTempFile_ByBaseName(QString basename) {
	return this->search_FileInDir(QDir(this->getTempFileUrl()), QRegExp(basename + "\\.[^\\.]*"));
}
QList<QFileInfo> S_TempFileManager::getTempFile_ByBaseName_WithAllSubfolders(QString basename) {
	return this->search_FileInWithAllSubfolders(QDir(this->getTempFileUrl()), QRegExp(basename + "\\.[^\\.]*"));
}
/* ----------------------------------------------------------------------------------------------------------------------------
------查找 - 获取指定后缀的文件
*/
QList<QFileInfo> S_TempFileManager::getTempFile_BySuffix(QString suffix) {
	return this->search_FileInDir(QDir(this->getTempFileUrl()), QRegExp(".*\\." + suffix));
}
QList<QFileInfo> S_TempFileManager::getTempFile_BySuffix_WithAllSubfolders(QString suffix) {
	return this->search_FileInWithAllSubfolders(QDir(this->getTempFileUrl()), QRegExp(".*\\." + suffix));
}



/* ----------------------------------------------------------------------------------------------------------------------------
------后缀管理 - 忽略后缀
*/
QStringList S_TempFileManager::suffix_getSkip() {
	return this->m_suffix_skip;
}
void S_TempFileManager::suffix_setSkip(QStringList suffix_list) {
	this->m_suffix_skip = suffix_list;
}
void S_TempFileManager::suffix_resetSkip() {
	this->m_suffix_skip = this->m_suffix_defaultSkip;
}
bool S_TempFileManager::suffix_isSkipFile(QFileInfo info){
	QString suffix = info.suffix().toLower();
	return this->m_suffix_skip.contains(suffix);
}
/* ----------------------------------------------------------------------------------------------------------------------------
------后缀管理 - 禁止后缀
*/
QStringList S_TempFileManager::suffix_getForbidden() {
	return this->m_suffix_forbidden;
}
void S_TempFileManager::suffix_setForbidden(QStringList suffix_list) {
	this->m_suffix_forbidden = suffix_list;
}
void S_TempFileManager::suffix_resetForbidden() {
	this->m_suffix_forbidden = this->m_suffix_defaultForbidden;
}
bool S_TempFileManager::suffix_isForbiddenFile(QFileInfo info){
	QString suffix = info.suffix().toLower();
	return this->m_suffix_forbidden.contains(suffix);
}
/* ----------------------------------------------------------------------------------------------------------------------------
------后缀管理 - 复制文件夹 A -> B （覆写）
*/
bool S_TempFileManager::copyDirPrivate(QString dirPath_from, QString dirPath_to){
	QDir dir_to(dirPath_to);
	QDir dir_from(dirPath_from);
	if (dir_to.absolutePath() == dir_from.absolutePath()){ return true; }		//（相同路径跳过）

	// > 路径生成
	if (dir_to.exists() == false){
		dir_to.mkdir(dirPath_to);
	}
	bool all_success = true;

	// > 子文件遍历（禁止后缀）
	QFileInfoList f_list = dir_from.entryInfoList(QDir::Files);
	for (int i = 0; i < f_list.size(); i++) {
		QFileInfo file_from_info(f_list.at(i));

		// > 禁止后缀出现后，立即终止操作
		if (this->suffix_isForbiddenFile(file_from_info)){ return false; }
	}

	// > 子文件遍历
	for (int i = 0; i < f_list.size(); i++) {
		QFileInfo file_from_info(f_list.at(i));
		QString file_to_name = dirPath_to + "/" + file_from_info.fileName();

		// > 忽略后缀
		if (this->suffix_isSkipFile(file_from_info)){
			continue;
		}

		bool success = this->copyFilePrivate(file_from_info.absoluteFilePath(), file_to_name);
		if (success == false){ all_success = false; }
	}
	return all_success;
}



/* ----------------------------------------------------------------------------------------------------------------------------
------log文件 - 生成log文件，追加
*/
void S_TempFileManager::addDebugLog(QString filename, QString logdata, bool record_time){
	QFile file_to(this->getTempFileUrl() + "/" + filename);
	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Append)) { return; }
	QString data = QString("\n");
	if (record_time == true){
		data.append(QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss"));
		data.append("\n");
	}
	data.append(logdata);
	QByteArray ba = data.toUtf8();
	file_to.seek(file_to.size());
	file_to.write(ba);
	file_to.close();
}
/* ----------------------------------------------------------------------------------------------------------------------------
------log文件 - 生成log文件，覆写
*/
void S_TempFileManager::replaceDebugLog(QString filename, QString logdata) {
	this->generateTempFileStrict(filename, logdata, "toUtf8");
}
/* ----------------------------------------------------------------------------------------------------------------------------
------log文件 - 生成log文件，覆写
*/
void S_TempFileManager::replaceDebugLog(QString filename, QByteArray logdata){
	QFile file_to(this->getTempFileUrl() + "/" + filename);
	if (file_to.exists()) {
		file_to.remove();
	}

	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return; }
	file_to.write(logdata);
	file_to.close();
}
/* ----------------------------------------------------------------------------------------------------------------------------
------log文件 - 生成bat文件，覆写
*/
void S_TempFileManager::replaceDebugBat(QString filename, QString logdata) {
	this->generateTempFileStrict(filename, logdata, "toLocal8Bit");
}



/* ----------------------------------------------------------------------------------------------------------------------------
------其他 - 打开缓存路径文件夹
*/
bool S_TempFileManager::openDesktopTempUrl() {
	QString buffer_path = this->getTempFileUrl();
	return QDesktopServices::openUrl(QUrl("file:/" + buffer_path + "/"));
}
/* ----------------------------------------------------------------------------------------------------------------------------
------其他 - 打开缓存路径中指定文件
*/
bool S_TempFileManager::openDesktopTempFile(QString filename) {
	if (!this->hasTempFile(filename)) { return false; }
	QString buffer_path = this->getTempFileUrl();
	return QDesktopServices::openUrl(QUrl("file:/" + buffer_path + "/"+ filename));
}

