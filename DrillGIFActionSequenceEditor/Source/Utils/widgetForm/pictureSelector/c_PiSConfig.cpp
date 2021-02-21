#include "stdafx.h"
#include "c_PiSConfig.h"

/*
-----==========================================================-----
		类：		图片配置 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		图片配置 的数据类。
					
		子功能：
					->空对象
					
		说明：		height高度是可以调整的，
					但是由于每次修改config，都需要重建贴图和bitmap，系统速度会变慢。
					
-----==========================================================-----
*/
C_PiSConfig::C_PiSConfig(){

	this->zeroFill = false;
	this->zeroFillCount = 3;
	this->zeroFillChar = '0';

	this->height = 140;				//高度（控件设置）
	this->isMultiSelect = false;	//多选开关
}
C_PiSConfig::~C_PiSConfig(){
}


/*-------------------------------------------------
		获取图片高度
*/
int C_PiSConfig::getItemHeight(){
	return this->height - 28;		//（固定28像素的距离）
}
/*-------------------------------------------------
		获取换行字符串（\n 撑开选中的 蓝色方块）
*/
QString C_PiSConfig::getLineExpand(){
	QString result = "　";
	int count = qFloor(this->getItemHeight() / 16);
	for (int i = 0; i < count; i++){
		result += "\n　";
	}
	return result;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_PiSConfig::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("zeroFill", this->zeroFill);
	obj.insert("zeroFillCount", this->zeroFillCount);
	obj.insert("zeroFillChar", QString(this->zeroFillChar));
	//（不含 高度 和 多选开关）
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_PiSConfig::setJsonObject(QJsonObject obj){

	if (obj.value("zeroFill").isUndefined() == false){ this->zeroFill = obj.value("zeroFill").toBool(); }
	if (obj.value("zeroFillCount").isUndefined() == false){ this->zeroFillCount = obj.value("zeroFillCount").toInt(); }
	if (obj.value("zeroFillChar").isUndefined() == false){ this->zeroFillChar = obj.value("zeroFillChar").toString().at(0); }
	//（不含 高度 和 多选开关）
}