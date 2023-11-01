#include "stdafx.h"
#include "I_PiS_Cell.h"

#include "Source/Utils/WidgetForm/PictureBitmapCache/S_PictureBitmapCache.h"

/*
-----==========================================================-----
		类：		图片选择块 单元胞.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		图片选择块的元胞对象。
-----==========================================================-----
*/

I_PiS_Cell::I_PiS_Cell(){

	// > 控件
	this->item = nullptr;
	this->widget = nullptr;
}
I_PiS_Cell::~I_PiS_Cell(){
}


/*-------------------------------------------------
		控件 - 获取对象
*/
P_PictureBlock* I_PiS_Cell::getBlock(){
	return this->widget;
}
/*-------------------------------------------------
		控件 - 获取对象数据
*/
C_PictureBlockDataPtr I_PiS_Cell::getBlockDataPtr(){
	if (this->widget == nullptr){ return C_PictureBlockDataPtr(); }
	return this->widget->getDataPtr();
}
