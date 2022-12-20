#pragma once
#include "stdafx.h"


/*
-----==========================================================-----
		类：		动画序列 数据类.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		动画序列 的数据类。
					（详细见cpp）
-----==========================================================-----
*/
class C_COAS_Data{

	public:
		C_COAS_Data();
		~C_COAS_Data();
		
	//-----------------------------------
	//----数据
	public:
		int m_COAS_id;							//序号
		QString m_COAS_name;					//名称（标签）
		QString m_COAS_type;					//类型
		QStringList m_state_default_randomSeq;
		QList<QJsonObject> m_act_tank;
		QList<QJsonObject> m_state_tank;
		QList<QJsonObject> m_stateNode_tank;
	public:
								//数据 - 清除数据
		void clearTankData();

	//-----------------------------------
	//----类属性
	public:
								//空判断
		bool isNull();
								//实体类 -> QJsonObject
		QJsonObject getJsonObject();
								//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
};