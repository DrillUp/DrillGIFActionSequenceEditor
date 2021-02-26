#include "stdafx.h"
#include "s_RmmvCaller_ActionSeq.h"

#include "Source/ProjectModule/file/s_TempFileManager.h"
#include "Source/PluginModule/storageData/s_PluginDataContainer.h"
#include "s_RmmvDataContainer.h"

/*
-----==========================================================-----
		类：		rmmv交互者（动作序列 专用）.cpp
		所属模块：	交互模块
		功能：		与rmmv交互的模块，只考虑 动作序列 相关文件。

		子功能：	->文件
						-> 读取插件文件
						-> 读取系统文件
					
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
		工程 - 覆盖资源文件
*/
void S_RmmvCaller_ActionSeq::coverSourceFileToTemp(C_RmmvProjectData rmmvProjectData){
	if (rmmvProjectData.isNull()){ return ; }

	// > img/Special__actionSeq
	QString temp_path = S_TempFileManager::getInstance()->getTempFileUrl() + "/Special__actionSeq";
	S_TempFileManager::getInstance()->remove_Dir(QDir(temp_path));
	S_TempFileManager::getInstance()->copy_Dir(QDir(rmmvProjectData.path + "/img/Special__actionSeq"), QDir(temp_path));

}



/*-------------------------------------------------
		工程 - 打开rmmv（继承）
*/
C_RmmvProjectData S_RmmvCaller_ActionSeq::callRmmvOpen(){
	C_RmmvProjectData result = P_RmmvCaller::callRmmvOpen();
	S_RmmvDataContainer::getInstance()->modify(result);		//打开后，立即记录位置
	return result;
}
/*-------------------------------------------------
		工程 - 保存操作（覆盖到rmmv）
*/
void S_RmmvCaller_ActionSeq::saveDataToRmmv(C_RmmvProjectData rmmvProjectData){
	//（虚函数直接覆写）

}