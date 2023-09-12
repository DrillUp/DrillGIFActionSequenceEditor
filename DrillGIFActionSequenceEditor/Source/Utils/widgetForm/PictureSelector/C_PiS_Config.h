#pragma once
#include "stdafx.h"


/*
-----==========================================================-----
		类：		图片选择块 配置类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		图片选择块 的配置类。
					（详细见cpp）
-----==========================================================-----
*/
class C_PiS_Config{

	public:
		C_PiS_Config();
		~C_PiS_Config();
		
	//-----------------------------------
	//----数据
	public:
		bool m_zeroFill;				//零填充 - 开关
		int m_zeroFillCount;			//零填充 - 位数
		QChar m_zeroFillChar;			//零填充 - 填充的字符

		int m_height;					//控件 - 图片单块高度
		bool m_isMaskEnabled;			//控件 - 是否显示马赛克背景
		bool m_isMultiSelect;			//控件 - 是否多选
	public:
									//数据 - 获取图片高度
		int getItemHeight();
									//数据 - 获取换行字符串（\n 撑开选中的 蓝色方块）
		QString getLineExpand();
		
	//-----------------------------------
	//----类属性
	public:
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj);
};