#pragma once
#include "stdafx.h"
#include "Source/Utils/WidgetForm/PictureSelector/C_PiS_Config.h"

/*
-----==========================================================-----
		类：		图片编辑块 配置类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		图片编辑块 的配置类。
					（详细见cpp）
-----==========================================================-----
*/

class C_PLE_Config : public C_PiS_Config {

	public:
		C_PLE_Config();
		~C_PLE_Config();
		
	//-----------------------------------
	//----类属性
	public:
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject() override;
									//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj) override;
};