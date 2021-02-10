#include "stdafx.h"
#include "c_ObjectSelectorConfig.h"


/*
-----==========================================================-----
		�ࣺ		����ѡ�� ����.cpp
		����ģ�飺	����ģ��
		���ܣ�		����ѡ�����Ĵ������ò�����
-----==========================================================-----
*/
C_ObjectSelectorConfig::C_ObjectSelectorConfig(){

	this->rowHeight = 24;
	this->zeroFill = true;
	this->zeroFillCount = 4;
	this->zeroFillChar = '0';

	this->pagePerNum = 30;
	this->showHavingDataInitials = false;

	this->excludeFirstData = false;
	this->addNullValue = false;
}
C_ObjectSelectorConfig::~C_ObjectSelectorConfig(){
}


/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_ObjectSelectorConfig::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("rowHeight", this->rowHeight);
	obj.insert("zeroFill", this->zeroFill);
	obj.insert("zeroFillCount", this->zeroFillCount);
	obj.insert("zeroFillChar", QString(this->zeroFillChar));

	obj.insert("pagePerNum", this->pagePerNum);
	obj.insert("showHavingDataInitials", this->showHavingDataInitials);

	obj.insert("excludeFirstData", this->excludeFirstData);
	obj.insert("addNullValue", this->addNullValue);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_ObjectSelectorConfig::setJsonObject(QJsonObject obj){

	if (obj.value("rowHeight").isUndefined() == false){ this->rowHeight = obj.value("rowHeight").toInt(); }
	if (obj.value("zeroFill").isUndefined() == false){ this->zeroFill = obj.value("zeroFill").toBool(); }
	if (obj.value("zeroFillCount").isUndefined() == false){ this->zeroFillCount = obj.value("zeroFillCount").toInt(); }
	if (obj.value("zeroFillChar").isUndefined() == false){ this->zeroFillChar = obj.value("zeroFillChar").toString().at(0); }
	
	if (obj.value("pagePerNum").isUndefined() == false){ this->pagePerNum = obj.value("pagePerNum").toInt(); }
	if (obj.value("showHavingDataInitials").isUndefined() == false){ this->showHavingDataInitials = obj.value("showHavingDataInitials").toBool(); }
	
	if (obj.value("excludeFirstData").isUndefined() == false){ this->excludeFirstData = obj.value("excludeFirstData").toBool(); }
	if (obj.value("addNullValue").isUndefined() == false){ this->addNullValue = obj.value("addNullValue").toBool(); }
}