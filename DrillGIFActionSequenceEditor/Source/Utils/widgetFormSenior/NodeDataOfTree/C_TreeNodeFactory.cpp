#include "stdafx.h"
#include "C_TreeNodeFactory.h"


/*
-----==========================================================-----
		类：		树节点工厂.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个树节点工厂+树节点容器，用于描述/存放一棵树。
					【此类包含工厂+容器，创建此类后需要 手动 加入到工厂容器中】

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

C_TreeNodeFactory::C_TreeNodeFactory() : C_NodeFactory(){
	this->m_NodeTank.clear();

	// > 错误
	this->m_errorInRecursion = false;
	this->m_errorMessage.clear();
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
	if (this->m_errorInRecursion == true){ return; }
	if (layer > 40){
		this->m_errorMessage.append("C_TreeNodeFactory中函数setTreeLayer_Recursion递归次数过载，请重新检查树节点连接情况。");
		qDebug() << "错误：" << this->m_errorMessage.last();
		this->m_errorInRecursion = true;
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
		错误 - 是否出现错误
*/
bool C_TreeNodeFactory::hasError(){
	return this->m_errorMessage.count() > 0;
}
/*-------------------------------------------------
		错误 - 获取错误信息
*/
QStringList C_TreeNodeFactory::getErrorMessage(){
	return this->m_errorMessage;
}


/*-------------------------------------------------
		访问器 - 获取子类类名
*/
QString C_TreeNodeFactory::get__CLASS_NAME__(){
	return "C_TreeNodeFactory";
}
/*-------------------------------------------------
		访问器 - 获取子类类名（中文名）
*/
QString C_TreeNodeFactory::get__CLASS_NAME__Chinese(){
	return "树节点工厂";
}
/*-------------------------------------------------
		访问器 - 获取所有继承的类名
*/
QStringList C_TreeNodeFactory::get__CLASS_INHERITED_LIST__(){
	return C_NodeFactory::get__CLASS_INHERITED_LIST__() << "C_TreeNodeFactory";
}
/*-------------------------------------------------
		访问器 - 是否继承了某类
*/
bool C_TreeNodeFactory::get__CLASS_IS_INSTANCE_OF__(QString class_name){
	return this->get__CLASS_INHERITED_LIST__().contains(class_name);
}
/*-------------------------------------------------
		类 - 判断类
*/
bool C_TreeNodeFactory::isClass_TreeNodeFactory(){
	return this->get__CLASS_IS_INSTANCE_OF__("C_TreeNodeFactory");
}

/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_TreeNodeFactory::getJsonObject_childData(){
	QJsonObject obj = C_NodeFactory::getJsonObject_childData();

	// > 树节点容器
	//（暂无）

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_TreeNodeFactory::setJsonObject_childData(QJsonObject obj){
	C_NodeFactory::setJsonObject_childData(obj);

	// > 树节点容器
	//（暂无）
}
