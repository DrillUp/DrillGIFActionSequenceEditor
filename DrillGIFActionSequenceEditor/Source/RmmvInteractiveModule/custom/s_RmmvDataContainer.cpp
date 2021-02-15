#include "stdafx.h"
#include "s_RmmvDataContainer.h"

/*
-----==========================================================-----
		类：		rmmv交互 数据容器.cpp
		所属模块：	交互模块
		功能：		存储相关数据的容器。	
-----==========================================================-----
*/
S_RmmvDataContainer::S_RmmvDataContainer(){
	this->clearAllData();
}
S_RmmvDataContainer::~S_RmmvDataContainer() {
}

/* --------------------------------------------------------------
----------RmmvFileManager 单例
*/
S_RmmvDataContainer* S_RmmvDataContainer::cur_container = NULL;
S_RmmvDataContainer* S_RmmvDataContainer::getInstance() {
	if (cur_container == NULL) {
		cur_container = new S_RmmvDataContainer();
	}
	return cur_container;
}

/*-------------------------------------------------
		数据 - 修改
*/
void S_RmmvDataContainer::modify(C_RmmvProjectData data){
	this->m_RmmvProjectData = data;
}
/*-------------------------------------------------
		数据 - 获取
*/
C_RmmvProjectData S_RmmvDataContainer::getRmmvProjectData(){
	return this->m_RmmvProjectData;
}

/*-------------------------------------------------
		数据 - 获取插件文件
*/
QFileInfo S_RmmvDataContainer::getRmmvFile_Plugin(QString plugin_name){
	QString name = plugin_name.replace(".js","");
	return QFileInfo(this->m_RmmvProjectData.path + "/js/plugins/" + plugin_name+ ".js");
}

/*-----------------------------------
		数据 - 获取存储的名称
*/
QString S_RmmvDataContainer::getSaveName() {
	return "S_RmmvDataContainer";
}
/*-----------------------------------
		数据 - 清除当前管理器数据
*/
void S_RmmvDataContainer::clearAllData() {
	this->m_RmmvProjectData = C_RmmvProjectData();
}
/*-----------------------------------
		数据 - 全部激励源数据 -> QJsonObject
*/
QJsonObject S_RmmvDataContainer::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	obj_all.insert("C_RmmvProjectData", this->m_RmmvProjectData.getJsonObject());	//rmmv工程数据
	
	return obj_all;
}

/*-----------------------------------
		数据 - QJsonObject -> 全部激励源数据
*/
void S_RmmvDataContainer::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	QJsonObject obj_RmmvProjectData = obj_all.value("C_RmmvProjectData").toObject();	//rmmv工程数据
	this->m_RmmvProjectData.setJsonObject(obj_RmmvProjectData);

	emit dataAllReloaded();
}
