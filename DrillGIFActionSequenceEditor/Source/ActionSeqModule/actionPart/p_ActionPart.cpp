#include "stdafx.h"

#include "p_ActionPart.h"

/*
-----==========================================================-----
		�ࣺ		����Ԫ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩ����Ԫ�༭���ܡ�
		
		Ŀ�꣺		1.

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_ActionPart::P_ActionPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	//-----------------------------------
	//----��ʼ��ui


	//-----------------------------------
	//----�¼���

}

P_ActionPart::~P_ActionPart(){
}


/*-------------------------------------------------
		���� - ��������
*/
void P_ActionPart::setData(QJsonObject actionData) {
	this->local_actionData = actionData;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
QJsonObject P_ActionPart::getData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->local_actionData;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_ActionPart::putDataToUi() {
	

}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_ActionPart::putUiToData() {
	

}
