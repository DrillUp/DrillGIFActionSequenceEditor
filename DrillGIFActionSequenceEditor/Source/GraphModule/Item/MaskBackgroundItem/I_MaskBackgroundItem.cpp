#include "stdafx.h"
#include "I_MaskBackgroundItem.h"

#include "Source/GraphModule/Item/MaskBackgroundGenerator/S_MaskBackgroundGenerator.h"


/*
-----==========================================================-----
		类：		马赛克背景.cpp
		版本：		v1.05
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
	QPixmap pixmap = S_MaskBackgroundGenerator::getInstance()->getMaskBackground_OneColor(width, height, block_width, block_height, color, opacity);
	this->setPixmap(pixmap);
}
/*-------------------------------------------------
		背景 - 设置背景（双色）
*/
void I_MaskBackgroundItem::setBackground_twoColor(int width, int height, int block_width, int block_height, QColor color1, QColor color2){
	this->m_blockWidth = block_width;
	this->m_blockHeight = block_height;
	QPixmap pixmap = S_MaskBackgroundGenerator::getInstance()->getMaskBackground_TwoColor(width, height, block_width, block_height, color1, color2);
	this->setPixmap(pixmap);
}
