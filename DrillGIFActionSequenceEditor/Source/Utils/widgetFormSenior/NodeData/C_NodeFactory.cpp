#include "stdafx.h"
#include "C_NodeFactory.h"


/*
-----==========================================================-----
		类：		节点工厂.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个节点工厂+节点容器，用于描述/存放无向图。
					【此类包含工厂+容器，创建此类后需要 手动 加入到工厂容器中】
					
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

C_NodeFactory::C_NodeFactory() : C_ContainerDataBase(){

	// > 类
	QString id = QUuid::createUuid().toString();
	id = id.replace("{", "");
	id = id.replace("}", "");
	id = id.replace("-", "");
	this->id = id;
	
	// > 容器
	this->m_NodeTank.clear();
}

C_NodeFactory::~C_NodeFactory() {
}


/*-----------------------------------
		工厂 - 创建 节点
*/
C_NodePtr C_NodeFactory::createNode(QString className){
	QString new_id = QUuid::createUuid().toString();
	new_id = new_id.replace("{", "");
	new_id = new_id.replace("}", "");
	new_id = new_id.replace("-", "");
	C_NodePtr ptr = C_NodePtr::create(new_id, this->id, className);
	
	// > 添加到工厂 类容器
	this->m_NodeTank.append(ptr);

	return ptr;
}
C_NodePtr C_NodeFactory::createNodeEmpty(){
	return this->createNode("");
}


/*-------------------------------------------------
		图关系 - 是否含自连接
*/
bool C_NodeFactory::hasSelfConnect(){
	for (int i = 0; i < this->m_NodeTank.count(); i++){
		C_NodePtr node = this->m_NodeTank.at(i);
		if (node->hasSelfConnect()){ return true; }
	}
	return false;
}
/*-------------------------------------------------
		图关系 - 是否含回路
*/
bool C_NodeFactory::hasLoop(){

	// > 有自连接，则一定含回路
	if (this->hasSelfConnect()){ return true; };

	// > 深度优先遍历
	this->ergodicNode(this->m_NodeTank.first());

	return this->m_temp_hasLoop;
}
/*-------------------------------------------------
		图关系 - 是否含孤岛
*/
bool C_NodeFactory::hasIsolated(){

	// > 深度优先遍历
	this->ergodicNode(this->m_NodeTank.first());

	return this->m_temp_hasIsolated;
}
/*-------------------------------------------------
		图关系 - 执行遍历
*/
void C_NodeFactory::ergodicNode(C_NodePtr root){
	if (root.isNull()) { return; }
	QList<C_NodePtr> actived_list;		//已经走过的点
	QStringList activedLine_list;		//已经走过的线

	// > 栈准备
	QStack<C_NodePtr> node_stack;
	node_stack.push(root);
	actived_list.append(root);
	this->m_temp_hasLoop = false;
	this->m_temp_hasIsolated = false;

	// > 深度优先遍历（DFS）
	while (!node_stack.isEmpty()) {
		C_NodePtr cur_node = node_stack.pop();
		//qDebug() << cur_node->getName();

		// > 向下遍历
		QList<C_NodePtr> node_list = cur_node->getConnectedNode_All();
		for (int i = 0; i < node_list.count(); i++){
			C_NodePtr next_node = node_list.at(i);

			// > 排除自连接的情况
			if (next_node->getId() == cur_node->getId()){ continue; }

			// > 排除自己已走过的线
			bool isRepeatLine = false;
			for (int j = 0; j < activedLine_list.count(); j+=2 ){
				QString a = activedLine_list.at(j);
				QString b = activedLine_list.at(j+1);
				if (a == cur_node->getId() && b == next_node->getId()){ isRepeatLine = true; break; }
				if (b == cur_node->getId() && a == next_node->getId()){ isRepeatLine = true; break; }
			}
			if (isRepeatLine == true){ continue; }

			// > 如果目标节点中存在 非走过的线 的连接，说明有环
			if (actived_list.contains(next_node)){ this->m_temp_hasLoop = true; continue; }
			actived_list.append(next_node);

			// > 记录走过的线
			activedLine_list.append(cur_node->getId());
			activedLine_list.append(next_node->getId());

			// > 入栈
			node_stack.push(next_node);
		}
	}

	// > 随机点遍历时，激活的数量不为所有数量，则说明存在孤岛
	if (actived_list.count() != this->m_NodeTank.count()){
		this->m_temp_hasIsolated = true;
	}
}


/*-------------------------------------------------
		算法 - 深度优先遍历（DFS）
				（可见：https://blog.csdn.net/weixin_44122303/article/details/122924246）
*/
void C_NodeFactory::demo_DFS(C_NodePtr root){
	if (root.isNull()) { return; }
	QList<C_NodePtr> actived_list;

	// > 栈准备
	QStack<C_NodePtr> node_stack;
	node_stack.push(root);
	actived_list.append(root);

	// > 深度优先遍历（DFS）
	while (!node_stack.isEmpty()) {
		C_NodePtr cur_node = node_stack.pop();
		qDebug() << cur_node->getName();

		// > 向下遍历
		QList<C_NodePtr> node_list = cur_node->getConnectedNode_All();
		for (int i = 0; i < node_list.count(); i++){
			C_NodePtr next_node = node_list.at(i);

			// > 排除自连接的情况
			if (next_node->getId() == cur_node->getId()){ continue; }

			// > 排除激活过的节点
			if (actived_list.contains(next_node)){ continue; }
			actived_list.append(next_node);

			// > 入栈
			node_stack.push(next_node);

			// > 其它操作
			//...
		}
	}
}
/*-------------------------------------------------
		算法 - 广度优先遍历（BFS）
				（可见：https://blog.csdn.net/weixin_44122303/article/details/122925663）
*/
void C_NodeFactory::demo_BFS(C_NodePtr root){
	if (root.isNull()) { return; }
	QList<C_NodePtr> actived_list;

	// > 队列准备
	QQueue<C_NodePtr> node_queue;
	node_queue.append(root);
	actived_list.append(root);

	// > 广度优先遍历（BFS）
	while (!node_queue.isEmpty()) {
		C_NodePtr cur_node = node_queue.first();
		node_queue.removeFirst();
		qDebug() << cur_node->getName();

		// > 向下遍历
		QList<C_NodePtr> node_list = cur_node->getConnectedNode_All();
		for (int i = 0; i < node_list.count(); i++){
			C_NodePtr next_node = node_list.at(i);

			// > 排除自连接的情况
			if (next_node->getId() == cur_node->getId()){ continue; }

			// > 排除激活过的节点
			if (actived_list.contains(next_node)){ continue; }
			actived_list.append(next_node);

			// > 入队列
			node_queue.append(next_node);

			// > 其它操作
			//...
		}
	}
}



/*-----------------------------------
		类容器 - 删除 - 空类
*/
void C_NodeFactory::removeNode_AllEmpty(){
	for (int i = this->m_NodeTank.count()-1; i >=0; i--){
		C_NodePtr ptr = this->m_NodeTank.last();
		if (ptr.isNull()){ this->m_NodeTank.removeLast(); }
		if (ptr->isNull()){ this->m_NodeTank.removeLast(); }
	}
}
/*-----------------------------------
		类容器 - 删除 - 根据ID
*/
bool C_NodeFactory::removeNode_ById(QString node_id){
	int index = this->getNodeIndex_ById(node_id);
	if (index == -1){ return false; }

	// > 断开连接
	C_NodePtr ptr = this->m_NodeTank.at(index);
	ptr->disconnectNodeAll();

	this->m_NodeTank.removeAt(index);
	ptr.clear();
	return true;
}
/*-----------------------------------
		类容器 - 删除 - 根据名称
*/
bool C_NodeFactory::removeNode_ByName(QString node_name){
	int index = this->getNodeIndex_ByName(node_name);
	if (index == -1){ return false; }

	// > 断开连接
	C_NodePtr ptr = this->m_NodeTank.at(index);
	ptr->disconnectNodeAll();

	this->m_NodeTank.removeAt(index);
	ptr.clear();
	return true;
}


/*-----------------------------------
		类容器 - 获取 - 全部（原装）
*/
QList<C_NodePtr> C_NodeFactory::getNode_AllOrg(){
	return this->m_NodeTank;
}
/*-----------------------------------
		类容器 - 获取 - 全部
*/
QList<C_NodePtr> C_NodeFactory::getNode_All(){
	QList<C_NodePtr> result_list;
	for (int i = 0; i < this->m_NodeTank.count(); i++){
		C_NodePtr Node = this->m_NodeTank.at(i);
		if (Node->isNull()){ continue; }
		result_list.append(Node);
	}
	return result_list;
}
/*-----------------------------------
		类容器 - 获取 - 全部Id
*/
QStringList C_NodeFactory::getNode_AllId(){
	QStringList result_list;
	for (int i = 0; i < this->m_NodeTank.count(); i++){
		C_NodePtr Node = this->m_NodeTank.at(i);
		if (Node->isNull()){ continue; }
		result_list.append(Node->getId());
	}
	return result_list;
}
/*-----------------------------------
		类容器 - 获取 - 全部名称
*/
QStringList C_NodeFactory::getNode_AllName(){
	QStringList result_list;
	for (int i = 0; i < this->m_NodeTank.count(); i++){
		C_NodePtr node = this->m_NodeTank.at(i);
		if (node->isNull()){ continue; }
		result_list.append(node->getName());
	}
	return result_list;
}
/*-----------------------------------
		类容器 - 获取 - 根据ID
*/
C_NodePtr C_NodeFactory::getNode_ById(QString node_id){
	int index = this->getNodeIndex_ById(node_id);
	if (index == -1){ return C_NodePtr(); }
	return this->m_NodeTank.at(index);
}
/*-----------------------------------
		类容器 - 获取 - 根据名称
*/
C_NodePtr C_NodeFactory::getNode_ByName(QString node_name){
	int index = this->getNodeIndex_ByName(node_name);
	if (index == -1){ return C_NodePtr(); }
	return this->m_NodeTank.at(index);
}
/*-----------------------------------
		类容器 - 是否含ID
*/
bool C_NodeFactory::hasNode_Id(QString node_id){
	return this->getNodeIndex_ById(node_id) != -1;
}
/*-----------------------------------
		类容器 - 是否含名称
*/
bool C_NodeFactory::hasNode_Name(QString node_name){
	return this->getNodeIndex_ByName(node_name) != -1;
}
/*-----------------------------------
		私有 - 获取索引（根据ID）
*/
int C_NodeFactory::getNodeIndex_ById(QString Node_id){
	for (int i = 0; i < this->m_NodeTank.count(); i++){
		C_NodePtr Node = this->m_NodeTank.at(i);
		if (Node->getId() == Node_id){
			return i;
		}
	}
	return -1;
}
/*-----------------------------------
		私有 - 获取索引（根据ID）
*/
int C_NodeFactory::getNodeIndex_ByName(QString node_name){
	for (int i = 0; i < this->m_NodeTank.count(); i++){
		C_NodePtr node = this->m_NodeTank.at(i);
		if (node->getName() == node_name){
			return i;
		}
	}
	return -1;
}


/*-------------------------------------------------
		访问器 - 获取子类类名
*/
QString C_NodeFactory::get__CLASS_NAME__(){
	return "C_NodeFactory";
}
/*-------------------------------------------------
		访问器 - 获取子类类名（中文名）
*/
QString C_NodeFactory::get__CLASS_NAME__Chinese(){
	return "节点工厂";
}
/*-------------------------------------------------
		访问器 - 获取所有继承的类名
*/
QStringList C_NodeFactory::get__CLASS_INHERITED_LIST__(){
	return C_ContainerDataBase::get__CLASS_INHERITED_LIST__() << "C_NodeFactory";
}
/*-------------------------------------------------
		访问器 - 是否继承了某类
*/
bool C_NodeFactory::get__CLASS_IS_INSTANCE_OF__(QString class_name){
	return this->get__CLASS_INHERITED_LIST__().contains(class_name);
}
/*-------------------------------------------------
		类 - 判断类
*/
bool C_NodeFactory::isClass_NodeFactory(){
	return this->get__CLASS_IS_INSTANCE_OF__("C_NodeFactory");
}


/*-------------------------------------------------
		运算符重载
*/
const bool C_NodeFactory::operator== (const C_NodeFactory& a)const {
	return this->id == a.id;
}
/*-------------------------------------------------
		判断空
*/
bool C_NodeFactory::isNull(){
	return C_ContainerDataBase::isNull();
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_NodeFactory::getJsonObject_childData(){
	QJsonObject obj;

	// > 容器
	QJsonArray arr;
	for (int i = 0; i < this->m_NodeTank.count(); i++){
		C_NodePtr node = this->m_NodeTank.at(i);
		arr.append(node->getJsonObject());
	}
	obj.insert("m_NodeTank", arr);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_NodeFactory::setJsonObject_childData(QJsonObject obj){

	// > 容器
	if (obj.value("m_NodeTank").isUndefined() == false){
		this->m_NodeTank.clear();
		QJsonArray arr = obj.value("m_NodeTank").toArray();
		for (int i = 0; i < arr.count(); i++){
			C_NodePtr node = this->createNodeEmpty();
			node->setJsonObject( arr.at(i).toObject() );
			this->m_NodeTank.append(node);
		}
	}

}
