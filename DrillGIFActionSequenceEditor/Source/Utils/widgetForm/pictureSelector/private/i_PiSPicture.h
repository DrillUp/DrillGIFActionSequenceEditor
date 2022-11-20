#ifndef I_PiSPicture_H
#define I_PiSPicture_H
#include "stdafx.h"

#include <QWidget>

/*
-----==========================================================-----
		类：		图片单控件.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		使得标签带有多个按钮，这些按钮相当于勾选框。
					（详细见cpp）
-----==========================================================-----
*/

class I_PiSPicture : public QWidget
{
	Q_OBJECT

	public:
		I_PiSPicture(int width, int height, QWidget* parent = 0);	//构造函数
		~I_PiSPicture();											//析构函数

	//-----------------------------------
	//----控件
	private:
		int m_pixmapWidth;
		int m_pixmapHeight;
		QPixmap m_pixmap;
	public:
									//控件 - 设置图片
		void setPixmap(QPixmap pixmap);
									//控件 - 马赛克设置
		void setMaskEnabled(bool maskEnabled);
		
	//-----------------------------------
	//----绘制
	protected:
		bool m_maskEnabled;			//马赛克
		int m_maskWidth;			//马赛克 块宽
		int m_maskHeight;			//马赛克 块高
		QColor m_maskColor;			//马赛克 颜色
	protected:
									//绘制 - 绘制图片
		void paintEvent(QPaintEvent*);
									//绘制 - 绘制马赛克方块
		QPixmap getBitmapMaskBackground();

};

#endif // I_PiSPicture_H
