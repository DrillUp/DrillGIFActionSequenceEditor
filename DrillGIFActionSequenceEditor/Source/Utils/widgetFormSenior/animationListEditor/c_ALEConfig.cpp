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
	this->m_sizeMode = "��";
	this->m_defaultInterval = 4;
	this->m_isUseFileSys = false;

	this->m_isMultiSelect = true;		//���̶�Ϊ��ѡģʽ��
}
C_ALEConfig::~C_ALEConfig(){
}


/*-------------------------------------------------
		���óߴ�ģʽ
*/
void C_ALEConfig::setSizeMode(QString sizeMode){
	if (sizeMode == ""){ return; }
	if (this->m_sizeMode == sizeMode){ return; }
	this->m_sizeMode = sizeMode;

	if (this->m_sizeMode == "��"){
		this->m_height = 200;
	}
	if (this->m_sizeMode == "��"){
		this->m_height = 140;
	}
	if (this->m_sizeMode == "С"){
		this->m_height = 100;
	}
}
QString C_ALEConfig::getSizeMode(){
	return this->m_sizeMode;
}

/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_ALEConfig::getJsonObject(){
	QJsonObject obj = C_PiSConfig::getJsonObject();
	obj.insert("m_sizeMode", this->m_sizeMode);
	obj.insert("m_defaultInterval", this->m_defaultInterval);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_ALEConfig::setJsonObject(QJsonObject obj){
	C_PiSConfig::setJsonObject(obj);
	this->m_isMultiSelect = true;		//���̶�Ϊ��ѡģʽ��

	if (obj.value("m_sizeMode").isUndefined() == false){ this->setSizeMode(obj.value("m_sizeModule").toString()); }
	if (obj.value("m_defaultInterval").isUndefined() == false){ this->m_defaultInterval = obj.value("m_defaultInterval").toInt(); }

}