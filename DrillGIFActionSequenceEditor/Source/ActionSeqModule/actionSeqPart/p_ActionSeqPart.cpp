#include "stdafx.h"

#include "p_ActionSeqPart.h"

/*
-----==========================================================-----
		类：		动作序列块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供动作序列放映，功能，并嵌套状态元、动作元的块。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_ActionSeqPart::P_ActionSeqPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数

	//-----------------------------------
	//----初始化ui
	this->m_actionPart = new P_ActionPart(parent);
	ui.verticalLayout_action->addWidget(this->m_actionPart);

	this->m_statePart = new P_StatePart(parent);
	ui.verticalLayout_state->addWidget(this->m_statePart);
	

	//-----------------------------------
	//----事件绑定

}

P_ActionSeqPart::~P_ActionSeqPart(){
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_ActionSeqPart::setData(QJsonObject actionSeq) {
	this->local_actionSeq = actionSeq;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QJsonObject P_ActionSeqPart::getData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_actionSeq;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_ActionSeqPart::putDataToUi() {
	

}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_ActionSeqPart::putUiToData() {
	

}
