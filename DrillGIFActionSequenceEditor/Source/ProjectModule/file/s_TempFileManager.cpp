#include "stdafx.h"

#include "s_TempFileManager.h"


/*
-----==========================================================-----
		�ࣺ		temp�ļ��й���.cpp
		�汾��		v1.19
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		��temp�ļ��е��κβ�������������С�
					ֱ����exe�ļ�Ŀ¼�½���temp�ļ��У����ļ���ת�ơ���ʱ����

		Ŀ�꣺		�ļ���
						->Ψһ�ļ�
						->ɾ��
						->���ƽ�
						->���Ƴ�
						->����debug/log�ļ�
					��������
						->���֣��ര��ʱ��
						->���������������
					����:
						->����ʱ����ָ����׺�ļ�
						
		��Ҫ���	1.�����ļ��У�
						copyResourceToTempFile ���Ϊ1��
						copyResourceToTempFileIncludChildDir ���Ϊ2��
					2.temp�ļ����Զ���exe�½�����
					3.�����޷�������ʱ����ļ��У���Ҫ���ֶ� destroyInstance()

		����˵����	1.��temp�ļ�����QDir::tempPath()�ļ�����ȫ��ͬ��������Ŀ��ʱ���ڴ洢��Ŀ¼��
					2.�޸�һ��Ҫ��ϸ��debug�鿴�ļ�·����QFileInfo��˽�б����������ڱ�����չ����
					3.�����ļ������ļ��У�
						absolutePath     - ��һ��Ŀ¼
						absoluteDir      - ��һ��Ŀ¼
						absoluteFilePath - �ļ����ļ��е�ȫ��
					4.entryInfoList����ȫ�������ļ��У���Ҫɸѡ����

		���÷�����	��ȡ������Ȼ��ֱ��ִ�з�����
						S_TempFileManager::getInstance()->clearAllTempFile();
						S_TempFileManager::getInstance()->copyResourceToTempFile("D:/aaa");
					��ȡtemp�ļ����е��ļ�����fileָ��Ͷ����޷����ݣ��ܱ���
						QFile f( S_TempFileManager::getInstance()->getTempFileUrl() + "/" + filename);
					�رճ���ʱ����Ҫ�ֶ����ٵ�����
						S_TempFileManager::getInstance()->destroyInstance();

-----==========================================================-----
*/

S_TempFileManager::S_TempFileManager(){
	init();
}
S_TempFileManager::~S_TempFileManager(){
}
/* -------------����------------ */
S_TempFileManager* S_TempFileManager::cur_instance = NULL;
S_TempFileManager* S_TempFileManager::getInstance() {
	if (cur_instance == NULL) {
		cur_instance = new S_TempFileManager();
	}
	return cur_instance;
}
/* -------------���ٵ���------------ */
void S_TempFileManager::destroyInstance() {
	this->clearAllTempFile();
	QDir dir(this->workspace_url);
	dir.removeRecursively();
	delete cur_instance;
}
/* -------------��ʼ��------------ */
void S_TempFileManager::init() {

	this->temp_file_url = QCoreApplication::applicationDirPath() + "/temp";		//����temp�ļ���
	QDir dir(this->temp_file_url);
	dir.mkdir(this->temp_file_url);

	int temp_id = 1;		//��������޸�
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

	this->workspace_url = this->temp_file_url + auto_name;	//����������
	QDir dir2(this->workspace_url);
	dir2.mkdir(this->workspace_url);

	this->default_skip_suffix = QStringList();						//���Ժ�׺
	this->default_forbidden_suffix = QStringList() << "exe" << "lnk";		//��ֹ��׺
	this->skip_suffix = this->default_skip_suffix;
	this->forbidden_suffix = this->default_forbidden_suffix;
}


/* ----------------------------------------------------------------------------------------------------------------------------
------��ȡ - �����ļ���·��
*/
QString S_TempFileManager::getMainUrl() {
	return this->temp_file_url;
}
/* ----------------------------------------------------------------------------------------------------------------------------
------��ȡ - ��ǰ���湤����·��
*/
QString S_TempFileManager::getTempFileUrl() {
	return this->workspace_url;
}
/* ----------------------------------------------------------------------------------------------------------------------------
------��ȡ - �Ƿ����ָ���ļ�
*/
bool S_TempFileManager::hasTempFile(QString filename) {
	return QFile(this->workspace_url + "/" + filename).exists();
}
/* ----------------------------------------------------------------------------------------------------------------------------
------��ȡ - ָ���ļ��Ƿ���temp�ļ�����
*/
bool S_TempFileManager::isInCurTempFile(QString filename) {
	return QFileInfo(filename).absoluteFilePath().contains(QFileInfo(this->workspace_url).absoluteFilePath());
}

/* ----------------------------------------------------------------------------------------------------------------------------
------��׺ - ���Ժ�׺
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
------��׺ - ��ֹ��׺
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
------ɾ�� - ɾ���ļ������ݺ�׺��
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
	// > �ݹ�ɾ�������0��
	this->removeFile_recursion( "*." + suffix, 0, this->workspace_url, 0);
}
void S_TempFileManager::removeInTemp_FileBySuffix_WithAllSubfolders(QString suffix) {
	// > �ݹ�ɾ�������-1��
	this->removeFile_recursion("*." + suffix, -1, this->workspace_url, 0);
}
void S_TempFileManager::removeInTemp_FileByNameNoSuffix(QString onlyname) {
	// > �ݹ�ɾ�������0��
	this->removeFile_recursion(onlyname + ".*", 0, this->workspace_url,0 );
}
void S_TempFileManager::removeInTemp_FileByNameNoSuffix_WithAllSubfolders(QString onlyname) {
	// > �ݹ�ɾ�������-1��
	this->removeFile_recursion(onlyname + ".*", -1, this->workspace_url, 0);
}

void S_TempFileManager::removeFile_recursion(QString nameFilter, int tar_depth, QString cur_url, int cur_depth) {
	QDir cur_from(cur_url);

	// > ɾ����ǰ����ļ�
	QFileInfoList f_list = cur_from.entryInfoList(QStringList() << nameFilter, QDir::Files, QDir::Name);
	for (int i = 0; i < f_list.count(); i++) {
		QFile::remove(f_list.at(i).absoluteFilePath());
	}

	// > ��ȼ��
	if (tar_depth != -1){				//��-1��������ȣ�
		if (tar_depth == cur_depth){	//���ﵽָ����Ⱥ��ٸ��ƣ�
			return;
		}
	}

	// > ���ļ���
	QFileInfoList d_list = cur_from.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);	// NoDotAndDotDotָ�ļ��е�ǰ������һ����Ŀ¼
	for (int i = 0; i < d_list.count(); i++){
		QFileInfo info = d_list.at(i);
		this->removeFile_recursion(nameFilter, tar_depth, info.absoluteFilePath(), cur_depth + 1);
	}
}
/* ----------------------------------------------------------------------------------------------------------------------------
------ɾ�� - ��������ļ��������ļ��У�
*/
void S_TempFileManager::clearAllTempFile() {

	QDir dir( this->workspace_url );
	QFileInfoList fileList;
	QFileInfo curfile;
	if (!dir.exists()) { return; }
	fileList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::Readable | QDir::Writable | QDir::Hidden | QDir::NoDotAndDotDot, QDir::Name);

	int delay = 0;

	int infoNum = fileList.size();
	for (int i = infoNum - 1; i >= 0; i--) {	//����ɾ���������Ͳ���Ҫ�������ˣ�
		curfile = fileList[i];
		if (curfile.isFile()) {
			QFile file_temp(curfile.filePath());
			file_temp.remove();
			fileList.removeAt(i);
		}
		if (curfile.isDir()) {
			QDir dir2(curfile.filePath());		//ɾ���ļ���
			dir2.removeRecursively();
			fileList.removeAt(i);
		}
	}
}
/* ----------------------------------------------------------------------------------------------------------------------------
------���� - ����ļ� -> temp�ļ��У�ǰ��Ŀ���ļ��� F:/aaa/bbb������Ŀ���ļ��б�["aa.xds","bb.vcut"��]��
*/
void S_TempFileManager::copyResourceToTemp_SeveralFile(QString src_url, QStringList file_names) {
	QFileInfo info(src_url);
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return; }	//��temp�ļ����ڲ����ܸ��ƣ�
	if (src_url.at(src_url.size()-1) == '/'){
		src_url = src_url.mid(0, src_url.size() - 1);
	}
	for (int i = 0; i < file_names.size(); i++){
		QString file_from_name = src_url + "/" + file_names.at(i);
		if (QFile(file_from_name).exists()){
			QString file_to_name = this->workspace_url + "/" + file_names.at(i);
			this->copyFile(file_from_name, file_to_name);
		}
	}
}
/* ----------------------------------------------------------------------------------------------------------------------------
------���� - ���temp�ļ� -> ָ���ļ��� ��ǰ��Ŀ���ļ��� F:/aaa/bbb������temp�ļ��б�["aa.xds","bb.vcut"��]��
*/
void S_TempFileManager::copyTempToTarget_SeveralFile(QString tar_url, QStringList file_names) {
	if (tar_url.at(tar_url.size() - 1) == '/'){
		tar_url = tar_url.mid(0, tar_url.size() - 1);
	}
	for (int i = 0; i < file_names.size(); i++){
		QString file_from_name = this->workspace_url + "/" + file_names.at(i);
		if (QFile(file_from_name).exists()){
			QString file_to_name = tar_url + "/" + file_names.at(i);
			this->copyFile(file_from_name, file_to_name);
		}
	}
}
/* ----------------------------------------------------------------------------------------------------------------------------
------���� - ָ���ļ�/�ļ��� -> temp�ļ��� ���ļ����ļ��ж����ԣ�F:/aaa/bbb ����·����
*/
bool S_TempFileManager::copyResourceToTemp_File(QString src_url) {
	QFileInfo info(src_url);
	if (!info.isFile()){ return false; }
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return false; }	//��temp�ļ����ڲ����ܸ��ƣ�
	return this->copyFile(info.absoluteFilePath(), this->workspace_url + "/" + info.fileName());
}
bool S_TempFileManager::copyResourceToTemp_Dir(QString src_url) {
	// > �ݹ鸴�ƣ����0��
	QFileInfo info(src_url);
	if (!info.isDir()){ return false; }
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return false; }	//��temp�ļ����ڲ����ܸ��ƣ�
	QString dir_from = src_url;
	QString dir_to = this->workspace_url;
	return this->copyDir_recursion(dir_to, 0, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyResourceToTemp_DirWithDepth(QString src_url, int depth) {
	// > �ݹ鸴�ƣ�����Զ��壩
	QFileInfo info(src_url);
	if (!info.isDir()){ return false; }
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return false; }	//��temp�ļ����ڲ����ܸ��ƣ�
	QString dir_from = src_url;
	QString dir_to = this->workspace_url;
	return this->copyDir_recursion(dir_to, depth, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyResourceToTemp_DirWithAllSubfolders(QString src_url) {
	// > �ݹ鸴�ƣ����-1��
	QFileInfo info(src_url);
	if (!info.isDir()){ return false; }
	if (this->isInCurTempFile(info.absoluteFilePath()) == true){ Q_ASSERT(false); return false; }	//��temp�ļ����ڲ����ܸ��ƣ�
	QString dir_from = src_url;
	QString dir_to = this->workspace_url;
	return this->copyDir_recursion(dir_to, -1, dir_from, dir_from, 0);
}

/* ----------------------------------------------------------------------------------------------------------------------------
------���� - temp�ļ��� -> ָ���ļ��� ��F:/aaa/bbb ����·����
*/
bool S_TempFileManager::copyTempToTarget_Dir(QString tar_url) {
	// > �ݹ鸴�ƣ����0��
	QString dir_from = this->workspace_url;
	QString dir_to = tar_url;
	return this->copyDir_recursion(dir_to, 0, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTarget_DirWithDepth(QString tar_url, int depth) {
	// > �ݹ鸴�ƣ�����Զ��壩
	QString dir_from = this->workspace_url;
	QString dir_to = tar_url;
	return this->copyDir_recursion(dir_to, depth, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTarget_DirWithAllSubfolders(QString tar_url) {
	// > �ݹ鸴�ƣ����-1��
	QString dir_from = this->workspace_url;
	QString dir_to = tar_url;
	return this->copyDir_recursion(dir_to, -1, dir_from, dir_from, 0);
}



/* ----------------------------------------------------------------------------------------------------------------------------
------���� - temp�ļ��� ���ļ����������µ�����
*/
void S_TempFileManager::copyTempToTemp_FileWithNewName(QString filename, QString file_new_name){
	if (filename == ""){ return; }
	if (file_new_name == ""){ return; }
	QString urlA = this->workspace_url + "/" + filename;
	QString urlB = this->workspace_url + "/" + file_new_name;
	this->copyFile(urlA, urlB);
}
/* ----------------------------------------------------------------------------------------------------------------------------
------���� - temp�ļ��� ���ļ��У��������µ�����
*/
bool S_TempFileManager::copyTempToTemp_Dir(QString dir_name, QString dir_new_name){
	QString dir_from = this->workspace_url + "/" + dir_name;
	QString dir_to = this->workspace_url + "/" + dir_new_name;
	return this->copyDir_recursion(dir_to, 0, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTemp_DirWithDepth(QString dir_name, QString dir_new_name, int depth){
	QString dir_from = this->workspace_url + "/" + dir_name;
	QString dir_to = this->workspace_url + "/" + dir_new_name;
	return this->copyDir_recursion(dir_to, depth, dir_from, dir_from, 0);
}
bool S_TempFileManager::copyTempToTemp_DirWithAllSubfolders(QString dir_name, QString dir_new_name){
	QString dir_from = this->workspace_url + "/" + dir_name;
	QString dir_to = this->workspace_url + "/" + dir_new_name;
	return this->copyDir_recursion(dir_to, -1, dir_from, dir_from, 0);
}


/* ----------------------------------------------------------------------------------------------------------------------------
------˽�� - �����ļ� A -> B �������ļ��У�
*/
bool S_TempFileManager::copyFile(QString filePath_from, QString filePath_to) {
	//qDebug() << file_from + " -> " + file_to;
	QFile file_from(filePath_from);
	QFile file_to(filePath_to);
	if (!file_from.open(QIODevice::ReadOnly)) { return false; }
	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return false; }
	QByteArray ba = file_from.readAll();
	file_to.write(ba);
	file_from.close();
	file_to.close();
	return true;
}
/* ----------------------------------------------------------------------------------------------------------------------------
------˽�� - �����ļ��� A -> B ��ֻ�������ļ���
*/
bool S_TempFileManager::copyDir(QString dirPath_from, QString dirPath_to){
	QFileInfo info_to(dirPath_to);
	QFileInfo info_from(dirPath_from);
	QDir dir_to(dirPath_to);
	QDir dir_from(dirPath_from);

	// > ·������
	dir_to.mkdir(dirPath_to);

	// > ��ֹ����
	QFileInfoList f_list = dir_from.entryInfoList(QDir::Files);
	for (int i = 0; i < f_list.size(); i++) {
		QFileInfo temp_info(f_list.at(i));
		if (this->isForbiddenFile(temp_info)){
			return false;
		}
	}

	// > ֱ�Ӹ���
	for (int i = 0; i < f_list.size(); i++) {
		QFileInfo temp_info(f_list.at(i));
		if (!this->isSkipFile(temp_info)){
			QString file_to_name = info_to.absoluteFilePath() + "/" + temp_info.fileName();
			this->copyFile(temp_info.absoluteFilePath(), file_to_name);
		}
	}
	return true;
}
/* ----------------------------------------------------------------------------------------------------------------------------
------˽�� - �����ļ��� A -> B ��������ȱ��������ļ��У�
*/
bool S_TempFileManager::copyDir_recursion(QString tar_parent_url, int tar_depth, QString cur_parent_url, QString cur_url, int cur_depth) {
	QDir parent_to(tar_parent_url);
	QDir parent_from(cur_parent_url);
	QDir cur_from(cur_url);

	// > ���Ƶ�ǰ����ļ���temp�еģ�
	QString child_path = cur_from.absolutePath();
	child_path = child_path.replace(parent_from.absolutePath(), parent_to.absolutePath());	//�����Ʒ����ļ���·�����滻��Ŀ�귽���ļ���·��
	QDir(child_path).mkpath(child_path);
	bool result = this->copyDir(cur_from.absolutePath(), child_path);

	// > ��ȼ��
	if (tar_depth != -1){				//��-1��������ȣ�
		if (tar_depth == cur_depth){	//���ﵽָ����Ⱥ��ٸ��ƣ�
			return result;
		}
	}

	// > ���ļ���
	QFileInfoList d_list = cur_from.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);	// NoDotAndDotDotָ�ļ��е�ǰ������һ����Ŀ¼
	for (int i = 0; i < d_list.count(); i++){
		QFileInfo info = d_list.at(i);
		result = result && this->copyDir_recursion(parent_to.absolutePath(), tar_depth, parent_from.absolutePath(), info.absoluteFilePath(), cur_depth + 1);
	}

	return result;
}


/* ----------------------------------------------------------------------------------------------------------------------------
------���� - ����һ���ļ���д�����ݣ��ļ��Ѵ��ڣ��򲻲�����
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
------���� - ����һ���ļ���д�����ݣ��ļ��Ѵ��ڣ��򸲸ǣ�
*/
void S_TempFileManager::generateTempFileStrict(QString filename, QString filedata, QString code = "utf8") {
	QFile file_to(this->workspace_url + "/" + filename);
	if (file_to.exists()) {
		file_to.remove();
	}

	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return; }
	QByteArray ba;
	if (code == "toLocal8Bit"){		//windows�����ַ�
		ba = filedata.toLocal8Bit();
	}
	else if (code == "toLatin1"){	//ISO-8859-1���룬ֻ��ASCII����֧�ֺ���
		ba = filedata.toLatin1();
	}
	else{
		ba = filedata.toUtf8();
	}
	file_to.write(ba);
	file_to.close();
	/*else if (code == "toUcs4"){	//4�ֽڵĿ��ַ�����
	ba = filedata.toUcs4();
	}*/
}

/* ----------------------------------------------------------------------------------------------------------------------------
------���� - ����log�ļ�����д
*/
void S_TempFileManager::replaceDebugLog(QString filename, QString logdata) {
	this->generateTempFileStrict(filename, logdata);
}
/* ----------------------------------------------------------------------------------------------------------------------------
------���� - ����log�ļ�����д
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
------���� - ����log�ļ���׷��
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
------���� - �򿪻���·���ļ���
*/
bool S_TempFileManager::openDesktopTempUrl() {
	QString buffer_path = this->getTempFileUrl();
	return QDesktopServices::openUrl(QUrl("file:/" + buffer_path + "/"));
}
/* ----------------------------------------------------------------------------------------------------------------------------
------���� - �򿪻���·����ָ���ļ�
*/
bool S_TempFileManager::openDesktopTempFile(QString filename) {
	if (!this->hasTempFile(filename)) { return false; }
	QString buffer_path = this->getTempFileUrl();
	return QDesktopServices::openUrl(QUrl("file:/" + buffer_path + "/"+ filename));
}