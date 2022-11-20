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
class C_FoldableTabPrivate{

	public:
		C_FoldableTabPrivate();
		~C_FoldableTabPrivate();

	public:
		QString name;				//控件块 名称
		QWidget* partWidget;		//控件块 对象
		bool isInChildWindow;		//在子窗口标记

		QJsonObject param;			//其他参数（暂时无用）
		
	public:
									//空判断
		bool isEmpty();
};