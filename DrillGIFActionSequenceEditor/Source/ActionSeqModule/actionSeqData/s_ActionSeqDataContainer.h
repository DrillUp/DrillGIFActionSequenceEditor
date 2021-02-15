#pragma once

#include "Source/PluginModule/storageData/c_PluginData.h"

/*
-----==========================================================-----
		类：		动作序列 数据容器.h
		所属模块：	动作序列模块
		功能：		动作序列的数据容器。
					会主动使用插件容器中的数据。
					（详细见.cpp）
-----==========================================================-----
*/
class S_ActionSeqDataContainer : public QObject
{
	Q_OBJECT

	public:
		S_ActionSeqDataContainer();
		~S_ActionSeqDataContainer();
		static S_ActionSeqDataContainer* cur_manager;			//单例
		static S_ActionSeqDataContainer* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）

	//-----------------------------------
	//----数据
	private:
		C_PluginData* data_ActionSeq;
	public:
										//数据 - 重设（自动从插件数据读取器中去拿）
		void resetPluginData();
										//数据 - 获取（插件数据）
		C_PluginData* getActionSeqData();
										//数据 - 获取（插件文件）
		QFileInfo getActionSeqPluginFile();
	signals:
										//数据 - 数据变化（信号）
		void actionSeqDataChanged();

};
