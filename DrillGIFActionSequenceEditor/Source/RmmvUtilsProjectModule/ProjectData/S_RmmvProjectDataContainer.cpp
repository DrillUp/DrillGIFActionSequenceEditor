#include "stdafx.h"
#include "S_RmmvProjectDataContainer.h"

/*
-----==========================================================-----
		类：		Rmmv工程数据 数据容器.cpp
		版本：		v1.03
		作者：		drill_up
		所属模块：	工程模块
		功能：		工程数据的容器。
					并提供一系列工程文件的获取功能。
-----==========================================================-----
*/
S_RmmvProjectDataContainer::S_RmmvProjectDataContainer(){
	this->clearAllData();
}
S_RmmvProjectDataContainer::~S_RmmvProjectDataContainer() {
}

/* --------------------------------------------------------------
----------单例
*/
S_RmmvProjectDataContainer* S_RmmvProjectDataContainer::cur_container = NULL;
S_RmmvProjectDataContainer* S_RmmvProjectDataContainer::getInstance() {
	if (cur_container == NULL) {
		cur_container = new S_RmmvProjectDataContainer();
	}
	return cur_container;
}

/*-------------------------------------------------
		工程数据 - 修改
*/
void S_RmmvProjectDataContainer::modifyData(C_RmmvProjectData data){
	this->data_RmmvProjectData = data;
}
/*-------------------------------------------------
		工程数据 - 获取
*/
C_RmmvProjectData S_RmmvProjectDataContainer::getData(){
	return this->data_RmmvProjectData;
}


//获取文件 - 插件
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Plugin(QString plugin_name){
	return this->data_RmmvProjectData.getRmmvFile_Plugin(plugin_name);
}
//获取文件 - 插件存储数据
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_PluginsData(){
	return this->data_RmmvProjectData.getRmmvFile_PluginsData();
}
//获取文件 - 地图
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Map(int i){
	return this->data_RmmvProjectData.getRmmvFile_Map(i);
}
//获取文件 - 地图信息
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_MapInfos(){
	return this->data_RmmvProjectData.getRmmvFile_MapInfos();
}
//获取文件 - 角色
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Actors(){
	return this->data_RmmvProjectData.getRmmvFile_Actors();
}
//获取文件 - 职业
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Classes(){
	return this->data_RmmvProjectData.getRmmvFile_Classes();
}
//获取文件 - 物品
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Items(){
	return this->data_RmmvProjectData.getRmmvFile_Items();
}
//获取文件 - 技能
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Skills(){
	return this->data_RmmvProjectData.getRmmvFile_Skills();
}
//获取文件 - 武器
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Weapons(){
	return this->data_RmmvProjectData.getRmmvFile_Weapons();
}
//获取文件 - 护甲
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Armors(){
	return this->data_RmmvProjectData.getRmmvFile_Armors();
}
//获取文件 - 状态
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_States(){
	return this->data_RmmvProjectData.getRmmvFile_States();
}
//获取文件 - 敌人
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Enemies(){
	return this->data_RmmvProjectData.getRmmvFile_Enemies();
}
//获取文件 - 敌群
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Troops(){
	return this->data_RmmvProjectData.getRmmvFile_Troops();
}
//获取文件 - 公共事件
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_CommonEvents(){
	return this->data_RmmvProjectData.getRmmvFile_CommonEvents();
}
//获取文件 - 动画
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Animations(){
	return this->data_RmmvProjectData.getRmmvFile_Animations();
}
//获取文件 - 图块
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_Tilesets(){
	return this->data_RmmvProjectData.getRmmvFile_Tilesets();
}
//获取文件 - 系统
QFileInfo S_RmmvProjectDataContainer::getRmmvFile_System(){
	return this->data_RmmvProjectData.getRmmvFile_System();
}


//获取文件夹 - 声音BGM（根路径/audio/bgm/）
QDir S_RmmvProjectDataContainer::getRmmvDir_Audio_BGM(){
	return this->data_RmmvProjectData.getRmmvDir_Audio_BGM();
}
//获取文件夹 - 声音BGS（根路径/audio/bgs/）
QDir S_RmmvProjectDataContainer::getRmmvDir_Audio_BGS(){
	return this->data_RmmvProjectData.getRmmvDir_Audio_BGS();
}
//获取文件夹 - 声音ME（根路径/audio/me/）
QDir S_RmmvProjectDataContainer::getRmmvDir_Audio_ME(){
	return this->data_RmmvProjectData.getRmmvDir_Audio_ME();
}
//获取文件夹 - 声音SE（根路径/audio/se/）
QDir S_RmmvProjectDataContainer::getRmmvDir_Audio_SE(){
	return this->data_RmmvProjectData.getRmmvDir_Audio_SE();
}
//获取文件夹 - 数据（根路径/data/）
QDir S_RmmvProjectDataContainer::getRmmvDir_Data(){
	return this->data_RmmvProjectData.getRmmvDir_Data();
}
//获取文件夹 - 字体（根路径/fonts/）
//【游戏中使用，编辑器用不到】
//获取文件夹 - 插件（根路径/icon/）
//【游戏中使用，编辑器用不到】
//获取文件夹 - 插件（根路径/img/）
QDir S_RmmvProjectDataContainer::getRmmvDir_Img(){
	return this->data_RmmvProjectData.getRmmvDir_Img();
}
//获取文件夹 - 插件（根路径/js/）
QDir S_RmmvProjectDataContainer::getRmmvDir_Js(){
	return this->data_RmmvProjectData.getRmmvDir_Js();
}
//获取文件夹 - 插件（根路径/js/plugins/）
QDir S_RmmvProjectDataContainer::getRmmvDir_Plugins(){
	return this->data_RmmvProjectData.getRmmvDir_Plugins();
}
//获取文件夹 - 插件（根路径/movies/）
QDir S_RmmvProjectDataContainer::getRmmvDir_Movies(){
	return this->data_RmmvProjectData.getRmmvDir_Movies();
}
//获取文件夹 - 存档（根路径/save/）
//【游戏中使用，编辑器用不到】



/*-----------------------------------
		数据 - 获取存储的名称
*/
QString S_RmmvProjectDataContainer::getSaveName() {
	return "S_RmmvProjectDataContainer";
}
/*-----------------------------------
		数据 - 清除当前管理器数据
*/
void S_RmmvProjectDataContainer::clearAllData() {
	this->data_RmmvProjectData = C_RmmvProjectData();
}
/*-----------------------------------
		数据 - 全部激励源数据 -> QJsonObject
*/
QJsonObject S_RmmvProjectDataContainer::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	// > 工程数据
	obj_all.insert("C_RmmvProjectData", this->data_RmmvProjectData.getJsonObject());
	
	return obj_all;
}
/*-----------------------------------
		数据 - QJsonObject -> 全部激励源数据
*/
void S_RmmvProjectDataContainer::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	// > 工程数据
	QJsonObject obj_RmmvProjectData = obj_all.value("C_RmmvProjectData").toObject();
	this->data_RmmvProjectData.setJsonObject(obj_RmmvProjectData);

	emit signal_dataAllReloaded();
}
