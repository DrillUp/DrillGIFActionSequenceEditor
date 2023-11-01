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

P_AnimationBlock::P_AnimationBlock(QWidget *parent)
	: P_PictureBlock(parent)
{

	// > 帧文本
	QLabel* label = new QLabel(this);
	label->setText("");
	this->m_FrameLabel = label;

}
P_AnimationBlock::~P_AnimationBlock(){
}


/*-------------------------------------------------
		控件 - 刷新UI
*/
void P_AnimationBlock::refreshUI(){
	P_PictureBlock::refreshUI();
}
/*-------------------------------------------------
		画布 - 刷新（继承）
*/
void P_AnimationBlock::refreshSize(){
	P_PictureBlock::refreshSize();

	// > 帧文本 刷新位置
	QLabel* label = this->m_FrameLabel;
	label->setGeometry(this->m_dataPtr->m_width - 3 - 46, this->m_dataPtr->m_height - 3 - 18, 46, 18);
	label->setAlignment(Qt::AlignCenter);
	this->refreshFrameLabelStyle();		//（刷新样式）
	ui.label_picture->stackUnder(label);
	ui.label_picture->stackUnder(ui.label_title);
}


/*-------------------------------------------------
		帧文本 - 设置
*/
void P_AnimationBlock::setFrameLabel_Text(QString name){
	if ( this->m_FrameLabel == nullptr){ return; }
	this->m_FrameLabel->setText(name);
}

/*-------------------------------------------------
		帧文本样式 - 字体加粗
*/
void P_AnimationBlock::setFrameLabel_Bold(bool enabled){
	this->m_isBold = enabled;
	this->refreshFrameLabelStyle();
}
/*-------------------------------------------------
		帧文本样式 - 字体加粗
*/
void P_AnimationBlock::refreshFrameLabelStyle(){
	if (this->m_FrameLabel == nullptr){ return; }
	QString style_sheet;

	// > 默认样式（方形描边，背景白色，文本黑色，文本13px大小）
	style_sheet.append("border: 1px solid #000000; background-color: #ffffff; color:#000000; font-size:13px; ");

	// > 加粗设置
	if (this->m_isBold == true){
		style_sheet.append("font-weight:bold; ");
	}

	this->m_FrameLabel->setStyleSheet(style_sheet);
}