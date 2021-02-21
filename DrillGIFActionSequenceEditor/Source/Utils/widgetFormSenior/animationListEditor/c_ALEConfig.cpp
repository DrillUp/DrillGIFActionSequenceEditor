#include "stdafx.h"
#include "c_ALEConfig.h"

/*
-----==========================================================-----
		类：		动画帧 数据类（配置）.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧 的数据类。
					
		说明：		该数据类继承于C_PiSConfig。
-----==========================================================-----
*/
C_ALEConfig::C_ALEConfig() : C_PiSConfig() {
	this->sizeMode = "中";
	this->isUseFileSys = false;
}
C_ALEConfig::~C_ALEConfig(){
}


/*-------------------------------------------------
		设置尺寸模式
*/
void C_ALEConfig::setSizeMode(QString sizeMode){
	if (sizeMode == ""){ return; }
	if (this->sizeMode == sizeMode){ return; }
	this->sizeMode = sizeMode;

	if (this->sizeMode == "大"){
		this->height = 200;
	}
	if (this->sizeMode == "中"){
		this->height = 140;
	}
	if (this->sizeMode == "小"){
		this->height = 100;
	}
}

/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_ALEConfig::getJsonObject(){
	QJsonObject obj = C_PiSConfig::getJsonObject();
	obj.insert("sizeMode", this->sizeMode);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ALEConfig::setJsonObject(QJsonObject obj){
	C_PiSConfig::setJsonObject(obj);
	if (obj.value("sizeMode").isUndefined() == false){ this->setSizeMode(obj.value("sizeModule").toString()); }
}