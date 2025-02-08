#pragma once
#include <QList>
#include <QJsonObject>

/*
-----==========================================================-----
		类：		插件数据 实体类.h
		版本：		v1.05
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
	//----数据切片
	protected:
		bool temp_inited;				//初始化标记
		QString temp_version;			//版本(用到时才初始化）
		QString temp_type;				//类型（用到时才初始化）
		QString temp_name;				//中文名（用到时才初始化）
	public:
										//数据切片 - 获取版本（切片来自 插件描述）
		QString getDescription_version();
		double getDescription_versionNum();
										//数据切片 - 获取类型（切片来自 插件描述）
		QString getDescription_type();
										//数据切片 - 获取中文名（切片来自 插件描述）
		QString getDescription_name();
	protected:
										//数据切片 - 初始化（私有）
		void initDescription_data();

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
