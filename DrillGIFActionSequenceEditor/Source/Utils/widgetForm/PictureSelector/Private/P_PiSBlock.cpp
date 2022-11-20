#include "stdafx.h"
#include "P_PiSBlock.h"

#include "Source/Utils/Common/TTool.h"
#include "I_PiSPicture.h"

/*
-----==========================================================-----
		类：		图片块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		一个单独的图片块，可以继承该类，添加其他QWidget。

		使用方法：
				>初始化

-----==========================================================-----
*/
P_PiSBlock::P_PiSBlock(int width, int height, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->m_width = width;
	this->m_height = height;
	this->_init();
}
P_PiSBlock::~P_PiSBlock(){
}

/*-------------------------------------------------
		初始化
*/
void P_PiSBlock::_init(){

	// > 图片区域
	this->m_i_PiSPicture = new I_PiSPicture( this->m_width - 10, this->m_height - 10, this );
	this->m_i_PiSPicture->move(5, 6);
	this->m_i_PiSPicture->stackUnder(ui.label);

	// > 名称区域
	ui.label->setText("");
	ui.label->move(2, 3);
}

/*-------------------------------------------------
		控件 - 设置文本
*/
void P_PiSBlock::setCountText(QString name){
	ui.label->setText(name);
}
/*-------------------------------------------------
		控件 - 获取文本
*/
QString P_PiSBlock::getCountText(){
	return ui.label->text();
}
/*-------------------------------------------------
		控件 - 设置图片
*/
void P_PiSBlock::setPixmap(QPixmap pixmap){
	this->m_i_PiSPicture->setPixmap(pixmap);
}
/*-------------------------------------------------
		控件 - 设置马赛克背景
*/
void P_PiSBlock::setMaskEnabled(bool enabled){
	this->m_i_PiSPicture->setMaskEnabled(enabled);
}
