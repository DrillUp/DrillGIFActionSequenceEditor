#include "stdafx.h"

#include "c_FCTClassify.h"

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
C_FCTClassify::C_FCTClassify(){
	this->name = "";
	this->description = "";
}
C_FCTClassify::C_FCTClassify(QString name, QString description){
	this->name = name;
	this->description = description;
}
C_FCTClassify::~C_FCTClassify(){
}


/*-------------------------------------------------
		���������
*/
const bool C_FCTClassify::operator== (const C_FCTClassify& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_FCTClassify::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("description", this->description);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_FCTClassify::setJsonObject(QJsonObject obj){

	this->name = obj.value("name").toString();
	this->description = obj.value("description").toString();
}
/*-------------------------------------------------
		���ж�
*/
bool C_FCTClassify::isNull(){
	return this->name == "" ;
}