#include "stdafx.h"
#include "s_ActionSeqDataContainer.h"

#include "DrillGIFActionSequenceEditor.h"
#include "Source/ProjectModule/file/s_TempFileManager.h"
#include "Source/PluginModule/storageData/s_PluginDataContainer.h"
#include "Source/PluginModule/lengthEditor/s_LEAnnotationReader.h"
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
	this->clearAllData();

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
	this->clearAllData();

	// > 插件数据
	QList<C_PluginData*> data_list = S_PluginDataContainer::getInstance()->getPluginData();
	for (int i = 0; i < data_list.count(); i++){
		C_PluginData* data = data_list.at(i);
		if (data->name == "Drill_CoreOfActionSequence"){		//（GIF动作序列核心）

			// > 插件对象
			this->data_ActionSeqPlugin = data;
			// > 插件配置的数据
			this->data_ActionSeqData = this->data_ActionSeqPlugin->parameters;
			// > 插件的长度数据
			this->data_ActionSeqLength = this->getActionSeqPluginLength();
		}
	}

	// > 工程中没有插件数据
	if (this->data_ActionSeqPlugin == nullptr){
		if (QMessageBox::information(nullptr, "提示", "该工程中，没有找到\"GIF动作序列核心\"的插件配置，是否添加？", "添加", "取消", 0, 1) == 0){

			DrillGIFActionSequenceEditor::getInstance()->rebuildActionSeqData();
			return;
		}
	}

	emit dataAllReloaded();
}
/*-------------------------------------------------
		插件数据 - 获取（插件数据）
*/
C_PluginData* S_ActionSeqDataContainer::getActionSeqPlugin() {
	return this->data_ActionSeqPlugin;
}
/*-------------------------------------------------
		插件数据 - 获取（插件文件）
*/
QFileInfo S_ActionSeqDataContainer::getActionSeqPluginFile() {
	return S_RmmvDataContainer::getInstance()->getRmmvFile_Plugin("Drill_CoreOfActionSequence");
}
/*-------------------------------------------------
		插件数据 - 获取长度
*/
C_ActionSeqLength S_ActionSeqDataContainer::getActionSeqPluginLength() {
	C_ActionSeqLength result = C_ActionSeqLength();
	QFileInfo plugin_file = this->getActionSeqPluginFile();
	if (plugin_file.exists() == false){ return result; }

	C_LEAnnotation* c_le = S_LEAnnotationReader::getInstance()->readPlugin(plugin_file);
	result.realLen_actionSeq = c_le->getParamByKey("动作序列-%d").getRealLen();
	result.realLen_action = c_le->getParamByKey("动作元-%d").getRealLen();
	result.realLen_state = c_le->getParamByKey("状态元-%d").getRealLen();
	return result;
}



/*-------------------------------------------------
		数据 - 设置
*/
void S_ActionSeqDataContainer::setActionSeqData(QJsonObject obj){
	this->data_ActionSeqData = obj;
}
/*-------------------------------------------------
		数据 - 获取
*/
QJsonObject S_ActionSeqDataContainer::getActionSeqData(){
	return this->data_ActionSeqData;	//（注意，从rmmv中读取的数据 和 编辑器配置的数据 不一样）
}
/*-------------------------------------------------
		数据 - 设置长度
*/
void S_ActionSeqDataContainer::setActionSeqLength(C_ActionSeqLength data){
	this->data_ActionSeqLength = data;
}
/*-------------------------------------------------
		数据 - 获取长度
*/
C_ActionSeqLength S_ActionSeqDataContainer::getActionSeqLength(){
	return this->data_ActionSeqLength;	//（注意，插件读取的长度 和 编辑器配置的长度 不一样）
}
/*-------------------------------------------------
		数据 - 根据长度建立一个空白的动作序列列表
*/
QJsonObject S_ActionSeqDataContainer::buildEmptyActionSeqData(C_ActionSeqLength data){
	QJsonObject result = QJsonObject();
	for (int i = 0; i < data.realLen_actionSeq; i++){
		result.insert("动作序列-" + QString::number(i + 1), "");
	}
	return result;
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
	this->data_ActionSeqPlugin = nullptr;
	this->data_treeConfig = QJsonObject();
	this->data_ActionSeqLength = C_ActionSeqLength();
	this->data_ActionSeqData = QJsonObject();
}
/*-----------------------------------
		数据 - 全部激励源数据 -> QJsonObject
*/
QJsonObject S_ActionSeqDataContainer::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	obj_all.insert("data_treeConfig", this->data_treeConfig);								//树配置数据
	obj_all.insert("data_ActionSeqLength", this->data_ActionSeqLength.getJsonObject());		//动作序列长度
	obj_all.insert("data_ActionSeqData", this->data_ActionSeqData);							//动作序列数据
	
	return obj_all;
}

/*-----------------------------------
		数据 - QJsonObject -> 全部激励源数据
*/
void S_ActionSeqDataContainer::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	this->data_treeConfig = obj_all.value("data_treeConfig").toObject();							//树配置数据
	this->data_ActionSeqLength.setJsonObject( obj_all.value("data_ActionSeqLength").toObject() );	//动作序列长度
	this->data_ActionSeqData = obj_all.value("data_ActionSeqData").toObject();						//动作序列数据

	emit dataAllReloaded();
}
