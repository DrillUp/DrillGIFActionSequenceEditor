#include "stdafx.h"
#include "s_RmmvDataContainer.h"

/*
-----==========================================================-----
		类：		rmmv交互 数据容器.cpp
		版本：		v1.01
		所属模块：	交互模块
		功能：		【只存储】rmmv工程数据的容器。
					并提供一系列工程文件的获取功能。
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
	this->data_RmmvProjectData = data;
}
/*-------------------------------------------------
		数据 - 获取
*/
C_RmmvProjectData S_RmmvDataContainer::getRmmvProjectData(){
	return this->data_RmmvProjectData;
}

/*-------------------------------------------------
		数据 - 获取文件
*/
QFileInfo S_RmmvDataContainer::getRmmvFile_Plugin(QString plugin_name){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	QString name = plugin_name.replace(".js","");
	return QFileInfo(this->data_RmmvProjectData.path + "/js/plugins/" + plugin_name+ ".js");
}
QFileInfo S_RmmvDataContainer::getRmmvFile_PluginsData(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/js/plugins.js");
}
//数据 - 获取文件 - 地图
QFileInfo S_RmmvDataContainer::getRmmvFile_Map(int i){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	QString name = QString("Map%1").arg(i, 3, 10, QLatin1Char('0'));
	return QFileInfo(this->data_RmmvProjectData.path + "/data/" + name + ".json");
}
QFileInfo S_RmmvDataContainer::getRmmvFile_MapInfos(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/MapInfos.json");
}
//数据 - 获取文件 - 角色
QFileInfo S_RmmvDataContainer::getRmmvFile_Actors(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Actors.json");
}
//数据 - 获取文件 - 职业
QFileInfo S_RmmvDataContainer::getRmmvFile_Classes(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Classes.json");
}
//数据 - 获取文件 - 物品
QFileInfo S_RmmvDataContainer::getRmmvFile_Items(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Items.json");
}
//数据 - 获取文件 - 技能
QFileInfo S_RmmvDataContainer::getRmmvFile_Skills(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Skills.json");
}
//数据 - 获取文件 - 武器
QFileInfo S_RmmvDataContainer::getRmmvFile_Weapons(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Weapons.json");
}
//数据 - 获取文件 - 护甲
QFileInfo S_RmmvDataContainer::getRmmvFile_Armors(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Armors.json");
}
//数据 - 获取文件 - 状态
QFileInfo S_RmmvDataContainer::getRmmvFile_States(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/States.json");
}
//数据 - 获取文件 - 敌人
QFileInfo S_RmmvDataContainer::getRmmvFile_Enemies(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Enemies.json");
}
//数据 - 获取文件 - 敌群
QFileInfo S_RmmvDataContainer::getRmmvFile_Troops(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Troops.json");
}
//数据 - 获取文件 - 公共事件
QFileInfo S_RmmvDataContainer::getRmmvFile_CommonEvents(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/CommonEvents.json");
}
//数据 - 获取文件 - 动画
QFileInfo S_RmmvDataContainer::getRmmvFile_Animations(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Animations.json");
}
//数据 - 获取文件 - 图块
QFileInfo S_RmmvDataContainer::getRmmvFile_Tilesets(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Tilesets.json");
}
//数据 - 获取文件 - 系统
QFileInfo S_RmmvDataContainer::getRmmvFile_System(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/System.json");
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
	this->data_RmmvProjectData = C_RmmvProjectData();
}
/*-----------------------------------
		数据 - 全部激励源数据 -> QJsonObject
*/
QJsonObject S_RmmvDataContainer::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	obj_all.insert("C_RmmvProjectData", this->data_RmmvProjectData.getJsonObject());	//rmmv工程数据
	
	return obj_all;
}

/*-----------------------------------
		数据 - QJsonObject -> 全部激励源数据
*/
void S_RmmvDataContainer::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	QJsonObject obj_RmmvProjectData = obj_all.value("C_RmmvProjectData").toObject();	//rmmv工程数据
	this->data_RmmvProjectData.setJsonObject(obj_RmmvProjectData);

	emit dataAllReloaded();
}
