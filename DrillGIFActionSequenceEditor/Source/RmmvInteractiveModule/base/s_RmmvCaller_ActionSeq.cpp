#include "stdafx.h"
#include "s_RmmvCaller_ActionSeq.h"

#include "Source/PluginModule/storageData/s_PluginDataContainer.h"

/*
-----==========================================================-----
		类：		rmmv数据文件管理器.cpp
		所属模块：	rmmv读取模块
		功能：		管理插件数据类的模块。

		目标：		-> 打开rmmv工程文件
					-> 
					
		使用方法：
				>打开：
					
			
-----==========================================================-----
*/

S_RmmvCaller_ActionSeq::S_RmmvCaller_ActionSeq(){
	this->init();
}
S_RmmvCaller_ActionSeq::~S_RmmvCaller_ActionSeq() {
}

/* --------------------------------------------------------------
----------RmmvFileManager 单例
*/
S_RmmvCaller_ActionSeq* S_RmmvCaller_ActionSeq::cur_manager = NULL;
S_RmmvCaller_ActionSeq* S_RmmvCaller_ActionSeq::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_RmmvCaller_ActionSeq();
	}
	return cur_manager;
}

/* --------------------------------------------------------------
----------初始化
*/
void S_RmmvCaller_ActionSeq::init(){
}

/*-------------------------------------------------
		工程 - 读取全部相关工程文件
*/
bool S_RmmvCaller_ActionSeq::loadAllRmmvFile(C_RmmvProjectData rmmvProjectData) {
	if (rmmvProjectData.isNull()){ return false; }

	// > 读取插件文件
	QFile plugin_file(rmmvProjectData.path + "/js/plugins.js");
	if (!plugin_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "未找到plugins.js文件。", QMessageBox::Yes);
		return false;
	}
	QString plugin_context = plugin_file.readAll();
	this->readPluginData(plugin_context);
	plugin_file.close();

	// > 读取系统文件
	QFile system_file(rmmvProjectData.path + "/data/System.json");
	if (!system_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "未找到system.js文件。", QMessageBox::Yes);
		return false;
	}
	QString system_context = system_file.readAll();
	this->readSystemData(system_context);
	system_file.close();

	return true;
}

/*-------------------------------------------------
		工程 - 读取文件 - plugin.js
*/
void S_RmmvCaller_ActionSeq::readPluginData(QString context) {

	S_PluginDataContainer::getInstance()->resetPluginDataFromText(context);
}

/*-------------------------------------------------
		工程 - 读取文件 - System.json
*/
void S_RmmvCaller_ActionSeq::readSystemData(QString context) {
	//...
}


/*-------------------------------------------------
		工程 - 保存操作（覆盖到rmmv）
*/
void S_RmmvCaller_ActionSeq::saveDataToRmmv(C_RmmvProjectData rmmvProjectData){
	//（虚函数直接覆写）

}