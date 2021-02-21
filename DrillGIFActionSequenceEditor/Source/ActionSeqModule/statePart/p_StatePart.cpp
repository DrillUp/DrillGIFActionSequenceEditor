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

	// > ����֡
	C_ALEData data = C_ALEData();
	data.setId(10);
	data.setSource("F:/rpg mv��/mog�������ȫ����(Drill_up)v2.41/�������ʾ��/img/enemies/", QList<QString>()
		<< "С����˿001"
		<< "С����˿002"
		<< "С����˿003"
		<< "С����˿004"
		<< "С����˿005"
		<< "С����˿006"
		<< "С����˿004"
		<< "С����˿006");
	data.setInterval(3, QList<int>() << 6 << 5 << 4 << 3 << 2 << 1);

	this->m_p_AnimationListEditor = new P_AnimationListEditor(ui.listWidget);
	this->m_p_AnimationListEditor->setSource(data);

	C_PiSConfig config = C_PiSConfig();
	config.isMultiSelect = true;
	this->m_p_AnimationListEditor->setConfigParam(config);

	//-----------------------------------
	//----�¼���
	connect(this->m_p_AnimationListEditor, &P_AnimationListEditor::selectedIndexChanged_Multi, this, &P_StatePart::tableChanged_Multi);

}

P_StatePart::~P_StatePart(){
}

/*-------------------------------------------------
		����֡ - ѡ��仯
*/
void P_StatePart::tableChanged_Multi(QList<int> index_list){
	QString text = "";
	for (int i = 0; i < index_list.count(); i++){
		text += QString::number( index_list.at(i) + 1 )+ "/";
	}
	//ui.label->setText("��ѡ���ˣ�" + text);
}

/*-------------------------------------------------
		���� - ��������
*/
void P_StatePart::setData(QList<QJsonObject> state) {
	this->local_stateDataList = state;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
QList<QJsonObject> P_StatePart::getData(){
	this->putUiToData();

	// > У��
	//...
	
	return this->local_stateDataList;
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
