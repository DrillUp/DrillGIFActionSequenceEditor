#pragma once
#include "Source/ProjectModule/storage/s_StorageManagerTemplate.h"

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
class S_ActionSeqDataContainer : public QObject, public S_StorageManagerTemplate
{
	Q_OBJECT

	public:
		S_ActionSeqDataContainer();
		~S_ActionSeqDataContainer();
		static S_ActionSeqDataContainer* cur_manager;			//单例
		static S_ActionSeqDataContainer* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）

	//-----------------------------------
	//----插件数据（只读，不存）
	private:
		C_PluginData* data_ActionSeq;
	public:
										//插件数据 - 重设（自动从插件数据读取器中去拿）
		void resetPluginData();
										//插件数据 - 获取（插件数据）
		C_PluginData* getActionSeqData();
										//插件数据 - 获取（插件文件）
		QFileInfo getActionSeqPluginFile();
	signals:
										//数据 - 数据变化（信号）
		void actionSeqDataChanged();
		
	//-----------------------------------
	//----数据
	private:
		QJsonObject data_treeConfig;
	public:
										//数据 - 修改
		void modifyTreeData(QJsonObject tree);
										//数据 - 获取
		QJsonObject getTreeData();
		
	//-----------------------------------
	//----信号
	signals:
										//信号 - 存储数据被读取
		void dataAllReloaded();

	//-----------------------------------
	//----存储数据（继承）
	public:
										//数据 - 获取存储的名称
		QString getSaveName();
										//数据 - 清除当前管理器数据
		void clearAllData();
										//数据 - 全部激励源数据 -> QJsonObject
		QJsonObject getAllDataOfJsonObject();
										//数据 - QJsonObject -> 全部激励源数据
		void setAllDataFromJsonObject(QJsonObject obj);

};
