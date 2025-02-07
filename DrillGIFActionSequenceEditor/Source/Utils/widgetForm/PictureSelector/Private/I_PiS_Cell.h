#pragma once
#include "stdafx.h"

#include "Source/Utils/WidgetForm/PictureBlock/P_PictureBlock.h"

/*
-----==========================================================-----
		类：		图片选择块 单元胞.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		控件块的元胞对象。
					（详细见cpp）
-----==========================================================-----
*/

class I_PiS_Cell{

	public:
		I_PiS_Cell();				//构造函数
		~I_PiS_Cell();				//析构函数
		
	//-----------------------------------
	//----控件
	public:
		QListWidgetItem* item = nullptr;	//项 对象
		P_PictureBlock* widget = nullptr;	//控件 对象
	public:
									//控件 - 获取对象
									//		【说明】：注意要判断空。
		virtual P_PictureBlock* getBlock();
									//控件 - 获取对象数据
									//		【说明】：注意要判断空。
		virtual C_PictureBlockDataPtr getBlockDataPtr();
		
};
