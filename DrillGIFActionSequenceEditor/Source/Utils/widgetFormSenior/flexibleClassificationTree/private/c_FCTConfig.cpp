#include "stdafx.h"

#include "c_FCTConfig.h"

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
C_FCTConfig::C_FCTConfig(){

	this->rowHeight = 24;
	this->zeroFill = true;
	this->zeroFillCount = 4;
	this->zeroFillChar = '0';

	this->pagePerNum = 30;
	this->sortType = "";
}
C_FCTConfig::~C_FCTConfig(){
}


/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_FCTConfig::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("rowHeight", this->rowHeight);
	obj.insert("zeroFill", this->zeroFill);
	obj.insert("zeroFillCount", this->zeroFillCount);
	obj.insert("zeroFillChar", QString(this->zeroFillChar));

	obj.insert("pagePerNum", this->pagePerNum);
	obj.insert("sortType", this->sortType);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_FCTConfig::setJsonObject(QJsonObject obj){

	if (obj.value("rowHeight").isUndefined() == false){ this->rowHeight = obj.value("rowHeight").toInt(); }
	if (obj.value("zeroFill").isUndefined() == false){ this->zeroFill = obj.value("zeroFill").toBool(); }
	if (obj.value("zeroFillCount").isUndefined() == false){ this->zeroFillCount = obj.value("zeroFillCount").toInt(); }
	if (obj.value("zeroFillChar").isUndefined() == false){ this->zeroFillChar = obj.value("zeroFillChar").toString().at(0); }

	if (obj.value("pagePerNum").isUndefined() == false){ this->pagePerNum = obj.value("pagePerNum").toInt(); }
	if (obj.value("sortType").isUndefined() == false){ this->sortType = obj.value("sortType").toString(); }
}