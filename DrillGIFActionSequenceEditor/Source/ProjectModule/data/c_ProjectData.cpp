#include "stdafx.h"
#include "c_ProjectData.h"

/*
-----==========================================================-----
		�ࣺ		��Ŀ���� ʵ����.cpp
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		�洢��Ŀ�Ļ������ݵ������ࡣ
-----==========================================================-----
*/
C_ProjectData::C_ProjectData(){
	this->name = "";														//��Ŀ��
	this->path = QCoreApplication::applicationDirPath() + "/workspace/";	//��Ŀ·��������Ŀ�� D:/aaa/vvv/��Ŀ��/��

	this->note = "";									//��Ŀ�ʼǣ���ע��
	this->createDate = QDateTime::currentDateTime();	//����ʱ��
	this->lastSaveDate = QDateTime::currentDateTime();	//��һ���޸�ʱ��
}

C_ProjectData::~C_ProjectData(){
}

QString C_ProjectData::softname = "GIF�������б༭��";	//�����
QString C_ProjectData::version = "v1.12";				//����汾
QString C_ProjectData::version_serial = "";				//����汾��

/*-------------------------------------------------
		���ж�
*/
bool C_ProjectData::isNull(){
	return this->name == "";
}
/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_ProjectData::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("url", this->path);
	obj.insert("note", this->note);
	obj.insert("createDate", this->createDate.toString("yyyy/MM/dd hh:mm:ss"));
	obj.insert("lastSaveDate", this->lastSaveDate.toString("yyyy/MM/dd hh:mm:ss"));
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_ProjectData::setJsonObject(QJsonObject obj){

	this->name = obj.value("name").toString();
	this->path = obj.value("url").toString();
	if (obj.value("note").isUndefined() == false){ this->note = obj.value("note").toString(); }
	if (obj.value("createDate").isUndefined() == false){ this->createDate = QDateTime::fromString( obj.value("createDate").toString(), "yyyy/MM/dd hh:mm:ss"); }
	if (obj.value("lastSaveDate").isUndefined() == false){ this->lastSaveDate = QDateTime::fromString(obj.value("lastSaveDate").toString(), "yyyy/MM/dd hh:mm:ss"); }

}
/*-------------------------------------------------
		��ȡ��Ŀȫ��
*/
QString C_ProjectData::getSoftname(){
	//return this->softname + "(" + this->version + "." + this->version_serial + ")";
	return this->softname + "(" + this->version + ")";
}

/*-------------------------------------------------
		��ȡ��Ŀ��
*/
QString C_ProjectData::getName() {
	return this->name;
}
/*-------------------------------------------------
		��ȡ��Ŀ��׺
*/
QString C_ProjectData::getSuffix() {
	return PROJECT_SUFFIX;
}
/*-------------------------------------------------
		������Ŀ��
*/
void C_ProjectData::setName(QString name) {
	this->name = name;
}
/*-------------------------------------------------
		������Ŀ·��
*/
void C_ProjectData::setPath(QString path) {
	this->path = path;
}
/*-------------------------------------------------
		��ȡ��Ŀ��Ŀ¼��D:/aaa/vvv/��
*/
QString C_ProjectData::getParentPath(){
	if (this->name == ""){
		return this->path;
	}
	else{
		if (this->path.at(this->path.length()-1) == '/'){
			int pos = this->path.mid(0, this->path.length() - 1).lastIndexOf("/");
			return this->path.mid(0, pos);
		}
		else{
			int pos = this->path.lastIndexOf("/");
			return this->path.mid(0, pos);
		}
	}
}
/*-------------------------------------------------
		��ȡ��Ŀ·����D:/aaa/vvv/��Ŀ��/��
*/
QString C_ProjectData::getProjectRootPath() {
	QString root_path = this->path;
	if (root_path.at(root_path.size() - 1) != '/' ){ root_path += "/"; }		//��б��
	return root_path;
}
/*-------------------------------------------------
		��ȡ��Ŀ�ļ��У�D:/aaa/vvv/��Ŀ��/��Ŀ��Files/��
*/
QString C_ProjectData::getProjectFilePath() {
	QString file_path = this->getProjectRootPath() + this->name + "Files/";
	QDir temp_dir;
	temp_dir.mkpath(file_path);
	return file_path;
}
/*-------------------------------------------------
		��ȡ��Ŀ�ļ���D:/aaa/vvv/��Ŀ��/��Ŀ��.xxx��
*/
QString C_ProjectData::getProjectFile() {
	return this->getProjectRootPath() + this->name + "." + PROJECT_SUFFIX;
}

