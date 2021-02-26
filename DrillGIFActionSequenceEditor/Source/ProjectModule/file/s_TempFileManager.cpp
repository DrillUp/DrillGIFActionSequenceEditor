#include "stdafx.h"
#include "s_TempFileManager.h"


/*
-----==========================================================-----
		类：		temp文件夹管理.cpp
		版本：		v1.21
		所属模块：	项目管理模块
		功能：		对temp文件夹的任何操作都在这里进行。
					直接在exe文件目录下建立temp文件夹，对文件作转移、临时处理。

		目标：		文件：
						->唯一文件
						->删除
						->复制进
						->复制出
						->生成debug/log文件
					工作区：
						->划分（多窗口时）
						->工作区间伴随销毁
					其它:
						->复制时忽略指定后缀文件
						
		必要事项：	1.搜索文件夹：
						copyResourceToTempFile 深度为1；
						copyResourceToTempFileIncludChildDir 深度为2；
					2.temp文件夹自动在exe下建立。
					3.单例无法在销毁时清空文件夹，需要你手动 destroyInstance()

		其它说明：	1.该temp文件夹与QDir::tempPath()文件夹完全不同，用于项目临时或长期存储的目录。
					2.修改一定要仔细用debug查看文件路径（QFileInfo是私有变量，不能在变量中展开）
					3.不管文件还是文件夹，
						absolutePath     - 上一级目录
						absoluteDir      - 上一级目录
						absoluteFilePath - 文件或文件夹的全名
					4.entryInfoList包含全部父级文件夹，需要筛选出来

		调用方法：	获取单例，然后直接执行方法。
						S_TempFileManager::getInstance()->clearAllTempFile();
						S_TempFileManager::getInstance()->copyResourceToTempFile("D:/aaa");
					获取temp文件夹中的文件：（file指针和对象都无法传递，总报错）
						QFile f( S_TempFileManager::getInstance()->getTempFileUrl() + "/" + filename);
					关闭程序时，需要手动销毁单例。
						S_TempFileManager::getInstance()->destroyInstance();

-----==========================================================-----
*/

S_TempFileManager::S_TempFileManager(){
	init();
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
/* -------------销毁单例------------ */
void S_TempFileManager::destroyInstance() {
	this->removeAllTempFile();
	QDir dir(this->workspace_url);
	dir.removeRecursively();
	delete cur_instance;
}
/* -------------初始化------------ */
void S_TempFileManager::init() {

	this->temp_file_url = QCoreApplication::applicationDirPath() + "/temp";		//建立temp文件夹
	QDir dir(this->temp_file_url);
	dir.mkdir(this->temp_file_url);

	int temp_id = 1;		//命名序号修改
	QString auto_name = "";
	while (true){
		QString temp = "/workspace_" + QString::number(temp_id);
		if ( QFile(this->temp_file_url + temp).exists() ){
			temp_id++; continue;
		}
		else{
			auto_name = temp; break;
		}
	}

	this->workspace_url = this->temp_file_url + auto_name;	//建立工作区
	QDir dir2(this->workspace_url);
	dir2.mkdir(this->workspace_url);

	this->default_skip_suffix = QStringList();						//忽略后缀
	this->default_forbidden_suffix = QStringList() << "exe" << "lnk";		//禁止后缀
	this->skip_suffix = this->default_skip_suffix;
	this->forbidden_suffix = this->default_forbidden_suffix;
}


/* ----------------------------------------------------------------------------------------------------------------------------
------获取 - 缓存文件夹路径
*/
QString S_TempFileManager::getMainUrl() {
	return this->temp_file_url;
}
/* ----------------------------------------------------------------------------------------------------------------------------
------获取 - 当前缓存工作区路径
*/
QString S_TempFileManager::getTempFileUrl() {
	return this->workspace_url;
}
/* ----------------------------------------------------------------------------------------------------------------------------
------获取 - 是否存在指定文件
*/
bool S_TempFileManager::hasTempFile(QString filename) {
	return QFile(this->workspace_url + "/" + filename).exists();
}
/* ----------------------------------------------------------------------------------------------------------------------------
------获取 - 指定文件是否在temp文件夹中
*/
bool S_TempFileManager::isInCurTempFile(QString filename) {
	return QFileInfo(filename).absoluteFilePath().contains(QFileInfo(this->workspace_url).absoluteFilePath());
}

/* ----------------------------------------------------------------------------------------------------------------------------
------后缀 - 忽略后缀
*/
QStringList S_TempFileManager::getSkipSuffix() {
	return this->skip_suffix;
}
void S_TempFileManager::setSkipSuffix(QStringList suffix_list) {
	this->skip_suffix = suffix_list;
}
void S_TempFileManager::resetSkipSuffix() {
	this->skip_suffix = this->default_skip_suffix;
}
bool S_TempFileManager::isSkipFile(QFileInfo info){
	QString suffix = info.suffix().toLower();
	return this->skip_suffix.contains(suffix);
}

/* ----------------------------------------------------------------------------------------------------------------------------
------后缀 - 禁止后缀
*/
QStringList S_TempFileManager::getForbiddenSuffix() {
	return this->forbidden_suffix;
}
void S_TempFileManager::setForbiddenSuffix(QStringList suffix_list) {
	this->forbidden_suffix = suffix_list;
}
void S_TempFileManager::resetForbiddenSuffix() {
	this->forbidden_suffix = this->default_forbidden_suffix;
}
bool S_TempFileManager::isForbiddenFile(QFileInfo info){
	QString suffix = info.suffix().toLower();
	return this->forbidden_suffix.contains(suffix);
}

/* ----------------------------------------------------------------------------------------------------------------------------
------删除 - 删除文件（根据后缀）
*/
void S_TempFileManager::removeInTemp_Dir(QString dirname) {
	QFileInfo info = QFileInfo(this->workspace_url + "/" + dirname);
	if (info.isDir()) {
		QDir dir(info.absoluteFilePath());
		dir.removeRecursively();
	}
}
void S_TempFileManager::removeInTemp_File(QString filename) {
	QFileInfo info = QFileInfo(this->workspace_url + "/" + filename);
	if (info.isFile()) {
		QFile::remove(info.absoluteFilePath());
	}
}
void S_TempFileManager::removeInTemp_FileBySuffix(QString suffix) {
	// > 递归删除（深度0）
	this->removeFilePrivate_recursion( "*." + suffix, 0, this->workspace_url, 0);
}
void S_TempFileManager::removeInTemp_FileBySuffix_WithAllSubfolders(QString suffix) {
	// > 递归删除（深度-1）
	this->removeFilePrivate_recursion("*." + suffix, -1, this->workspace_url, 0);
}
void S_TempFileManager::removeInTemp_FileByNameNoSuffix(QString onlyname) {
	// > 递归删除（深度0）
	this->removeFilePrivate_recursion(onlyname + ".*", 0, this->workspace_url, 0);
}
void S_TempFileManager::removeInTemp_FileByNameNoSuffix_WithAllSubfolders(QString onlyname) {
	// > 递归删除（深度-1）
	this->removeFilePrivate_recursion(onlyname + ".*", -1, this->workspace_url, 0);
}

/* ----------------------------------------------------------------------------------------------------------------------------
------删除 - 清空所有文件（包括文件夹）
*/
void S_TempFileManager::removeAllTempFile() {

	QDir dir( this->workspace_url );
	QFileInfoList fileList;
	QFileInfo curfile;
	if (!dir.exists()) { return; }
	fileList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::Readable | QDir::Writable | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);

	int delay = 0;

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
------复制 - 多个文件 -> temp文件夹（前者目标文件夹 F:/aaa/bbb，后者目标文件列表：["aa.xds","bb.vcut"…]）
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
			QString file_to_name = this->workspace_url + "/" + file_names.at(i);
			this->copyFilePrivate(file_from_name, file_to_name);
		}
	}
}
/* ----------------------------------------------------------------------------------------------------------------------------
------复制 - 多个temp文件 -> 指定文件夹 （前者目标文件夹 F:/aaa/bbb，后者temp文件列表：["aa.xds","bb.vcut"…]）
*/
void S_TempFileManager::copyTempToTarget_SeveralFile(QString tar_url, QStringList file_names) {
	if (tar_url.at(tar_url.size() - 1) == '/'){
		tar_url = tar_url.mid(0, tar_url.size() - 1);
	}
	for (int i = 0; i < file_names.size(); i++){
		QString file_from_name = this->workspace_url + "/" + file_names.at(i);
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
	return this->copyFilePrivate(info.absoluteFilePath(), this->workspace_url + "/" + info.fileName());
}
bool S_TempFileManager::copyResourceToTemp_Dir(QString src_url) {
	// > 递归复制（深度0）
	QFileInfo info(src_url);
	if (!info.isDir()){ return false; }
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return false; }	//（temp文件夹内部不能复制）
	QString dir_from = src_url;
	QString dir_to = this->workspace_url;
	return this->copyDirPrivate_recursion(dir_to, 0, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyResourceToTemp_DirWithDepth(QString src_url, int depth) {
	// > 递归复制（深度自定义）
	QFileInfo info(src_url);
	if (!info.isDir()){ return false; }
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return false; }	//（temp文件夹内部不能复制）
	QString dir_from = src_url;
	QString dir_to = this->workspace_url;
	return this->copyDirPrivate_recursion(dir_to, depth, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyResourceToTemp_DirWithAllSubfolders(QString src_url) {
	// > 递归复制（深度-1）
	QFileInfo info(src_url);
	if (!info.isDir()){ return false; }
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return false; }	//（temp文件夹内部不能复制）
	QString dir_from = src_url;
	QString dir_to = this->workspace_url;
	return this->copyDirPrivate_recursion(dir_to, -1, dir_from, dir_from, 0);
}

/* ----------------------------------------------------------------------------------------------------------------------------
------复制 - temp文件夹 -> 指定文件夹 （F:/aaa/bbb 完整路径）
*/
bool S_TempFileManager::copyTempToTarget_Dir(QString tar_url) {
	// > 递归复制（深度0）
	QString dir_from = this->workspace_url;
	QString dir_to = tar_url;
	return this->copyDirPrivate_recursion(dir_to, 0, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTarget_DirWithDepth(QString tar_url, int depth) {
	// > 递归复制（深度自定义）
	QString dir_from = this->workspace_url;
	QString dir_to = tar_url;
	return this->copyDirPrivate_recursion(dir_to, depth, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTarget_DirWithAllSubfolders(QString tar_url) {
	// > 递归复制（深度-1）
	QString dir_from = this->workspace_url;
	QString dir_to = tar_url;
	return this->copyDirPrivate_recursion(dir_to, -1, dir_from, dir_from, 0);
}



/* ----------------------------------------------------------------------------------------------------------------------------
------复制 - temp文件夹 的文件，并命名新的名字
*/
void S_TempFileManager::copyTempToTemp_FileWithNewName(QString filename, QString file_new_name){
	if (filename == ""){ return; }
	if (file_new_name == ""){ return; }
	QString urlA = this->workspace_url + "/" + filename;
	QString urlB = this->workspace_url + "/" + file_new_name;
	this->copyFilePrivate(urlA, urlB);
}
/* ----------------------------------------------------------------------------------------------------------------------------
------复制 - temp文件夹 的文件夹，并命名新的名字
*/
bool S_TempFileManager::copyTempToTemp_Dir(QString dir_name, QString dir_new_name){
	QString dir_from = this->workspace_url + "/" + dir_name;
	QString dir_to = this->workspace_url + "/" + dir_new_name;
	return this->copyDirPrivate_recursion(dir_to, 0, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTemp_DirWithDepth(QString dir_name, QString dir_new_name, int depth){
	QString dir_from = this->workspace_url + "/" + dir_name;
	QString dir_to = this->workspace_url + "/" + dir_new_name;
	return this->copyDirPrivate_recursion(dir_to, depth, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTemp_DirWithAllSubfolders(QString dir_name, QString dir_new_name){
	QString dir_from = this->workspace_url + "/" + dir_name;
	QString dir_to = this->workspace_url + "/" + dir_new_name;
	return this->copyDirPrivate_recursion(dir_to, -1, dir_from, dir_from, 0);
}


/* ----------------------------------------------------------------------------------------------------------------------------
------私有 - 复制文件夹 A -> B （覆写）
*/
bool S_TempFileManager::copyDirPrivate(QString dirPath_from, QString dirPath_to){
	QFileInfo info_to(dirPath_to);
	QFileInfo info_from(dirPath_from);
	QDir dir_to(dirPath_to);
	QDir dir_from(dirPath_from);
	if (dir_to.absolutePath() == dir_from.absolutePath()){ return true; }		//（相同路径跳过）

	// > 路径生成
	if (dir_to.exists() == false){
		dir_to.mkdir(dirPath_to);
	}
	bool all_success = true;

	// > 禁止复制
	QFileInfoList f_list = dir_from.entryInfoList(QDir::Files);
	for (int i = 0; i < f_list.size(); i++) {
		QFileInfo temp_info(f_list.at(i));
		if (this->isForbiddenFile(temp_info)){
			return false;	//（出现后，立即终止）
		}
	}

	// > 直接复制
	for (int i = 0; i < f_list.size(); i++) {
		QFileInfo temp_info(f_list.at(i));
		if (!this->isSkipFile(temp_info)){
			QString file_to_name = info_to.absoluteFilePath() + "/" + temp_info.fileName();
			bool success = this->copyFilePrivate(temp_info.absoluteFilePath(), file_to_name);
			if ( success == false){ all_success = false; }
		}
	}
	return all_success;
}


/* ----------------------------------------------------------------------------------------------------------------------------
------生成 - 生成一个文件，写入数据（文件已存在，则不操作）
*/
void S_TempFileManager::generateTempFile(QString filename, QString filedata) {
	if (this->hasTempFile(filename)) {
		return;
	}
	QFile file_to(this->workspace_url +"/"+filename);
	if (!file_to.open(QIODevice::WriteOnly)) { return; }
	QByteArray ba = filedata.toUtf8();
	file_to.write(ba);
	file_to.close();
}
/* ----------------------------------------------------------------------------------------------------------------------------
------生成 - 生成一个文件，写入数据（文件已存在，则覆盖）
*/
void S_TempFileManager::generateTempFileStrict(QString filename, QString filedata, QString code = "utf8") {
	QFile file_to(this->workspace_url + "/" + filename);
	if (file_to.exists()) {
		file_to.remove();
	}

	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return; }
	QByteArray ba;
	if (code == "toLocal8Bit"){		//windows本地字符
		ba = filedata.toLocal8Bit();
	}
	else if (code == "toLatin1"){	//ISO-8859-1编码，只有ASCII，不支持汉字
		ba = filedata.toLatin1();
	}
	else{
		ba = filedata.toUtf8();
	}
	file_to.write(ba);
	file_to.close();
	/*else if (code == "toUcs4"){	//4字节的宽字符编码
	ba = filedata.toUcs4();
	}*/
}

/* ----------------------------------------------------------------------------------------------------------------------------
------混杂 - 生成log文件，覆写
*/
void S_TempFileManager::replaceDebugLog(QString filename, QString logdata) {
	this->generateTempFileStrict(filename, logdata);
}
/* ----------------------------------------------------------------------------------------------------------------------------
------混杂 - 生成log文件，覆写
*/
void S_TempFileManager::replaceDebugLog(QString filename, QByteArray logdata){
	QFile file_to(this->workspace_url + "/" + filename);
	if (file_to.exists()) {
		file_to.remove();
	}

	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return; }
	file_to.write(logdata);
	file_to.close();
}

/* ----------------------------------------------------------------------------------------------------------------------------
------混杂 - 生成log文件，追加
*/
void S_TempFileManager::addDebugLog(QString filename, QString logdata){
	QFile file_to(this->workspace_url + "/" + filename);
	if (file_to.exists()) {
		file_to.remove();
	}

	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Append)) { return; }
	QByteArray ba = logdata.toUtf8();
	file_to.seek(file_to.size());
	file_to.write(ba);
	file_to.close();
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