#include "stdafx.h"
#include "c_ALEConfig.h"

/*
-----==========================================================-----
		�ࣺ		����֡ �����ࣨ���ã�.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		����֡ �������ࡣ
					
		˵����		��������̳���C_PiSConfig��
-----==========================================================-----
*/
C_ALEConfig::C_ALEConfig() : C_PiSConfig() {
	this->sizeMode = "��";
	this->isUseFileSys = false;
}
C_ALEConfig::~C_ALEConfig(){
}


/*-------------------------------------------------
		���óߴ�ģʽ
*/
void C_ALEConfig::setSizeMode(QString sizeMode){
	if (sizeMode == ""){ return; }
	if (this->sizeMode == sizeMode){ return; }
	this->sizeMode = sizeMode;

	if (this->sizeMode == "��"){
		this->height = 200;
	}
	if (this->sizeMode == "��"){
		this->height = 140;
	}
	if (this->sizeMode == "С"){
		this->height = 100;
	}
}

/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_ALEConfig::getJsonObject(){
	QJsonObject obj = C_PiSConfig::getJsonObject();
	obj.insert("sizeMode", this->sizeMode);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_ALEConfig::setJsonObject(QJsonObject obj){
	C_PiSConfig::setJsonObject(obj);
	if (obj.value("sizeMode").isUndefined() == false){ this->setSizeMode(obj.value("sizeModule").toString()); }
}