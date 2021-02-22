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
	
	// > �༭���
	this->m_table = new P_RadioTable(ui.tableWidget);
	QJsonObject obj_config = QJsonObject();
	obj_config.insert("zeroFillCount", 2);
	obj_config.insert("rowHeight", 22);
	this->m_table->setConfigParam(obj_config);	//�̶�����

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

	C_ALEConfig config = C_ALEConfig();
	this->m_p_AnimationListEditor->setConfigParam_ALE(config);

	// > ������
	this->m_p_AnimationListPlayer = new P_AnimationListPlayer();
	this->m_p_AnimationListPlayer->setAnimationListEditor(m_p_AnimationListEditor);
	this->m_p_AnimationListPlayer->setPlayType(QStringList() << "����ѭ��");
	QHBoxLayout* layout = new QHBoxLayout(ui.groupBox_animPlayer);
	ui.groupBox_animPlayer->setLayout(layout);
	layout->setMargin(3);
	layout->addWidget(this->m_p_AnimationListPlayer);

	//-----------------------------------
	//----�¼���
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_StatePart::currentIndexChanged);
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
		�ؼ� - ��ȡ״̬Ԫ����
*/
QStringList P_StatePart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		result.append(this->local_stateDataList.at(i).value("״̬Ԫ����").toString());
	}
	return result;
}

/*-------------------------------------------------
		�ؼ� - ����Ԫ�л�
*/
void P_StatePart::currentIndexChanged(int index){

	// > �ɵ����ݴ洢


	// > �����µ�����
	ui.lineEdit_name->setText(this->getNameList().at(index));
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

	this->m_table->setSource(this->getNameList());
}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_StatePart::putUiToData() {
	

}
