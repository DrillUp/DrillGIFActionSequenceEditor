#include "stdafx.h"
#include "I_FCTLeaf.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		叶子.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		叶子控件类。
-----==========================================================-----
*/

I_FCTLeaf::I_FCTLeaf(C_FCTConfig* config, QTreeWidgetItem *parent) : QTreeWidgetItem(parent)
{
	this->m_config = config;


}
I_FCTLeaf::~I_FCTLeaf(){
}

/*-------------------------------------------------
		控件 - 根据数据自我刷新
*/
void I_FCTLeaf::refreshItemSelf(){

	this->setText(0, this->getShowingText());		//（刷新显示的名称）
	this->setToolTip(0, this->getName());
}
/*-------------------------------------------------
		控件 - 获取显示文本
*/
QString I_FCTLeaf::getShowingText(){
	int id = this->getId();				//标识
	QString name = this->getName();		//名称

	QString result = "";
	if (this->m_config->zeroFill){
		result = TTool::_zeroFill_(id, this->m_config->zeroFillCount, QLatin1Char(this->m_config->zeroFillChar.toLatin1())) + " " + name;
	}else{
		result = QString::number(id) + " " + name;
	}
	return result;
}