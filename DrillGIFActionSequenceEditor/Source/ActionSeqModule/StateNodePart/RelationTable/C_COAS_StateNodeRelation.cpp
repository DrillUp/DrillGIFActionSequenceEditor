#include "stdafx.h"
#include "C_COAS_StateNodeRelation.h"

#include "C_COAS_StateNodeRelationFactory.h"
#include "Source/Utils/WidgetFormSenior/NodeData/S_NodeFactoryContainer.h"
#include "Source/Utils/Common/TTool.h"


/*
-----==========================================================-----
		类：		状态节点（图数据）.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个状态节点，用于存放状态节点、状态元数据（临时的数据）。
					功能基于 树节点，能进行树功能的解析。
					
		使用方法：	见父类 树节点 。
-----==========================================================-----
*/
C_COAS_StateNodeRelation::C_COAS_StateNodeRelation(QString id, QString factory_id, QString nodeName) : C_TreeNode(id, factory_id, nodeName){
	
	// > 状态节点
	this->m_isPtrData_State = false;
	this->m_isPtrData_StateNode = false;
	this->m_curPtrData_State.clear();
	this->m_curPtrData_StateNode.clear();
}

C_COAS_StateNodeRelation::~C_COAS_StateNodeRelation(){
}


//状态节点 - 设置状态元数据
void C_COAS_StateNodeRelation::setPtrData_State(C_COAS_StatePtr data){
	this->m_isPtrData_State = true;
	this->m_isPtrData_StateNode = false;
	this->m_curPtrData_State = data;
}
//状态节点 - 设置状态节点数据
void C_COAS_StateNodeRelation::setPtrData_StateNode(C_COAS_StateNodePtr data){
	this->m_isPtrData_State = false;
	this->m_isPtrData_StateNode = true;
	this->m_curPtrData_StateNode = data;
}

//状态节点 - 是否为状态元数据
bool C_COAS_StateNodeRelation::isPtrData_State(){
	return this->m_isPtrData_State;
}
//状态节点 - 是否为状态节点数据
bool C_COAS_StateNodeRelation::isPtrData_StateNode(){
	return this->m_isPtrData_StateNode;
}
//状态节点 - 播放类型 - 状态元
bool C_COAS_StateNodeRelation::isPlayType_State(){
	if (this->isPtrData_StateNode() == false){ return false; }
	return this->m_curPtrData_StateNode->play_type == "随机播放状态元" || 
		this->m_curPtrData_StateNode->play_type == "顺序播放状态元";
}
//状态节点 - 播放类型 - 状态节点
bool C_COAS_StateNodeRelation::isPlayType_StateNode(){
	if (this->isPtrData_StateNode() == false){ return false; }
	return this->m_curPtrData_StateNode->play_type == "随机播放嵌套集合" || 
		this->m_curPtrData_StateNode->play_type == "顺序播放嵌套集合";
}
//状态节点 - 获取子节点 - 状态元
QStringList C_COAS_StateNodeRelation::getChildList_State(){
	if (this->isPtrData_StateNode() == false){ return QStringList(); }
	if (this->m_curPtrData_StateNode->play_type == "随机播放状态元"){
		return this->m_curPtrData_StateNode->play_randomStateSeq;
	}
	if (this->m_curPtrData_StateNode->play_type == "顺序播放状态元"){
		return this->m_curPtrData_StateNode->play_plainStateSeq;
	}
	return QStringList();
}
//状态节点 - 获取子节点 - 状态节点
QStringList C_COAS_StateNodeRelation::getChildList_StateNode(){
	if (this->isPtrData_StateNode() == false){ return QStringList(); }
	if (this->m_curPtrData_StateNode->play_type == "随机播放嵌套集合"){
		return this->m_curPtrData_StateNode->play_randomNodeSeq;
	}
	if (this->m_curPtrData_StateNode->play_type == "顺序播放嵌套集合"){
		return this->m_curPtrData_StateNode->play_plainNodeSeq;
	}
	return QStringList();
}
//状态节点 - 获取节点描述
QString C_COAS_StateNodeRelation::getNodeDescription(){
	if (this->isPtrData_StateNode() == false){ return ""; }
	if (this->m_curPtrData_StateNode->play_type == "随机播放状态元" || 
		this->m_curPtrData_StateNode->play_type == "随机播放嵌套集合"){
		int max_num = this->m_curPtrData_StateNode->play_randomMax;
		return "R" + QString::number(max_num)+"→";
	}
	if (this->m_curPtrData_StateNode->play_type == "顺序播放状态元" || 
		this->m_curPtrData_StateNode->play_type == "顺序播放嵌套集合"){
		return "L→";
	}
	return "";
}


/*-------------------------------------------------
		类 - 获取类名
*/
QStringList C_COAS_StateNodeRelation::classInherited(){
	return C_Node::classInherited() << "C_COAS_StateNodeRelation";
}
/*-------------------------------------------------
		类 - 判断类
*/
bool C_COAS_StateNodeRelation::isClass_StateTreeNode(){
	return this->classIsInstanceOf("C_COAS_StateNodeRelation");
}
/*-------------------------------------------------
		类 - 创建自己
*/
C_TreeNodePtr C_COAS_StateNodeRelation::_factoryTree_newNode(){
	return this->_factoryTree_get()->createTreeNodeEmpty();
}
/*-------------------------------------------------
		类 - 获取类
*/
C_TreeNodePtr C_COAS_StateNodeRelation::_factoryTree_getNodeById(QString node_id){
	return this->_factoryTree_get()->getTreeNode_ById(node_id);
}
/*-------------------------------------------------
		类 - 获取工厂
*/
C_TreeNodeFactoryPtr C_COAS_StateNodeRelation::_factoryTree_get(){
	C_NodeFactoryPtr factory = S_NodeFactoryContainer::getInstance()->getFactory_ById(this->factory_id);
	if (factory->classIsInstanceOf("C_COAS_StateNodeRelationFactory") == false){ return C_COAS_StateNodeRelationFactoryPtr(); }
	return factory.dynamicCast<C_COAS_StateNodeRelationFactory>();
}


/*-------------------------------------------------
		复制数据 当前类 -> 目标类
*/
void C_COAS_StateNodeRelation::copyTo(C_NodePtr data_to){
	C_TreeNode::copyTo(data_to);
	if (data_to->classIsInstanceOf("C_COAS_StateNodeRelation") == false){ return; }
	C_COAS_StateNodeRelationPtr treeData_to = data_to.dynamicCast<C_COAS_StateNodeRelation>();

	//（不操作）
}
/*-------------------------------------------------
		复制数据 目标类 -> 当前类
*/
void C_COAS_StateNodeRelation::copyFrom(C_NodePtr data_from){
	C_TreeNode::copyFrom(data_from);
	if (data_from->classIsInstanceOf("C_COAS_StateNodeRelation") == false){ return; }
	C_COAS_StateNodeRelationPtr treeData_from = data_from.dynamicCast<C_COAS_StateNodeRelation>();

	//（不操作）
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_COAS_StateNodeRelation::getJsonObject(){
	QJsonObject obj = C_TreeNode::getJsonObject();

	//（不操作）

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_COAS_StateNodeRelation::setJsonObject(QJsonObject obj){
	C_TreeNode::setJsonObject(obj);

	//（不操作）
}

