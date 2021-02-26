#pragma once

#include "Source/Utils/widgetFastForm/fastForm/c_FastClass.h"

/*
-----==========================================================-----
		类：		动作序列 快速类定义器.h
		所属模块：	动作序列模块
		功能：		动作序列的快速类定义。
-----==========================================================-----
*/
class S_ActionSeqDataDefiner : public QObject
{
	Q_OBJECT

	public:
		S_ActionSeqDataDefiner();
		~S_ActionSeqDataDefiner();
		static S_ActionSeqDataDefiner* cur_manager;			//单例
		static S_ActionSeqDataDefiner* getInstance();		//单例，获取自己
		

	//-----------------------------------
	//----快速类定义
	public:
										//定义 - 动作元 数据表
		C_FastClass getTable_Action();
										//定义 - 状态元 数据表
		C_FastClass getTable_State();

};

