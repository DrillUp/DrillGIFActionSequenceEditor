#pragma once
#include <QList>
#include <QJsonObject>

/*
-----==========================================================-----
		类：		插件数据 实体类.h
		作者：		drill_up
		所属模块：	插件模块
		功能：		存储插件信息的类，对应plugin.js中每条插件的配置数据。
					（详细见cpp）
-----==========================================================-----
*/
class C_PluginData{

	public:
		C_PluginData();
		~C_PluginData();
		
	//-----------------------------------
	//----存储数据
	public:
		QString name;					//存储数据 - 插件名
		bool status;					//存储数据 - 状态
		QString description;			//存储数据 - 插件描述
		QJsonObject parameters;			//存储数据 - 参数
	
	//-----------------------------------
	//----类属性
	public:
												//实体类 -> QJsonObject
		QJsonObject getJsonObject();
												//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
												//运算符重载
		const bool operator== (const C_PluginData& a)const;
};
