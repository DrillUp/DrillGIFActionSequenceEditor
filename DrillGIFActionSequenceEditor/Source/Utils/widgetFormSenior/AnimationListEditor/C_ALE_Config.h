﻿#pragma once
#include "stdafx.h"
#include "Source/Utils/WidgetForm/PictureListEditor/C_PLE_Config.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 配置类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧编辑块 的配置类。
					（详细见cpp）
-----==========================================================-----
*/

class C_ALE_Config : public C_PLE_Config 
{

	public:
		C_ALE_Config();
		~C_ALE_Config();
		
	//-----------------------------------
	//----帧配置
	public:
		bool m_frameText_isBold;		//帧文本加粗

	//-----------------------------------
	//----类属性
	public:
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject() override;
									//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj) override;
};