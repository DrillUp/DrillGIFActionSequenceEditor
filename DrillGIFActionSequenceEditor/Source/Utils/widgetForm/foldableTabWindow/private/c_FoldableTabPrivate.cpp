#include "stdafx.h"
#include "c_FoldableTabPrivate.h"

/*
-----==========================================================-----
		�ࣺ		�۵�ѡ� �ؼ��ࣨ˽�У�.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�۵�����ʹ�õ�˽���ࡣ
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
		���ж�
*/
bool C_FoldableTabPrivate::isEmpty(){
	if (this->name == ""){ return true; }
	if (this->partWidget == nullptr){ return true; }
	return false;
}