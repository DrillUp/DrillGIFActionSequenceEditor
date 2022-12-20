#include "stdafx.h"
#include "C_COAS_Length.h"

/*
-----==========================================================-----
		类：		最大值 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		最大值 的数据类。
					
		子功能：
					->空对象
					
-----==========================================================-----
*/
C_COAS_Length::C_COAS_Length(){

	this->realLen_actionSeq = 0;			//长度 - 动画序列
	this->realLen_action = 0;				//长度 - 动作元
	this->realLen_state = 0;				//长度 - 状态元
	this->realLen_stateNode = 0;			//长度 - 状态节点
}
C_COAS_Length::~C_COAS_Length(){
}


/*-------------------------------------------------
		空判断
*/
bool C_COAS_Length::isNull(){
	if (this->realLen_actionSeq < 4){ return true; }
	if (this->realLen_action < 4){ return true; }
	if (this->realLen_state < 4){ return true; }
	//（状态节点为特殊情况）
	return false;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_COAS_Length::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("realLen_actionSeq", this->realLen_actionSeq);
	obj.insert("realLen_action", this->realLen_action);
	obj.insert("realLen_state", this->realLen_state);
	obj.insert("realLen_stateNode", this->realLen_stateNode);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_COAS_Length::setJsonObject(QJsonObject obj){

	if (obj.value("realLen_actionSeq").isUndefined() == false){ this->realLen_actionSeq = obj.value("realLen_actionSeq").toInt(40); }
	if (obj.value("realLen_action").isUndefined() == false){ this->realLen_action = obj.value("realLen_action").toInt(10); }
	if (obj.value("realLen_state").isUndefined() == false){ this->realLen_state = obj.value("realLen_state").toInt(10); }
	if (obj.value("realLen_stateNode").isUndefined() == false){ this->realLen_stateNode = obj.value("realLen_stateNode").toInt(10); }
	
	// > 默认值
	if (this->isNull()){
		this->realLen_actionSeq = 40;
		this->realLen_action = 10;
		this->realLen_state = 10;
		this->realLen_stateNode = 10;
	}
}