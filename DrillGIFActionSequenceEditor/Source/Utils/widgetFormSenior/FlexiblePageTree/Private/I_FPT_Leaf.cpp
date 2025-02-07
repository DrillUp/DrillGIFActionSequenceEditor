#include "stdafx.h"
#include "I_FPT_Leaf.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		叶子.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		叶子控件类。
-----==========================================================-----
*/

I_FPT_Leaf::I_FPT_Leaf(C_FPT_Config* config, QTreeWidgetItem *parent) : QTreeWidgetItem(parent)
{
	this->m_config = config;


}
I_FPT_Leaf::~I_FPT_Leaf(){
}

/*-------------------------------------------------
		控件 - 根据数据自我刷新
*/
void I_FPT_Leaf::refreshItemSelf(){

	this->setText(0, this->getShowingText());		//（刷新显示的名称）
	this->setToolTip(0, this->getName());
}
/*-------------------------------------------------
		控件 - 获取显示文本
*/
QString I_FPT_Leaf::getShowingText(){
	int id = this->getId();				//标识
	QString name = this->getName();		//名称

	return this->m_config->get_id_FormatedId(id) + " " + name;
}