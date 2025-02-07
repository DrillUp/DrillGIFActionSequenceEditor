#include "stdafx.h"
#include "C_FCT_Classify.h"

/*
-----==========================================================-----
		类：		种类 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		灵活分类树的 种类 数据类。
					
		子功能：
					->空对象
					
-----==========================================================-----
*/
C_FCT_Classify::C_FCT_Classify(){
	this->name = "";
	this->description = "";
}
C_FCT_Classify::~C_FCT_Classify(){
}


/*-------------------------------------------------
		运算符重载
*/
QString C_FCT_Classify::getName(){
	return this->name;
}
void C_FCT_Classify::setName(QString name){
	this->name = name;
}
QString C_FCT_Classify::getDescription(){
	return this->description;
}
void C_FCT_Classify::setDescription(QString description){
	this->description = description;
}


/*-------------------------------------------------
		自定义数据 - 获取数据
*/
QJsonObject C_FCT_Classify::getCustomData(){
	return this->data;
}
/*-------------------------------------------------
		自定义数据 - 获取类名
*/
QString C_FCT_Classify::getCustomClassName(){
	return "种类";
}
/*-------------------------------------------------
		自定义数据 - 获取对象名称
*/
QString C_FCT_Classify::getCustomObjectName(){
	if (this->name == ""){ return "未分类"; }
	return this->name;
}


/*-------------------------------------------------
		模板复制
*/
void C_FCT_Classify::copyBaseDataFrom(C_FCT_Classify* base){
	this->description = base->description;
	this->data = base->data;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_FCT_Classify::getJsonObject(){
	QJsonObject obj;

	// > 基本数据
	obj.insert("name", this->name);
	obj.insert("description", this->description);

	// > 自定义数据
	obj.insert("data", this->data);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_FCT_Classify::setJsonObject(QJsonObject obj){

	// > 基本数据
	this->name = obj.value("name").toString();
	this->description = obj.value("description").toString();

	// > 自定义数据
	this->data = obj.value("data").toObject();
}
/*-------------------------------------------------
		空判断
*/
bool C_FCT_Classify::isNull(){
	return this->name == "" ;
}