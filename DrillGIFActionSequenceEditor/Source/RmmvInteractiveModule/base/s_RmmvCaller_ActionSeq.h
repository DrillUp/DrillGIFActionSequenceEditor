#pragma once
#include "p_RmmvCaller.h"

/*
-----==========================================================-----
		类：		rmmv数据文件管理器.h
		所属模块：	rmmv读取模块
		功能：		管理插件数据类的模块。
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
										//工程 - 读取全部相关工程文件
										//		【说明】：读取失败返回false
		bool loadAllRmmvFile(C_RmmvProjectData rmmvProjectData);
										//工程 - 读取文件 - plugin.js
		void readPluginData(QString context);
										//工程 - 读取文件 - System.json
		void readSystemData(QString context);

	protected:
										//工程 - 保存操作（覆盖到rmmv）
		virtual void saveDataToRmmv(C_RmmvProjectData rmmvProjectData);

};
