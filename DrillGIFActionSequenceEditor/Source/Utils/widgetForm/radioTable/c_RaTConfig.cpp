#include "stdafx.h"

#include "c_RaTConfig.h"

/*
-----==========================================================-----
		�ࣺ		������� ������.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ѡ������� �������ࡣ
					
		�ӹ��ܣ�
					->�ն���
					
-----==========================================================-----
*/
C_RaTConfig::C_RaTConfig(){

	this->showNumber = true;			//��ʾ���
	this->zeroFill = true;				//�����
	this->zeroFillCount = 4;			//���λ��
	this->zeroFillChar = '0';			//����ַ�

	this->rowHeight = 24;				//�и�
	this->isMultiSelect = false;		//��ѡ����
}
C_RaTConfig::~C_RaTConfig(){
}


/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_RaTConfig::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("showNumber", this->showNumber);
	obj.insert("zeroFill", this->zeroFill);
	obj.insert("zeroFillCount", this->zeroFillCount);
	obj.insert("zeroFillChar", QString(this->zeroFillChar));

	obj.insert("rowHeight", this->rowHeight);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_RaTConfig::setJsonObject(QJsonObject obj){

	if (obj.value("showNumber").isUndefined() == false){ this->showNumber = obj.value("showNumber").toBool(); }
	if (obj.value("zeroFill").isUndefined() == false){ this->zeroFill = obj.value("zeroFill").toBool(); }
	if (obj.value("zeroFillCount").isUndefined() == false){ this->zeroFillCount = obj.value("zeroFillCount").toInt(); }
	if (obj.value("zeroFillChar").isUndefined() == false){ this->zeroFillChar = obj.value("zeroFillChar").toString().at(0); }

	if (obj.value("rowHeight").isUndefined() == false){ this->rowHeight = obj.value("rowHeight").toInt(); }
}