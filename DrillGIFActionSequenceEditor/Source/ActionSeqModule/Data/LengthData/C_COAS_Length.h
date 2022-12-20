﻿#pragma once


/*
-----==========================================================-----
		类：		最大值 数据类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		最大值 的数据类。
					（详细见cpp）
-----==========================================================-----
*/
class C_COAS_Length{

	public:
		C_COAS_Length();
		~C_COAS_Length();
		
	//-----------------------------------
	//----数据
	public:
		int realLen_actionSeq;			//长度 - 动画序列
		int realLen_action;				//长度 - 动作元
		int realLen_state;				//长度 - 状态元
		int realLen_stateNode;			//长度 - 状态节点

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