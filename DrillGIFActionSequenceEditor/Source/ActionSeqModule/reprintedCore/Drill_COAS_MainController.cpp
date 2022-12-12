﻿#include "stdafx.h"
#include "Drill_COAS_MainController.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动画序列主控制器【复刻类】.cpp
		作者：		drill_up
		所属模块：	动画序列数据
		功能：		> 定义一个专门控制 动画序列 的数据类。
					> 该类可被存到存档中。
					
		说明：		当前复刻版本：[v1.5]
-----==========================================================-----
*/
Drill_COAS_MainController::Drill_COAS_MainController(){
	this->_drill_data = QJsonObject();									//深拷贝数据
	this->_drill_controllerSerial = QUuid::createUuid().toString();		//（生成一个不重复的序列号）
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("{", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("}", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("-", "");
	this->_drill_node_curController = nullptr;
	this->_drill_act_curController = nullptr;
	this->drill_initData_Main();										//初始化数据
	this->drill_initPrivateData_Main();									//私有数据初始化
	this->drill_COAS_resetData_Main(this->_drill_data);
	//$gameTemp._drill_COAS_lastCreatedMainController = this;			//（记录上一个动画序列）
}
Drill_COAS_MainController::Drill_COAS_MainController(QJsonObject data){
	this->_drill_data = data;											//深拷贝数据
	this->_drill_controllerSerial = QUuid::createUuid().toString();		//（生成一个不重复的序列号）
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("{", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("}", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("-", "");
	this->_drill_node_curController = nullptr;
	this->_drill_act_curController = nullptr;
	this->drill_initData_Main();										//初始化数据
	this->drill_initPrivateData_Main();									//私有数据初始化
	this->drill_COAS_resetData_Main(this->_drill_data);
	//$gameTemp._drill_COAS_lastCreatedMainController = this;			//（记录上一个动画序列）
}
Drill_COAS_MainController::~Drill_COAS_MainController(){
}


/*-------------------------------------------------
		动画序列 - 帧刷新【标准函数】
*/
void Drill_COAS_MainController::drill_COAS_update(){
	if (this->isNull()){ return; }
	if (this->_drill_data["pause"].toBool(false)){ return; }
	this->_drill_curTime += 1;				//帧刷新 - 时间流逝
	this->drill_COAS_updateStateNode();		//帧刷新 - 状态节点
	this->drill_COAS_updateAct();			//帧刷新 - 动画序列
}
/*-------------------------------------------------
		动画序列 - 空判断
*/
bool Drill_COAS_MainController::isNull(){
	return this->_drill_data.isEmpty();
}


/*-------------------------------------------------
		动画序列 - 初始化数据【标准默认值】
*/
void Drill_COAS_MainController::drill_initData_Main(){
	QJsonObject data = this->_drill_data;

	// > 常规
	if (data["id"].isUndefined() == true){ data["id"] = -1; }						//标识
	if (data["visible"].isUndefined() == true){ data["visible"] = true; }			//显示情况
	if (data["pause"].isUndefined() == true){ data["pause"] = false; }				//暂停情况
	
	// > 容器
	if (data["state_tank"].isUndefined() == true){ data["state_tank"] = QJsonArray(); }								//容器 - 状态元
	if (data["stateNode_tank"].isUndefined() == true){ data["stateNode_tank"] = QJsonArray(); }						//容器 - 状态节点
	if (data["act_tank"].isUndefined() == true){ data["act_tank"] = QJsonArray(); }									//容器 - 动画序列
	if (data["state_default_randomSeq"].isUndefined() == true){ data["state_default_randomSeq"] = QJsonArray(); }	//默认的状态元集合

	// > 子插件用参数
	if (data["waitForPreload"].isUndefined() == true){ data["waitForPreload"] = true; }		//预加载等待
}
/*-------------------------------------------------
		动画序列 - 私有数据初始化
*/
void Drill_COAS_MainController::drill_initPrivateData_Main(){
	QJsonObject data = this->_drill_data;

	// > 常规
	this->_drill_curTime = 0;										//常规 - 当前时间
	this->_drill_needDestroy = false;								//常规 - 销毁
	this->_drill_checkArrayEnabled = true;							//常规 - 校验数据是否为数组

	// > GIF - 输出数据
	this->_drill_curBitmapName = "";								//输出数据 - 当前的对象名
	this->_drill_curBitmapPath = "";								//输出数据 - 当前的路径
	this->_drill_curBitmapTint = 0;									//输出数据 - 当前的色调
	this->_drill_curBitmapSmooth = false;							//输出数据 - 当前的模糊


	// > 默认的状态元集合
	QJsonObject node_data;
	node_data["name"] = "默认的状态元集合";
	node_data["priority"] = 0;
	node_data["proportion"] = 40;
	node_data["play_type"] = "随机播放状态元";
	node_data["play_randomStateSeq"] = data["state_default_randomSeq"];
	node_data["play_randomMax"] = 5;
	node_data["note"] = "";
	QJsonArray stateNode_tank = data["stateNode_tank"].toArray();
	stateNode_tank.append(node_data);
	data["stateNode_tank"] = stateNode_tank;

	// > 状态节点
	//this._drill_node_curName = "";	//（立刻变化，没有缓冲设置）
	this->_drill_node_curSerial = -1;
	this->_drill_node_curController = new Drill_COAS_StateNodeController(node_data);
	this->_drill_node_curController->drill_COAS_setParentDataId(data["id"].toInt());
	this->_drill_node_curController->drill_COAS_setLayer(0);

	// > 动作元
	this->_drill_act_curName = "";
	this->_drill_act_curSerial = -1;
	this->_drill_act_curController = new Drill_COAS_ActController();
	this->_drill_act_interrupt = false;

	// > 数据量查看
	qDebug() << node_data;
}
/*-------------------------------------------------
		动画序列 - 重设数据（私有）
*/
void Drill_COAS_MainController::drill_COAS_resetData_Main(QJsonObject data){

	// > 判断数据重复情况
	if (this->_drill_data.isEmpty() == false){
		QStringList keys = data.keys();
		bool is_same = true;
		for (int i = 0; i < keys.length(); i++){
			QString key = keys[i];
			if (this->_drill_data[key] != data[key]){
				is_same = false;
			}
		}
		if (is_same == true){ return; }
	}
	// > 补充未设置的数据
	QStringList keys = this->_drill_data.keys();
	for (int i = 0; i < keys.length(); i++){
		QString key = keys[i];
		if (data[key].isUndefined()){
			data[key] = this->_drill_data[key];
		}
	}

	// > 执行重置
	this->_drill_data = data;												//深拷贝
	this->_drill_controllerSerial = QUuid::createUuid().toString();			//（生成一个不重复的序列号）
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("{", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("}", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("-", "");
	this->drill_initData_Main();											//初始化数据
	this->drill_initPrivateData_Main();										//私有数据初始化
}
/*-------------------------------------------------
		动画序列 - 校验数据是否为数组
*/
void Drill_COAS_MainController::drill_COAS_checkArray(QJsonValue arr){
	if( this->_drill_checkArrayEnabled != true ){ return; }
		
	if (arr.isArray()){
		// > 通过
	}else{
		// > 报错提示
		qDebug() << "【Drill_CoreOfActionSequence.js 系统 - GIF动画序列核心】\n" << "接口调用错误，数组接口获取到了 非数组 参数：";
		qDebug() << arr; 
		qDebug() << " 。";
		this->_drill_checkArrayEnabled = false;
	}	
}



/*-------------------------------------------------
		动画序列-状态元 - 获取数据 - 全部【开放函数】
*/
QJsonArray Drill_COAS_MainController::drill_COAS_getStateData_All(){
	return this->_drill_data["state_tank"].toArray();
}
/*-------------------------------------------------
		动画序列-状态元 - 获取数据 - 根据名称【开放函数】
*/
QJsonObject Drill_COAS_MainController::drill_COAS_getStateData_ByName(QString state_name){
	if (state_name == ""){ return QJsonObject(); }
	QJsonObject data = this->_drill_data;
	QJsonArray state_tank = data["state_tank"].toArray();
	for (int i = 0; i < state_tank.count(); i++){
		QJsonObject data_state = state_tank[i].toObject();
		if (data_state["name"].toString() == state_name){
			return data_state;
		}
	}
	return QJsonObject();
}
/*-------------------------------------------------
		动画序列-状态元 - 获取数据 - 全部名称【开放函数】
*/
QStringList Drill_COAS_MainController::drill_COAS_getStateData_AllName(){
	QJsonObject data = this->_drill_data;
	QStringList result_list;
	QJsonArray state_tank = data["state_tank"].toArray();
	for (int i = 0; i < state_tank.count(); i++){
		QJsonObject data_state = state_tank[i].toObject();
		if (data_state["name"].toString() != ""){
			result_list.append(data_state["name"].toString());
		}
	}
	return result_list;
}


/*-------------------------------------------------
		动画序列-状态节点 - 获取数据 - 全部【开放函数】
*/
QJsonArray Drill_COAS_MainController::drill_COAS_getStateNodeData_All(){
	return this->_drill_data["stateNode_tank"].toArray();
}
/*-------------------------------------------------
		动画序列-状态节点 - 获取数据 - 根据名称【开放函数】
*/
QJsonObject Drill_COAS_MainController::drill_COAS_getStateNodeData_ByName(QString stateNode_name){
	if (stateNode_name == ""){ return QJsonObject(); }
	QJsonObject data = this->_drill_data;
	QJsonArray stateNode_tank = data["stateNode_tank"].toArray();
	for (int i = 0; i < stateNode_tank.count(); i++){
		QJsonObject data_node = stateNode_tank[i].toObject();
		if (data_node["name"].toString() == stateNode_name){
			return data_node;
		}
	}
	return QJsonObject();
}
/*-------------------------------------------------
		动画序列-状态节点 - 获取数据 - 全部名称【开放函数】
*/
QStringList Drill_COAS_MainController::drill_COAS_getStateNodeData_AllName(){
	QJsonObject data = this->_drill_data;
	QStringList result_list;
	QJsonArray stateNode_tank = data["stateNode_tank"].toArray();
	for (int i = 0; i < stateNode_tank.count(); i++){
		QJsonObject data_node = stateNode_tank[i].toObject();
		if (data_node["name"].toString() != ""){
			result_list.append(data_node["name"].toString());
		}
	}
	return result_list;
}
/*-------------------------------------------------
		动画序列-状态节点 - 帧刷新
*/
void Drill_COAS_MainController::drill_COAS_updateStateNode(){
	if (this->drill_COAS_isPlayingAct() == true){ return; }		//动作播放时，不操作
	
	// > 动作元打断
	if (this->_drill_act_interrupt == true){
		this->_drill_act_interrupt = false;
		if (this->_drill_node_curController->drill_COAS_canBeInterrupted()){
			this->_drill_node_curController->_drill_curIndex = 0;		//（指针重置）
			this->_drill_node_curController->drill_COAS_refreshNext();
		}
	}

	// > 状态节点 数据刷新情况
	if (this->_drill_node_curSerial != this->_drill_node_curController->_drill_controllerSerial){
		this->_drill_node_curController->drill_COAS_refreshNext();
		this->_drill_node_curSerial = this->_drill_node_curController->_drill_controllerSerial;
	}

	// > 状态节点 播放完毕情况
	if (this->_drill_node_curController->drill_COAS_isNodeEnd() == true){
		this->_drill_node_curController->_drill_curIndex = 0;		//（指针重置）
		this->_drill_node_curController->drill_COAS_refreshNext();
	}

	// > 状态节点 帧刷新
	this->_drill_node_curController->drill_COAS_update();
	this->_drill_curBitmapName = this->_drill_node_curController->drill_COAS_curBitmapName();
	this->_drill_curBitmapPath = this->_drill_node_curController->drill_COAS_curBitmapPath();
	this->_drill_curBitmapTint = this->_drill_node_curController->drill_COAS_curBitmapTint();
	this->_drill_curBitmapSmooth = this->_drill_node_curController->drill_COAS_curBitmapSmooth();
}
/*-------------------------------------------------
		动画序列-状态节点 - 操作 - 获取当前状态元名称【开放函数】
*/
QString Drill_COAS_MainController::drill_COAS_getCurStateName(){
	return this->_drill_node_curController->drill_COAS_getCurStateName();
}
/*-------------------------------------------------
		动画序列-状态节点 - 操作 - 获取当前状态元名称（全路径）【开放函数】
*/
QString Drill_COAS_MainController::drill_COAS_getCurStateName_AllRoot(){
	return this->_drill_node_curController->drill_COAS_getCurStateName_AllRoot();
}
/*-------------------------------------------------
		动画序列-状态节点 - 操作 - 获取当前状态元优先级【开放函数】
*/
int Drill_COAS_MainController::drill_COAS_getCurStatePriority(){
	return this->_drill_node_curController->drill_COAS_getCurStatePriority();
}
/*-------------------------------------------------
		动画序列-状态节点 - 操作 - 播放默认的状态元集合【开放函数】
*/
void Drill_COAS_MainController::drill_COAS_setStateNodeDefault(){
	this->drill_COAS_setStateNode("默认的状态元集合");
}
/*-------------------------------------------------
		动画序列-状态节点 - 操作 - 播放状态节点【开放函数】
*/
void Drill_COAS_MainController::drill_COAS_setStateNode(QString node_name){
	QJsonObject data = this->_drill_data;
	QJsonObject node_data = this->drill_COAS_getStateNodeData_ByName(node_name);
	if (node_data.isEmpty()){ return; }
	this->_drill_node_curController->drill_COAS_resetData_Node(node_data);				//集合 - 重设数据
	this->_drill_node_curController->drill_COAS_setParentDataId(data["id"].toInt());	//集合 - 设置父数据id
	this->_drill_node_curController->drill_COAS_setLayer(0);							//集合 - 设置当前层数
	this->_drill_node_curController->drill_COAS_refreshNext();							//集合 - 刷新子节点
}
/*-------------------------------------------------
		动画序列-状态节点 - 操作 - 播放简单状态元集合【开放函数】
*/
void Drill_COAS_MainController::drill_COAS_setSimpleStateNode(QStringList state_nameList){
	//this->drill_COAS_checkArray(state_nameList);	//（不需要检查）
	this->_drill_node_curController->drill_COAS_setNewStateNameList(state_nameList);
}
/*-------------------------------------------------
		动画序列-状态节点 - 操作 - 播放状态元 根据标签【开放函数】
*/
void Drill_COAS_MainController::drill_COAS_setAnnotation(QStringList annotation_list){

	// > 找到符合注解数量最多的状态元名
	int max_fit_count = 0;			//（最大符合数量）
	QJsonArray fit_seq;				//（最大符合的索引列表）
	QJsonArray stateData_list = this->drill_COAS_getStateData_All();
	//...
}


/*-------------------------------------------------
		动画序列-动作元 - 获取数据 - 全部【开放函数】
*/
QJsonArray Drill_COAS_MainController::drill_COAS_getActData_All(){
	return this->_drill_data["act_tank"].toArray();
}
/*-------------------------------------------------
		动画序列-动作元 - 获取数据 - 根据名称【开放函数】
*/
QJsonObject Drill_COAS_MainController::drill_COAS_getActData_ByName(QString stateNode_name){
	if (stateNode_name == ""){ return QJsonObject(); }
	QJsonObject data = this->_drill_data;
	QJsonArray act_tank = data["act_tank"].toArray();
	for (int i = 0; i < act_tank.count(); i++){
		QJsonObject data_act = act_tank[i].toObject();
		if (data_act["name"].toString() == stateNode_name){
			return data_act;
		}
	}
	return QJsonObject();
}
/*-------------------------------------------------
		动画序列-动作元 - 获取数据 - 全部名称【开放函数】
*/
QStringList Drill_COAS_MainController::drill_COAS_getActData_AllName(){
	QJsonObject data = this->_drill_data;
	QStringList result_list;
	QJsonArray act_tank = data["act_tank"].toArray();
	for (int i = 0; i < act_tank.count(); i++){
		QJsonObject data_act = act_tank[i].toObject();
		if (data_act["name"].toString() != ""){
			result_list.append(data_act["name"].toString());
		}
	}
	return result_list;
}
/*-------------------------------------------------
		动画序列-动作元 - 帧刷新
*/
void Drill_COAS_MainController::drill_COAS_updateAct(){
	if (this->drill_COAS_isPlayingAct() == false){ return; }	//动作未播放时，不操作

	// > 动作元打断 锁
	this->_drill_act_interrupt = true;

	// > 动作元 数据刷新情况
	if (this->_drill_act_curSerial != this->_drill_act_curController->_drill_controllerSerial){
		QJsonObject data_act = this->drill_COAS_getActData_ByName(this->_drill_act_curName);
		if (data_act.isEmpty() == false){
			this->_drill_act_curController->drill_COAS_resetData_Act(data_act);
		}
		this->_drill_act_curSerial = this->_drill_act_curController->_drill_controllerSerial;
	}

	// > 动作元 播放完毕情况
	if (this->_drill_act_curController->drill_COAS_isActEnd() == true){
		this->_drill_act_curName = "";
		this->_drill_act_curSerial = -1;
	}

	// > 动作元 帧刷新
	this->_drill_act_curController->drill_COAS_update();
	this->_drill_curBitmapName = this->_drill_act_curController->drill_COAS_curBitmapName();
	this->_drill_curBitmapPath = this->_drill_act_curController->drill_COAS_curBitmapPath();
	this->_drill_curBitmapTint = this->_drill_act_curController->drill_COAS_curBitmapTint();
	this->_drill_curBitmapSmooth = this->_drill_act_curController->drill_COAS_curBitmapSmooth();
}
/*-------------------------------------------------
		动画序列-动作元 - 判断播放
*/
bool Drill_COAS_MainController::drill_COAS_isPlayingAct(){
	return this->_drill_act_curName != "";
}
/*-------------------------------------------------
		动画序列-动作元 - 操作 - 获取当前动作元名称【开放函数】
*/
QString Drill_COAS_MainController::drill_COAS_getCurActName(){
	return this->_drill_act_curName;
}
/*-------------------------------------------------
		动画序列-动作元 - 操作 - 播放动作元【开放函数】
*/
void Drill_COAS_MainController::drill_COAS_setAct(QString act_name){
	if (this->_drill_act_curName == act_name){ return; }

	// > 检查高优先级状态元
	if (this->_drill_act_curName == ""){
		QJsonObject data_act = this->drill_COAS_getActData_ByName(act_name);
		int state_priority = this->drill_COAS_getCurStatePriority();
		if (state_priority > data_act["priority"].toInt()){	//（同级的动作元可以覆盖状态元）
			return;
		}
	}

	// > 动作正在播放时
	if (this->_drill_act_curName != ""){
		QJsonObject data_act = this->drill_COAS_getActData_ByName(act_name);
		QJsonObject cur_act = this->drill_COAS_getActData_ByName(this->_drill_act_curName);

		if (cur_act["priority"].toInt() >= data_act["priority"].toInt()){	//（只能覆盖小的优先级，不包括同级）
			return;
		}
	}

	this->_drill_act_curName = act_name;
}
/*-------------------------------------------------
		动画序列-动作元 - 操作 - 立刻终止动作【开放函数】
*/
void Drill_COAS_MainController::drill_COAS_stopAct(){
	this->_drill_act_curName = "";
	this->_drill_act_curSerial = -1;
}



