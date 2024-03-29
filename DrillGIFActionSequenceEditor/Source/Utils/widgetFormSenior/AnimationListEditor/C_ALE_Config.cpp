﻿#include "stdafx.h"
#include "C_ALE_Config.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 配置类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧编辑块 的配置类。
					
		说明：		该数据类继承于C_PLE_Config。
-----==========================================================-----
*/

C_ALE_Config::C_ALE_Config()
	: C_PLE_Config()
{
	this->m_isMultiSelect = true;		//（固定为多选模式）

	// > 帧配置
	this->m_frameText_isBold = false;
}
C_ALE_Config::~C_ALE_Config(){
}


/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_ALE_Config::getJsonObject(){
	QJsonObject obj = C_PiS_Config::getJsonObject();

	// > 帧配置
	obj.insert("m_frameText_isBold", this->m_frameText_isBold);
	
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ALE_Config::setJsonObject(QJsonObject obj){
	C_PiS_Config::setJsonObject(obj);
	this->m_isMultiSelect = true;		//（固定为多选模式）

	// > 帧配置
	if (obj.value("m_frameText_isBold").isUndefined() == false){ this->m_frameText_isBold = obj.value("m_frameText_isBold").toBool(); }

}