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
		QList<C_PluginData*> data_PluginDataTank;
	public:
										//数据 - 获取全部插件数据
										//		【说明】：获取数据前，要确保你之前 手动 执行了read读取，不然获取不到数据。
		QList<C_PluginData*> getPluginDataTank();
										//数据 - 获取对应的插件数据
										//		【说明】：获取数据前，要确保你之前 手动 执行了read读取，不然获取不到数据。
										//		【说明】：没有则返回空指针。
		C_PluginData* getPluginData(QString pluginName);
	signals:
										//数据 - 数据变化（信号）
		void pluginDataReloaded();

	//-----------------------------------
	//----操作
	public:
										//操作 - 添加
		void op_add(C_PluginData* data);
										//操作 - 修改
		void op_modify(C_PluginData* data);
										//操作 - 删除
		void op_remove(C_PluginData* data);


	//-----------------------------------
	//----读取
	public:
										//读取 - 读取数据（plugins.js文本）
		void loadPluginData(QString data_context);
										//读取 - 读取数据（plugins.js文本，不发信号）
		void loadPluginDataNoSignal(QString data_context);
	private:
										//读取 - 读取数据（私有）
		QList<C_PluginData*> readPluginData(QString data_context);
		
	//-----------------------------------
	//----写入
	public:
										//写入 - 写入数据（plugins.js文本）
		QString writePluginData();



};
