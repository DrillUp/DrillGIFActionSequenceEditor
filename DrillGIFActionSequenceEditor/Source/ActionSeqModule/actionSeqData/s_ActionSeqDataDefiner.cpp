#include "stdafx.h"
#include "s_ActionSeqDataDefiner.h"

/*
-----==========================================================-----
		类：		动画序列 快速类定义器.cpp
		所属模块：	动画序列模块
		功能：		动画序列的快速类定义。
-----==========================================================-----
*/
S_ActionSeqDataDefiner::S_ActionSeqDataDefiner(){
}
S_ActionSeqDataDefiner::~S_ActionSeqDataDefiner() {
}

/* --------------------------------------------------------------
----------XDS_DatabaseDataDefiner 单例
*/
S_ActionSeqDataDefiner* S_ActionSeqDataDefiner::cur_manager = NULL;
S_ActionSeqDataDefiner* S_ActionSeqDataDefiner::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_ActionSeqDataDefiner();
	}
	return cur_manager;
}


/*-----------------------------------
		定义 - 动作元 数据表
*/
C_FastClass S_ActionSeqDataDefiner::getTable_Action() {
	C_FastClass result_class;
	
	result_class = C_FastClass("动作元");
	result_class.addQString						("动作元名称", "");
	result_class.addInt							("动作元优先级", "0");
	result_class.setInt_UI_ToQSpinBox			("动作元优先级", 0, 999999, 1);
	//	资源-状态元 --> 动画帧编辑块
	//	帧间隔 --> 动画帧编辑块
	//	帧间隔-明细表 --> 动画帧编辑块
	result_class.addBoolean						("是否倒放", "false");
	result_class.addInt							("图像-色调值", "0");
	result_class.setInt_UI_ToQSpinBox			("图像-色调值", 0, 360, 1);
	result_class.addBoolean						("图像-模糊边缘", "false");

	return result_class;
}
/*-----------------------------------
		定义 - 状态元 数据表
*/
C_FastClass S_ActionSeqDataDefiner::getTable_State() {
	C_FastClass result_class;

	result_class = C_FastClass("状态元");
	result_class.addQString						("状态元名称", "");
	result_class.addInt							("状态元权重", "40");
	result_class.setInt_UI_ToQSpinBox			("状态元权重", 1, 999999, 1);
	result_class.addInt							("状态元优先级", "0");
	result_class.setInt_UI_ToQSpinBox			("状态元优先级", 0, 999999, 1);
	//	资源-状态元 --> 动画帧编辑块
	//	帧间隔 --> 动画帧编辑块
	//	帧间隔列表 --> 动画帧编辑块
	result_class.addBoolean						("是否倒放", "false");
	result_class.addInt							("图像-色调值", "0");
	result_class.setInt_UI_ToQSpinBox			("图像-色调值", 0, 360, 1);
	result_class.addBoolean						("图像-模糊边缘", "false");

	return result_class;
}
