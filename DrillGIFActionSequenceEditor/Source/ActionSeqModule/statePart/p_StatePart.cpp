#include "stdafx.h"

#include "p_StatePart.h"

/*
-----==========================================================-----
		�ࣺ		״̬Ԫ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩ״̬Ԫ�༭���ܡ�
		
		Ŀ�꣺		1.

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_StatePart::P_StatePart(QWidget *parent)
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

P_StatePart::~P_StatePart(){
}


/*-------------------------------------------------
		���� - ��������
*/
void P_StatePart::setData(QJsonObject state) {
	this->local_state = state;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
QJsonObject P_StatePart::getData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->local_state;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_StatePart::putDataToUi() {
	

}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_StatePart::putUiToData() {
	

}
