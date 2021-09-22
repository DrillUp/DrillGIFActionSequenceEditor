#include "stdafx.h"
#include "C_PluginData.h"

/*
-----==========================================================-----
		类：		插件数据 实体类.cpp
		作者：		drill_up
		所属模块：	插件模块
		功能：		存储插件信息的类，对应plugin.js中每条插件的配置数据。

		说明：		1.由于底层基于QJsonDocument，重写后，顺序一定会乱。
					2.parameters的值，不是 字符串object 。
-----==========================================================-----
*/
C_PluginData::C_PluginData(){
	this->name = "";					//存储数据 - 插件名
	this->status = false;				//存储数据 - 状态
	this->description = "";				//存储数据 - 插件描述
	this->parameters = QJsonObject();	//存储数据 - 参数
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
