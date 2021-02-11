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
									//rmmv - 打开rmmv（注意可能返回空）
		C_RmmvProjectData callRmmvOpen();
									//rmmv - 保存rmmv
		void callRmmvSave(C_RmmvProjectData rmmvProjectData);
									//rmmv - 运行rmmv
		void callRmmvRun(C_RmmvProjectData rmmvProjectData);

	protected:
									//rmmv - 保存操作（覆盖到rmmv）
		virtual void saveDataToRmmv(C_RmmvProjectData rmmvProjectData) = 0;

};
