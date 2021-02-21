#include "stdafx.h"
#include "I_PiSPicture.h"

#include "Source/Utils/common/TTool.h"

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
	this->m_pixmapWidth = width;
	this->m_pixmapHeight = height;

	this->m_maskEnabled = true;					//马赛克 绘制
	this->m_maskWidth = 24;						//马赛克 块宽
	this->m_maskHeight = 24;					//马赛克 块高
	this->m_maskColor = QColor(245, 245, 245);	//马赛克 颜色

	//-----------------------------------
	//----ui初始化
	this->setFixedWidth(this->m_pixmapWidth);
	this->setMinimumWidth(this->m_pixmapWidth);
	this->setMaximumWidth(this->m_pixmapWidth);
	this->setFixedHeight(this->m_pixmapHeight);
	this->setMinimumHeight(this->m_pixmapHeight);
	this->setMaximumHeight(this->m_pixmapHeight);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

}
I_PiSPicture::~I_PiSPicture(){
}

/*-------------------------------------------------
		控件 - 设置图片
*/
void I_PiSPicture::setPixmap(QPixmap pixmap){
	this->m_pixmap = pixmap;
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
	QPixmap scaledPixmap = this->m_pixmap.scaled(QSize(this->m_pixmapWidth, this->m_pixmapHeight), Qt::KeepAspectRatio);

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
		painter->drawPixmap(QPoint(0, 0), this->getBitmapMaskBackground());
	}

	// > 居中点
	QPoint p = QPoint((this->m_pixmapWidth - scaledPixmap.width())*0.5, (this->m_pixmapHeight - scaledPixmap.height())*0.5);
	painter->drawPixmap(p, scaledPixmap);

	painter->end();
}
/*-------------------------------------------------
		绘制 - 绘制马赛克方块
*/
QPixmap I_PiSPicture::getBitmapMaskBackground(){
	int ww = this->m_pixmapWidth;
	int hh = this->m_pixmapHeight;
	int pw = this->m_maskWidth;
	int ph = this->m_maskHeight;

	QPixmap img = QPixmap(ww, hh);				//画板
	QPainter painter(&img);						//画家
	img.fill(this->m_maskColor);				//填充底色

	painter.setPen(QPen(QColor(0, 0, 0, 0)));
	painter.setBrush(QBrush(QColor(0, 0, 0, 30), Qt::BrushStyle::SolidPattern));
	int i_count = qCeil(ww / (double)pw);
	int j_count = qCeil(hh / (double)ph);
	for (int i = 0; i < i_count; i++){
		for (int j = 0; j < j_count; j++){
			painter.drawRect(i*pw + 0, j*ph+ 0, pw * 0.5, ph * 0.5);
			painter.drawRect(i*pw + pw*0.5, j*ph + ph * 0.5, pw * 0.5, ph * 0.5);
		}
	}
	painter.end();
	//（注意，painter.fillRect函数是无效的！）

	return img;
}