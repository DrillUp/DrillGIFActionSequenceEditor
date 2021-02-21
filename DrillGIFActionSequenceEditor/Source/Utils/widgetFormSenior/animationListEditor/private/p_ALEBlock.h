#ifndef P_ALEBlock_H
#define P_ALEBlock_H

#include "Source/Utils/widgetForm/pictureSelector/private/p_PiSBlock.h"

/*
-----==========================================================-----
		类：		动画帧块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		一个单独的图片块，可以继承该类，添加其他QWidget。
					（详细见cpp）
-----==========================================================-----
*/
class P_ALEBlock : public P_PiSBlock
{
	Q_OBJECT

	public:
		P_ALEBlock(int width, int height, QWidget* parent = 0);	//构造函数
		~P_ALEBlock();											//析构函数
		void _init2();											//初始化

	//-----------------------------------
	//----控件
	private:
		QLabel* m_FrameLabel;
	public:
							//控件 - 设置帧文本
		void setFrameText(QString name);
		
};

#endif // P_ALEBlock_H
