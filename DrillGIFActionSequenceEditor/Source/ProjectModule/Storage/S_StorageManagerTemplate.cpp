#include "stdafx.h"

#include "S_StorageManagerTemplate.h"
#include "S_StorageManager.h"


/*
-----==========================================================-----
		类：		存储模板块.cpp
		作者：		drill_up
		所属模块：	项目管理模块
		功能：		存储的模板，用于规范子类实现的数据结构。
					详细见s_StorageManager.cpp。
-----==========================================================-----
*/

S_StorageManagerTemplate::S_StorageManagerTemplate(){

	// > 参数初始化
	this->m_storage_fileInfo = QFileInfo("");

	// > 添加管理器
	S_StorageManager::getInstance()->addManager(this);
}
S_StorageManagerTemplate::~S_StorageManagerTemplate() {
}

/* ----------------------------------------------------------------------------------
		数据 - 获取存储的名称（旧）
*/
QStringList S_StorageManagerTemplate::getOldNamesForRead(){
	return QStringList();
}