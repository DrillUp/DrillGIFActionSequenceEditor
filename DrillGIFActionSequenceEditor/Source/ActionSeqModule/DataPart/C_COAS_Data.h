#pragma once
#include "stdafx.h"

#include "../ActionPart/C_COAS_Action.h"
#include "../StatePart/C_COAS_State.h"
#include "../StateNodePart/C_COAS_StateNode.h"

/*
-----==========================================================-----
		类：		动画序列 数据类.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		动画序列 的数据类。
					（详细见cpp）
-----==========================================================-----
*/
CLASS_PTR(C_COAS_Data);
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
		QList<C_COAS_ActionPtr> m_act_tank;
		QList<C_COAS_StatePtr> m_state_tank;
		QList<C_COAS_StateNodePtr> m_stateNode_tank;
	public:
								//数据 - 清除数据
		void clearTankData();

	//-----------------------------------
	//----类属性
	public:
								//空判断
		bool isNull();
								//实体类 -> QJsonObject
		QJsonObject getJsonObject_Chinese();
								//QJsonObject -> 实体类
		void setJsonObject_Chinese(QJsonObject obj);
};