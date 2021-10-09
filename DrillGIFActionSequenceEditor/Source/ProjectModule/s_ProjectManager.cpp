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
		�ࣺ		��Ŀ����.cpp
		�汾��		v1.15
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		��Ŀ����������ݡ���ز�������������Ҫ���ơ�
					> ��������
						> ��Ŀ�ļ��У�
							������Ŀʱ��Ĭ�Ͻ���һ����Ŀ�ļ��� ��Ŀ��Files���洢��������ļ���
							�����ļ� ��Ŀ��.xxx �洢��Ŀ¼������Ŀ�ļ���ͬ����
							��Ŀ��֧��"xxx.xx.xx.xx"���������������
						> ��Ŀ�Ķ�������
							�����Ŀ�г������κ�ֵ�ĸĶ��ύ���ͻᱻ��¼�������������"*"�š�
							�ر��ļ�ʱ�����Ƿ�Ķ�����ʾ"��ǰ��δ������޸ģ��Ƿ񱣴棿"��
						> ��ģ̬������
							������ģ̬��ʱ��ִ����Ŀ����������������ʹ�ô����ڱ༭�ڼ䣬����ִ���½�/��/����/���Ϊ��Ҳ���ܹرմ��ڡ�
						> ����򿪵��ļ�����history��
							��Ŀ������¼����򿪵���Ŀ��
					> temp�ļ���
						> ת���ļ���
							ֱ����exe�ļ�Ŀ¼�½���temp�ļ��У����ļ���ת�ơ���ʱ����
							������Ŀ���ļ����ڵ���������ļ����ᱣ�浽��Ŀ�ļ����С�
						> Ψһ�ļ���
							��ĳ���ļ����������ΪuuidΨһ�ļ�����ȷ�����Ե����ظ��ļ��������޸���Ŀ�е��ļ�����
						> log�ļ���
							�ṩ��������debug�õ��ı��ļ�������temp�ļ����С�
					> �洢�ļ�����
						> json�洢�� 
							�ܹ�������ģ����������ݶ��洢��"PROJECT_SUFFIX"�ĺ�׺�ļ��С�
							Ҫ���д洢���ܵ������̳�ʵ�ִ洢ģ���4��������
							���ݽṹ����Ҫʵ��"getJsonObject"������"setJsonObject"������ȷ�����������඼�ܹ�ת��json���ݡ�
			
		ע�����	1.�ù�������Ҫ�޸Ĵ������֣��ᷢ���źš�
					2.PROJECT_SUFFIX ��c_ProjectData���ж��塣
-----==========================================================-----
*/

S_ProjectManager::S_ProjectManager(){
	S_TempFileManager::getInstance()->setSkipSuffix(S_TempFileManager::getInstance()->getSkipSuffix() << PROJECT_SUFFIX << "cbf");
	
	// > ������ʼ��
	this->m_isDirty = false;
	this->m_lockingWidgets.clear();
	
	// > ��ʷ��¼��ʼ��
	this->m_historyProjectTank.clear();
	this->m_historyDirTank.clear();
	this->m_historyProjectMax = 10;
	this->m_historyDirMax = 6;
	this->loadHistory();
}
S_ProjectManager::~S_ProjectManager() {
}

/* --------------------------------------------------------------
----------ProjectManager ����
*/
S_ProjectManager* S_ProjectManager::project_manager = NULL;
S_ProjectManager* S_ProjectManager::getInstance() {
	if (project_manager == NULL) {
		project_manager = new S_ProjectManager();
	}
	return project_manager;
}


/* --------------------------------------------------------------
		ȫ�� - �����������޸�
*/
void S_ProjectManager::setDirty() {
	if (this->m_isDirty == false) {
		this->m_isDirty = true;
		this->changeTitle();
	}
}
/* --------------------------------------------------------------
		ȫ�� - ������Ŀ
*/
void S_ProjectManager::addLock(QWidget* widget) {
	if (this->m_lockingWidgets.size() == 0) {
		emit lockChanged(true);
	}
	this->m_lockingWidgets.push_back(widget);
}
/* --------------------------------------------------------------
		ȫ�� - ������Ŀ
*/
void S_ProjectManager::removeLock(QWidget* widget) {
	this->m_lockingWidgets.removeOne(widget);
	if (this->m_lockingWidgets.size() == 0) {
		emit lockChanged(false);
	}
}
/* --------------------------------------------------------------
		#ȫ�� - �޸�����
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
		ȫ�� - �ж�����
*/
bool S_ProjectManager::isLocked() {
	return this->m_lockingWidgets.size() > 0;
}
/* --------------------------------------------------------------
		ȫ�� - �ж��޸�
*/
bool S_ProjectManager::isDirty() {
	return this->m_isDirty;
}
/* --------------------------------------------------------------
		ȫ�� - �����޸Ĳ�����ʾ
*/
bool S_ProjectManager::dirtyTip() {

	if (this->m_isDirty) {
		switch (QMessageBox::question(PROJECT_INSTANCE, "��ʾ", "��ǰ��δ������޸ģ��Ƿ񱣴棿", "����", "������", "ȡ��", 0)) {
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
		��Ŀ - �½����Ի���
*/
bool S_ProjectManager::newProject() {

	// > �޸���ʾ��ȡ���򷵻�false��
	if (this->dirtyTip() == false) { return false; }

	this->clearProject();					//������Ŀ
	this->changeTitle();					//�޸ı���
	emit newProjectFinished();				//��Ŀ�½���պ��źţ�

	return true;
}


/* --------------------------------------------------------------
		��Ŀ - �򿪣��Ի���
*/
bool S_ProjectManager::openProject() {

	// > �޸���ʾ��ȡ���򷵻�false��
	if (this->dirtyTip() == false) { return false; }

	// > �û�ѡ����Ŀ�ļ�
	QString file_path;
	QFileDialog fd;
	fd.setWindowTitle("����Ŀ");
	fd.setAcceptMode(QFileDialog::AcceptOpen);
	fd.setDirectory(this->data_ProjectData.getParentPath());	
	fd.setNameFilters(QStringList()	<< QString("��Ŀ�ļ�(*.") + PROJECT_SUFFIX + ")");
	fd.setViewMode(QFileDialog::Detail);
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()) {
			//��û��ѡ���ļ�����������
			return false;
		}
		file_path = fd.selectedFiles().at(0);
	}else {
		//������رջ���ȡ�������������
		return false;
	}

	// > ����ָ���ļ���
	this->openProjectDirectly(file_path);
	return true;
}
/* --------------------------------------------------------------
		��Ŀ - �򿪣�����ֵ��
*/
bool S_ProjectManager::openProjectByArgs(int argc, char *argv[]) {
	if (argc == 2) {
		//qDebug() << argv[0];	//exe��·��
		//qDebug() << argv[1];	//�����ļ���·��
		QString exe_path = argv[0];
		QString file_path = argv[1];
		if (QFileInfo(file_path).suffix() == PROJECT_SUFFIX) {
			this->openProjectDirectly(file_path);		//�����ļ���
			return true;
		}
	}
	return false;
}
/* --------------------------------------------------------------
		��Ŀ - �򿪣���ק�ļ���
*/
bool S_ProjectManager::openProjectByDropFile(QDropEvent *event) {

	// > �޸���ʾ��ȡ���򷵻�false��
	if (this->dirtyTip() == false) { return false; }

	// > �û�ѡ����Ŀ�ļ���
	QList<QUrl> urls = event->mimeData()->urls();		//ֻҪ��һ������"drill"��ȡ�ļ������
	for (int i = 0; i < urls.count(); i++) {
		QString file_path = urls.at(i).path();
		if (file_path.at(0) == '/') {					//���˲����������ַ���ȥ��
			file_path = file_path.mid(1);
		}
		if (QFileInfo(file_path).suffix() == PROJECT_SUFFIX) {
			this->openProjectDirectly(file_path);		//�����ļ���
			return true;
		}
	}

	return false;
}
/* --------------------------------------------------------------
		��Ŀ - ���棨�Ի���
*/
bool S_ProjectManager::saveProject() {

	// > ��Ŀδ�����
	if (this->data_ProjectData.isNull()) {
		W_ProjectCreate d(PROJECT_INSTANCE);
		d.setDataInModifyMode(this->data_ProjectData);
		bool accepted = d.exec();
		if (accepted == QDialog::Accepted) {
			this->data_ProjectData = d.getData();	//ˢ����Ŀ����
			this->changeTitle();					//�޸ı���

			this->saveAll(this->data_ProjectData.getProjectRootPath());
		}
		return accepted;
	}
	// > ��Ŀ�Ѵ����
	else{
		this->saveAll(this->data_ProjectData.getProjectRootPath());
		return true;
	}

}

/* --------------------------------------------------------------
		��Ŀ - ���Ϊ���Ի���
*/
bool S_ProjectManager::saveAs() {

	W_ProjectCreate d(PROJECT_INSTANCE);
	d.setDataInModifyMode(this->data_ProjectData);
	d.setWindowTitle("��Ŀ���Ϊ");
	bool accepted = d.exec();
	if (accepted == QDialog::Accepted) {
		this->data_ProjectData = d.getData();	//ˢ����Ŀ����
		this->changeTitle();					//�޸ı���

		this->saveAll(this->data_ProjectData.getProjectRootPath());
	}
	return accepted;

}
/* --------------------------------------------------------------
		��Ŀ - ǿ�Ʊ���
*/
bool S_ProjectManager::saveInForce() {

	// > ��Ŀδ�����
	if (this->data_ProjectData.isNull()) { return false; }

	// > ��Ŀ�Ѵ����
	this->saveAll(this->data_ProjectData.getProjectRootPath());
	return true;
}

/* --------------------------------------------------------------
		#��Ŀ - �����Ŀ����
*/
void S_ProjectManager::clearProject() {

	// > ��Ŀ���в�����ʼ��
	S_StorageManager::getInstance()->clearAllApplicationData();		//����洢����
	this->data_ProjectData = C_ProjectData();						//����ǰ��Ŀ����
	this->m_isDirty = false;										//������Ŀ�޸ĺۼ�

	// > �����������
	S_TempFileManager::getInstance()->removeAllTempFile();

}

/* --------------------------------------------------------------
		#��Ŀ - �򿪣������ļ�����
*/
void S_ProjectManager::openProjectDirectly(QString open_path) {

	// > ������Ŀ
	this->clearProject();

	// > ˢ����Ŀ·��
	QFileInfo info(open_path);
	this->data_ProjectData.setPath(info.absolutePath());
	this->data_ProjectData.setName(info.completeBaseName());
	this->addHistoryProject(info.absoluteFilePath());
	this->addHistoryDir(info.absolutePath());
	this->saveHistory();

	// > �����ļ���temp��
	bool success = S_TempFileManager::getInstance()->copyResourceToTemp_Dir(this->data_ProjectData.getProjectFilePath());
	if (success == false){
		QMessageBox message(QMessageBox::Information, "��ʾ", "��ǰ��Ŀ�а����Ƿ��ļ�·������ʧ�ܡ�\n�뽫�����ļ��ƶ���һ���յ��ļ����У��ٴ򿪹��̡�");
		message.exec();
		return;
	}

	// > ��ȡ�洢�ļ�
	this->readSaveFile();

	// > �޸Ĵ�����Ŀ��
	emit changeWindowTitle(this->data_ProjectData.getSoftname() + " - " + this->data_ProjectData.getName());

	// > ��Ŀ�򿪺��źţ�
	emit openProjectFinished();
}
/* --------------------------------------------------------------
		#��Ŀ - ���棨Ŀ���ļ��У�
*/
void S_ProjectManager::saveAll(QString url) {

	// > �����ļ���
	if (url.at(url.size() - 1) != '/') { url += "/"; }
	QDir temp_dir(url);
	temp_dir.mkpath(url);

	// > �����¼
	this->data_ProjectData.lastSaveDate = QDateTime::currentDateTime();

	// > ���ɴ洢�ļ�
	this->createSaveFile();

	// > ����ۼ�
	this->m_isDirty = false;
	this->changeTitle();

	// > ��Ŀ������źţ�
	emit saveProjectFinished();
}
/* --------------------------------------------------------------
		#��Ŀ - ���棨�ļ���
*/
void S_ProjectManager::createSaveFile() {
	// > �洢������
	S_StorageManager::getInstance()->createSaveFile(this->data_ProjectData.getProjectFile());
	this->addHistoryProject(this->data_ProjectData.getProjectFile());
	this->addHistoryDir(this->data_ProjectData.getProjectRootPath());
	this->saveHistory();

	// > ���� temp �� src
	S_TempFileManager::getInstance()->copyTempToTarget_DirWithAllSubfolders(this->data_ProjectData.getProjectFilePath());

}

/* --------------------------------------------------------------
		#��Ŀ - ��ȡ���ļ���
*/
void S_ProjectManager::readSaveFile() {
	// > ȷ�������ļ�ʱ�������ָ�������
	S_TempFileManager::getInstance()->removeInTemp_File(this->data_ProjectData.getName() + "." + this->data_ProjectData.getSuffix());

	// > �洢������
	S_StorageManager::getInstance()->readSaveFile(this->data_ProjectData.getProjectFile());

	// > ���� src �� temp
	S_TempFileManager::getInstance()->copyResourceToTemp_DirWithAllSubfolders(this->data_ProjectData.getProjectFilePath());

}


/* --------------------------------------------------------------
		#��Ŀ - ��ȡ�ļ������ݺ�׺��F:/aaa/bbb.suf����·����
*/
QString S_ProjectManager::getOneFileBySuffix(QString url, QString suffix) {
	suffix = suffix.replace(".", "");
	QDir dir(url);
	QString result;
	QFileInfoList list = dir.entryInfoList();
	for (int i = 0; i < list.size(); i++) {
		QString file_sufx = list.at(i).suffix();
		if ( file_sufx == suffix ) {
			result = list.at(i).absoluteFilePath();		//����·��
			break;
		}
	}
	return result;
}

/*-----------------------------------
		���� - ��ȡ�洢������
*/
QString S_ProjectManager::getSaveName() {
	return "S_ProjectManager";
}
/*-----------------------------------
		���� - �����ǰ����������
*/
void S_ProjectManager::clearAllData() {
	
	this->data_ProjectData = C_ProjectData();
	
}

/*-----------------------------------
		���� - ȫ������������� -> QJsonObject
*/
QJsonObject S_ProjectManager::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	QJsonObject obj_ProjectData = QJsonObject();			//��Ŀ��������
	obj_ProjectData.insert("type", "ProjectData");
	obj_ProjectData.insert("data", this->data_ProjectData.getJsonObject());

	obj_all.insert("ProjectData", obj_ProjectData);
	return obj_all;
}

/*-----------------------------------
		���� - QJsonObject -> ȫ�������������
*/
void S_ProjectManager::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	QJsonObject obj_ProjectData = obj_all.value("ProjectData").toObject();	//��Ŀ��������
	this->data_ProjectData.setJsonObject(obj_ProjectData.value("data").toObject());

	/*----------------��Ŀ·����Ҫ����ˢ��ǰ����·��----------------*/
	this->data_ProjectData.setPath( this->m_storage_fileInfo.absolutePath());
}


/* --------------------------------------------------------------
		��ʷ��¼ - ��ȡ��Ŀ
*/
QList<QFileInfo> S_ProjectManager::getHistoryProjectTank(){
	return this->m_historyProjectTank;
}
/* --------------------------------------------------------------
		��ʷ��¼ - �����Ŀ
*/
void S_ProjectManager::addHistoryProject(QFileInfo info){
	
	// > �Ѵ���ʱ��ǰ��
	if (this->m_historyProjectTank.contains(info)){
		this->m_historyProjectTank.removeOne(info);
		this->m_historyProjectTank.push_front(info);
		return; 
	}

	// > ������ʱ�����
	this->m_historyProjectTank.insert(0, info);
	
	if (this->m_historyProjectTank.count() > this->m_historyProjectMax){
		this->m_historyProjectTank.removeLast();
	}
}
/* --------------------------------------------------------------
		��ʷ��¼ - ȥ����Ŀ
*/
void S_ProjectManager::removeHistoryProject(int index){
	if (index < 0){ return; }
	if (index >= this->m_historyProjectTank.count()){ return; }
	this->m_historyProjectTank.removeAt(index);
}
/* --------------------------------------------------------------
		��ʷ��¼ - ��ȡ��Ŀ¼
*/
QList<QDir> S_ProjectManager::getHistoryDirTank(){
	return this->m_historyDirTank;
}
/* --------------------------------------------------------------
		��ʷ��¼ - ��Ӹ�Ŀ¼
*/
void S_ProjectManager::addHistoryDir(QDir dir){

	// > �Ѵ���ʱ��ǰ��
	if (this->m_historyDirTank.contains(dir)){
		this->m_historyDirTank.removeOne(dir);
		this->m_historyDirTank.push_front(dir);
		return;
	}

	// > ������ʱ�����
	this->m_historyDirTank.insert(0, dir);

	if (this->m_historyDirTank.count() > this->m_historyDirMax){
		this->m_historyDirTank.removeLast();
	}
}
/* --------------------------------------------------------------
		��ʷ��¼ - ȥ����Ŀ¼
*/
void S_ProjectManager::removeHistoryDir(int index){
	if (index < 0){ return; }
	if (index >= this->m_historyDirTank.count()){ return; }
	this->m_historyDirTank.removeAt(index);
}
/* --------------------------------------------------------------
		��ʷ��¼ - �����¼��˽�У�
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
		��ʷ��¼ - ��ȡ��¼��˽�У�
*/
void S_ProjectManager::loadHistory(){
	QStringList project_list = S_IniManager::getInstance()->getConfig("COAS_historyProject").split("|");	//�������߻��֣�
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
		���� - ����Ŀ·���ļ���
*/
bool S_ProjectManager::openDesktopProjectUrl() {
	QString buffer_path = this->data_ProjectData.getProjectRootPath();
	return QDesktopServices::openUrl(QUrl("file:/" + buffer_path + "/"));
}

/* --------------------------------------------------------------
		���� - ����Ŀ·����ָ���ļ�
*/
bool S_ProjectManager::openDesktopProjectFile(QString filename) {
	QString buffer_path = this->data_ProjectData.getProjectRootPath();
	return QDesktopServices::openUrl(QUrl("file:/" + buffer_path + "/" + filename));
}