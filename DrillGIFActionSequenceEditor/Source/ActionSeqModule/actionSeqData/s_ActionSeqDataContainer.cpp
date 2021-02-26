#include "stdafx.h"
#include "s_ActionSeqDataContainer.h"

#include "Source/ProjectModule/file/s_TempFileManager.h"
#include "Source/PluginModule/storageData/s_PluginDataContainer.h"
#include "Source/RmmvInteractiveModule/custom/s_RmmvDataContainer.h"

/*
-----==========================================================-----
		类：		动作序列 数据容器.cpp
		所属模块：	动作序列模块
		功能：		动作序列的数据容器。
					会主动使用插件容器中的数据。
			
		使用方法：
				>初始化：
					S_ActionSeqDataContainer::getInstance();
-----==========================================================-----
*/
S_ActionSeqDataContainer::S_ActionSeqDataContainer(){
	this->data_ActionSeq = nullptr;
	this->data_treeConfig = QJsonObject();

	//-----------------------------------
	//----事件绑定
	connect(S_PluginDataContainer::getInstance(), &S_PluginDataContainer::pluginDataChanged, this, &S_ActionSeqDataContainer::resetPluginData);

}
S_ActionSeqDataContainer::~S_ActionSeqDataContainer() {
}

/* --------------------------------------------------------------
----------PluginDataManager 单例
*/
S_ActionSeqDataContainer* S_ActionSeqDataContainer::cur_manager = NULL;
S_ActionSeqDataContainer* S_ActionSeqDataContainer::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_ActionSeqDataContainer();
	}
	return cur_manager;
}


/*-------------------------------------------------
		插件数据 - 重设
*/
void S_ActionSeqDataContainer::resetPluginData() {
	this->data_ActionSeq = nullptr;

	QList<C_PluginData*> data_list = S_PluginDataContainer::getInstance()->getPluginData();
	for (int i = 0; i < data_list.count(); i++){
		C_PluginData* data = data_list.at(i);
		if (data->name == "Drill_CoreOfActionSequence"){		//（GIF动作序列核心）
			this->data_ActionSeq = data;
		}
	}

	if (this->data_ActionSeq == nullptr){
		if (QMessageBox::information(nullptr, "提示", "该工程中，没有找到\"GIF动作序列核心\"的插件配置，是否添加？", "添加", "取消", 0, 1) == 0){
			//...
		}
	}

	emit actionSeqDataChanged();
}

/*-------------------------------------------------
		插件数据 - 获取（插件数据）
*/
C_PluginData* S_ActionSeqDataContainer::getActionSeqData() {
	return this->data_ActionSeq;
}
/*-------------------------------------------------
		插件数据 - 获取（插件文件）
*/
QFileInfo S_ActionSeqDataContainer::getActionSeqPluginFile() {
	return S_RmmvDataContainer::getInstance()->getRmmvFile_Plugin("Drill_CoreOfActionSequence");
}

/*-------------------------------------------------
		常量 - 获取软件保存图片路径
*/
QString S_ActionSeqDataContainer::getActionSeqDir(){
	QString dir_path = S_TempFileManager::getInstance()->getTempFileUrl() +"/Special__actionSeq";
	QDir dir(dir_path);
	if (dir.exists() == false){ dir.mkdir(dir_path); }
	return dir_path;
}

/*-------------------------------------------------
		树数据 - 修改
*/
void S_ActionSeqDataContainer::modifyTreeData(QJsonObject tree){
	this->data_treeConfig = tree;
}
/*-------------------------------------------------
		树数据 - 获取
*/
QJsonObject S_ActionSeqDataContainer::getTreeData(){
	return this->data_treeConfig;
}

/*-----------------------------------
		数据 - 获取存储的名称
*/
QString S_ActionSeqDataContainer::getSaveName() {
	return "S_ActionSeqDataContainer";
}
/*-----------------------------------
		数据 - 清除当前管理器数据
*/
void S_ActionSeqDataContainer::clearAllData() {
	this->data_treeConfig = QJsonObject();
}
/*-----------------------------------
		数据 - 全部激励源数据 -> QJsonObject
*/
QJsonObject S_ActionSeqDataContainer::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	obj_all.insert("data_treeConfig", this->data_treeConfig);	//树配置数据
	
	return obj_all;
}

/*-----------------------------------
		数据 - QJsonObject -> 全部激励源数据
*/
void S_ActionSeqDataContainer::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	this->data_treeConfig = obj_all.value("data_treeConfig").toObject();	//树配置数据

	emit dataAllReloaded();
}
