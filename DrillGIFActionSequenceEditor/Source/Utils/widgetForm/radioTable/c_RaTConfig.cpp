#include "stdafx.h"

#include "c_RaTConfig.h"

/*
-----==========================================================-----
		类：		表格配置 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		单选表格配置 的数据类。
					
		子功能：
					->空对象
					
-----==========================================================-----
*/
C_RaTConfig::C_RaTConfig(){

	this->rowHeight = 24;
	this->zeroFill = true;
	this->zeroFillCount = 4;
	this->zeroFillChar = '0';

	this->isMultiSelect = false;
}
C_RaTConfig::~C_RaTConfig(){
}


/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_RaTConfig::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("rowHeight", this->rowHeight);
	obj.insert("zeroFill", this->zeroFill);
	obj.insert("zeroFillCount", this->zeroFillCount);
	obj.insert("zeroFillChar", QString(this->zeroFillChar));

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_RaTConfig::setJsonObject(QJsonObject obj){

	if (obj.value("rowHeight").isUndefined() == false){ this->rowHeight = obj.value("rowHeight").toInt(); }
	if (obj.value("zeroFill").isUndefined() == false){ this->zeroFill = obj.value("zeroFill").toBool(); }
	if (obj.value("zeroFillCount").isUndefined() == false){ this->zeroFillCount = obj.value("zeroFillCount").toInt(); }
	if (obj.value("zeroFillChar").isUndefined() == false){ this->zeroFillChar = obj.value("zeroFillChar").toString().at(0); }

}