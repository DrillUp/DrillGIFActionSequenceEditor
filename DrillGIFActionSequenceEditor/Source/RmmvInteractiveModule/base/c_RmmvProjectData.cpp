#include "stdafx.h"
#include "C_RmmvProjectData.h"

/*
-----==========================================================-----
		�ࣺ		rmmv�������� ʵ����.cpp
		�汾��		v1.01	�����಻�ٸ��¡�
		����ģ�飺	����ģ��
		���ܣ�		���̻�����Ϣ���ݡ�
-----==========================================================-----
*/
C_RmmvProjectData::C_RmmvProjectData(){
	this->name = "";			//������
	this->version = "";			//���̰汾
	this->path = "";			//���̸�·��

	this->optional_backup = true;
}
C_RmmvProjectData::~C_RmmvProjectData(){
}


/*-------------------------------------------------
		���� - ���������ļ�
*/
void C_RmmvProjectData::initFromFile(QString full_filePath){
	QString dir_path = QFileInfo(full_filePath).absolutePath();		//��ȡ��Ŀ¼

	// > ��ȡ�����ļ�
	QFile project_file(full_filePath);
	if (!project_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "����", "δ�ҵ�Game.rpgproject�ļ���", QMessageBox::Yes);
		return;
	}
	QString version = project_file.readAll();
	project_file.close();

	// > ��ȡindex�ļ�
	QFile index_file(dir_path + "/index.html");
	if (!index_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "����", "δ�ҵ�index.html�ļ���", QMessageBox::Yes);
		return;
	}
	QString index_context = index_file.readAll();
	index_file.close();

	// > ����������
	int i_a = index_context.indexOf("<title>") + 7;
	int i_b = index_context.indexOf("</title>", i_a);
	this->name = index_context.mid(i_a, i_b - i_a);
	this->version = version;
	this->path = dir_path;
}
/*-------------------------------------------------
		���� - ��ȡ��������
*/
QString C_RmmvProjectData::getName(){
	return this->name;
}
/*-------------------------------------------------
		���� - ��ȡ���̰汾
*/
QString C_RmmvProjectData::getVersion(){
	return this->version;
}
/*-------------------------------------------------
		���� - ��ȡ���̸�·��
*/
QString C_RmmvProjectData::getRootPath(){
	return this->path;
}
/*-------------------------------------------------
		���� - �жϹ����Ƿ����
*/
bool C_RmmvProjectData::isProjectExist(){
	if (QFileInfo(this->path).exists() == false){ return false; }
	if (QFileInfo(this->path + "/index.html").exists() == false){ return false; }
	return true;
}

/*-------------------------------------------------
		���ж�
*/
bool C_RmmvProjectData::isNull(){
	return this->name == "" || this->path == "";
}
/*-------------------------------------------------
		���������
*/
const bool C_RmmvProjectData::operator== (const C_RmmvProjectData& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_RmmvProjectData::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("version", this->version); 
	obj.insert("path", this->path);
	obj.insert("optional_backup", this->optional_backup);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_RmmvProjectData::setJsonObject(QJsonObject obj){
	this->name = obj.value("name").toString();
	this->version = obj.value("version").toString();
	this->path = obj.value("path").toString();
	this->optional_backup = obj.value("optional_backup").toBool();
}
