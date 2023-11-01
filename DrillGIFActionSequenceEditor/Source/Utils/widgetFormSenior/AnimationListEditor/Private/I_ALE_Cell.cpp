#include "stdafx.h"
#include "I_ALE_Cell.h"

#include "Source/Utils/WidgetForm/PictureBitmapCache/S_PictureBitmapCache.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 单元胞.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧编辑块的元胞对象。
-----==========================================================-----
*/

I_ALE_Cell::I_ALE_Cell()
	: I_PiS_Cell()
{

}
I_ALE_Cell::~I_ALE_Cell(){
}


/*-------------------------------------------------
		控件 - 获取对象
*/
P_AnimationBlock* I_ALE_Cell::getBlock_ALE(){
	return dynamic_cast<P_AnimationBlock*>(this->widget);
}
/*-------------------------------------------------
		控件 - 获取对象数据
*/
C_AnimationBlockDataPtr I_ALE_Cell::getBlockDataPtr_ALE(){
	if (this->widget == nullptr){ return C_AnimationBlockDataPtr(); }
	return this->widget->getDataPtr().dynamicCast<C_AnimationBlockData>();
}
