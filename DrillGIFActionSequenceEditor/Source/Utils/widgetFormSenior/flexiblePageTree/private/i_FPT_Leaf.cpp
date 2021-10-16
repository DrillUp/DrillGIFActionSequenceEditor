#include "stdafx.h"
#include "i_FPT_Leaf.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		Ҷ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		Ҷ�ӿؼ��ࡣ
-----==========================================================-----
*/

I_FPT_Leaf::I_FPT_Leaf(C_FPT_Config* config, QTreeWidgetItem *parent) : QTreeWidgetItem(parent)
{
	this->m_config = config;


}
I_FPT_Leaf::~I_FPT_Leaf(){
}

/*-------------------------------------------------
		�ؼ� - ������������ˢ��
*/
void I_FPT_Leaf::refreshItemSelf(){

	this->setText(0, this->getShowingText());		//��ˢ����ʾ�����ƣ�
	this->setToolTip(0, this->getName());
}
/*-------------------------------------------------
		�ؼ� - ��ȡ��ʾ�ı�
*/
QString I_FPT_Leaf::getShowingText(){
	int id = this->getId();				//��ʶ
	QString name = this->getName();		//����

	return this->m_config->get_id_FormatedId(id) + " " + name;
}