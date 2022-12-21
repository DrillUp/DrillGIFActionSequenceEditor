#pragma once
#include "stdafx.h"


/*
-----==========================================================-----
		类：		状态节点 数据类.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		状态节点 的数据类。
					（详细见cpp）
-----==========================================================-----
*/
CLASS_PTR(C_COAS_StateNode);
class C_COAS_StateNode{

	public:
		C_COAS_StateNode();
		~C_COAS_StateNode();
		
	//-----------------------------------
	//----数据
	public:
		int id;								//序号

		QString name;						//常规 - 节点名称
		QStringList tag_tank;				//常规 - 节点标签
		int priority;						//常规 - 节点优先级
		int proportion;						//常规 - 节点权重
		bool canBeInterrupted;				//常规 - 可被动作元打断

		QString play_type;					//播放列表 - 播放方式
		QStringList play_randomStateSeq;	//播放列表 - 随机播放状态元
		QStringList play_plainStateSeq;		//播放列表 - 顺序播放状态元
		QStringList play_randomNodeSeq;		//播放列表 - 随机播放嵌套集合
		QStringList play_plainNodeSeq;		//播放列表 - 顺序播放嵌套集合
		int play_randomMax;					//播放列表 - 随机播放的次数上限

		QString note;						//杂项 - 备注
	public:
								//数据 - 清除数据
		void clearData();
								//数据 - 判断空节点
		bool isEmptyNode();

	//-----------------------------------
	//----类属性
	public:
								//空判断
		bool isNull();
								//实体类 -> QJsonObject
		QJsonObject getJsonObject_Chinese();
								//QJsonObject -> 实体类
		void setJsonObject_Chinese(QJsonObject obj, int id);
};