#include "stdafx.h"
#include "P_AnimationBlock.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		动画帧单块（继承于 图片单块）.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		单独的动画帧块，作为列表中显示的一个基本块对象。

-----==========================================================-----
*/

P_AnimationBlock::P_AnimationBlock(int width, int height, QWidget *parent)
	: P_PictureBlock(width, height, parent)
{

	// > 名称区域
	QLabel* label = new QLabel(this);
	label->setText("");
	label->setGeometry(this->m_width - 3 - 46, this->m_height - 3 - 18, 46, 18);
	label->setStyleSheet("border: 1px solid #000000; background-color: #ffffff; color:#000000; font-size:13px; ");
	label->setAlignment(Qt::AlignCenter);
	ui.label_picture->stackUnder(label);
	ui.label_picture->stackUnder(ui.label_title);
	this->m_FrameLabel = label;

}
P_AnimationBlock::~P_AnimationBlock(){
}


/*-------------------------------------------------
		控件 - 设置帧文本
*/
void P_AnimationBlock::setFrameText(QString name){
	if ( this->m_FrameLabel == nullptr){ return; }
	this->m_FrameLabel->setText(name);
}
