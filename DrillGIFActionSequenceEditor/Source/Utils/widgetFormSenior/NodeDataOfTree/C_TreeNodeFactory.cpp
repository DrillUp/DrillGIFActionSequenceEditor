#include "stdafx.h"
#include "C_TreeNodeFactory.h"


/*
-----==========================================================-----
		类：		树节点工厂.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		存放快速类对象的类容器。
					【此容器用于主键外键之间识别用，相当于一个库】
-----==========================================================-----
*/

C_TreeNodeFactory::C_TreeNodeFactory() : C_NodeFactory(){
	this->m_NodeTank.clear();
}
C_TreeNodeFactory::~C_TreeNodeFactory() {
}


/*-----------------------------------
		工厂 - 创建 节点
*/
C_NodePtr C_TreeNodeFactory::createNode(QString nodeName){
	return this->createTreeNode(nodeName);
}
C_NodePtr C_TreeNodeFactory::createNodeEmpty(){
	return this->createTreeNodeEmpty();
}
/*-----------------------------------
		工厂 - 创建 树节点
*/
C_TreeNodePtr C_TreeNodeFactory::createTreeNode(QString nodeName){
	QString new_id = QUuid::createUuid().toString();
	new_id = new_id.replace("{", "");
	new_id = new_id.replace("}", "");
	new_id = new_id.replace("-", "");
	C_TreeNodePtr ptr = C_TreeNodePtr::create(new_id, this->id, nodeName);
	
	// > 添加到工厂 类容器
	this->m_NodeTank.append(ptr);

	return ptr;
}
C_TreeNodePtr C_TreeNodeFactory::createTreeNodeEmpty(){
	return this->createTreeNode("");
}


/*-----------------------------------
		树关系 - 是否为树
*/
bool C_TreeNodeFactory::isTree(){
	if (this->hasLoop()){ return false; }
	if (this->hasIsolated()){ return false; }
	return true;
}
/*-----------------------------------
		树关系 - 设置树根
*/
void C_TreeNodeFactory::setTreeRoot(C_TreeNodePtr tree_node){
	if (this->isTree() == false){ return; }
	this->setTreeLayer_Recursion(tree_node, 0);
}
/*-----------------------------------
		树关系 - 根据树根设置层级（私有）
*/
void C_TreeNodeFactory::setTreeLayer_Recursion(C_TreeNodePtr tree_node, int layer){
	if (layer > 40){
		QMessageBox::about(nullptr, "提示", "C_TreeNodeFactory中函数setTreeLayer_Recursion递归次数过载，请重新检查树节点连接情况。");
		return;
	}
	if (tree_node->getTree_CurLayer() != -1){ return; }

	// > 赋值 - 当前层级
	tree_node->setTree_CurLayer_FactoryOnly(layer);
	
	// > 赋值 - 子节点列表（只能连向层级为 -1 的节点）
	QStringList childId_list;
	QList<C_NodePtr> c_node_list = tree_node->getConnectedNode_All();
	for (int i = 0; i < c_node_list.count(); i++){
		C_TreeNodePtr tree_node = c_node_list.at(i).dynamicCast<C_TreeNode>();
		if (tree_node->getTree_CurLayer() == -1){
			childId_list.append(tree_node->getId());
		}
	}
	tree_node->setTree_ChildIdList_FactoryOnly(childId_list);

	// > 向下递归
	QList<C_TreeNodePtr> node_list = tree_node->getChildNode_Current();
	for (int i = 0; i < node_list.count(); i++){
		C_TreeNodePtr next_node = node_list.at(i);
		this->setTreeLayer_Recursion(next_node, layer + 1);
	}
}
/*-----------------------------------
		树关系 - 是否含树根
*/
bool C_TreeNodeFactory::hasTreeRoot(){
	return this->getTreeRoot().isNull() == false;
}
/*-----------------------------------
		树关系 - 获取 - 树根
*/
C_TreeNodePtr C_TreeNodeFactory::getTreeRoot(){
	QList<C_TreeNodePtr> node_list = this->getTreeNode_All();
	for (int i = 0; i < node_list.count(); i++){
		C_TreeNodePtr node = node_list.at(i);
		if (node->isTreeRoot()){
			return node;
		}
	}
	return C_TreeNodePtr();
}
/*-----------------------------------
		树关系 - 获取 - 树枝（包含树根）
*/
QList<C_TreeNodePtr> C_TreeNodeFactory::getTreeBranchAll(){
	QList<C_TreeNodePtr> result;
	QList<C_TreeNodePtr> node_list = this->getTreeNode_All();
	for (int i = 0; i < node_list.count(); i++){
		C_TreeNodePtr node = node_list.at(i);
		if (node->isTreeBranch()){
			result.append(node);
		}
	}
	return result;
}
int C_TreeNodeFactory::getTreeBranchCount(){
	int result = 0;
	QList<C_TreeNodePtr> node_list = this->getTreeNode_All();
	for (int i = 0; i < node_list.count(); i++){
		C_TreeNodePtr node = node_list.at(i);
		if (node->isTreeBranch()){
			result += 1;
		}
	}
	return result;
}
/*-----------------------------------
		树关系 - 获取 - 树叶
*/
QList<C_TreeNodePtr> C_TreeNodeFactory::getTreeLeafAll(){
	QList<C_TreeNodePtr> result;
	QList<C_TreeNodePtr> node_list = this->getTreeNode_All();
	for (int i = 0; i < node_list.count(); i++){
		C_TreeNodePtr node = node_list.at(i);
		if (node->isTreeLeaf()){
			result.append(node);
		}
	}
	return result;
}
int C_TreeNodeFactory::getTreeLeafCount(){
	int result = 0;
	QList<C_TreeNodePtr> node_list = this->getTreeNode_All();
	for (int i = 0; i < node_list.count(); i++){
		C_TreeNodePtr node = node_list.at(i);
		if (node->isTreeLeaf()){
			result += 1;
		}
	}
	return result;
}
/*-----------------------------------
		树关系 - 获取 - 树深度
*/
int C_TreeNodeFactory::getTreeDeep(){
	int result = 0;
	QList<C_TreeNodePtr> node_list = this->getTreeNode_All();
	for (int i = 0; i < node_list.count(); i++){
		C_TreeNodePtr node = node_list.at(i);
		if (result < node->getTree_CurLayer()){
			result = node->getTree_CurLayer();
		}
	}
	return result + 1;
}
/*-----------------------------------
		树关系 - 获取 - 树节点（根据深度）
*/
QList<C_TreeNodePtr> C_TreeNodeFactory::getTreeNode_ByDeep(int deep){
	QList<C_TreeNodePtr> result;
	QList<C_TreeNodePtr> node_list = this->getTreeNode_All();
	for (int i = 0; i < node_list.count(); i++){
		C_TreeNodePtr node = node_list.at(i);
		if (node->getTree_CurLayer() == deep){
			result.append(node);
		}
	}
	return result;
}


/*-----------------------------------
		类容器 - 获取 - 全部（原装）
*/
QList<C_TreeNodePtr> C_TreeNodeFactory::getTreeNode_AllOrg(){
	QList<C_TreeNodePtr> result_list;
	for (int i = 0; i < this->m_NodeTank.count(); i++){
		C_NodePtr node = this->m_NodeTank.at(i);
		result_list.append(node.dynamicCast<C_TreeNode>());
	}
	return result_list;
}
/*-----------------------------------
		类容器 - 获取 - 全部Id
*/
QList<C_TreeNodePtr> C_TreeNodeFactory::getTreeNode_All(){
	QList<C_TreeNodePtr> result_list;
	for (int i = 0; i < this->m_NodeTank.count(); i++){
		C_NodePtr node = this->m_NodeTank.at(i);
		if (node->isNull()){ continue; }
		result_list.append(node.dynamicCast<C_TreeNode>());
	}
	return result_list;
}
/*-----------------------------------
		类容器 - 获取 - 全部Id
*/
QStringList C_TreeNodeFactory::getTreeNode_AllId(){
	return C_NodeFactory::getNode_AllId();
}
/*-----------------------------------
		类容器 - 获取 - 全部名称
*/
QStringList C_TreeNodeFactory::getTreeNode_AllName(){
	return C_NodeFactory::getNode_AllName();
}
/*-----------------------------------
		类容器 - 获取 - 根据ID
*/
C_TreeNodePtr C_TreeNodeFactory::getTreeNode_ById(QString node_id){
	C_NodePtr node = C_NodeFactory::getNode_ById(node_id);
	return node.dynamicCast<C_TreeNode>();
}
/*-----------------------------------
		类容器 - 获取 - 根据名称
*/
C_TreeNodePtr C_TreeNodeFactory::getTreeNode_ByName(QString node_name){
	C_NodePtr node = C_NodeFactory::getNode_ByName(node_name);
	return node.dynamicCast<C_TreeNode>();
}
/*-----------------------------------
		类容器 - 是否含ID
*/
bool C_TreeNodeFactory::hasTreeNode_Id(QString node_id){
	return this->getTreeNodeIndex_ById(node_id) != -1;
}
/*-----------------------------------
		类容器 - 是否含名称
*/
bool C_TreeNodeFactory::hasTreeNode_Name(QString node_name){
	return this->getTreeNodeIndex_ByName(node_name) != -1;
}
/*-----------------------------------
		私有 - 获取索引（根据ID）
*/
int C_TreeNodeFactory::getTreeNodeIndex_ById(QString node_id){
	return C_NodeFactory::getNodeIndex_ById(node_id);
}
/*-----------------------------------
		私有 - 获取索引（根据ID）
*/
int C_TreeNodeFactory::getTreeNodeIndex_ByName(QString node_name){
	return C_NodeFactory::getNodeIndex_ByName(node_name);
}


/*-------------------------------------------------
		类 - 获取类名
*/
QStringList C_TreeNodeFactory::classInherited(){
	return C_NodeFactory::classInherited() << "C_TreeNodeFactory";
}
/*-------------------------------------------------
		类 - 判断类
*/
bool C_TreeNodeFactory::isClass_TreeNodeFactory(){
	return this->classIsInstanceOf("C_TreeNodeFactory");
}

/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_TreeNodeFactory::getJsonObject(){
	QJsonObject obj = C_NodeFactory::getJsonObject();

	// > 树节点容器
	//（暂无）

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_TreeNodeFactory::setJsonObject(QJsonObject obj){
	C_NodeFactory::setJsonObject(obj);

	// > 树节点容器
	//（暂无）
}
