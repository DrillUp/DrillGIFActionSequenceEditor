#pragma once
#include "stdafx.h"


/*
-----==========================================================-----
		类：		动画序列 数据类.cpp
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		动画序列 的数据类。
					
		说明：		
-----==========================================================-----
*/
class C_ActionSeq{

	public:
		C_ActionSeq();
		~C_ActionSeq();
		
	//-----------------------------------
	//----数据
	public:
		QString m_name;
		QStringList m_state_default_randomSeq;
		QList<QJsonObject> m_act_tank;
		QList<QJsonObject> m_state_tank;
		QList<QJsonObject> m_stateNode_tank;

	//-----------------------------------
	//----类属性
	public:
								//实体类 -> QJsonObject
		QJsonObject getJsonObject();
								//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
};