#include "stdafx.h"
#include "I_FCTLeaf.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		Ҷ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		Ҷ�ӿؼ��ࡣ
-----==========================================================-----
*/

I_FCTLeaf::I_FCTLeaf(C_FCTConfig* config, QTreeWidgetItem *parent) : QTreeWidgetItem(parent)
{
	this->m_config = config;


}
I_FCTLeaf::~I_FCTLeaf(){
}

/*-------------------------------------------------
		�ؼ� - ������������ˢ��
*/
void I_FCTLeaf::refreshItemSelf(){

	this->setText(0, this->getShowingText());		//��ˢ����ʾ�����ƣ�
	this->setToolTip(0, this->getName());
}
/*-------------------------------------------------
		�ؼ� - ��ȡ��ʾ�ı�
*/
QString I_FCTLeaf::getShowingText(){
	int id = this->getId();				//��ʶ
	QString name = this->getName();		//����

	QString result = "";
	if (this->m_config->zeroFill){
		result = TTool::_zeroFill_(id, this->m_config->zeroFillCount, QLatin1Char(this->m_config->zeroFillChar.toLatin1())) + " " + name;
	}else{
		result = QString::number(id) + " " + name;
	}
	return result;
}