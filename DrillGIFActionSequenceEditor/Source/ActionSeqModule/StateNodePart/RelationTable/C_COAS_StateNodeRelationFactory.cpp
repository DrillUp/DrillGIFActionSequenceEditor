#include "stdafx.h"
#include "C_COAS_StateNodeRelationFactory.h"


/*
-----==========================================================-----
		类：		状态节点工厂（图数据）.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个树节点工厂+树节点容器，用于描述/存放一棵树。
-----==========================================================-----
*/

C_COAS_StateNodeRelationFactory::C_COAS_StateNodeRelationFactory() : C_TreeNodeFactory(){
	this->m_NodeTank.clear();
}
C_COAS_StateNodeRelationFactory::~C_COAS_StateNodeRelationFactory() {
}


/*-----------------------------------
		工厂 - 创建 树节点
*/
C_TreeNodePtr C_COAS_StateNodeRelationFactory::createTreeNode(QString nodeName){
	return this->createStateTreeNode(nodeName);
}
C_TreeNodePtr C_COAS_StateNodeRelationFactory::createTreeNodeEmpty(){
	return this->createStateTreeNodeEmpty();
}
/*-----------------------------------
		工厂 - 创建 状态节点
*/
C_COAS_StateNodeRelationPtr C_COAS_StateNodeRelationFactory::createStateTreeNode(QString nodeName){
	QString new_id = QUuid::createUuid().toString();
	new_id = new_id.replace("{", "");
	new_id = new_id.replace("}", "");
	new_id = new_id.replace("-", "");
	C_COAS_StateNodeRelationPtr ptr = C_COAS_StateNodeRelationPtr::create(new_id, this->id, nodeName);
	
	// > 添加到工厂 类容器
	this->m_NodeTank.append(ptr);

	return ptr;
}
C_COAS_StateNodeRelationPtr C_COAS_StateNodeRelationFactory::createStateTreeNodeEmpty(){
	return this->createStateTreeNode("");
}


/*-----------------------------------
		类容器 - 获取 - 全部（原装）
*/
QList<C_COAS_StateNodeRelationPtr> C_COAS_StateNodeRelationFactory::getStateTreeNode_AllOrg(){
	QList<C_COAS_StateNodeRelationPtr> result_list;
	for (int i = 0; i < this->m_NodeTank.count(); i++){
		C_NodePtr node = this->m_NodeTank.at(i);
		result_list.append(node.dynamicCast<C_COAS_StateNodeRelation>());
	}
	return result_list;
}
/*-----------------------------------
		类容器 - 获取 - 全部
*/
QList<C_COAS_StateNodeRelationPtr> C_COAS_StateNodeRelationFactory::getStateTreeNode_All(){
	QList<C_COAS_StateNodeRelationPtr> result_list;
	for (int i = 0; i < this->m_NodeTank.count(); i++){
		C_NodePtr node = this->m_NodeTank.at(i);
		if (node->isNull()){ continue; }
		result_list.append(node.dynamicCast<C_COAS_StateNodeRelation>());
	}
	return result_list;
}
/*-----------------------------------
		类容器 - 获取 - 全部Id
*/
QStringList C_COAS_StateNodeRelationFactory::getStateTreeNode_AllId(){
	return C_NodeFactory::getNode_AllId();
}
/*-----------------------------------
		类容器 - 获取 - 全部名称
*/
QStringList C_COAS_StateNodeRelationFactory::getStateTreeNode_AllName(){
	return C_NodeFactory::getNode_AllName();
}
/*-----------------------------------
		类容器 - 获取 - 根据ID
*/
C_COAS_StateNodeRelationPtr C_COAS_StateNodeRelationFactory::getStateTreeNode_ById(QString node_id){
	C_NodePtr node = C_NodeFactory::getNode_ById(node_id);
	return node.dynamicCast<C_COAS_StateNodeRelation>();
}
/*-----------------------------------
		类容器 - 获取 - 根据名称
*/
C_COAS_StateNodeRelationPtr C_COAS_StateNodeRelationFactory::getStateTreeNode_ByName(QString node_name){
	C_NodePtr node = C_NodeFactory::getNode_ByName(node_name);
	return node.dynamicCast<C_COAS_StateNodeRelation>();
}
/*-----------------------------------
		类容器 - 是否含ID
*/
bool C_COAS_StateNodeRelationFactory::hasStateTreeNode_Id(QString node_id){
	return this->getStateTreeNodeIndex_ById(node_id) != -1;
}
/*-----------------------------------
		类容器 - 是否含名称
*/
bool C_COAS_StateNodeRelationFactory::hasStateTreeNode_Name(QString node_name){
	return this->getStateTreeNodeIndex_ByName(node_name) != -1;
}
/*-----------------------------------
		私有 - 获取索引（根据ID）
*/
int C_COAS_StateNodeRelationFactory::getStateTreeNodeIndex_ById(QString node_id){
	return C_NodeFactory::getNodeIndex_ById(node_id);
}
/*-----------------------------------
		私有 - 获取索引（根据ID）
*/
int C_COAS_StateNodeRelationFactory::getStateTreeNodeIndex_ByName(QString node_name){
	return C_NodeFactory::getNodeIndex_ByName(node_name);
}


/*-------------------------------------------------
		类 - 获取类名
*/
QStringList C_COAS_StateNodeRelationFactory::classInherited(){
	return C_NodeFactory::classInherited() << "C_COAS_StateNodeRelationFactory";
}
/*-------------------------------------------------
		类 - 判断类
*/
bool C_COAS_StateNodeRelationFactory::isClass_StateTreeNodeFactory(){
	return this->classIsInstanceOf("C_COAS_StateNodeRelationFactory");
}

/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_COAS_StateNodeRelationFactory::getJsonObject(){
	QJsonObject obj = C_NodeFactory::getJsonObject();

	// > 树节点容器
	//（暂无）

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_COAS_StateNodeRelationFactory::setJsonObject(QJsonObject obj){
	C_NodeFactory::setJsonObject(obj);

	// > 树节点容器
	//（暂无）
}
