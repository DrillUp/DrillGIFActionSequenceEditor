#include "stdafx.h"
#include "S_RmmvOperateBoardConfig.h"

#include "Source/ProjectModule/StorageGlobal/S_IniManager.h"

/*
-----==========================================================-----
		类：		操作面板 数据容器.cpp
		作者：		drill_up
		所属模块：	交互模块
		功能：		操作面板的数据配置。
-----==========================================================-----
*/
S_RmmvOperateBoardConfig::S_RmmvOperateBoardConfig(){
	this->m_bakEnabled = true;

	QString str = S_IniManager::getInstance()->getConfig("Rmmv_bakEnabled");
	if (str == "false"){
		this->m_bakEnabled = false;
	}else{
		this->m_bakEnabled = true;
	}
}
S_RmmvOperateBoardConfig::~S_RmmvOperateBoardConfig() {
}

/* --------------------------------------------------------------
----------单例
*/
S_RmmvOperateBoardConfig* S_RmmvOperateBoardConfig::cur_container = NULL;
S_RmmvOperateBoardConfig* S_RmmvOperateBoardConfig::getInstance() {
	if (cur_container == NULL) {
		cur_container = new S_RmmvOperateBoardConfig();
	}
	return cur_container;
}

/*-------------------------------------------------
		数据 - 修改备份设置
*/
void S_RmmvOperateBoardConfig::setBakEnabled(bool enabled){
	this->m_bakEnabled = enabled;
	if (enabled){
		S_IniManager::getInstance()->setConfig("Rmmv_bakEnabled","true");
	}else{
		S_IniManager::getInstance()->setConfig("Rmmv_bakEnabled","false");
	}
}
/*-------------------------------------------------
		数据 - 是否要备份
*/
bool S_RmmvOperateBoardConfig::isBakEnabled(){
	return this->m_bakEnabled;
}

