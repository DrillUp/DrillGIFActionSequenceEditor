#include "stdafx.h"

#include "p_ActionPart.h"

/*
-----==========================================================-----
		类：		动作元块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供动作元编辑功能。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_ActionPart::P_ActionPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数

	//-----------------------------------
	//----初始化ui


	//-----------------------------------
	//----事件绑定

}

P_ActionPart::~P_ActionPart(){
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_ActionPart::setData(QJsonObject actionData) {
	this->local_actionData = actionData;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QJsonObject P_ActionPart::getData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_actionData;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_ActionPart::putDataToUi() {
	

}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_ActionPart::putUiToData() {
	

}
