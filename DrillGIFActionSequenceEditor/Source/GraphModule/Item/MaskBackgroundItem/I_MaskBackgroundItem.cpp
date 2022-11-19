#include "stdafx.h"
#include "I_MaskBackgroundItem.h"


/*
-----==========================================================-----
		类：		马赛克背景.cpp
		版本：		v1.04
		作者：		drill_up
		所属模块：	图形模块
		功能：		提供一个直接生成背景的item对象。

		使用方法：	
				> 初始化：
					this->m_MaskBackgroundItem = new I_MaskBackgroundItem(this);
					this->m_MaskBackgroundItem->setPixelGrid(m_pixelWidth, m_pixelHeight);

-----==========================================================-----
*/
I_MaskBackgroundItem::I_MaskBackgroundItem()
	: QGraphicsPixmapItem()
{
	this->init();
}
I_MaskBackgroundItem::~I_MaskBackgroundItem(){
}

/*-------------------------------------------------
		初始化
*/
void I_MaskBackgroundItem::init(){

	// > 参数初始化
	this->m_blockWidth = 24;
	this->m_blockHeight = 24;

	// > 自身属性初始化
	this->setFlag(QGraphicsItem::ItemIsMovable, false);
	this->setFlag(QGraphicsItem::ItemIsSelectable, false);
	this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
}

/*-------------------------------------------------
		背景 - 设置背景（单色）
*/
void I_MaskBackgroundItem::setBackground_oneColor(int width, int height, int block_width, int block_height, QColor color, int opacity){
	this->m_blockWidth = block_width;
	this->m_blockHeight = block_height;
	QPixmap bitmap = this->getBackgroundPixmap_oneColor(width, height, block_width, block_height, color, opacity);
	this->setPixmap(bitmap);
}
/*-------------------------------------------------
		背景 - 设置背景（双色）
*/
void I_MaskBackgroundItem::setBackground_twoColor(int width, int height, int block_width, int block_height, QColor color1, QColor color2){
	this->m_blockWidth = block_width;
	this->m_blockHeight = block_height;
	QPixmap bitmap = this->getBackgroundPixmap_twoColor(width, height, block_width, block_height, color1, color2);
	this->setPixmap(bitmap);
}

/*-------------------------------------------------
		背景 - 获取图像（单色）
*/
QPixmap I_MaskBackgroundItem::getBackgroundPixmap_oneColor(int ww, int hh, int pw, int ph, QColor color, int opacity){

	QPixmap bitmap = QPixmap(ww, hh);				//画板
	QPainter painter(&bitmap);						//画家
	bitmap.fill(color);								//填充底色

	painter.setPen(QPen(QColor(0, 0, 0, 0)));
	painter.setBrush(QBrush(QColor(0, 0, 0, opacity), Qt::BrushStyle::SolidPattern));
	int i_count = qCeil(ww / (double)pw);
	int j_count = qCeil(hh / (double)ph);
	for (int i = 0; i < i_count; i++){
		for (int j = 0; j < j_count; j++){
			painter.drawRect(i*pw + 0, j*ph + 0, pw * 0.5, ph * 0.5);
			painter.drawRect(i*pw + pw*0.5, j*ph + ph * 0.5, pw * 0.5, ph * 0.5);
		}
	}
	painter.end();
	//（注意，painter.fillRect函数是无效的！）

	return bitmap;
}
/*-------------------------------------------------
		背景 - 获取图像（双色）
*/
QPixmap I_MaskBackgroundItem::getBackgroundPixmap_twoColor(int ww, int hh, int pw, int ph, QColor color1, QColor color2){

	QPixmap bitmap = QPixmap(ww, hh);
	bitmap.setMask(QBitmap(bitmap));				//打开alpha层
	bitmap.fill(Qt::transparent);					//透明背景
	QPainter painter(&bitmap);						//画家

	int i_count = qCeil(ww / (double)pw);
	int j_count = qCeil(hh / (double)ph);
	painter.setPen(QPen(QColor(0, 0, 0, 0)));
	painter.setBrush(QBrush(color1, Qt::BrushStyle::SolidPattern));
	for (int i = 0; i < i_count; i++){
		for (int j = 0; j < j_count; j++){
			painter.drawRect(i*pw + 0, j*ph + 0, pw * 0.5, ph * 0.5);
			painter.drawRect(i*pw + pw*0.5, j*ph + ph * 0.5, pw * 0.5, ph * 0.5);
		}
	}
	painter.setBrush(QBrush(color2, Qt::BrushStyle::SolidPattern));
	for (int i = 0; i < i_count; i++){
		for (int j = 0; j < j_count; j++){
			painter.drawRect(i*pw + 0, j*ph + ph * 0.5, pw * 0.5, ph * 0.5);
			painter.drawRect(i*pw + pw*0.5, j*ph + 0, pw * 0.5, ph * 0.5);
		}
	}
	painter.end();

	return bitmap;
}
