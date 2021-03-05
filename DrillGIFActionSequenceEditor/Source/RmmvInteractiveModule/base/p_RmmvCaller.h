#pragma once
#include "c_RmmvProjectData.h"

/*
-----==========================================================-----
		类：		rmmv交互者（父类）.h
		版本：		v1.00
		所属模块：	交互模块
		功能：		与rmmv软件交互的单例，只包含一些基本独立的操作。
					（详细见.cpp）
-----==========================================================-----
*/

class P_RmmvCaller : public QObject
{
	Q_OBJECT

	public:
		P_RmmvCaller();
		~P_RmmvCaller();
		
	//-----------------------------------
	//----rmmv
	public:
									//rmmv - 选择rmmv（注意可能返回空）
		virtual C_RmmvProjectData callRmmvSelect();
									//rmmv - 打开rmmv编辑器
		virtual void callRmmvSoftware(C_RmmvProjectData rmmvProjectData);
									//rmmv - 保存rmmv（让rmmv先保存，然后自己存）
		virtual void callRmmvSave(C_RmmvProjectData rmmvProjectData);
									//rmmv - 保存rmmv（要求用户关闭rmmv，才能保存）
		virtual void callRmmvSave_RequestingClose(C_RmmvProjectData rmmvProjectData);
									//rmmv - 运行rmmv
		virtual void callRmmvRun(C_RmmvProjectData rmmvProjectData);

	protected:
									//rmmv - 保存操作（覆盖到rmmv）
		virtual void saveDataToRmmv(C_RmmvProjectData rmmvProjectData) = 0;

};
