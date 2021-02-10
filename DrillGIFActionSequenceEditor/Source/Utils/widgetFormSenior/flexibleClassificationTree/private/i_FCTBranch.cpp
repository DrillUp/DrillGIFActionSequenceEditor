#include "stdafx.h"
#include "i_FCTBranch.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		��֦.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��֦�ؼ��ࡣ
-----==========================================================-----
*/

I_FCTBranch::I_FCTBranch(QTreeWidgetItem *parent) : QTreeWidgetItem(parent)
{



}
I_FCTBranch::~I_FCTBranch(){
}

/*-------------------------------------------------
		�ؼ� - ������������ˢ��
*/
void I_FCTBranch::refreshItemSelf(){

	this->setText(0, this->getShowingText());		//��ˢ����ʾ�����ƣ�
	this->setToolTip(0, this->getShowingText());
}
/*-------------------------------------------------
		�ؼ� - ��ȡ��ʾ�ı�
*/
QString I_FCTBranch::getShowingText(){

	// > ���Ʒ�֧ ��ʾ
	if (this->getBranch_id_Name() != ""){ return this->getBranch_id_Name(); }

	// > ���Ʒ�֧ ��ʾ
	if (this->getBranch_name_Showing() != ""){ return this->getBranch_name_Showing(); }

	// > �Զ����֧ ��ʾ
	if (this->getBranch_type_Name() != ""){ return this->getBranch_type_Name(); }

	return "δ����";
}