#pragma once
#include <QList>
#include <QJsonObject>

/*
-----==========================================================-----
		类：		插件数据 实体类.h
		版本：		v1.04
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
	//----数据
	public:
		QString name;					//插件名
		bool status;					//状态
		QString description;			//插件描述
		QJsonObject parameters;			//参数
	
	//-----------------------------------
	//----临时截断数据
	protected:
		bool temp_inited;				//标记
		QString temp_version;			//版本(调取时才初始化）
		QString temp_type;				//类型（调取时才初始化）
		QString temp_name;				//中文名（调取时才初始化）
	public:
										//数据 - 初始化截取段
		void initDescription_data();
										//数据 - 获取版本（插件描述 截取段）
		QString getDescription_version();
		double getDescription_versionNum();
										//数据 - 获取类型（插件描述 截取段）
		QString getDescription_type();
										//数据 - 获取中文名（插件描述 截取段）
		QString getDescription_name();

	//-----------------------------------
	//----类属性
	public:
										//空判断
		bool isNull();	
										//实体类 -> QJsonObject
		QJsonObject getJsonObject();
										//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
										//运算符重载
		const bool operator== (const C_PluginData& a)const;
};
