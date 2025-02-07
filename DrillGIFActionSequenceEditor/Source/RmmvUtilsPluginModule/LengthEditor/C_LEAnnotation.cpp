﻿#include "stdafx.h"
#include "C_LEAnnotation.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		插件注解 参数.cpp（只读）
		所属模块：	核心模块
		功能：		插件的固定参数数据。

		说明：		参数真实只存几个需要的数据。
					其他结构调用此 实体类 都是数据的各种组合结果。
					【另外，尽可能保持数据唯一性，避免"阶段-%d"和"阶段"存成两个变量。】
-----==========================================================-----
*/
C_LEAnnotation_Param::C_LEAnnotation_Param(){
	this->paramKey = "";				//变量键（@Drill_LE_param）
	this->parentKey = "";				//变量组键（@Drill_LE_parentKey）
	this->var = "";						//定义名（@Drill_LE_var）
	this->varLen = 0;					//定义最大值
	this->realLen = 0;					//实际最大值
}
C_LEAnnotation_Param::~C_LEAnnotation_Param(){
}

/*-------------------------------------------------
		参数 - 初始化
*/
void C_LEAnnotation_Param::initParam(QString param, QString parentKey, QString var){
	this->paramKey = param;
	this->parentKey = parentKey;
	this->var = var;
}
/*-------------------------------------------------
		获取 - 变量键
*/
QString C_LEAnnotation_Param::getParamKey(){
	return this->paramKey;
}
/*-------------------------------------------------
		获取 - 变量组键
*/
QString C_LEAnnotation_Param::getParentKey(){
	return this->parentKey;
}
/*-------------------------------------------------
		获取 - 定义名
*/
QString C_LEAnnotation_Param::getVarName(){
	return this->var;
}
/*-------------------------------------------------
		获取 - 定义最大值
*/
void C_LEAnnotation_Param::setVarLen(int len){
	this->varLen = len;
}
int C_LEAnnotation_Param::getVarLen(){
	return varLen;
}
/*-------------------------------------------------
		获取 - 实际最大值
*/
void C_LEAnnotation_Param::setRealLen(int len){
	this->realLen = len;
}
int C_LEAnnotation_Param::getRealLen(){
	return realLen;
}
/*-------------------------------------------------
		获取 - 变量显示名
*/
QString C_LEAnnotation_Param::getParamShowingName(){
	return this->paramKey.split("-").at(0);
}
/*-------------------------------------------------
		获取 - 变量名（从1开始计数）
*/
QString C_LEAnnotation_Param::getParamName(int index){
	QString i_str = QString::number(index);
	QString result = this->paramKey;
	result = result.replace("%d", i_str);
	return result;
}
/*-------------------------------------------------
		获取 - 变量组名（从1开始计数）
*/
QString C_LEAnnotation_Param::getParentName(int index){
	if (this->isParentGrouping() == false){ return ""; }
	
	index--;
	int i = floor(index / 20);
	int start = 20 * i + 1;
	int end = 20 * (i + 1);
	QString str_start = QString::number(start);
	if (start == 1){ str_start = " 1"; };
	QString result = this->getParentKey();
	result.replace(result.lastIndexOf("%d"), 2, QString::number(end));
	result.replace("%d", str_start);

	return result;
}
/*-------------------------------------------------
		获取 - 变量注释名（全词匹配用）
*/
QRegExp C_LEAnnotation_Param::getParamCommentRe(int index){
	QString param_re = "@param[ ]*" + this->getParamName(index) + "$";
	return QRegExp(param_re);
}
/*-------------------------------------------------
		获取 - 变量注释组名（全词匹配用）
*/
QRegExp C_LEAnnotation_Param::getParentCommentRe(int index){
	QString parent_re = "@param[ ]*" + this->getParentName(index) + "$";
	return QRegExp(parent_re);
}
/*-------------------------------------------------
		获取 - 是否分组
*/
bool C_LEAnnotation_Param::isParentGrouping(){
	return this->parentKey.contains("%d");
}

/*
-----==========================================================-----
		类：		插件信息.cpp（只读）
-----==========================================================-----
*/
C_LEAnnotation::C_LEAnnotation(){
	this->pluginName = "";								//插件名
	this->pluginDesc = "";								//插件简介（@plugindesc）
	this->pluginAuthor = "";							//插件作者（@author）
	this->paramList = QList<C_LEAnnotation_Param>();	//插件参数
	this->paramForbidden = false;						//禁止编辑（@Drill_LE_editForbidden）

	this->fullPath = "";				//插件文件路径
	this->context = "";					//插件文本
	this->message = "";					//插件消息（读取后，显示的字符串）	
	this->editMessage = "";				//插件修改消息（编辑后，编辑数据的字符串）
}
C_LEAnnotation::~C_LEAnnotation(){
}
C_LEAnnotation_Param C_LEAnnotation::getParamByKey(QString paramKey){
	for (int i = 0; i < this->paramList.count(); i++){
		C_LEAnnotation_Param p = this->paramList.at(i);
		if (p.getParamKey() == paramKey){
			return p;
		}
	}
	return C_LEAnnotation_Param();
}
/*-------------------------------------------------
		判断 - 空判断
*/
bool C_LEAnnotation::isNull(){
	return this->pluginName == "" || this->pluginDesc == "";
}
/*-------------------------------------------------
		判断 - 是否被禁用
*/
bool C_LEAnnotation::isForbidden(){
	return this->paramForbidden;
}
/*-------------------------------------------------
		判断 - 是否可编辑
*/
bool C_LEAnnotation::isEditable(){
	if (this->paramList.count() == 0){ return false; }
	return true;
}
/*-------------------------------------------------
		判断 - 完整性检查
*/
bool C_LEAnnotation::isIntegrity(){
	for (int i = 0; i < this->paramList.count(); i++){
		C_LEAnnotation_Param pluginParam = this->paramList.at(i);

		if (pluginParam.getRealLen() == 0){ return false; }
		if (pluginParam.getVarName() == "null"){ continue; }
		if (pluginParam.getVarLen() == 0){ return false; }

		// > 对比
		if (pluginParam.getRealLen() != pluginParam.getVarLen()){
			return false;
		}
	}
	return true;
}

