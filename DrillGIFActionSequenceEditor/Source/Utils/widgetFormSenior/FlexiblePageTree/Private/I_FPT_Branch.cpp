#include "stdafx.h"
#include "I_FPT_Branch.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		树枝.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		树枝控件类。
-----==========================================================-----
*/

I_FPT_Branch::I_FPT_Branch(C_FPT_Config* config, QTreeWidgetItem *parent) : QTreeWidgetItem(parent){
	this->m_config = config;
}
I_FPT_Branch::~I_FPT_Branch(){
}

/*-------------------------------------------------
		控件 - 刷新数据
*/
void I_FPT_Branch::refreshItemSelf(){
	this->setText(0, this->getShowingText());		//（刷新显示的名称）
	this->setToolTip(0, this->getToolTipText());
}
/*-------------------------------------------------
		控件 - 获取显示文本
*/
QString I_FPT_Branch::getShowingText(){

	// > ID分支 显示
	if (this->m_config->isSortMode_id()){ return this->m_config->get_id_PageNameByIndex(this->getBranch_id_Index()); }

	// > 名称分支 显示
	if (this->m_config->isSortMode_name()){ return this->getBranch_name_Showing(); }

	// > 种类分支 显示
	if (this->getBranch_type_Name() != ""){ return this->getBranch_type_Name(); }

	return "未分类";
}
/*-------------------------------------------------
		控件 - 获取悬浮文本
*/
QString I_FPT_Branch::getToolTipText(){

	// > ID分支 显示
	if (this->m_config->isSortMode_id()){ return this->m_config->get_id_PageNameByIndex(this->getBranch_id_Index()); }

	// > 名称分支 显示
	if (this->m_config->isSortMode_name()){ return this->getBranch_name_Showing(); }

	// > 种类分支 显示
	if (this->getBranch_type_Description() != ""){ return this->getBranch_type_Description(); }

	return "";
}