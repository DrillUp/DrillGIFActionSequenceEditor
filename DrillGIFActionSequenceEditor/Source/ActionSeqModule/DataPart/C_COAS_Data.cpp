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
					
		说明：		
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
		空判断
*/
bool C_COAS_Data::isNull(){
	if (this->m_COAS_id == -1){ return true; }
	if (this->m_COAS_name == ""){ return true; }
	return false;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_COAS_Data::getJsonObject(){
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
			QJsonObject obj = this->m_state_tank.at(i);
			QString state_name = obj.value("状态元名称").toString();
			if (state_name != ""){
				this->m_state_default_randomSeq.append(state_name);
				break;
			}
		}
	}
	obj_actionSeq.insert("默认的状态元集合", TTool::_JSON_stringify_(this->m_state_default_randomSeq));

	// > 动作元列表
	QStringList actionTank_str = TTool::_QList_QJsonObjectToQString_(this->m_act_tank);
	for (int i = 0; i < actionTank_str.count(); i++){
		obj_actionSeq.insert("动作元-" + QString::number(i + 1), actionTank_str.at(i));
	}

	// > 状态元列表
	QStringList stateTank_str = TTool::_QList_QJsonObjectToQString_(this->m_state_tank);
	for (int i = 0; i < stateTank_str.count(); i++){
		obj_actionSeq.insert("状态元-" + QString::number(i + 1), stateTank_str.at(i));
	}

	// > 状态节点列表
	QStringList stateNodeTank_str = TTool::_QList_QJsonObjectToQString_(this->m_stateNode_tank);
	for (int i = 0; i < stateNodeTank_str.count(); i++){
		obj_actionSeq.insert("状态节点-" + QString::number(i + 1), stateNodeTank_str.at(i));
	}

	return obj_actionSeq;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_COAS_Data::setJsonObject(QJsonObject obj_actionSeq){

	// > 树数据
	this->m_COAS_id = obj_actionSeq.value("COAS_id").toInt();
	this->m_COAS_name = obj_actionSeq.value("COAS_name").toString();
	this->m_COAS_type = obj_actionSeq.value("COAS_type").toString();

	// > 标签
	this->m_COAS_name = obj_actionSeq.value("标签").toString();

	// > 默认的状态元集合
	this->m_state_default_randomSeq = TTool::_JSON_parse_To_QListQString_(obj_actionSeq.value("默认的状态元集合").toString());

	// > 动作元列表
	QStringList actionTank_str = QStringList();
	for (int i = 0; i < S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_action; i++){
		QJsonValue value = obj_actionSeq.value("动作元-" + QString::number(i + 1));
		actionTank_str.append(value.toString());
	}
	this->m_act_tank = TTool::_QList_QStringToQJsonObject_(actionTank_str);

	// > 状态元列表
	QStringList stateTank_str = QStringList();
	for (int i = 0; i < S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_state; i++){
		QJsonValue value = obj_actionSeq.value("状态元-" + QString::number(i + 1));
		stateTank_str.append(value.toString());
	}
	this->m_state_tank = TTool::_QList_QStringToQJsonObject_(stateTank_str);

	// > 状态节点列表
	QStringList stateNodeTank_str = QStringList();
	for (int i = 0; i < S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_stateNode; i++){
		QJsonValue value = obj_actionSeq.value("状态节点-" + QString::number(i + 1));
		stateNodeTank_str.append(value.toString());
	}
	this->m_stateNode_tank = TTool::_QList_QStringToQJsonObject_(stateNodeTank_str);
}