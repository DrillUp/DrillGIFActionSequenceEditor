#include "stdafx.h"

#include "p_ActionSeqPart.h"

/*
-----==========================================================-----
		�ࣺ		�������п�.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩ�������з�ӳ�����ܣ���Ƕ��״̬Ԫ������Ԫ�Ŀ顣
		
		Ŀ�꣺		1.

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_ActionSeqPart::P_ActionSeqPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	//-----------------------------------
	//----��ʼ��ui
	this->m_actionPart = new P_ActionPart(parent);
	ui.verticalLayout_action->addWidget(this->m_actionPart);

	this->m_statePart = new P_StatePart(parent);
	ui.verticalLayout_state->addWidget(this->m_statePart);
	

	//-----------------------------------
	//----�¼���

}

P_ActionSeqPart::~P_ActionSeqPart(){
}


/*-------------------------------------------------
		���� - ��������
*/
void P_ActionSeqPart::setData(QJsonObject actionSeq) {
	this->local_actionSeq = actionSeq;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
QJsonObject P_ActionSeqPart::getData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->local_actionSeq;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_ActionSeqPart::putDataToUi() {
	

}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_ActionSeqPart::putUiToData() {
	

}
