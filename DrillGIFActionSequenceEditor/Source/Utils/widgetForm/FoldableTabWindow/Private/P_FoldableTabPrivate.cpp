#include "stdafx.h"
#include "P_FoldableTabPrivate.h"

/*
-----==========================================================-----
		类：		折叠选项卡 控件类（私有）.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		折叠功能使用的私有类。
-----==========================================================-----
*/
P_FoldableTabPrivate::P_FoldableTabPrivate(){
	this->name = "";
	this->partWidget = nullptr;
	this->isInChildWindow = false;
	this->option = QJsonObject();
}
P_FoldableTabPrivate::~P_FoldableTabPrivate(){
}

/*-------------------------------------------------
		控件块 - 在选项卡中
*/
bool P_FoldableTabPrivate::isInTab(){
	return this->isInChildWindow == false;
}
/*-------------------------------------------------
		控件块 - 在子窗口中
*/
bool P_FoldableTabPrivate::isInWindow(){
	return this->isInChildWindow == true;
}
/*-------------------------------------------------
		控件块 - 空判断
*/
bool P_FoldableTabPrivate::isEmpty(){
	if (this->name == ""){ return true; }
	if (this->partWidget == nullptr){ return true; }
	return false;
}