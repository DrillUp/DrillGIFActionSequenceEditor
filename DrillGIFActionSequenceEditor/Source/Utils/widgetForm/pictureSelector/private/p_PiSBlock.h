#ifndef P_PiSBlock_H
#define P_PiSBlock_H
#include "stdafx.h"

#include <QtWidgets>
#include "ui_P_PiSBlock.h"

/*
-----==========================================================-----
		类：		图片块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		一个单独的图片块，可以继承该类，添加其他QWidget。
					（详细见cpp）
-----==========================================================-----
*/
class I_PiSPicture;
class P_PiSBlock : public QWidget
{
	Q_OBJECT

	public:
		P_PiSBlock(int width, int height, QWidget* parent = 0);	//构造函数
		~P_PiSBlock();											//析构函数
		void _init();											//初始化（该函数 子类继承没用）

	//-----------------------------------
	//----控件
	protected:
		int m_width;
		int m_height;
		I_PiSPicture* m_i_PiSPicture;
	public:
							//控件 - 设置文本
		void setCountText(QString name);
							//控件 - 获取文本
		QString getCountText();
							//控件 - 设置图片
		void setPixmap(QPixmap pixmap);
							//控件 - 设置马赛克背景
		void setMaskEnabled(bool enabled);
		
	//-----------------------------------
	//----窗口
	protected:
		Ui::P_PiSBlock ui;
};

#endif // P_PiSBlock_H
