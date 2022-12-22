#include "stdafx.h"
#include "C_COAS_Data.h"

#include "../Data/LengthData/W_COAS_Length.h"
#include "../Data/S_ActionSeqDataContainer.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		动画序列 数据类.cpp
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		动画序列 的数据类。
-----==========================================================-----
*/
C_COAS_Data::C_COAS_Data(){
	this->m_COAS_id = -1;
	this->m_COAS_name = "";
	this->m_COAS_type = "";
	this->m_state_default_randomSeq.clear();
	this->m_act_tank.clear();
	this->m_state_tank.clear();
	this->m_stateNode_tank.clear();
}
C_COAS_Data::~C_COAS_Data(){
}

/*-------------------------------------------------
		数据 - 清除数据
*/
void C_COAS_Data::clearTankData(){
	this->m_COAS_name = "";
	this->m_state_default_randomSeq.clear();
	this->m_act_tank.clear();
	this->m_state_tank.clear();
	this->m_stateNode_tank.clear();
}
/*-------------------------------------------------
		数据 - 刷新容器大小
*/
void C_COAS_Data::refreshTankLength(C_COAS_Length len){
	
	// > 动作元刷新
	if (this->m_act_tank.count() < len.realLen_action){
		for (int i = this->m_act_tank.count(); i < len.realLen_action; i++){
			C_COAS_ActionPtr action_ptr = C_COAS_ActionPtr::create();
			action_ptr->setJsonObject_Chinese(TTool::_JSON_parse_To_Obj_(""), i);
			this->m_act_tank.append(action_ptr);
		};
	}
	if (this->m_act_tank.count() > len.realLen_action){
		int count = this->m_act_tank.count() - len.realLen_action;
		for (int i = 0; i < count; i++){
			this->m_act_tank.removeLast();
		};
	}

	// > 状态元刷新
	if (this->m_state_tank.count() < len.realLen_state){
		for (int i = this->m_state_tank.count(); i < len.realLen_state; i++){
			C_COAS_StatePtr state_ptr = C_COAS_StatePtr::create();
			state_ptr->setJsonObject_Chinese(TTool::_JSON_parse_To_Obj_(""), i);
			this->m_state_tank.append(state_ptr);
		};
	}
	if (this->m_state_tank.count() > len.realLen_state){
		int count = this->m_state_tank.count() - len.realLen_state;
		for (int i = 0; i < count; i++){
			this->m_state_tank.removeLast();
		};
	}

	// > 状态节点刷新
	if (this->m_stateNode_tank.count() < len.realLen_stateNode){
		for (int i = this->m_stateNode_tank.count(); i < len.realLen_stateNode; i++){
			C_COAS_StateNodePtr node_ptr = C_COAS_StateNodePtr::create();
			node_ptr->setJsonObject_Chinese(TTool::_JSON_parse_To_Obj_(""), i);
			this->m_stateNode_tank.append(node_ptr);
		};
	}
	if (this->m_stateNode_tank.count() > len.realLen_stateNode){
		int count = this->m_stateNode_tank.count() - len.realLen_stateNode;
		for (int i = 0; i < count; i++){
			this->m_stateNode_tank.removeLast();
		};
	}
}
/*-------------------------------------------------
		数据 - 获取全部关联文件名（去重）
*/
QStringList C_COAS_Data::getFileNameList(){
	QStringList result = QStringList();

	// > 动作元资源
	for (int i = 0; i < this->m_act_tank.count(); i++){
		C_COAS_ActionPtr action_ptr = this->m_act_tank.at(i);
		result.append(action_ptr->gif_src);
	}

	// > 状态元资源
	for (int i = 0; i < this->m_state_tank.count(); i++){
		C_COAS_StatePtr state_ptr = this->m_state_tank.at(i);
		result.append(state_ptr->gif_src);
	}

	// > 状态节点资源
	//（无资源）

	// > 去重
	result = result.toSet().toList();
	return result;
}
/*-------------------------------------------------
		数据 - 获取名称
*/
QStringList C_COAS_Data::getNameList_State(){
	QStringList result = QStringList();
	for (int i = 0; i < this->m_state_tank.count(); i++){
		C_COAS_StatePtr state_ptr = this->m_state_tank.at(i);
		result.append(state_ptr->name);
	}
	return result;
}
QStringList C_COAS_Data::getNameList_StateNode(){
	QStringList result = QStringList();
	for (int i = 0; i < this->m_stateNode_tank.count(); i++){
		C_COAS_StateNodePtr stateNode_ptr = this->m_stateNode_tank.at(i);
		result.append(stateNode_ptr->name);
	}
	return result;
}
QStringList C_COAS_Data::getNameList_Action(){
	QStringList result = QStringList();
	for (int i = 0; i < this->m_act_tank.count(); i++){
		C_COAS_ActionPtr action_ptr = this->m_act_tank.at(i);
		result.append(action_ptr->name);
	}
	return result;
}


/*-------------------------------------------------
		空判断
*/
bool C_COAS_Data::isNull(){
	if (this->m_COAS_id == -1){ return true; }
	if (this->m_COAS_name == ""){ return true; }
	return false;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
			【所有 列表/结构体 都需要转成JSON字符串，不能直接存QJsonObject/QJsonArray】
*/
QJsonObject C_COAS_Data::getJsonObject_Chinese(){
	QJsonObject obj_actionSeq = QJsonObject();

	// > 树数据
	obj_actionSeq.insert("COAS_id", this->m_COAS_id);
	obj_actionSeq.insert("COAS_name", this->m_COAS_name);
	obj_actionSeq.insert("COAS_type", this->m_COAS_type);

	// > 标签
	obj_actionSeq.insert("标签", this->m_COAS_name);

	// > 默认状态元集合
	if (this->m_state_default_randomSeq.count() == 0){		//（如果为空则自动填充）
		for (int i = 0; i < this->m_state_tank.count(); i++){
			C_COAS_StatePtr state_ptr = this->m_state_tank.at(i);
			QString state_name = state_ptr->name;
			if (state_name != ""){
				this->m_state_default_randomSeq.append(state_name);
				break;
			}
		}
	}
	obj_actionSeq.insert("默认的状态元集合", TTool::_JSON_stringify_(this->m_state_default_randomSeq));

	// > 动作元列表
	for (int i = 0; i < this->m_act_tank.count(); i++){
		C_COAS_ActionPtr action_ptr = this->m_act_tank.at(i);
		QJsonObject obj = action_ptr->getJsonObject_Chinese();
		obj_actionSeq.insert("动作元-" + QString::number(i + 1), TTool::_JSON_stringify_(obj));
	}

	// > 状态元列表
	for (int i = 0; i < this->m_state_tank.count(); i++){
		C_COAS_StatePtr state_ptr = this->m_state_tank.at(i);
		QJsonObject obj = state_ptr->getJsonObject_Chinese();
		obj_actionSeq.insert("状态元-" + QString::number(i + 1), TTool::_JSON_stringify_(obj));
	}

	// > 状态节点列表
	for (int i = 0; i < this->m_stateNode_tank.count(); i++){
		C_COAS_StateNodePtr node_ptr = this->m_stateNode_tank.at(i);
		QJsonObject obj = node_ptr->getJsonObject_Chinese();
		obj_actionSeq.insert("状态节点-" + QString::number(i + 1), TTool::_JSON_stringify_(obj));
	}

	return obj_actionSeq;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
			【所有 列表/结构体 在读取时，要先转字符串，再解析为可用对象】
*/
void C_COAS_Data::setJsonObject_Chinese(QJsonObject obj_actionSeq){

	// > 树数据
	this->m_COAS_id = obj_actionSeq.value("COAS_id").toInt();	//（这个参数是特殊例外，其它参数都需要先转字符串）
	this->m_COAS_name = obj_actionSeq.value("COAS_name").toString();
	this->m_COAS_type = obj_actionSeq.value("COAS_type").toString();

	// > 标签
	this->m_COAS_name = obj_actionSeq.value("标签").toString();

	// > 默认的状态元集合
	this->m_state_default_randomSeq = TTool::_JSON_parse_To_QListQString_(obj_actionSeq.value("默认的状态元集合").toString());

	// > 动作元列表
	this->m_act_tank.clear();
	for (int i = 0; i < S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_action; i++){
		QString action_str = obj_actionSeq.value("动作元-" + QString::number(i + 1)).toString();
		C_COAS_ActionPtr action_ptr = C_COAS_ActionPtr::create();
		action_ptr->setJsonObject_Chinese(TTool::_JSON_parse_To_Obj_(action_str), i);
		this->m_act_tank.append(action_ptr);
	};

	// > 状态元列表
	this->m_state_tank.clear();
	for (int i = 0; i < S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_state; i++){
		QString state_str = obj_actionSeq.value("状态元-" + QString::number(i + 1)).toString();
		C_COAS_StatePtr state_ptr = C_COAS_StatePtr::create();
		state_ptr->setJsonObject_Chinese(TTool::_JSON_parse_To_Obj_(state_str), i);
		this->m_state_tank.append(state_ptr);
	};

	// > 状态节点列表
	this->m_stateNode_tank.clear();
	int node_max = S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_stateNode;
	if (node_max < 4){ node_max = 4; }
	for (int i = 0; i < node_max; i++){
		QString stateNode_str = obj_actionSeq.value("状态节点-" + QString::number(i + 1)).toString();
		C_COAS_StateNodePtr stateNode_ptr = C_COAS_StateNodePtr::create();
		stateNode_ptr->setJsonObject_Chinese(TTool::_JSON_parse_To_Obj_(stateNode_str), i);
		this->m_stateNode_tank.append(stateNode_ptr);
	}
}