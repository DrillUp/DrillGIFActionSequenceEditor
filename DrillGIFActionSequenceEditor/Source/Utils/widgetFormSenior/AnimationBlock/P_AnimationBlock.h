#ifndef P_AnimationBlock_H
#define P_AnimationBlock_H
#include "stdafx.h"

#include "Source/Utils/WidgetForm/PictureBlock/P_PictureBlock.h"

/*
-----==========================================================-----
		类：		动画帧单块（继承于 图片单块）.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		单独的动画帧块，作为列表中显示的一个基本块对象。
					（详细见cpp）
-----==========================================================-----
*/

class P_AnimationBlock : public P_PictureBlock
{

	public:
		P_AnimationBlock(QWidget* parent = 0);		//构造函数
		~P_AnimationBlock();						//析构函数
		

	//-----------------------------------
	//----控件
	public:
									//控件 - 刷新UI（继承）
		virtual void refreshUI() override;

	//-----------------------------------
	//----画布
	public:
									//画布 - 刷新（继承）
		virtual void refreshSize() override;


	//-----------------------------------
	//----帧文本
	protected:
		QLabel* m_FrameLabel;
	public:
									//帧文本 - 设置
		void setFrameLabel_Text(QString name);

	//-----------------------------------
	//----帧文本样式
	protected:
		bool m_isBold;
	public:
									//帧文本样式 - 字体加粗
		void setFrameLabel_Bold(bool enabled);
	protected:
									//帧文本样式 - 刷新样式
		void refreshFrameLabelStyle();
		
};

#endif // P_AnimationBlock_H
