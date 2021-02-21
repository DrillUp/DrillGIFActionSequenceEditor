#include "stdafx.h"
#include "P_ALEBlock.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动画帧块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		一个单独的图片块，可以继承该类，添加其他QWidget。

		使用方法：
				>初始化

-----==========================================================-----
*/
P_ALEBlock::P_ALEBlock(int width, int height, QWidget *parent)
	: P_PiSBlock(width, height, parent)
{
	this->_init2();			//（父类的函数_init继承没用）
}
P_ALEBlock::~P_ALEBlock(){
}

/*-------------------------------------------------
		初始化
*/
void P_ALEBlock::_init2(){

	// > 名称区域
	QLabel* label = new QLabel(this);
	label->setText("");
	label->setGeometry(this->m_width - 45, this->m_height - 21, 40, 18);
	label->setStyleSheet("border: 1px solid #000000; background-color: #ffffff; color:#000000; font-size:13px; ");
	label->setAlignment(Qt::AlignCenter);
	label->stackUnder(ui.label);
	this->m_FrameLabel = label;

}

/*-------------------------------------------------
		控件 - 设置帧文本
*/
void P_ALEBlock::setFrameText(QString name){
	if ( this->m_FrameLabel == nullptr){ return; }
	this->m_FrameLabel->setText(name);
}
