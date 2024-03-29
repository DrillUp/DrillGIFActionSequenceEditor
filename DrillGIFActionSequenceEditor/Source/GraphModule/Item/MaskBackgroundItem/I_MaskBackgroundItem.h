﻿#ifndef I_MaskBackgroundItem_H
#define I_MaskBackgroundItem_H
#include "stdafx.h"

#include <QGraphicsPixmapItem>

/*
-----==========================================================-----
		类：		马赛克背景.h
		版本：		v1.05
		作者：		drill_up
		所属模块：	图形模块
		功能：		提供一个直接生成背景的item对象。
					（详细见.cpp）
-----==========================================================-----
*/
class I_MaskBackgroundItem : public QGraphicsPixmapItem
{
	
	public:
		I_MaskBackgroundItem();				//构造函数
		~I_MaskBackgroundItem();			//析构函数
		void init();						//初始化

	//-----------------------------------
	//----背景
	protected:
		int m_blockWidth;
		int m_blockHeight;
	public:
											//背景 - 设置背景（单色）
		void setBackground_oneColor(int width, int height, int block_width, int block_height, QColor color = QColor(255, 255, 255), int opacity = 30);
											//背景 - 设置背景（双色）
		void setBackground_twoColor(int width, int height, int block_width, int block_height, QColor color1, QColor color2);
		

};

#endif // I_MaskBackgroundItem_H
