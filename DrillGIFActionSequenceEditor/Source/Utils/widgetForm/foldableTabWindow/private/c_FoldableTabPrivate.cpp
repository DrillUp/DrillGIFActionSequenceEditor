#include "stdafx.h"
#include "C_FoldableTabPrivate.h"

/*
-----==========================================================-----
		类：		折叠选项卡 控件类（私有）.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		折叠功能使用的私有类。
-----==========================================================-----
*/
C_FoldableTabPrivate::C_FoldableTabPrivate(){
	this->name = "";
	this->partWidget = nullptr;
	this->isInChildWindow = false;
	this->param = QJsonObject();
}
C_FoldableTabPrivate::~C_FoldableTabPrivate(){
}

/*-------------------------------------------------
		空判断
*/
bool C_FoldableTabPrivate::isEmpty(){
	if (this->name == ""){ return true; }
	if (this->partWidget == nullptr){ return true; }
	return false;
}