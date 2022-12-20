#pragma once
#include "stdafx.h"


/*
-----==========================================================-----
		类：		动作元 数据类.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		动作元 的数据类。
					（详细见cpp）
-----==========================================================-----
*/
CLASS_PTR(C_COAS_Action);
class C_COAS_Action{

	public:
		C_COAS_Action();
		~C_COAS_Action();
		
	//-----------------------------------
	//----数据
	public:
		int id;							//序号

		QString name;					//常规 - 动作元名称
		QStringList tag_tank;			//常规 - 动作元标签
		int priority;					//常规 - 动作元优先级
		
		QStringList gif_src;			//GIF - 资源
		QString gif_src_file;			//GIF - 资源文件夹
		QList<int> gif_intervalTank;	//GIF - 帧间隔-明细表
		int gif_interval;				//GIF - 帧间隔
		bool gif_back_run;				//GIF - 是否倒放
		bool gif_preload;				//GIF - 是否预加载

		int tint;						//图像 - 色调值
		bool smooth;					//图像 - 模糊边缘

		QString note;					//杂项 - 备注
	public:
								//数据 - 清除数据
		void clearData();

	//-----------------------------------
	//----类属性
	public:
								//空判断
		bool isNull();
								//实体类 -> QJsonObject
		QJsonObject getJsonObject_Chinese();
								//QJsonObject -> 实体类
		void setJsonObject_Chinese(QJsonObject obj, int i);
};