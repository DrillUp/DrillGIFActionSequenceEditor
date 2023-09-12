#ifndef P_PictureBlock_H
#define P_PictureBlock_H
#include "stdafx.h"

#include <QtWidgets>
#include "ui_P_PictureBlock.h"

/*
-----==========================================================-----
		类：		图片单块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		单独的图片块，作为列表中显示的一个基本块对象。
					（详细见cpp）
-----==========================================================-----
*/

class P_PictureBlock : public QWidget
{
	Q_OBJECT

	public:
		P_PictureBlock(int width, int height, QWidget* parent = 0);	//构造函数
		~P_PictureBlock();											//析构函数


	//-----------------------------------
	//----控件
	protected:
		int m_width;
		int m_height;
	public:
									//控件 - 设置标题
		void setTitle(QString title);
		QString getTitle();
									//控件 - 获取画布高宽
		int getDrawingWidth();
		int getDrawingHeight();
		
		
	//-----------------------------------
	//----图片
	protected:
		QPixmap m_pixmapOrg;		//图片（不需要传指针，QPixmap提供了"隐式数据共享"）
	public:
									//图片 - 设置
		void setPixmap(QPixmap pixmap);
		QPixmap getPixmap();
									//图片 - 刷新
		void refreshPixmap();
	

	//-----------------------------------
	//----马赛克背景
	protected:
		bool m_maskEnabled;			//马赛克 开关
		int m_maskWidth;			//马赛克 块宽
		int m_maskHeight;			//马赛克 块高
		QColor m_maskColor;			//马赛克 颜色
	public:
									//马赛克背景 - 设置
		void setMaskEnabled(bool maskEnabled);
									//马赛克背景 - 刷新
		void refreshMask();


	//-----------------------------------
	//----块
	protected:
		Ui::P_PictureBlock ui;
};

#endif // P_PictureBlock_H
