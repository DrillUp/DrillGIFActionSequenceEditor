#include "stdafx.h"
#include "C_ObjectSortData.h"


/*
-----==========================================================-----
		类：		排序数据类.cpp
		所属模块：	工具模块
		功能：		用于排序的对象数据。
					（该类不含QJsonObject的回转，因为symbol标识不明确）
-----==========================================================-----
*/
C_ObjectSortData::C_ObjectSortData(){
	this->arrIndex = -1;
	this->id = -1;
	this->name = "";
	this->type = "";
}
C_ObjectSortData::C_ObjectSortData(int arr_index, int id, QString name, QString type){
	this->arrIndex = arr_index;
	this->id = id;
	this->name = name;
	this->type = type;
}
C_ObjectSortData::~C_ObjectSortData(){
}


/*-------------------------------------------------
		运算符重载
*/
const bool C_ObjectSortData::operator== (const C_ObjectSortData& a)const {
	return this->id == a.id;
}
/*-------------------------------------------------
		空判断
*/
bool C_ObjectSortData::isNull(){
	return this->id == -1;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ObjectSortData::setJsonObject(QJsonObject obj){
	if (obj.isEmpty()){ return; }

	QJsonValue v_id = obj.value("id");
	if (v_id.isUndefined()){ v_id = obj.value("Id"); }
	if (v_id.isUndefined()){ v_id = obj.value("ID"); }
	if (v_id.isUndefined()){ return; }
	this->id = v_id.toInt();

	QString name = obj.value("name").toString();
	if (name == ""){ name = obj.value("Name").toString(); }
	if (name == ""){ name = obj.value("NAME").toString(); }
	this->name = name;
	
	QString type = obj.value("type").toString();
	if (type == ""){ type = obj.value("Type").toString(); }
	if (type == ""){ type = obj.value("TYPE").toString(); }
	this->type = type;

}
void C_ObjectSortData::setJsonObject(QJsonObject obj, QString id_symbol, QString name_symbol, QString type_symbol){
	if (obj.isEmpty()){ return; }

	QJsonValue v_id = obj.value(id_symbol);
	if (v_id.isUndefined()){ return; }
	if (v_id.isString()){ this->id = v_id.toString().toInt(); }
	else{ this->id = v_id.toInt(); }

	this->name = obj.value(name_symbol).toString();
	this->type = obj.value(type_symbol).toString();
}