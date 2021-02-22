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
	this->m_sizeMode = "中";
	this->m_defaultInterval = 4;
	this->m_isUseFileSys = false;

	this->m_isMultiSelect = true;		//（固定为多选模式）
}
C_ALEConfig::~C_ALEConfig(){
}


/*-------------------------------------------------
		设置尺寸模式
*/
void C_ALEConfig::setSizeMode(QString sizeMode){
	if (sizeMode == ""){ return; }
	if (this->m_sizeMode == sizeMode){ return; }
	this->m_sizeMode = sizeMode;

	if (this->m_sizeMode == "大"){
		this->m_height = 200;
	}
	if (this->m_sizeMode == "中"){
		this->m_height = 140;
	}
	if (this->m_sizeMode == "小"){
		this->m_height = 100;
	}
}
QString C_ALEConfig::getSizeMode(){
	return this->m_sizeMode;
}

/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_ALEConfig::getJsonObject(){
	QJsonObject obj = C_PiSConfig::getJsonObject();
	obj.insert("m_sizeMode", this->m_sizeMode);
	obj.insert("m_defaultInterval", this->m_defaultInterval);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ALEConfig::setJsonObject(QJsonObject obj){
	C_PiSConfig::setJsonObject(obj);
	this->m_isMultiSelect = true;		//（固定为多选模式）

	if (obj.value("m_sizeMode").isUndefined() == false){ this->setSizeMode(obj.value("m_sizeModule").toString()); }
	if (obj.value("m_defaultInterval").isUndefined() == false){ this->m_defaultInterval = obj.value("m_defaultInterval").toInt(); }

}