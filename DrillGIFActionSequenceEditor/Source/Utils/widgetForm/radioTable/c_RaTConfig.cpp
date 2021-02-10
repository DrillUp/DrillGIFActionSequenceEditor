#include "stdafx.h"

#include "c_RaTConfig.h"

/*
-----==========================================================-----
		�ࣺ		��ѡ������� ������.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ѡ������� �������ࡣ
					
		�ӹ��ܣ�
					->�ն���
					
-----==========================================================-----
*/
C_RaTConfig::C_RaTConfig(){

	this->rowHeight = 24;
	this->zeroFill = true;
	this->zeroFillCount = 4;
	this->zeroFillChar = '0';

}
C_RaTConfig::~C_RaTConfig(){
}


/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_RaTConfig::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("rowHeight", this->rowHeight);
	obj.insert("zeroFill", this->zeroFill);
	obj.insert("zeroFillCount", this->zeroFillCount);
	obj.insert("zeroFillChar", QString(this->zeroFillChar));

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_RaTConfig::setJsonObject(QJsonObject obj){

	if (obj.value("rowHeight").isUndefined() == false){ this->rowHeight = obj.value("rowHeight").toInt(); }
	if (obj.value("zeroFill").isUndefined() == false){ this->zeroFill = obj.value("zeroFill").toBool(); }
	if (obj.value("zeroFillCount").isUndefined() == false){ this->zeroFillCount = obj.value("zeroFillCount").toInt(); }
	if (obj.value("zeroFillChar").isUndefined() == false){ this->zeroFillChar = obj.value("zeroFillChar").toString().at(0); }

}