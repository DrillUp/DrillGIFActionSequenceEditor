#include "stdafx.h"

#include "s_StorageManagerTemplate.h"
#include "s_StorageManager.h"


/*
-----==========================================================-----
		类：		存储模板块.cpp
		所属模块：	项目管理模块
		功能：		存储的模板，用于规范子类实现的数据结构。
					详细见s_StorageManager.cpp。
-----==========================================================-----
*/

S_StorageManagerTemplate::S_StorageManagerTemplate(){
	S_StorageManager::getInstance()->addManager(this);
	this->m_storage_fileInfo = QFileInfo("");
}
S_StorageManagerTemplate::~S_StorageManagerTemplate() {
}

QStringList S_StorageManagerTemplate::getOldNamesForRead(){
	return QStringList();
}