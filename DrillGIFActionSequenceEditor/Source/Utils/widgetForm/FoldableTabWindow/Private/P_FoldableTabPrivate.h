#pragma once
#include "stdafx.h"

#include <QWidget>

/*
-----==========================================================-----
		类：		折叠选项卡 控件类（私有）.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		折叠功能使用的私有类。
-----==========================================================-----
*/
class P_FoldableTabPrivate{

	public:
		P_FoldableTabPrivate();
		~P_FoldableTabPrivate();

	public:
		QString name;				//控件块 名称
		QWidget* partWidget;		//控件块 对象

		bool isInChildWindow;		//是否在子窗口中

		QJsonObject option;			//选项参数（暂时无用）
		
	public:
									//控件块 - 在选项卡中
		bool isInTab();
									//控件块 - 在子窗口中
		bool isInWindow();
									//控件块 - 空判断
		bool isEmpty();
};