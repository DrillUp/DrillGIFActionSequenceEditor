#include "stdafx.h"
#include "C_PluginData.h"

/*
-----==========================================================-----
		�ࣺ		������� ʵ����.cpp
		����ģ�飺	���ģ��
		���ܣ�		�洢�����Ϣ���ݡ�

		��Ҫ���	1.�������Ϊһ��json��ʽ���ݡ�
					
-----==========================================================-----
*/
C_PluginData::C_PluginData(){
	this->name = "";
	this->status = false;
	this->description = "";
	this->parameters = QJsonObject();
}
C_PluginData::~C_PluginData(){
}


/*-------------------------------------------------
		���������
*/
const bool C_PluginData::operator== (const C_PluginData& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_PluginData::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("status", this->status);
	obj.insert("description", this->description);
	obj.insert("parameters", this->parameters);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_PluginData::setJsonObject(QJsonObject obj){
	this->name = obj.value("name").toString();
	this->status = obj.value("status").toBool();
	this->description = obj.value("description").toString();
	this->parameters = obj.value("parameters").toObject();
}
