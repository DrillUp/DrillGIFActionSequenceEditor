#include "stdafx.h"
#include "I_PiSPicture.h"

#include "Source/GraphModule/Item/MaskBackgroundGenerator/S_MaskBackgroundGenerator.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		图片块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		使得标签带有多个按钮，这些按钮相当于勾选框。


		使用方法：
				>初始化

-----==========================================================-----
*/
I_PiSPicture::I_PiSPicture(int width, int height, QWidget *parent)
	: QWidget(parent)
{
	//-----------------------------------
	//----参数初始化
	this->m_width = width;
	this->m_height = height;

	this->m_maskEnabled = true;					//马赛克 绘制
	this->m_maskWidth = 24;						//马赛克 块宽
	this->m_maskHeight = 24;					//马赛克 块高
	this->m_maskColor = QColor(245, 245, 245);	//马赛克 颜色

	//-----------------------------------
	//----ui初始化
	this->setFixedWidth(this->m_width);
	this->setMinimumWidth(this->m_width);
	this->setMaximumWidth(this->m_width);
	this->setFixedHeight(this->m_height);
	this->setMinimumHeight(this->m_height);
	this->setMaximumHeight(this->m_height);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

}
I_PiSPicture::~I_PiSPicture(){
}

/*-------------------------------------------------
		控件 - 设置图片
*/
void I_PiSPicture::setPixmap(QPixmap pixmap){
	this->m_pixmapOrg = pixmap;
	this->update();
}
/*-------------------------------------------------
		控件 - 马赛克
*/
void I_PiSPicture::setMaskEnabled(bool maskEnabled){
	if (this->m_maskEnabled == maskEnabled){ return; }
	this->m_maskEnabled = maskEnabled;
	this->update();
}

/*-------------------------------------------------
		绘制 - 绘制图片
*/
void I_PiSPicture::paintEvent(QPaintEvent*){

	// > 按比例缩放
	QPixmap scaledPixmap = this->m_pixmapOrg.scaled(QSize(this->m_width, this->m_height), Qt::KeepAspectRatio);

	// > 绘制
	QPainter* painter = new QPainter();
	painter->begin(this);

	//QPen pen;		//（画一条斜线，测试用）
	//pen.setWidthF(0.4);
	//pen.setColor(QColor(0, 0, 0));
	//painter->setPen(pen);
	//painter->drawLine(0, 0, 7, 7);

	// > 背景绘制
	if (this->m_maskEnabled){
		QPixmap backgroundPixmap = S_MaskBackgroundGenerator::getInstance()->getMaskBackground_OneColor(this->m_width, this->m_height, this->m_maskWidth, this->m_maskHeight, QColor(255, 255, 255));
		painter->drawPixmap(QPoint(0, 0), backgroundPixmap);
	}

	// > 居中点
	QPoint p = QPoint((this->m_width - scaledPixmap.width())*0.5, (this->m_height - scaledPixmap.height())*0.5);
	painter->drawPixmap(p, scaledPixmap);

	painter->end();
}