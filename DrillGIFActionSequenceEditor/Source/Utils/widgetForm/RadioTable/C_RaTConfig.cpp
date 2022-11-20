#include "stdafx.h"
#include "C_RaTConfig.h"

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

	this->showNumber = true;			//显示序号
	this->zeroFill = true;				//零填充
	this->zeroFillCount = 4;			//填充位数
	this->zeroFillChar = '0';			//填充字符

	this->rowHeight = 24;				//行高
	this->isMultiSelect = false;		//多选开关
}
C_RaTConfig::~C_RaTConfig(){
}


/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_RaTConfig::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("showNumber", this->showNumber);
	obj.insert("zeroFill", this->zeroFill);
	obj.insert("zeroFillCount", this->zeroFillCount);
	obj.insert("zeroFillChar", QString(this->zeroFillChar));

	obj.insert("rowHeight", this->rowHeight);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_RaTConfig::setJsonObject(QJsonObject obj){

	if (obj.value("showNumber").isUndefined() == false){ this->showNumber = obj.value("showNumber").toBool(); }
	if (obj.value("zeroFill").isUndefined() == false){ this->zeroFill = obj.value("zeroFill").toBool(); }
	if (obj.value("zeroFillCount").isUndefined() == false){ this->zeroFillCount = obj.value("zeroFillCount").toInt(); }
	if (obj.value("zeroFillChar").isUndefined() == false){ this->zeroFillChar = obj.value("zeroFillChar").toString().at(0); }

	if (obj.value("rowHeight").isUndefined() == false){ this->rowHeight = obj.value("rowHeight").toInt(); }
}