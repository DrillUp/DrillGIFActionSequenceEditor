#pragma once
#include "stdafx.h"
#include "Source/Utils/WidgetForm/PictureSelector/C_PiS_Config.h"

/*
-----==========================================================-----
		类：		动画帧 数据类（配置）.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧 的数据类。
					（详细见cpp）
-----==========================================================-----
*/
class C_ALE_Config : public C_PiS_Config {

	public:
		C_ALE_Config();
		~C_ALE_Config();
		
	//-----------------------------------
	//----常规配置
	protected:
		QString m_sizeMode;			//尺寸模式
	public:
		int m_defaultInterval;		//默认帧间隔
		bool m_isUseFileSys;		//使用文件夹管理器

	public:
									//设置尺寸模式
		void setSizeMode(QString sizeMode);
		QString getSizeMode();

	//-----------------------------------
	//----类属性
	public:
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj);
};