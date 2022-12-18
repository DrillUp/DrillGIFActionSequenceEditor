#include "stdafx.h"
#include "S_NodeFactoryContainer.h"


/*
-----==========================================================-----
		类：		节点工厂 容器.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		存放工厂对象的容器，用于节点获取工厂对象用。
-----==========================================================-----
*/

S_NodeFactoryContainer::S_NodeFactoryContainer(){
	this->m_NodeFactoryTank.clear();
}

S_NodeFactoryContainer::~S_NodeFactoryContainer() {
}

/* --------------------------------------------------------------
----------SourceManager 单例
*/
S_NodeFactoryContainer* S_NodeFactoryContainer::cur_manager = NULL;
S_NodeFactoryContainer* S_NodeFactoryContainer::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_NodeFactoryContainer();
	}
	return cur_manager;
}


/*-----------------------------------
		工厂容器 - 添加
*/
void S_NodeFactoryContainer::addFactory(C_NodeFactoryPtr factory){
	if (this->hasFactory(factory->getId())){ return; }
	this->m_NodeFactoryTank.append(factory);
}
/*-----------------------------------
		工厂容器 - 删除
*/
void S_NodeFactoryContainer::removeFactory(QString factory_id){
	int index = this->getFactoryIndex_ById(factory_id);
	if (index == -1){ return; }
	this->m_NodeFactoryTank.removeAt(index);
}

/*-----------------------------------
		工厂容器 - 获取 - 全部
*/
QList<C_NodeFactoryPtr> S_NodeFactoryContainer::getFactory_All(){
	return this->m_NodeFactoryTank;
}
/*-----------------------------------
		工厂容器 - 获取 - 全部Id
*/
QStringList S_NodeFactoryContainer::getFactory_AllId(){
	QStringList result_list;
	for (int i = 0; i < this->m_NodeFactoryTank.count(); i++){
		C_NodeFactoryPtr factory = this->m_NodeFactoryTank.at(i);
		result_list.append(factory->getId());
	}
	return result_list;
}
/*-----------------------------------
		工厂容器 - 获取 - 根据Id
*/
C_NodeFactoryPtr S_NodeFactoryContainer::getFactory_ById(QString factory_id){
	int index = this->getFactoryIndex_ById(factory_id);
	if (index == -1){ return C_NodeFactoryPtr(); }
	return this->m_NodeFactoryTank.at(index);
}
/*-----------------------------------
		工厂容器 - 获取 - 是否含工厂
*/
bool S_NodeFactoryContainer::hasFactory(QString factory_id){
	int index = this->getFactoryIndex_ById(factory_id);
	if (index == -1){ return false; }
	return true;
}

/*-----------------------------------
		私有 - 获取索引
*/
int S_NodeFactoryContainer::getFactoryIndex_ById(QString id){
	for (int i = 0; i < this->m_NodeFactoryTank.count(); i++){
		C_NodeFactoryPtr factory = this->m_NodeFactoryTank.at(i);
		if (factory->getId() == id){
			return i;
		}
	}
	return -1;
}

