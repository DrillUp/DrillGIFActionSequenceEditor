#include "stdafx.h"
#include "C_PluginData.h"

/*
-----==========================================================-----
		类：		插件数据 实体类.cpp
		所属模块：	插件模块
		功能：		存储插件信息数据。

		必要事项：	1.插件本体为一个json格式数据。
					
-----==========================================================-----
*/
C_PluginData::C_PluginData(){
	this->name = "";
	this->status = false;
	this->description = "";
	this->parameters = QJsonObject();
}
C_PluginData::~C_PluginData(){
}


/*-------------------------------------------------
		运算符重载
*/
const bool C_PluginData::operator== (const C_PluginData& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_PluginData::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("status", this->status);
	obj.insert("description", this->description);
	obj.insert("parameters", this->parameters);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_PluginData::setJsonObject(QJsonObject obj){
	this->name = obj.value("name").toString();
	this->status = obj.value("status").toBool();
	this->description = obj.value("description").toString();
	this->parameters = obj.value("parameters").toObject();
}
