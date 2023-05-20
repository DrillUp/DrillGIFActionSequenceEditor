#include "stdafx.h"
#include "C_Node.h"

#include "C_NodeFactory.h"
#include "S_NodeFactoryContainer.h"
#include "Source/Utils/Common/TTool.h"


/*
-----==========================================================-----
		类：		节点.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个节点，用于描述无向图。
					
		使用方法：
				> 创建节点（需要先创建工厂，再通过工厂创建节点）
					this->m_C_NodeFactoryPtr = C_NodeFactoryPtr::create();
					S_NodeFactoryContainer::getInstance()->addFactory(this->m_C_NodeFactoryPtr);//（工厂创建后，需手动加入到工厂容器中）
					C_NodePtr node1 = this->m_C_NodeFactoryPtr->createNode("节点1");
					C_NodePtr node2 = this->m_C_NodeFactoryPtr->createNode("节点2");
				> 连接节点
					node1->connectNode(node2);
				> 删除节点
					this->m_C_NodeFactoryPtr->removeNode_ById(node1->getId());
				> 无向图主要功能
					bool result = this->m_C_NodeFactoryPtr->hasSelfConnect();	//是否含自连接
					bool result = this->m_C_NodeFactoryPtr->hasLoop();			//是否含回路
					bool result = this->m_C_NodeFactoryPtr->hasIsolated();		//是否含孤岛
-----==========================================================-----
*/
C_Node::C_Node(QString id, QString factory_id, QString nodeName) : C_ContainerDataBase(){
	
	// > 类
	this->id = id;							//标识
	this->name = nodeName;					//名称
	this->factory_id = factory_id;			//工厂标识
	
	// > 节点
	this->m_connectedNodeIdList.clear();	//连接的节点
	
	// > 自连接
	this->m_canSelfConnect = true;			//自连接
}

C_Node::~C_Node(){
}



/*-------------------------------------------------
		节点 - 连接节点
*/
void C_Node::connectNode(C_NodePtr node){
	this->connectNodeId(node->getId());
}
void C_Node::connectNodeId(QString node_id){
	
	// > 自连接情况
	if (node_id == this->getId()){
		this->connectSelf();
		return;
	}

	// > 该节点连接
	if (this->m_connectedNodeIdList.contains(node_id)){ return; }
	this->m_connectedNodeIdList.append(node_id);

	// > 目标节点连接
	C_NodePtr tar_node = this->_factory_getNodeById(node_id);
	if (tar_node == nullptr){ return; }
	tar_node->connectNodeId(this->getId());
}
/*-------------------------------------------------
		节点 - 断开节点
*/
void C_Node::disconnectNode(C_NodePtr node){
	this->disconnectNodeId(node->getId());
}
void C_Node::disconnectNodeId(QString node_id){

	// > 自连接断开情况
	if (node_id == this->getId()){
		this->disconnectSelf();
		return;
	}

	// > 该节点断开
	if (this->m_connectedNodeIdList.contains(node_id) == false){ return; }
	this->m_connectedNodeIdList.removeOne(node_id);

	// > 目标节点断开
	C_NodePtr tar_node = this->_factory_getNodeById(node_id);
	if (tar_node == nullptr){ return; }
	tar_node->disconnectNodeId(this->getId());
}
void C_Node::disconnectNodeAll(){

	// > 断开全部
	for (int i = 0; i < this->m_connectedNodeIdList.count(); i++){
		C_NodePtr tar_node = this->_factory_getNodeById(this->m_connectedNodeIdList.at(i));
		if (tar_node == nullptr){ continue; }
		tar_node->disconnectNodeId(this->getId());
	}
	this->m_connectedNodeIdList.clear();
}

/*-------------------------------------------------
		节点 - 获取 - 连接的节点
*/
QList<C_NodePtr> C_Node::getConnectedNode_All(){
	QList<C_NodePtr> result_list;
	for (int i = 0; i < this->m_connectedNodeIdList.count(); i++){
		QString node_id = this->m_connectedNodeIdList.at(i);
		C_NodePtr node = this->_factory_getNodeById(node_id);
		result_list.append(node);
	}
	return result_list;
}
/*-------------------------------------------------
		节点 - 获取 - 连接的节点Id
*/
QStringList C_Node::getConnectedNode_AllId(){
	return this->m_connectedNodeIdList;
}
/*-------------------------------------------------
		节点 - 获取 - 连接的节点数量
*/
int C_Node::getConnectedNode_AllCount(){
	return this->m_connectedNodeIdList.count();
}
/*-------------------------------------------------
		节点 - 获取 - 是否含某节点连接
*/
bool C_Node::hasConnect(C_NodePtr node){
	return this->hasConnectId(node->getId());
}
bool C_Node::hasConnectId(QString node_id){
	return this->m_connectedNodeIdList.contains(node_id);
}


/*-------------------------------------------------
		自连接 - 设置
*/
void C_Node::setCanSelfConnect(bool enabled){
	this->m_canSelfConnect = enabled;
	if (enabled == false){
		this->disconnectSelf();
	}
}
bool C_Node::canSelfConnect(){
	return this->m_canSelfConnect;
}
/*-------------------------------------------------
		自连接 - 是否含有自连接
*/
bool C_Node::hasSelfConnect(){
	return this->m_connectedNodeIdList.contains(this->getId());
}
/*-------------------------------------------------
		自连接 - 自连接
*/
void C_Node::connectSelf(){
	if (this->m_canSelfConnect == false){ return; }
	this->m_connectedNodeIdList.append(this->getId());
}
/*-------------------------------------------------
		自连接 - 断开自连接
*/
void C_Node::disconnectSelf(){
	this->m_connectedNodeIdList.removeOne(this->getId());
}


/*-------------------------------------------------
		类 - 访问器 - 名称
*/
void C_Node::setName(QString nodeName){
	this->name = nodeName;
}
QString C_Node::getName(){
	return this->name;
}
/*-------------------------------------------------
		类 - 访问器 - 工厂标识
*/
void C_Node::setFactoryId(QString factory_id){
	this->factory_id = factory_id;
}
QString C_Node::getFactoryId(){
	return this->factory_id;
}
/*-------------------------------------------------
		访问器 - 获取子类类名
*/
QString C_Node::get__CLASS_NAME__(){
	return "C_Node";
}
/*-------------------------------------------------
		访问器 - 获取子类类名（中文名）
*/
QString C_Node::get__CLASS_NAME__Chinese(){
	return "节点";
}
/*-------------------------------------------------
		访问器 - 获取所有继承的类名
*/
QStringList C_Node::get__CLASS_INHERITED_LIST__(){
	return C_ContainerDataBase::get__CLASS_INHERITED_LIST__() << "C_Node";
}
/*-------------------------------------------------
		访问器 - 是否继承了某类
*/
bool C_Node::get__CLASS_IS_INSTANCE_OF__(QString class_name){
	return this->get__CLASS_INHERITED_LIST__().contains(class_name);
}
/*-------------------------------------------------
		类 - 判断类
*/
bool C_Node::isClass_Node(){
	return this->get__CLASS_IS_INSTANCE_OF__("C_Node");
}
/*-------------------------------------------------
		类 - 创建自己
*/
C_NodePtr C_Node::_factory_newNode(){
	if (this->_factory_get().isNull()){ return C_NodePtr(); };
	return this->_factory_get()->createNodeEmpty();
}
/*-------------------------------------------------
		类 - 获取类
*/
C_NodePtr C_Node::_factory_getNodeById(QString id){
	if (this->_factory_get().isNull()){ return C_NodePtr(); };
	return this->_factory_get()->getNode_ById(id);
}
/*-------------------------------------------------
		类 - 获取工厂
*/
C_NodeFactoryPtr C_Node::_factory_get(){
	return S_NodeFactoryContainer::getInstance()->getFactory_ById(this->factory_id);
}


/*-------------------------------------------------
		运算符重载
*/
const bool C_Node::operator== (const C_Node& a)const {
	return this->id == a.id;
}
/*-------------------------------------------------
		判断空
*/
bool C_Node::isNull(){
	if (this->name == ""){ return true; }
	return C_ContainerDataBase::isNull();
}
/*-------------------------------------------------
		复制对象
*/
C_NodePtr C_Node::clone(){
	C_NodePtr result = this->_factory_newNode();
	this->copyTo(result);
	return result;
}
/*-------------------------------------------------
		复制数据 当前类 -> 目标类
*/
void C_Node::copyTo(C_NodePtr data_to){

	// > 类
	//data_to->id = this->id;（不含id）
	data_to->name = this->name;
	data_to->factory_id = this->factory_id;

	// > 节点
	//data_to->m_connectedNodeIdList = this->m_connectedNodeIdList;（不含节点连接）

	// > 自连接
	data_to->m_canSelfConnect = this->m_canSelfConnect;
}
/*-------------------------------------------------
		复制数据 目标类 -> 当前类
*/
void C_Node::copyFrom(C_NodePtr data_from){

	// > 类
	//this->id = data_from->id;（不含id）
	this->name = data_from->name;
	this->factory_id = data_from->factory_id;

	// > 节点
	//this->m_connectedNodeIdList = data_from->m_connectedNodeIdList; （不含节点连接）

	// > 自连接
	this->m_canSelfConnect = data_from->m_canSelfConnect;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_Node::getJsonObject_childData(){
	QJsonObject obj;

	// > 类
	obj.insert("name", this->name);
	obj.insert("factory_id", this->factory_id);

	// > 节点
	obj.insert("m_connectedNodeIdList", this->m_connectedNodeIdList.join("___"));

	// > 自连接
	obj.insert("m_canSelfConnect", this->m_canSelfConnect);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_Node::setJsonObject_childData(QJsonObject obj){

	// > 类
	if (obj.value("name").isUndefined() == false){ this->name = obj.value("name").toString(); }
	if (obj.value("factory_id").isUndefined() == false){ this->factory_id = obj.value("factory_id").toString(); }

	// > 节点
	if (obj.value("m_connectedNodeIdList").isUndefined() == false){ this->m_connectedNodeIdList = obj.value("m_childrenIdList").toString().split("___"); }

	// > 自连接
	if (obj.value("m_canSelfConnect").isUndefined() == false){ this->m_canSelfConnect = obj.value("m_canSelfConnect").toBool(); }
}

