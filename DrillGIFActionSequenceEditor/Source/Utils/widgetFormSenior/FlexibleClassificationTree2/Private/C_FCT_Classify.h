#pragma once
#include "stdafx.h"

#include <QJsonObject>

/*
-----==========================================================-----
		类：		种类 数据类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		灵活分类树的 种类 数据类。
					（详细见cpp）
-----==========================================================-----
*/
class C_FCT_Classify{

	public:
		C_FCT_Classify();
		~C_FCT_Classify();
		
	//-----------------------------------
	//----基本数据
	protected:
		QString name;			//名称（不能重复）
		QString description;	//描述
	public:
									//基本数据 - 访问器
		QString getName();
		void setName(QString name);
		QString getDescription();
		void setDescription(QString description);

	//-----------------------------------
	//----自定义数据
	public:
		QJsonObject data;
	public:
									//自定义数据 - 获取数据
		QJsonObject getCustomData();
									//自定义数据 - 获取类名
		virtual QString getCustomClassName();	
									//自定义数据 - 获取对象名称
		virtual QString getCustomObjectName();	

	//-----------------------------------
	//----类属性
	public:
									//模板复制
		void copyBaseDataFrom(C_FCT_Classify* base);
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj);
									//空判断
		virtual bool isNull();

};