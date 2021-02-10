#include "stdafx.h"

#include "c_FCTClassify.h"

/*
-----==========================================================-----
		类：		类型 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		灵活分类树的 类型 数据类。
					
		子功能：
					->空对象
					
-----==========================================================-----
*/
C_FCTClassify::C_FCTClassify(){
	this->name = "";
	this->description = "";
}
C_FCTClassify::C_FCTClassify(QString name, QString description){
	this->name = name;
	this->description = description;
}
C_FCTClassify::~C_FCTClassify(){
}


/*-------------------------------------------------
		运算符重载
*/
const bool C_FCTClassify::operator== (const C_FCTClassify& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_FCTClassify::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("description", this->description);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_FCTClassify::setJsonObject(QJsonObject obj){

	this->name = obj.value("name").toString();
	this->description = obj.value("description").toString();
}
/*-------------------------------------------------
		空判断
*/
bool C_FCTClassify::isNull(){
	return this->name == "" ;
}