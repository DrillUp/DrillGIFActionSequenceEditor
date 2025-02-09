#include "stdafx.h"
#include "C_LEConfigData.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		待修改的值.cpp
		作者：		drill_up
		所属模块：	插件模块
		功能：		写入器需要该数据类，填入待修改的值，才能执行修改。
-----==========================================================-----
*/
C_LEConfigData::C_LEConfigData(){

	// > 修改值
	this->pluginName = "";
	this->paramKey = "";
	this->configLen = -1;
}
C_LEConfigData::~C_LEConfigData(){
}

/*-------------------------------------------------
		修改值 - 初始化
*/
void C_LEConfigData::initParam(QString pluginName, QString paramKey, int realLen){
	this->pluginName = pluginName;
	this->paramKey = paramKey;
	this->configLen = realLen;
}
/*-------------------------------------------------
		修改值 - 获取 插件名（英文）
*/
QString C_LEConfigData::getPluginName(){
	return this->pluginName;
}
/*-------------------------------------------------
		修改值 - 获取 变量键（"阶段-%d"）
*/
QString C_LEConfigData::getParamKey(){
	return this->paramKey;
}
/*-------------------------------------------------
		修改值 - 获取 变量显示名（"阶段"）
*/
QString C_LEConfigData::getParamShowingName(){
	return this->paramKey.split("-").at(0);
}
/*-------------------------------------------------
		修改值 - 获取 当前配置最大值
*/
int C_LEConfigData::getConfigLen(){
	return this->configLen;
}

/*-------------------------------------------------
		空判断
*/
bool C_LEConfigData::isNull(){
	return this->paramKey == "" || this->pluginName == "";
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_LEConfigData::getJsonObject(){
	QJsonObject obj = QJsonObject();

	// > 修改值
	obj.insert("pluginName", this->pluginName);
	obj.insert("paramKey", this->paramKey);
	obj.insert("realLen", this->configLen);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_LEConfigData::setJsonObject(QJsonObject obj){

	// > 修改值
	this->pluginName = obj.value("pluginName").toString();
	this->paramKey = obj.value("paramKey").toString();
	this->configLen = obj.value("realLen").toInt();
}
/*-------------------------------------------------
		运算符重载
*/
const bool C_LEConfigData::operator== (const C_LEConfigData& a)const {
	return this->pluginName == a.pluginName && this->paramKey == a.paramKey;
}