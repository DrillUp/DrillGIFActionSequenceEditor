﻿#ifndef I_FCTBranch_H
#define I_FCTBranch_H
#include "stdafx.h"

#include "Source/Utils/Common/TTool.h"
#include "C_FPT_Config.h"

/*
-----==========================================================-----
		类：		树枝.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		树枝控件类。
-----==========================================================-----
*/
class I_FPT_Branch : public QTreeWidgetItem{

	public:
		I_FPT_Branch(C_FPT_Config* config, QTreeWidgetItem *parent = nullptr);
		~I_FPT_Branch();

	//-----------------------------------
	//----数据
		TTOOL_DATA_PROPERTY(int, Id);							//序号

		TTOOL_DATA_PROPERTY(int, Branch_id_Index);				//ID分支 - 当前页索引的位置
		TTOOL_DATA_PROPERTY(int, Branch_id_Bottom);				//ID分支 - 底
		TTOOL_DATA_PROPERTY(int, Branch_id_Top);				//ID分支 - 顶

		TTOOL_DATA_PROPERTY(int, Branch_name_Index);			//名称分支 - 当前页索引的位置
		TTOOL_DATA_PROPERTY(QString, Branch_name_Showing);		//名称分支 - 名称查找字符的标志
		TTOOL_DATA_PROPERTY(QString, Branch_name_Symbol);		//名称分支 - 名称查找字符的标志

		TTOOL_DATA_PROPERTY(QString, Branch_type_Name);			//种类分支 - 类型
		TTOOL_DATA_PROPERTY(QString, Branch_type_Description);	//种类分支 - 描述
		
	protected:
		C_FPT_Config* m_config;		//树设置

	//-----------------------------------
	//----控件
	public:
										//控件 - 获取显示文本
		QString getShowingText();
										//控件 - 根据数据自我刷新
		void refreshItemSelf();	

};

#endif // I_FCTBranch_H
