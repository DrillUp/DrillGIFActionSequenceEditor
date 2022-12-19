#include "stdafx.h"
#include "C_StateTreeNode.h"

#include "C_StateTreeNodeFactory.h"
#include "Source/Utils/widgetFormSenior/NodeData/S_NodeFactoryContainer.h"
#include "Source/Utils/Common/TTool.h"


/*
-----==========================================================-----
		类：		状态节点（图数据）.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个状态节点，用于存放状态节点、状态元数据。
					功能基于 树节点，能进行树功能的解析。
					
		使用方法：	见父类 树节点 。
-----==========================================================-----
*/
C_StateTreeNode::C_StateTreeNode(QString id, QString factory_id, QString nodeName) : C_TreeNode(id, factory_id, nodeName){
	
	// > 状态节点
	this->m_isObjData_State = false;
	this->m_isObjData_StateNode = false;
	this->m_curObjData = QJsonObject();

}

C_StateTreeNode::~C_StateTreeNode(){
}


//状态节点 - 设置状态元数据
void C_StateTreeNode::setObjData_State(QJsonObject data){
	this->m_isObjData_State = true;
	this->m_isObjData_StateNode = false;
	this->m_curObjData = data;
}
//状态节点 - 设置状态节点数据
void C_StateTreeNode::setObjData_StateNode(QJsonObject data){
	this->m_isObjData_State = false;
	this->m_isObjData_StateNode = true;
	this->m_curObjData = data;
}

//状态节点 - 是否为状态元数据
bool C_StateTreeNode::isObjData_State(){
	return this->m_isObjData_State;
}
//状态节点 - 是否为状态节点数据
bool C_StateTreeNode::isObjData_StateNode(){
	return this->m_isObjData_StateNode;
}
//状态节点 - 播放类型 - 状态元
bool C_StateTreeNode::isPlayType_State(){
	if (this->isObjData_StateNode() == false){ return false; }
	QString playType = this->m_curObjData["播放方式"].toString();
	return playType == "随机播放状态元" || playType == "顺序播放状态元";
}
//状态节点 - 播放类型 - 状态节点
bool C_StateTreeNode::isPlayType_StateNode(){
	if (this->isObjData_StateNode() == false){ return false; }
	QString playType = this->m_curObjData["播放方式"].toString();
	return playType == "随机播放嵌套集合" || playType == "顺序播放嵌套集合";
}
//状态节点 - 获取子节点 - 状态元
QStringList C_StateTreeNode::getChildList_State(){
	if (this->isObjData_StateNode() == false){ return QStringList(); }
	QString playType = this->m_curObjData["播放方式"].toString();
	if (playType == "随机播放状态元" ){
		return TTool::_JSON_parse_To_QListQString_(this->m_curObjData.value("随机播放状态元").toString());
	}
	if (playType == "顺序播放状态元"){
		return TTool::_JSON_parse_To_QListQString_(this->m_curObjData.value("顺序播放状态元").toString());
	}
	return QStringList();
}
//状态节点 - 获取子节点 - 状态节点
QStringList C_StateTreeNode::getChildList_StateNode(){
	if (this->isObjData_StateNode() == false){ return QStringList(); }
	QString playType = this->m_curObjData["播放方式"].toString();
	if (playType == "随机播放嵌套集合"){
		return TTool::_JSON_parse_To_QListQString_(this->m_curObjData.value("随机播放嵌套集合").toString());
	}
	if (playType == "顺序播放嵌套集合"){
		return TTool::_JSON_parse_To_QListQString_(this->m_curObjData.value("顺序播放嵌套集合").toString());
	}
	return QStringList();
}
//状态节点 - 获取节点描述
QString C_StateTreeNode::getNodeDescription(){
	if (this->isObjData_StateNode() == false){ return ""; }
	QString playType = this->m_curObjData["播放方式"].toString();
	if (playType == "随机播放状态元" || playType == "随机播放嵌套集合"){
		int max_num = this->m_curObjData["随机播放的次数上限"].toString().toInt();
		return "R" + QString::number(max_num)+"→";
	}
	if (playType == "顺序播放状态元" || playType == "顺序播放嵌套集合"){
		return "L→";
	}
	return "";
}


/*-------------------------------------------------
		类 - 获取类名
*/
QStringList C_StateTreeNode::classInherited(){
	return C_Node::classInherited() << "C_StateTreeNode";
}
/*-------------------------------------------------
		类 - 判断类
*/
bool C_StateTreeNode::isClass_StateTreeNode(){
	return this->classIsInstanceOf("C_StateTreeNode");
}
/*-------------------------------------------------
		类 - 创建自己
*/
C_TreeNodePtr C_StateTreeNode::_factoryTree_newNode(){
	return this->_factoryTree_get()->createTreeNodeEmpty();
}
/*-------------------------------------------------
		类 - 获取类
*/
C_TreeNodePtr C_StateTreeNode::_factoryTree_getNodeById(QString node_id){
	return this->_factoryTree_get()->getTreeNode_ById(node_id);
}
/*-------------------------------------------------
		类 - 获取工厂
*/
C_TreeNodeFactoryPtr C_StateTreeNode::_factoryTree_get(){
	C_NodeFactoryPtr factory = S_NodeFactoryContainer::getInstance()->getFactory_ById(this->factory_id);
	if (factory->classIsInstanceOf("C_StateTreeNodeFactory") == false){ return C_StateTreeNodeFactoryPtr(); }
	return factory.dynamicCast<C_StateTreeNodeFactory>();
}


/*-------------------------------------------------
		复制数据 当前类 -> 目标类
*/
void C_StateTreeNode::copyTo(C_NodePtr data_to){
	C_TreeNode::copyTo(data_to);
	if (data_to->classIsInstanceOf("C_StateTreeNode") == false){ return; }
	C_StateTreeNodePtr treeData_to = data_to.dynamicCast<C_StateTreeNode>();

	// > 树节点
	//treeData_to->m_tree_curLayer = this->m_tree_curLayer;
	//treeData_to->m_tree_childIdList = this->m_tree_childIdList;
}
/*-------------------------------------------------
		复制数据 目标类 -> 当前类
*/
void C_StateTreeNode::copyFrom(C_NodePtr data_from){
	C_TreeNode::copyFrom(data_from);
	if (data_from->classIsInstanceOf("C_StateTreeNode") == false){ return; }
	C_StateTreeNodePtr treeData_from = data_from.dynamicCast<C_StateTreeNode>();

	// > 树节点
	//this->m_tree_curLayer = treeData_from->m_tree_curLayer;
	//this->m_tree_childIdList = treeData_from->m_tree_childIdList;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_StateTreeNode::getJsonObject(){
	QJsonObject obj = C_TreeNode::getJsonObject();

	// > 树节点
	//obj.insert("m_tree_curLayer", this->m_tree_curLayer);
	//obj.insert("m_tree_childIdList", this->m_tree_childIdList.join("___"));

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_StateTreeNode::setJsonObject(QJsonObject obj){
	C_TreeNode::setJsonObject(obj);

	// > 树节点
	//if (obj.value("m_tree_curLayer").isUndefined() == false){ this->m_tree_curLayer = obj.value("m_tree_curLayer").toInt(); }
	//if (obj.value("m_tree_childIdList").isUndefined() == false){ this->m_tree_childIdList = obj.value("m_tree_childIdList").toString().split("___"); }
}

