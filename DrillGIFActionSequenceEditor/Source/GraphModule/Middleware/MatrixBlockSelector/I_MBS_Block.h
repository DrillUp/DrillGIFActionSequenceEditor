#pragma once
#include "stdafx.h"
#include <QGraphicsRectItem>

/*
-----==========================================================-----
		类：		阵列方块.h
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		鼠标点击后，呈现不同显示状态的方块。
					【这个方块的x、y、w、h被完全锁定】
					（详细见cpp）
-----==========================================================-----
*/
class I_MBS_Block : public QGraphicsRectItem{

	public:
		I_MBS_Block();
		~I_MBS_Block();
		void _init();

	//-----------------------------------
	//----状态位
	protected:
		int m_state;					//状态位
		QList<QColor> m_blockColorTank;	//（不同状态位设置不同颜色）
	public:
								//状态位 - 设置方块颜色容器
		void setColorTank(QList<QColor> color_tank);
								//状态位 - 设置当前状态
		void setState(int state);
								//状态位 - 获取当前状态
		int getState();
		
	//-----------------------------------
	//----矩阵
	protected:
		int m_width;
		int m_height;
		int m_matrixX;
		int m_matrixY;
	public:
								//矩阵 - 设置方块大小
		void setBlockSize(int width, int height);
								//矩阵 - 设置位置（单位图块）
		void setMatrixPosition(int x,int y);
								//矩阵 - 获取矩阵位置X
		int getMatrixX();
								//矩阵 - 获取矩阵位置Y
		int getMatrixY();
	protected:
								//矩阵 - 刷新方块绘制
		void refreshRect();
		
	//-----------------------------------
	//----类属性
	public:
								//运算符重载
		const bool operator== (const I_MBS_Block& a)const;

};