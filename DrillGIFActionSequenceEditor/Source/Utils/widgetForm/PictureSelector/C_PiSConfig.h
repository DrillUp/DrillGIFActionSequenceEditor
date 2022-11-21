#pragma once
#include "stdafx.h"


/*
-----==========================================================-----
		类：		图片配置 数据类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		图片配置 的数据类。
					（详细见cpp）
-----==========================================================-----
*/
class C_PiSConfig{

	public:
		C_PiSConfig();
		~C_PiSConfig();
		
	//-----------------------------------
	//----数据
	public:
		bool m_zeroFill;				//填充开关
		int m_zeroFillCount;			//填充数量
		QChar m_zeroFillChar;			//填充的字符
		bool m_isMaskEnabled;			//马赛克背景开关

		int m_height;					//高度（控件设置）
		bool m_isMultiSelect;			//多选开关
		
	//-----------------------------------
	//----类属性
	public:
									//获取 - 图片高度
		int getItemHeight();
									//获取 - 换行字符串（\n 撑开选中的 蓝色方块）
		QString getLineExpand();
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj);
};