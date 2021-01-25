#include "stdafx.h"

#include "p_StatePart.h"

/*
-----==========================================================-----
		类：		状态元块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供状态元编辑功能。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_StatePart::P_StatePart(QWidget *parent)
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

P_StatePart::~P_StatePart(){
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_StatePart::setData(QJsonObject state) {
	this->local_state = state;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QJsonObject P_StatePart::getData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_state;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_StatePart::putDataToUi() {
	

}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_StatePart::putUiToData() {
	

}
