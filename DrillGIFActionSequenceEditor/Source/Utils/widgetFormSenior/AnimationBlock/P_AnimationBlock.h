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
		P_AnimationBlock(int width, int height, QWidget* parent = 0);	//构造函数
		~P_AnimationBlock();											//析构函数


	//-----------------------------------
	//----控件
	private:
		QLabel* m_FrameLabel;
	public:
								//控件 - 设置帧文本
		void setFrameText(QString name);
		
};

#endif // P_AnimationBlock_H
