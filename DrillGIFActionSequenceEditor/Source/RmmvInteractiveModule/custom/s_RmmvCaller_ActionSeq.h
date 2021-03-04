#pragma once
#include "../base/p_RmmvCaller.h"

/*
-----==========================================================-----
		类：		rmmv交互者（动作序列 专用）.h
		所属模块：	交互模块
		功能：		与rmmv交互的模块，只考虑 动作序列 相关文件。
					（详细见.cpp）
-----==========================================================-----
*/

class S_RmmvCaller_ActionSeq : public P_RmmvCaller
{
	Q_OBJECT

	public:
		S_RmmvCaller_ActionSeq();
		~S_RmmvCaller_ActionSeq();
		static S_RmmvCaller_ActionSeq* cur_manager;			//单例
		static S_RmmvCaller_ActionSeq* getInstance();		//单例，获取自己
		void init();										//初始化
		
	//-----------------------------------
	//----工程
	public:
										//工程 - 打开rmmv（继承）
		virtual C_RmmvProjectData callRmmvOpen();

										//工程 - 读取全部相关工程文件
										//		【说明】：读取失败返回false
		bool loadAllRmmvFile(C_RmmvProjectData rmmvProjectData);
										//工程 - 覆盖资源文件
		void coverSourceFileToTemp(C_RmmvProjectData rmmvProjectData);


	protected:
										//工程 - 保存操作（覆盖到rmmv）
		virtual void saveDataToRmmv(C_RmmvProjectData rmmvProjectData);

};
