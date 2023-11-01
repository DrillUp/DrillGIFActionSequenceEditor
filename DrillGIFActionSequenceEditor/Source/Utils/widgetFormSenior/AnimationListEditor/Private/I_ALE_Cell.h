#pragma once
#include "stdafx.h"

#include "Source/Utils/WidgetForm/PictureSelector/Private/I_PiS_Cell.h"
#include "Source/Utils/WidgetFormSenior/AnimationBlock/C_AnimationBlockData.h"
#include "Source/Utils/WidgetFormSenior/AnimationBlock/P_AnimationBlock.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 单元胞.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧编辑块的元胞对象。
					（详细见cpp）
-----==========================================================-----
*/

class I_ALE_Cell : public I_PiS_Cell
{

	public:
		I_ALE_Cell();				//构造函数
		~I_ALE_Cell();				//析构函数
		
	//-----------------------------------
	//----控件
	public:
									//控件 - 获取对象
									//		【说明】：注意要判断空。
		P_AnimationBlock* getBlock_ALE();
									//控件 - 获取对象数据
									//		【说明】：注意要判断空。
		C_AnimationBlockDataPtr getBlockDataPtr_ALE();
		
};
