#pragma once
#include "stdafx.h"


/*
-----==========================================================-----
		类：		图片选择块 配置类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		控件块的数据配置类。
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

		bool m_isMaskEnabled;			//控件 - 是否显示马赛克背景
		bool m_isMaskBorderEnabled;		//控件 - 是否显示马赛克背景边线

		bool m_isMultiSelect;			//控件 - 是否多选

	//-----------------------------------
	//----尺寸
	protected:
		QString m_size_mode;			//尺寸 - 模式（小、中、大、自定义）
		int m_size_customHeight;		//尺寸 - 自定义单块高度
	public:
									//尺寸 - 设置模式（小、中、大）
		void setSize_Mode(QString sizeMode);
									//尺寸 - 设置模式（自定义）
		void setSize_Custom(int customHeight);
									//尺寸 - 获取模式
		QString getSize_Mode();

									//尺寸 - 获取单块宽度
		int getSize_ItemWidth();
									//尺寸 - 获取单块高度
		int getSize_ItemHeight();
									//尺寸 - 获取父对象高度
		int getSize_ParentHeight();

									//尺寸 - 获取换行字符串（\n 撑开选中的 蓝色方块）
		QString getSize_LineExpand();
		
	//-----------------------------------
	//----类属性
	public:
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj);
};