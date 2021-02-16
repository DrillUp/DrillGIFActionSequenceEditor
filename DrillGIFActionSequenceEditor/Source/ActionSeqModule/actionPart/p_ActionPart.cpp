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
	this->m_table = new P_RadioTable(ui.tableWidget);
	QJsonObject obj_config = QJsonObject();
	obj_config.insert("zeroFillCount", 2);
	obj_config.insert("rowHeight", 22);
	this->m_table->setConfigParam(obj_config);	//�̶�����

	//-----------------------------------
	//----�¼���
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_ActionPart::currentIndexChanged);

}

P_ActionPart::~P_ActionPart(){
}

/*-------------------------------------------------
		�ؼ� - ��ȡ����Ԫ����
*/
QStringList P_ActionPart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		result.append(this->local_actionDataList.at(i).value("����Ԫ����").toString());
	}
	return result;
}

/*-------------------------------------------------
		�ؼ� - ����Ԫ�л�
*/
void P_ActionPart::currentIndexChanged(int index){

	// > �ɵ����ݴ洢


	// > �����µ�����
	ui.lineEdit_name->setText(this->getNameList().at(index));
}


/*-------------------------------------------------
		���� - ��������
*/
void P_ActionPart::setData(QList<QJsonObject> actionData) {
	this->local_actionDataList = actionData;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
QList<QJsonObject> P_ActionPart::getData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->local_actionDataList;
}
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_ActionPart::putDataToUi() {
	
	this->m_table->setSource(this->getNameList());

}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_ActionPart::putUiToData() {
	

}
