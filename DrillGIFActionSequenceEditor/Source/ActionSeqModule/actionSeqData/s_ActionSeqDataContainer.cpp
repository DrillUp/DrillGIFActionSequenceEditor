#include "stdafx.h"
#include "s_ActionSeqDataContainer.h"

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
		数据 - 重设
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
		数据 - 获取（插件数据）
*/
C_PluginData* S_ActionSeqDataContainer::getActionSeqData() {
	return this->data_ActionSeq;
}
/*-------------------------------------------------
		数据 - 获取（插件文件）
*/
QFileInfo S_ActionSeqDataContainer::getActionSeqPluginFile() {
	return S_RmmvDataContainer::getInstance()->getRmmvFile_Plugin("Drill_CoreOfActionSequence");
}