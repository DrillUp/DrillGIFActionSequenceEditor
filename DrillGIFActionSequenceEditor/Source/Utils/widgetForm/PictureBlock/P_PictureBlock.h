#ifndef P_PictureBlock_H
#define P_PictureBlock_H
#include "stdafx.h"

#include <QtWidgets>
#include "ui_P_PictureBlock.h"
#include "C_PictureBlockData.h"

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
		P_PictureBlock(QWidget* parent = 0);		//构造函数
		~P_PictureBlock();							//析构函数


	//-----------------------------------
	//----控件
	protected:
		C_PictureBlockDataPtr m_dataPtr;
	public:
									//控件 - 设置数据
									//		【说明】：创建单块后，需要手动执行此函数进行初始化。（数据对象也要先初始化）
		virtual void setDataPtr(C_PictureBlockDataPtr data);
									//控件 - 获取数据
									//		【说明】：图片单块的函数可以直接修改到数据；但如果直接修改了数据，则要手动刷新 图片单块 。
		C_PictureBlockDataPtr getDataPtr();
									//控件 - 刷新UI
									//		【说明】：可以多次调用刷新UI。
		virtual void refreshUI();
		

	//-----------------------------------
	//----画布
	public:
									//画布 - 设置高宽
									//		【说明】：修改高宽后，必然执行 刷新UI。
		void setSize(int width, int height);
									//画布 - 获取高宽
		int getDrawingWidth();
		int getDrawingHeight();
									//画布 - 刷新
		virtual void refreshSize();
		

	//-----------------------------------
	//----标题
	public:
									//标题 - 设置
		void setTitle(QString title);
		void setTitleNum(int title_num);
		void setTitleNumWithZeroFill(int title_num, int zeroFillCount, QChar zeroFillChar);
		QString getTitle();
		int getTitleNum();
									//标题 - 刷新
		void refreshTitle();
									//标题 - 设置颜色
		void setTitleColorType(QString type);
		QString getTitleColorType();
									//标题 - 修改颜色
		void setTitleColor(QColor color_a, QColor color_b);
									//标题 - 刷新颜色
		void refreshTitleColor();
		

	//-----------------------------------
	//----图片
	public:
									//图片 - 设置
		void setBitmapPath(QString bitmapPath);
		QString getBitmapPath();
									//图片 - 刷新
		void refreshPixmap();
	

	//-----------------------------------
	//----马赛克背景
	public:
									//马赛克背景 - 设置
		void setMaskConfig(bool maskEnabled, bool borderEnabled);
									//马赛克背景 - 刷新
		void refreshMask();


	//-----------------------------------
	//----块
	protected:
		Ui::P_PictureBlock ui;
};

#endif // P_PictureBlock_H
