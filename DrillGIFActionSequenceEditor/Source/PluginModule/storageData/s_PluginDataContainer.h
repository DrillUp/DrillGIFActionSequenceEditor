﻿#pragma once

#include "C_PluginData.h"

/*
-----==========================================================-----
		类：		插件数据 容器.h
		版本：		v1.04
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
										//读取 - 一次性读取数据（plugins.js文本，读取后直接返回列表，不纳入容器）
		QList<C_PluginData*> loadPluginDataDirectly(QString data_context);
	signals:
										//读取 - 数据已读取（信号）
		void pluginDataReloaded();
	private:
										//读取 - 读取数据（私有）
		QList<C_PluginData*> readPluginDataPrivate(QString data_context);
		

	//-----------------------------------
	//----写入
	public:
										//写入 - 写入数据（plugins.js文本）
		QString writePluginData();
										//写入 - 一次性写入数据（plugins.js文本，不包括容器的数据）
		QString writePluginDataDirectly(QList<C_PluginData*> data_list);
	private:
										//写入 - 写入数据（私有）
		QString writePluginDataPrivate(QList<C_PluginData*> data_list);


};
