#include "stdafx.h"

#include "s_StorageManagerTemplate.h"
#include "s_StorageManager.h"


/*
-----==========================================================-----
		�ࣺ		�洢ģ���.cpp
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		�洢��ģ�壬���ڹ淶����ʵ�ֵ����ݽṹ��
					��ϸ��s_StorageManager.cpp��
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