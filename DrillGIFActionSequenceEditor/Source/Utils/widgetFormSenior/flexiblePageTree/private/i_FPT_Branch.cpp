#include "stdafx.h"
#include "i_FPT_Branch.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		��֦.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��֦�ؼ��ࡣ
-----==========================================================-----
*/

I_FPT_Branch::I_FPT_Branch(C_FPT_Config* config, QTreeWidgetItem *parent) : QTreeWidgetItem(parent)
{
	this->m_config = config;

}
I_FPT_Branch::~I_FPT_Branch(){
}

/*-------------------------------------------------
		�ؼ� - ������������ˢ��
*/
void I_FPT_Branch::refreshItemSelf(){

	this->setText(0, this->getShowingText());		//��ˢ����ʾ�����ƣ�
	this->setToolTip(0, this->getShowingText());
}
/*-------------------------------------------------
		�ؼ� - ��ȡ��ʾ�ı�
*/
QString I_FPT_Branch::getShowingText(){

	// > ID��֧ ��ʾ
	if (this->m_config->is_id_Mode()){ return this->m_config->get_id_PageNameByIndex(this->getBranch_id_Index()); }

	// > ���Ʒ�֧ ��ʾ
	if (this->m_config->is_name_Mode()){ return this->getBranch_name_Showing(); }

	// > �Զ����֧ ��ʾ
	if (this->getBranch_type_Name() != ""){ return this->getBranch_type_Name(); }

	return "δ����";
}