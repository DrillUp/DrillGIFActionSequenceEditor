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
	//----读取器（只读，不存）
	private:
		C_PluginData* data_ActionSeqPlugin;
	public:
										//读取器 - 重设插件数据（自动从插件数据读取器中去拿）
		void resetPluginData();
										//读取器 - 获取插件数据
		C_PluginData* getActionSeqPlugin();
										//读取器 - 获取插件文件
		QFileInfo getActionSeqPluginFile();
		
	//-----------------------------------
	//----动作序列数据
	private:
		QJsonObject data_ActionSeqData;
	public:
										//动作序列数据 - 设置
		void setActionSeqData(QJsonObject data);
										//动作序列数据 - 获取
		QJsonObject getActionSeqData();

	//-----------------------------------
	//----树数据
	private:
		QJsonObject data_treeConfig;
	public:
										//树数据 - 修改
		void modifyTreeData(QJsonObject tree);
										//树数据 - 获取
		QJsonObject getTreeData();

	//-----------------------------------
	//----常量
	public:
										//常量 - 获取软件保存图片路径（F:/xxx/xxx/Special__actionSeq）
										//		【说明】：不是rmmv的文件路径，rmmv相关数据都在交互模块。
		QString getActionSeqDir();
		

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
		
	//-----------------------------------
	//----信号
	signals:
										//信号 - 存储数据被读取
		void dataAllReloaded();
};
