#pragma once

#include "C_PluginData.h"

/*
-----==========================================================-----
		类：		插件数据 容器.h
		版本：		v1.05
		作者：		drill_up
		所属模块：	插件模块
		功能：		管理plugin.js中的全部插件配置数据。
					【封装的容器结构，注意接收读取的信号】
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
		static S_PluginDataContainer* getInstance();		//单例，获取自己


	//-----------------------------------
	//----数据
	protected:
		QList<C_PluginData*> data_PluginDataTank;
	public:
										//数据 - 获取全部插件数据
										//		【说明】：获取数据前，要确保你之前 手动 执行了read读取，不然获取不到数据。
		QList<C_PluginData*> getPluginDataTank();
										//数据 - 获取对应的插件数据
										//		【说明】：获取数据前，要确保你之前 手动 执行了read读取，不然获取不到数据。
										//		【说明】：没有则返回空指针。
		C_PluginData* getPluginData(QString pluginName);

	//-----------------------------------
	//----数据操作
	public:
										//数据操作 - 添加
										//		【说明】：写入数据时用。
		void op_add(C_PluginData* data);
										//数据操作 - 修改
										//		【说明】：写入数据时用。
		void op_modify(C_PluginData* data);
										//数据操作 - 删除
										//		【说明】：写入数据时用。
		void op_remove(C_PluginData* data);


	//-----------------------------------
	//----读取
	public:
										//读取 - 读取数据
										//		【说明】：读取包括plugins.js的全部文本。
		void loadPluginData(QString data_context);
										//读取 - 读取数据（不发信号）
										//		【说明】：读取包括plugins.js的全部文本。
		void loadPluginDataNoSignal(QString data_context);
										//读取 - 读取数据（一次性）
										//		【说明】：一次性读取数据，不纳入当前的数据容器。
		QList<C_PluginData*> loadPluginDataDirectly(QString data_context);
	public:
	signals:
										//读取 - 读取全部数据时（信号）
		void signal_pluginDataReloaded();
	protected:
										//读取 - 读取数据（私有）
		QList<C_PluginData*> readPluginDataPrivate(QString data_context);
		

	//-----------------------------------
	//----写入
	public:
										//写入 - 写入数据
										//		【说明】：plugins.js中的文本会被清空，并写入全部文本。
		QString writePluginData();
										//写入 - 写入数据（一次性）
										//		【说明】：一次性写入数据，不包括当前的数据容器。
		QString writePluginDataDirectly(QList<C_PluginData*> data_list);
	protected:
										//写入 - 写入数据（私有）
		QString writePluginDataPrivate(QList<C_PluginData*> data_list);


};
