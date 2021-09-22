#include "stdafx.h"
#include "C_PluginData.h"

/*
-----==========================================================-----
		�ࣺ		������� ʵ����.cpp
		���ߣ�		drill_up
		����ģ�飺	���ģ��
		���ܣ�		�洢�����Ϣ���࣬��Ӧplugin.js��ÿ��������������ݡ�

		˵����		1.���ڵײ����QJsonDocument����д��˳��һ�����ҡ�
					2.parameters��ֵ������ �ַ���object ��
-----==========================================================-----
*/
C_PluginData::C_PluginData(){
	this->name = "";					//�洢���� - �����
	this->status = false;				//�洢���� - ״̬
	this->description = "";				//�洢���� - �������
	this->parameters = QJsonObject();	//�洢���� - ����
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
