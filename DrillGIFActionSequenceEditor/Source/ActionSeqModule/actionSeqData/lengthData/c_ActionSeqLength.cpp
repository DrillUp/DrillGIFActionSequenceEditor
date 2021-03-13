#include "stdafx.h"
#include "c_ActionSeqLength.h"

/*
-----==========================================================-----
		�ࣺ		���ֵ ������.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���ֵ �������ࡣ
					
		�ӹ��ܣ�
					->�ն���
					
-----==========================================================-----
*/
C_ActionSeqLength::C_ActionSeqLength(){

	this->realLen_actionSeq = 0;			//���� - ��������
	this->realLen_action = 0;				//���� - ����Ԫ
	this->realLen_state = 0;				//���� - ״̬Ԫ
}
C_ActionSeqLength::~C_ActionSeqLength(){
}


/*-------------------------------------------------
		���ж�
*/
bool C_ActionSeqLength::isNull(){
	if (this->realLen_actionSeq < 4){ return true; }
	if (this->realLen_action < 4){ return true; }
	if (this->realLen_state < 4){ return true; }
	return false;
}
/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_ActionSeqLength::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("realLen_actionSeq", this->realLen_actionSeq);
	obj.insert("realLen_action", this->realLen_action);
	obj.insert("realLen_state", this->realLen_state);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_ActionSeqLength::setJsonObject(QJsonObject obj){

	if (obj.value("realLen_actionSeq").isUndefined() == false){ this->realLen_actionSeq = obj.value("realLen_actionSeq").toInt(); }
	if (obj.value("realLen_action").isUndefined() == false){ this->realLen_action = obj.value("realLen_action").toInt(); }
	if (obj.value("realLen_state").isUndefined() == false){ this->realLen_state = obj.value("realLen_state").toInt(); }
	
	// > Ĭ��ֵ
	if (this->isNull()){
		this->realLen_actionSeq = 40;
		this->realLen_action = 10;
		this->realLen_state = 10;
	}
}