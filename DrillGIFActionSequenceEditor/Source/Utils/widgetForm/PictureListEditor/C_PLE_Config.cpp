#include "stdafx.h"
#include "C_PLE_Config.h"

/*
-----==========================================================-----
		类：		图片编辑块 配置类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		图片编辑块 的配置类。
					
		说明：		该数据类继承于C_PiSConfig。
-----==========================================================-----
*/

C_PLE_Config::C_PLE_Config() : C_PiS_Config() {
	this->m_isMultiSelect = true;		//（固定为多选模式）
}
C_PLE_Config::~C_PLE_Config(){
}


/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_PLE_Config::getJsonObject(){
	QJsonObject obj = C_PiS_Config::getJsonObject();

	//（暂无）

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_PLE_Config::setJsonObject(QJsonObject obj){
	C_PiS_Config::setJsonObject(obj);
	this->m_isMultiSelect = true;		//（固定为多选模式）

	//（暂无）

}