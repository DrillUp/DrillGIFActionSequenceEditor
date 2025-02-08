#include "stdafx.h"
#include "C_LEConfigData.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		配置参数.cpp（存储）
-----==========================================================-----
*/
C_LEConfigData::C_LEConfigData(){
	this->pluginName = "";
	this->paramKey = "";
	this->configLen = -1;
}
C_LEConfigData::~C_LEConfigData(){
}

/*-------------------------------------------------
		参数 - 初始化
*/
void C_LEConfigData::initParam(QString pluginName, QString paramKey, int realLen){
	this->pluginName = pluginName;
	this->paramKey = paramKey;
	this->configLen = realLen;
}
/*-------------------------------------------------
		获取 - 插件名（英文）
*/
QString C_LEConfigData::getPluginName(){
	return this->pluginName;
}
/*-------------------------------------------------
		获取 - 变量键 "阶段-%d"
*/
QString C_LEConfigData::getParamKey(){
	return this->paramKey;
}
/*-------------------------------------------------
		获取 - 变量显示名 "阶段"
*/
QString C_LEConfigData::getParamShowingName(){
	return this->paramKey.split("-").at(0);
}
/*-------------------------------------------------
		获取 - 配置最大值
*/
int C_LEConfigData::getConfigLen(){
	return this->configLen;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_LEConfigData::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("pluginName", this->pluginName);
	obj.insert("paramKey", this->paramKey);
	obj.insert("realLen", this->configLen);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_LEConfigData::setJsonObject(QJsonObject obj){
	this->pluginName = obj.value("pluginName").toString();
	this->paramKey = obj.value("paramKey").toString();
	this->configLen = obj.value("realLen").toInt();
}
/*-------------------------------------------------
		空判断
*/
bool C_LEConfigData::isNull(){
	return this->paramKey == "" || this->pluginName == "";
}
/*-------------------------------------------------
		运算符重载
*/
const bool C_LEConfigData::operator== (const C_LEConfigData& a)const {
	return this->pluginName == a.pluginName && this->paramKey == a.paramKey;
}