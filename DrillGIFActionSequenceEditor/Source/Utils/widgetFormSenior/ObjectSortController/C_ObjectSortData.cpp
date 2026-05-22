#include "stdafx.h"
#include "C_ObjectSortData.h"

#include "Source/Utils/Manager/ChineseManager/S_ChineseManager.h"


/*
-----==========================================================-----
		类：		排序数据类.cpp
		所属模块：	工具模块
		功能：		用于排序的对象数据。
					（该类不含QJsonObject的回转，因为不知道obj的 key-value 中的key值）
-----==========================================================-----
*/
C_ObjectSortData::C_ObjectSortData(){

	// > 原数据
	this->m_org_arrIndex = -1;
	this->m_org_id = -1;
	this->m_org_name.clear();
	this->m_org_type.clear();

	// > 排序用数据
	this->m_sort_id = -1;
	this->m_sort_name.clear();
	this->m_sort_type.clear();
}
C_ObjectSortData::~C_ObjectSortData(){
}



/*-------------------------------------------------
		原数据 - 执行初始化
*/
void C_ObjectSortData::initData(int arrIndex, int id, QString name, QString type){
	this->m_org_arrIndex = arrIndex;
	this->setID(id);
	this->setName(name);
	this->setType(type);
}
/*-------------------------------------------------
		原数据 - 名称初始化
*/
void C_ObjectSortData::setID(int id){
	this->m_org_id = id;
	this->m_sort_id = id;
}
/*-------------------------------------------------
		原数据 - 名称初始化
*/
void C_ObjectSortData::setName(QString name){
	this->m_org_name = name;
	this->m_sort_name = S_ChineseManager::getInstance()->getChineseFirstSpell_ExcludeBracket(name);
}
/*-------------------------------------------------
		原数据 - 所属分类初始化
*/
void C_ObjectSortData::setType(QString type){
	this->m_org_type = type;
	this->m_sort_type = S_ChineseManager::getInstance()->getChineseFirstSpell_ExcludeBracket(type);
}


/*-------------------------------------------------
		获取 - 数组中所在位置
*/
int C_ObjectSortData::getArrIndex(){
	return this->m_org_arrIndex;
}
/*-------------------------------------------------
		获取 - ID
*/
int C_ObjectSortData::getID_Sort(){
	return this->m_sort_id;
}
int C_ObjectSortData::getID_Org(){
	return this->m_org_id;
}
/*-------------------------------------------------
		获取 - 名称
*/
QString C_ObjectSortData::getName_Sort(){
	return this->m_sort_name;
}
QString C_ObjectSortData::getName_Org(){
	return this->m_org_name;
}
/*-------------------------------------------------
		获取 - 所属分类
*/
QString C_ObjectSortData::getType_Sort(){
	return this->m_sort_type;
}
QString C_ObjectSortData::getType_Org(){
	return this->m_org_type;
}



/*-------------------------------------------------
		运算符重载
*/
const bool C_ObjectSortData::operator== (const C_ObjectSortData& a)const {
	return this->m_org_id == a.m_org_id;
}
/*-------------------------------------------------
		空判断
*/
bool C_ObjectSortData::isNull(){
	return this->m_org_id == -1;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ObjectSortData::setJsonObject(QJsonObject obj){
	if (obj.isEmpty()){ return; }			//（空对象也可以排序，只是没有意义）

	QJsonValue temp_id = obj.value("id");
	if (temp_id.isUndefined()){ temp_id = obj.value("Id"); }
	if (temp_id.isUndefined()){ temp_id = obj.value("ID"); }
	if (temp_id.isUndefined()){ return; }	//（空id也可以排序，只是没有意义）
	this->setID(temp_id.toInt());

	QString temp_name = obj.value("name").toString();
	if (temp_name == ""){ temp_name = obj.value("Name").toString(); }
	if (temp_name == ""){ temp_name = obj.value("NAME").toString(); }
	this->setName(temp_name);
	
	QString temp_type = obj.value("type").toString();
	if (temp_type == ""){ temp_type = obj.value("Type").toString(); }
	if (temp_type == ""){ temp_type = obj.value("TYPE").toString(); }
	this->setType(temp_type);

}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ObjectSortData::setJsonObject(QJsonObject obj, QString key_id, QString key_name, QString key_type){
	if (obj.isEmpty()){ return; }			//（空对象也可以排序，只是没有意义）

	QJsonValue temp_id = obj.value(key_id);
	if (temp_id.isUndefined()){ return; }	//（空id也可以排序，只是没有意义）
	if (temp_id.isString()){
		this->setID( temp_id.toString().toInt() );
	}else{
		this->setID( temp_id.toInt() );
	}

	this->setName(obj.value(key_name).toString());
	this->setType(obj.value(key_type).toString());
}