#pragma once

#include "c_PluginData.h"

/*
-----==========================================================-----
		类：		插件 存储数据容器.h
		所属模块：	插件模块
		功能：		插件编辑并存储数据的容器。
					（详细见.cpp）
-----==========================================================-----
*/
class S_PluginDataContainer : public QObject
{
	Q_OBJECT

	public:
		S_PluginDataContainer();
		~S_PluginDataContainer();
		static S_PluginDataContainer* cur_manager;			//单例
		static S_PluginDataContainer* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）

	//-----------------------------------
	//----数据
	private:
		QList<C_PluginData*> data_PluginDatas;
	public:
										//数据 - 重设（数据列表）
		void resetPluginData(QList<C_PluginData*> data_list);
										//数据 - 重设（plugins.js文本）
		void resetPluginDataFromText(QString data_context);
										//数据 - 获取
		QList<C_PluginData*> getPluginData();
	signals:
										//数据 - 数据变化（信号）
		void pluginDataChanged();

};
