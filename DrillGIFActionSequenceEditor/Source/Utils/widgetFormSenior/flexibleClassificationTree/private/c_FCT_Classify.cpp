#include "stdafx.h"
#include "c_FCT_Classify.h"

/*
-----==========================================================-----
		�ࣺ		���� ������.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���������� ���� �����ࡣ
					
		�ӹ��ܣ�
					->�ն���
					
-----==========================================================-----
*/
C_FCT_Classify::C_FCT_Classify(){
	this->name = "";
	this->description = "";
}
C_FCT_Classify::~C_FCT_Classify(){
}


/*-------------------------------------------------
		���������
*/
QString C_FCT_Classify::getName(){
	return this->name;
}
void C_FCT_Classify::setName(QString name){
	this->name = name;
}
QString C_FCT_Classify::getDescription(){
	return this->description;
}
void C_FCT_Classify::setDescription(QString description){
	this->description = description;
}


/*-------------------------------------------------
		�Զ������� - ��ȡ����
*/
QJsonObject C_FCT_Classify::getCustomData(){
	return this->data;
}
/*-------------------------------------------------
		�Զ������� - ��ȡģ��
*/
C_FastClass C_FCT_Classify::getCustomClass(){
	return C_FastClass();
}
/*-------------------------------------------------
		�Զ������� - ��ȡ����
*/
QString C_FCT_Classify::getCustomClassName(){
	return "����";
}
/*-------------------------------------------------
		�Զ������� - ��ȡ��������
*/
QString C_FCT_Classify::getCustomObjectName(){
	if (this->name == ""){ return "δ����"; }
	return this->name;
}


/*-------------------------------------------------
		ģ�帴��
*/
void C_FCT_Classify::copyBaseDataFrom(C_FCT_Classify* base){
	this->description = base->description;
	this->data = base->data;
}
/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_FCT_Classify::getJsonObject(){
	QJsonObject obj = QJsonObject();

	// > ��������
	obj.insert("name", this->name);
	obj.insert("description", this->description);

	// > �Զ�������
	obj.insert("data", this->data);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_FCT_Classify::setJsonObject(QJsonObject obj){

	// > ��������
	this->name = obj.value("name").toString();
	this->description = obj.value("description").toString();

	// > �Զ�������
	this->data = obj.value("data").toObject();
}
/*-------------------------------------------------
		���ж�
*/
bool C_FCT_Classify::isNull(){
	return this->name == "" ;
}