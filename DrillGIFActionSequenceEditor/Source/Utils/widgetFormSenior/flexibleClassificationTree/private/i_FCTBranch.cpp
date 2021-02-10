#include "stdafx.h"
#include "i_FCTBranch.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		树枝.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		树枝控件类。
-----==========================================================-----
*/

I_FCTBranch::I_FCTBranch(QTreeWidgetItem *parent) : QTreeWidgetItem(parent)
{



}
I_FCTBranch::~I_FCTBranch(){
}

/*-------------------------------------------------
		控件 - 根据数据自我刷新
*/
void I_FCTBranch::refreshItemSelf(){

	this->setText(0, this->getShowingText());		//（刷新显示的名称）
	this->setToolTip(0, this->getShowingText());
}
/*-------------------------------------------------
		控件 - 获取显示文本
*/
QString I_FCTBranch::getShowingText(){

	// > 名称分支 显示
	if (this->getBranch_id_Name() != ""){ return this->getBranch_id_Name(); }

	// > 名称分支 显示
	if (this->getBranch_name_Showing() != ""){ return this->getBranch_name_Showing(); }

	// > 自定义分支 显示
	if (this->getBranch_type_Name() != ""){ return this->getBranch_type_Name(); }

	return "未分类";
}