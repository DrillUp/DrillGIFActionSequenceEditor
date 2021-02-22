#include "stdafx.h"
#include "p_PlayingPart.h"

#include "Source/Utils/manager/GIFManager/s_GIFManager.h"

/*
-----==========================================================-----
		�ࣺ		��ӳ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ò����ṩ����Ԫ�༭���ܡ�
		
		Ŀ�꣺		1.

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_PlayingPart::P_PlayingPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	// > �༭���
	this->m_table_action = new P_RadioTable(ui.tableWidget_action);
	this->m_table_state = new P_RadioTable(ui.tableWidget_state);
	QJsonObject obj_config = QJsonObject();
	obj_config.insert("zeroFillCount", 2);
	obj_config.insert("rowHeight", 22);
	this->m_table_action->setConfigParam(obj_config);	//�̶�����
	this->m_table_state->setConfigParam(obj_config);	

	//-----------------------------------
	//----��ʼ��ui


	//-----------------------------------
	//----�¼���


}

P_PlayingPart::~P_PlayingPart(){
}

/*-------------------------------------------------
		��ӳ�� - ��ʼ����
*/
void P_PlayingPart::openPlay() {
	//S_GIFManager::getInstance()->dismantlingGIF_testApi();
	
}

/*-------------------------------------------------
		�ؼ� - ��ȡ״̬Ԫ����
*/
QStringList P_PlayingPart::getStateNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		result.append(this->local_stateDataList.at(i).value("״̬Ԫ����").toString());
	}
	return result;
}
/*-------------------------------------------------
		�ؼ� - ��ȡ����Ԫ����
*/
QStringList P_PlayingPart::getActionNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		result.append(this->local_actionDataList.at(i).value("����Ԫ����").toString());
	}
	return result;
}

/*-------------------------------------------------
		���� - ��������
*/
void P_PlayingPart::setSource(QList<QJsonObject> stateDataList, QList<QJsonObject> actionDataList){
	this->local_stateDataList = stateDataList;
	this->local_actionDataList = actionDataList;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ��������
*/
void P_PlayingPart::setDefaultStateData(QStringList defaultStateList) {
	this->local_defaultStateList = defaultStateList;
	ui.label_defaultState->setText(this->local_defaultStateList.join("\n"));
}
/*-------------------------------------------------
		���� - ȡ������
*/
QStringList P_PlayingPart::getDefaultStateData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->local_defaultStateList;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_PlayingPart::putDataToUi() {

	this->m_table_action->setSource(this->getActionNameList());
	this->m_table_state->setSource(this->getStateNameList());
}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_PlayingPart::putUiToData() {
	

}
