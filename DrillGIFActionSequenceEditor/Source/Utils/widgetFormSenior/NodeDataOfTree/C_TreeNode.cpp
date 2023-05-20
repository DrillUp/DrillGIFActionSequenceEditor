#include "stdafx.h"
#include "C_TreeNode.h"

#include "C_TreeNodeFactory.h"
#include "../NodeData/S_NodeFactoryContainer.h"
#include "Source/Utils/Common/TTool.h"


/*
-----==========================================================-----
		类：		树节点.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个树节点，用于建立一棵树。
					
		使用方法：
				> 创建树节点（需要先创建工厂，再通过工厂创建节点）
					this->m_C_TreeNodeFactoryPtr = C_TreeNodeFactoryPtr::create();
					S_NodeFactoryContainer::getInstance()->addFactory(this->m_C_TreeNodeFactoryPtr);//（工厂创建后，需手动加入到工厂容器中）
					C_TreeNodePtr node1 = this->m_C_TreeNodeFactoryPtr->createNode("节点1");
					C_TreeNodePtr node2 = this->m_C_TreeNodeFactoryPtr->createNode("节点2");
				> 连接树节点
					node1->connectNode(node2);
				> 删除树节点
					this->m_C_TreeNodeFactoryPtr->removeNode_ById(node1->getId());
				> 树图判定功能
					bool result = this->m_C_TreeNodeFactoryPtr->hasSelfConnect();	//是否含自连接
					bool result = this->m_C_TreeNodeFactoryPtr->hasLoop();			//是否含回路
					bool result = this->m_C_TreeNodeFactoryPtr->hasIsolated();		//是否含孤岛
					bool result = this->m_C_TreeNodeFactoryPtr->isTree();			//是否为树
				> 树图主要功能
					this->m_C_TreeNodeFactoryPtr->setTreeRoot(node1);				//设置树根
					int result = this->m_C_TreeNodeFactoryPtr->getTreeDeep();		//获取树深度
-----==========================================================-----
*/
C_TreeNode::C_TreeNode(QString id, QString factory_id, QString nodeName) : C_Node(id, factory_id, nodeName){

	// > 类
	this->id = id;							//标识
	this->name = nodeName;					//名称
	this->factory_id = factory_id;			//工厂标识

	// > 节点
	this->m_connectedNodeIdList.clear();	//连接的节点

	// > 自连接
	this->m_canSelfConnect = false;			//自连接（树节点禁止自连接）

	// > 树节点
	this->m_tree_curLayer = -1;
	this->m_tree_childIdList.clear();

	// > 错误
	this->m_errorInRecursion = false;
	this->m_errorMessage.clear();
}

C_TreeNode::~C_TreeNode(){
}


/*-------------------------------------------------
		树节点 - 设置当前层级
*/
void C_TreeNode::setTree_CurLayer_FactoryOnly(int layer){
	this->m_tree_curLayer = layer;
}
/*-------------------------------------------------
		树节点 - 设置子节点
*/
void C_TreeNode::setTree_ChildIdList_FactoryOnly(QStringList id_list){
	this->m_tree_childIdList = id_list;
}

/*-------------------------------------------------
		树节点 - 获取当前层级
*/
int C_TreeNode::getTree_CurLayer(){
	return this->m_tree_curLayer;
}
/*-------------------------------------------------
		树节点 - 获取父节点
*/
C_TreeNodePtr C_TreeNode::getTree_Parent(){
	return this->_factoryTree_get()->getTreeNode_ById(this->getTree_ParentId());
}
QString C_TreeNode::getTree_ParentId(){
	if (this->m_tree_curLayer == -1){ return ""; }

	QList<C_TreeNodePtr> treeNode_list = this->_factoryTree_get()->getTreeNode_All();
	for (int i = 0; i < treeNode_list.count(); i++){
		C_TreeNodePtr treeNode = treeNode_list.at(i);
		QStringList id_list = treeNode->getChildNodeId_Current();
		if (id_list.contains(this->getId())){		//（由于是树结构，所以只要找到包含 该节点id 的树节点，即父节点）
			return treeNode->getId();
		}
	}
	return "";
}

/*-------------------------------------------------
		树节点 - 是否为树根
*/
bool C_TreeNode::isTreeRoot(){
	return this->m_tree_curLayer == 0;
}
/*-------------------------------------------------
		树节点 - 是否为树枝（包含树根）
*/
bool C_TreeNode::isTreeBranch(){
	if (this->m_tree_curLayer == -1){ return false; }
	return this->m_tree_childIdList.count() > 0;
}
/*-------------------------------------------------
		树节点 - 是否为树叶
*/
bool C_TreeNode::isTreeLeaf(){
	if (this->m_tree_curLayer == -1){ return false; }
	return this->m_tree_childIdList.count() == 0;
}
/*-------------------------------------------------
		树节点 - 获取树节点数量
*/
QList<C_TreeNodePtr> C_TreeNode::getChildNode_Current(){
	QList<C_TreeNodePtr> result_list;
	for (int i = 0; i < this->m_tree_childIdList.count(); i++){
		QString node_id = this->m_tree_childIdList.at(i);
		C_TreeNodePtr node = this->_factoryTree_get()->getTreeNode_ById(node_id);
		result_list.append(node);
	}
	return result_list;
}
QList<C_TreeNodePtr> C_TreeNode::getChildNode_Recursion(int layer_deep){
	if (this->m_errorInRecursion == true){ return QList<C_TreeNodePtr>(); }
	if (layer_deep > 40){
		this->m_errorMessage.append("C_TreeNode中函数getChildNode_Recursion递归次数过载，请重新检查树节点连接情况。");
		qDebug() << "错误：" << this->m_errorMessage.last();
		this->m_errorInRecursion = true;
		return QList<C_TreeNodePtr>();
	}
	QList<C_TreeNodePtr> result_list;
	for (int i = 0; i < this->m_tree_childIdList.count(); i++){
		QString node_id = this->m_tree_childIdList.at(i);
		C_TreeNodePtr node = this->_factoryTree_get()->getTreeNode_ById(node_id);
		result_list.append(node);
		result_list.append(node->getChildNode_Recursion(layer_deep + 1));
	}
	return result_list;
}
/*-------------------------------------------------
		树节点 - 获取 - 树节点Id
*/
QStringList C_TreeNode::getChildNodeId_Current(){
	return this->m_tree_childIdList;
}
QStringList C_TreeNode::getChildNodeId_Recursion(int layer_deep){
	if (this->m_errorInRecursion == true){ return QStringList(); }
	if (layer_deep > 40){
		this->m_errorMessage.append("C_TreeNode中函数getChildNodeId_Recursion递归次数过载，请重新检查树节点连接情况。");
		qDebug() << "错误：" << this->m_errorMessage.last();
		this->m_errorInRecursion = true;
		return QStringList();
	}
	QStringList result_list;
	for (int i = 0; i < this->m_tree_childIdList.count(); i++){
		QString node_id = this->m_tree_childIdList.at(i);
		C_TreeNodePtr node = this->_factoryTree_get()->getTreeNode_ById(node_id);
		result_list.append( node->getChildNodeId_Recursion(layer_deep + 1) );
	}
	return result_list;
}
/*-------------------------------------------------
		树节点 - 获取 - 树节点数量
*/
int C_TreeNode::getChildNodeCount_Current(){
	return this->m_tree_childIdList.count();
}
int C_TreeNode::getChildNodeCount_Recursion(int layer_deep){
	if (this->m_errorInRecursion == true){ return 0; }
	if (layer_deep > 40){
		this->m_errorMessage.append("C_TreeNode中函数getChildNodeCount_Recursion递归次数过载，请重新检查树节点连接情况。");
		qDebug() << "错误：" << this->m_errorMessage.last();
		this->m_errorInRecursion = true;
		return 0;
	}

	// > 当前为叶子
	if (this->isTreeLeaf()){
		return 0;
	}

	// > 当前为树枝
	int count = 0;
	for (int i = 0; i < this->m_tree_childIdList.count(); i++){
		QString node_id = this->m_tree_childIdList.at(i);
		C_TreeNodePtr node = this->_factoryTree_get()->getTreeNode_ById(node_id);
		count += node->getChildNodeCount_Recursion(layer_deep + 1);
	}
	return count;
}
/*-------------------------------------------------
		树节点 - 获取 - 树枝
*/
QList<C_TreeNodePtr> C_TreeNode::getChildBranch_Current(){
	QList<C_TreeNodePtr> result_list;
	for (int i = 0; i < this->m_tree_childIdList.count(); i++){
		QString node_id = this->m_tree_childIdList.at(i);
		C_TreeNodePtr node = this->_factoryTree_get()->getTreeNode_ById(node_id);
		if (node->isTreeBranch()){
			result_list.append(node);
		}
	}
	return result_list;
}
QList<C_TreeNodePtr> C_TreeNode::getChildBranch_Recursion(int layer_deep){
	if (this->m_errorInRecursion == true){ return QList<C_TreeNodePtr>(); }
	if (layer_deep > 40){
		this->m_errorMessage.append("C_TreeNode中函数getChildNode_Recursion递归次数过载，请重新检查树节点连接情况。");
		qDebug() << "错误：" << this->m_errorMessage.last();
		this->m_errorInRecursion = true;
		return QList<C_TreeNodePtr>();
	}

	// > 当前为叶子
	if (this->isTreeLeaf()){
		return QList<C_TreeNodePtr>();
	}

	// > 当前为树枝
	QList<C_TreeNodePtr> result_list;
	for (int i = 0; i < this->m_tree_childIdList.count(); i++){
		QString node_id = this->m_tree_childIdList.at(i);
		C_TreeNodePtr node = this->_factoryTree_get()->getTreeNode_ById(node_id);
		if (node->isTreeBranch()){
			result_list.append(node);
		}
		result_list.append(node->getChildBranch_Recursion(layer_deep + 1));
	}
	return result_list;
}
/*-------------------------------------------------
		树节点 - 获取 - 叶子
*/
QList<C_TreeNodePtr> C_TreeNode::getChildLeaf_Current(){
	QList<C_TreeNodePtr> result_list;
	for (int i = 0; i < this->m_tree_childIdList.count(); i++){
		QString node_id = this->m_tree_childIdList.at(i);
		C_TreeNodePtr node = this->_factoryTree_get()->getTreeNode_ById(node_id);
		if (node->isTreeBranch()){
			result_list.append(node);
		}
	}
	return result_list;
}
QList<C_TreeNodePtr> C_TreeNode::getChildLeaf_Recursion(int layer_deep){
	if (this->m_errorInRecursion == true){ return QList<C_TreeNodePtr>(); }
	if (layer_deep > 40){
		this->m_errorMessage.append("C_TreeNode中函数getChildLeaf_Recursion递归次数过载，请重新检查树节点连接情况。");
		qDebug() << "错误：" << this->m_errorMessage.last();
		this->m_errorInRecursion = true;
		return QList<C_TreeNodePtr>();
	}

	// > 当前为叶子
	if (this->isTreeLeaf()){
		return QList<C_TreeNodePtr>();
	}

	// > 当前为树枝
	QList<C_TreeNodePtr> result_list;
	for (int i = 0; i < this->m_tree_childIdList.count(); i++){
		QString node_id = this->m_tree_childIdList.at(i);
		C_TreeNodePtr node = this->_factoryTree_get()->getTreeNode_ById(node_id);
		if (node->isTreeLeaf()){
			result_list.append(node);
		}
		result_list.append(node->getChildLeaf_Recursion(layer_deep + 1));
	}
	return result_list;
}

/*-------------------------------------------------
		错误 - 是否出现错误
*/
bool C_TreeNode::hasError(){
	return this->m_errorMessage.count() > 0;
}
/*-------------------------------------------------
		错误 - 获取错误信息
*/
QStringList C_TreeNode::getErrorMessage(){
	return this->m_errorMessage;
}


/*-------------------------------------------------
		访问器 - 获取子类类名
*/
QString C_TreeNode::get__CLASS_NAME__(){
	return "C_TreeNode";
}
/*-------------------------------------------------
		访问器 - 获取子类类名（中文名）
*/
QString C_TreeNode::get__CLASS_NAME__Chinese(){
	return "树节点";
}
/*-------------------------------------------------
		访问器 - 获取所有继承的类名
*/
QStringList C_TreeNode::get__CLASS_INHERITED_LIST__(){
	return C_Node::get__CLASS_INHERITED_LIST__() << "C_TreeNode";
}
/*-------------------------------------------------
		访问器 - 是否继承了某类
*/
bool C_TreeNode::get__CLASS_IS_INSTANCE_OF__(QString class_name){
	return this->get__CLASS_INHERITED_LIST__().contains(class_name);
}
/*-------------------------------------------------
		类 - 判断类
*/
bool C_TreeNode::isClass_TreeNode(){
	return this->get__CLASS_IS_INSTANCE_OF__("C_TreeNode");
}
/*-------------------------------------------------
		类 - 创建自己
*/
C_NodePtr C_TreeNode::_factory_newNode(){
	return this->_factoryTree_get()->createNodeEmpty();
}
/*-------------------------------------------------
		类 - 获取类
*/
C_NodePtr C_TreeNode::_factory_getNodeById(QString node_id){
	return this->_factoryTree_get()->getTreeNode_ById(node_id);
}
/*-------------------------------------------------
		类 - 获取工厂
*/
C_NodeFactoryPtr C_TreeNode::_factory_get(){
	return this->_factoryTree_get();
}
/*-------------------------------------------------
		类 - 创建自己
*/
C_TreeNodePtr C_TreeNode::_factoryTree_newNode(){
	return this->_factoryTree_get()->createTreeNodeEmpty();
}
/*-------------------------------------------------
		类 - 获取类
*/
C_TreeNodePtr C_TreeNode::_factoryTree_getNodeById(QString node_id){
	return this->_factoryTree_get()->getTreeNode_ById(node_id);
}
/*-------------------------------------------------
		类 - 获取工厂
*/
C_TreeNodeFactoryPtr C_TreeNode::_factoryTree_get(){
	C_NodeFactoryPtr factory = S_NodeFactoryContainer::getInstance()->getFactory_ById(this->factory_id);
	if (factory->get__CLASS_IS_INSTANCE_OF__("C_TreeNodeFactory") == false){ return C_TreeNodeFactoryPtr(); }
	return factory.dynamicCast<C_TreeNodeFactory>();
}


/*-------------------------------------------------
		复制数据 当前类 -> 目标类
*/
void C_TreeNode::copyTo(C_NodePtr data_to){
	C_Node::copyTo(data_to);
	if (data_to->get__CLASS_IS_INSTANCE_OF__("C_TreeNode") == false){ return; }
	C_TreeNodePtr treeData_to = data_to.dynamicCast<C_TreeNode>();

	// > 树节点
	treeData_to->m_tree_curLayer = this->m_tree_curLayer;
	treeData_to->m_tree_childIdList = this->m_tree_childIdList;
}
/*-------------------------------------------------
		复制数据 目标类 -> 当前类
*/
void C_TreeNode::copyFrom(C_NodePtr data_from){
	C_Node::copyFrom(data_from);
	if (data_from->get__CLASS_IS_INSTANCE_OF__("C_TreeNode") == false){ return; }
	C_TreeNodePtr treeData_from = data_from.dynamicCast<C_TreeNode>();

	// > 树节点
	this->m_tree_curLayer = treeData_from->m_tree_curLayer;
	this->m_tree_childIdList = treeData_from->m_tree_childIdList;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_TreeNode::getJsonObject_childData(){
	QJsonObject obj = C_Node::getJsonObject_childData();

	// > 树节点
	obj.insert("m_tree_curLayer", this->m_tree_curLayer);
	obj.insert("m_tree_childIdList", this->m_tree_childIdList.join("___"));

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_TreeNode::setJsonObject_childData(QJsonObject obj){
	C_Node::setJsonObject_childData(obj);

	// > 树节点
	if (obj.value("m_tree_curLayer").isUndefined() == false){ this->m_tree_curLayer = obj.value("m_tree_curLayer").toInt(); }
	if (obj.value("m_tree_childIdList").isUndefined() == false){ this->m_tree_childIdList = obj.value("m_tree_childIdList").toString().split("___"); }
}

