#include "stdafx.h"
#include "W_DefaultStateGroupEdit.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		Ĭ��״̬Ԫ���� �༭.cpp
		����ģ�飺	��������ģ��
		���ܣ�		Ĭ��״̬Ԫ���ϵĴ��ڡ�

		ʹ�÷�����
				>�򿪴���
					W_DefaultStateGroupEdit d;
					d.exec();

-----==========================================================-----
*/
W_DefaultStateGroupEdit::W_DefaultStateGroupEdit(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);


	//-----------------------------------
	//----�ؼ���ʼ��
	this->m_p_RadioTable = new P_RadioTable(ui.tableWidget);
	C_RaTConfig rat_config = C_RaTConfig();
	rat_config.showNumber = false;
	rat_config.isMultiSelect = true;
	this->m_p_RadioTable->setConfigParam(rat_config);	//�̶�����

	connect(this->m_p_RadioTable, &P_RadioTable::currentIndexChanged_Multi, this, &W_DefaultStateGroupEdit::selectedChanged);

	//-----------------------------------
	//----ui��ʼ��
	TTool::_chinese_(ui.buttonBox);
}

W_DefaultStateGroupEdit::~W_DefaultStateGroupEdit(){
}


/*-------------------------------------------------
		�ؼ� - ѡ�б仯
*/
void W_DefaultStateGroupEdit::selectedChanged(QList<int> index_list){
	
	QStringList data_list = this->m_p_RadioTable->getTextByIndex(index_list);
	ui.label->setText("ֵ��[" + data_list.join(",") + "]");
}


/*-------------------------------------------------
		���� - ��������
*/
void W_DefaultStateGroupEdit::setData(QStringList data, QStringList selected){
	this->m_p_RadioTable->setSource(data);
	this->m_p_RadioTable->selectText_Multi(selected);
}
/*-------------------------------------------------
		���� - ��ȡ����
*/
QStringList W_DefaultStateGroupEdit::getData(){
	return this->m_p_RadioTable->getSelectedText_Multi();
}