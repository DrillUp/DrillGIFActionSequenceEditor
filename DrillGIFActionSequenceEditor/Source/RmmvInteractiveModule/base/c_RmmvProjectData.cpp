#include "stdafx.h"
#include "C_RmmvProjectData.h"

/*
-----==========================================================-----
		类：		rmmv工程数据 实体类.cpp
		版本：		v1.00
		所属模块：	交互模块
		功能：		工程基本信息数据。
-----==========================================================-----
*/
C_RmmvProjectData::C_RmmvProjectData(){
	this->name = "";			//工程名
	this->version = "";			//工程版本
	this->path = "";			//工程根路径
}
C_RmmvProjectData::~C_RmmvProjectData(){
}


/*-------------------------------------------------
		空判断
*/
bool C_RmmvProjectData::isNull(){
	return this->name == "" || this->path == "";
}
/*-------------------------------------------------
		运算符重载
*/
const bool C_RmmvProjectData::operator== (const C_RmmvProjectData& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_RmmvProjectData::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("version", this->version);
	obj.insert("path", this->path);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_RmmvProjectData::setJsonObject(QJsonObject obj){
	this->name = obj.value("name").toString();
	this->version = obj.value("version").toString();
	this->path = obj.value("path").toString();
}
