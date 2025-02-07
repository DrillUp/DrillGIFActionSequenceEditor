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

I_FPT_Branch::I_FPT_Branch(C_FPT_Config* config, QTreeWidgetItem *parent) : QTreeWidgetItem(parent)
{
	this->m_config = config;

}
I_FPT_Branch::~I_FPT_Branch(){
}

/*-------------------------------------------------
		控件 - 根据数据自我刷新
*/
void I_FPT_Branch::refreshItemSelf(){

	this->setText(0, this->getShowingText());		//（刷新显示的名称）
	this->setToolTip(0, this->getShowingText());
}
/*-------------------------------------------------
		控件 - 获取显示文本
*/
QString I_FPT_Branch::getShowingText(){

	// > ID分支 显示
	if (this->m_config->is_id_Mode()){ return this->m_config->get_id_PageNameByIndex(this->getBranch_id_Index()); }

	// > 名称分支 显示
	if (this->m_config->is_name_Mode()){ return this->getBranch_name_Showing(); }

	// > 自定义分支 显示
	if (this->getBranch_type_Name() != ""){ return this->getBranch_type_Name(); }

	return "未分类";
}