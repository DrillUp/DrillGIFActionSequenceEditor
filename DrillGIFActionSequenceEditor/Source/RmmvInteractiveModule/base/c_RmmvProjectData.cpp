#include "stdafx.h"
#include "C_RmmvProjectData.h"

/*
-----==========================================================-----
		�ࣺ		rmmv�������� ʵ����.cpp
		�汾��		v1.00
		����ģ�飺	����ģ��
		���ܣ�		���̻�����Ϣ���ݡ�
-----==========================================================-----
*/
C_RmmvProjectData::C_RmmvProjectData(){
	this->name = "";			//������
	this->version = "";			//���̰汾
	this->path = "";			//���̸�·��
}
C_RmmvProjectData::~C_RmmvProjectData(){
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

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_RmmvProjectData::setJsonObject(QJsonObject obj){
	this->name = obj.value("name").toString();
	this->version = obj.value("version").toString();
	this->path = obj.value("path").toString();
}
