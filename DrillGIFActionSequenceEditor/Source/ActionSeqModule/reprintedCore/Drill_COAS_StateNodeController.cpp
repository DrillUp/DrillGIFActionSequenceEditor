#include "stdafx.h"
#include "Drill_COAS_StateNodeController.h"

#include "Drill_COAS_Init.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动画序列数据【复刻类】.cpp
		作者：		drill_up
		所属模块：	动画序列数据
		功能：		动画序列核心插件的复刻类。
					
		说明：		当前复刻版本：[v1.5]
-----==========================================================-----
*/
Drill_COAS_StateNodeController::Drill_COAS_StateNodeController(){
	this->_drill_data = QJsonObject();									//深拷贝数据
	this->_drill_controllerSerial = QUuid::createUuid().toString();		//（生成一个不重复的序列号）
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("{", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("}", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("-", "");
	this->_drill_curState = nullptr;
	this->_drill_curNode = nullptr;
	this->drill_initData_Node();										//初始化数据
	this->drill_initPrivateData_Node();									//私有数据初始化
	this->drill_COAS_resetData_Node(this->_drill_data);
}
Drill_COAS_StateNodeController::Drill_COAS_StateNodeController(QJsonObject data){
	this->_drill_data = data;											//深拷贝数据
	this->_drill_controllerSerial = QUuid::createUuid().toString();		//（生成一个不重复的序列号）
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("{", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("}", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("-", "");
	this->_drill_curState = nullptr;
	this->_drill_curNode = nullptr;
	this->drill_initData_Node();										//初始化数据
	this->drill_initPrivateData_Node();									//私有数据初始化
	this->drill_COAS_resetData_Node(this->_drill_data);
}
Drill_COAS_StateNodeController::~Drill_COAS_StateNodeController(){
}

/*-------------------------------------------------
		状态节点 - 帧刷新【标准函数】
*/
void Drill_COAS_StateNodeController::drill_COAS_update(){
	if (this->isNull()){ return; }
	this->_drill_curTime += 1;				//时间+1
	this->drill_COAS_updateNode();			//刷新状态节点
}
/*-------------------------------------------------
		状态节点 - 空判断
*/
bool Drill_COAS_StateNodeController::isNull(){
	return this->_drill_data.isEmpty();
}


/*-------------------------------------------------
		数据 - 初始化数据【标准默认值】
*/
void Drill_COAS_StateNodeController::drill_initData_Node(){
	QJsonObject data = this->_drill_data;

	// > 常规
	if (data["name"].isUndefined() == true){ data["name"] = ""; }								//节点名称
	if (data["tag_tank"].isUndefined() == true){ data["tag_tank"] = QJsonArray(); }				//节点标签
	if (data["priority"].isUndefined() == true){ data["priority"] = 0; }						//节点优先级
	if (data["proportion"].isUndefined() == true){ data["proportion"] = 40; }					//节点权重
	if (data["canBeInterrupted"].isUndefined() == true){ data["canBeInterrupted"] = false; }	//可被动作元打断

	// > 播放列表
	if (data["play_type"].isUndefined() == true){ data["play_type"] = "随机播放状态元"; }					//播放列表 - 播放方式
	if (data["play_randomStateSeq"].isUndefined() == true){ data["play_randomStateSeq"] = QJsonArray(); }	//播放列表 - 随机播放状态元
	if (data["play_plainStateSeq"].isUndefined() == true){ data["play_plainStateSeq"] = QJsonArray(); }		//播放列表 - 顺序播放状态元
	if (data["play_randomNodeSeq"].isUndefined() == true){ data["play_randomNodeSeq"] = QJsonArray(); }		//播放列表 - 随机播放嵌套集合
	if (data["play_plainNodeSeq"].isUndefined() == true){ data["play_plainNodeSeq"] = QJsonArray(); }		//播放列表 - 顺序播放嵌套集合
	if (data["play_randomMax"].isUndefined() == true){ data["play_randomMax"] = QJsonArray(); }				//播放列表 - 随机播放的次数上限

	// > 杂项
	if (data["note"].isUndefined() == true){ data["note"] = ""; }											//杂项 - 备注
}
/*-------------------------------------------------
		数据 - 私有数据初始化
*/
void Drill_COAS_StateNodeController::drill_initPrivateData_Node(){
	QJsonObject data = this->_drill_data;

	// > 常规
	this->_drill_curTime = 0;					//常规 - 当前时间
	this->_drill_needDestroy = false;			//常规 - 销毁

	// > 节点
	this->_drill_parentDataId = -1;				//节点 - 父数据ID
	this->_drill_curLayer = 0;					//节点 - 当前层数

	// > 集合对象初始化
	//this->_drill_curState = null;	//（不要置空，后续可能还会再次使用）
	//this->_drill_curNode = null;

	// > 播放时间重置
	this->drill_COAS_resetTimer(data);
}
/*-------------------------------------------------
		状态节点 - 重设数据【标准函数】
*/
void Drill_COAS_StateNodeController::drill_COAS_resetData_Node(QJsonObject data){

	// > 播放时间重置
	this->drill_COAS_resetTimer(data);

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
	this->drill_initData_Node();											//初始化数据
	this->drill_initPrivateData_Node();										//私有数据初始化
}
/*-------------------------------------------------
		状态节点 - 播放时间重置
*/
void Drill_COAS_StateNodeController::drill_COAS_resetTimer(QJsonObject data){
	if (data.isEmpty()){ data = this->_drill_data; }

	// > 播放
	this->_drill_curIndex = 0;					//播放 - 当前索引
	this->_drill_tarIndex = 0;					//播放 - 索引结束位置

	// > 播放 - 索引结束位置
	if (data["play_type"].toString() == "随机播放状态元"){
		this->_drill_tarIndex = data["play_randomMax"].toInt();	//（随机播放的次数上限）
	}
	if (data["play_type"].toString() == "顺序播放状态元"){
		this->_drill_tarIndex = data["play_plainStateSeq"].toArray().count();
	}
	if (data["play_type"].toString() == "随机播放嵌套集合"){
		this->_drill_tarIndex = data["play_randomMax"].toInt();	//（随机播放的次数上限）
	}
	if (data["play_type"].toString() == "顺序播放嵌套集合"){
		this->_drill_tarIndex = data["play_plainNodeSeq"].toArray().count();
	}
}
/*-------------------------------------------------
		状态节点 - 子节点 - 刷新子节点
*/
void Drill_COAS_StateNodeController::drill_COAS_refreshNext_Private(){
	QJsonObject data = this->_drill_data;
	//qDebug() << data;

	// > 结束播放后，停止刷新子节点
	if (this->drill_COAS_isNodeEnd()){ return; }

	QString play_type = data["play_type"].toString();
	if (play_type == "随机播放状态元"){

		// > 准备数据
		QJsonArray data_list;
		QJsonArray play_randomStateSeq = data["play_randomStateSeq"].toArray();
		for (int i = 0; i < play_randomStateSeq.count(); i++){
			QJsonObject state_data = Drill_COAS_Init::getInstance()->drill_COAS_getStateData(this->_drill_parentDataId, play_randomStateSeq[i].toString());
			if (state_data.isEmpty()){ continue; }
			data_list.append(state_data);
		}

		// > 随机抽取数据
		QJsonObject next_data = this->drill_COAS_rollObjData(data_list);
		if (next_data.isEmpty()){	//（空数据时直接报错提示）
			QString message = "【Drill_CoreOfActionSequence.js 系统 - GIF动画序列核心】\n";
			message.append("错误，状态节点\"" + data["name"].toString() + "\"未找到资源名列表。\n");
			message.append("当前为\"" + play_type + "\"，序列数据为：" + TTool::_QJsonArray_AToQString_(play_randomStateSeq).join(",") + "。");
			QMessageBox::about(nullptr, "提示", message);
			data["play_type"] = "";
			return;
		}	

		// > 刷新状态元
		this->drill_COAS_refreshNextState(next_data);
	}
	if (play_type == "顺序播放状态元"){

		// > 顺序抽取数据
		QJsonArray play_plainStateSeq = data["play_plainStateSeq"].toArray();
		QString next_name = play_plainStateSeq[this->_drill_curIndex].toString();
		QJsonObject next_data = Drill_COAS_Init::getInstance()->drill_COAS_getStateData(this->_drill_parentDataId, next_name);
		if (next_data.isEmpty()){	//（空数据时直接报错提示）
			QString message = "【Drill_CoreOfActionSequence.js 系统 - GIF动画序列核心】\n";
			message.append("错误，状态节点\"" + data["name"].toString() + "\"未找到资源名列表。\n");
			message.append("当前为\"" + play_type + "\"，序列数据为：" + TTool::_QJsonArray_AToQString_(play_plainStateSeq).join(",") + "。");
			QMessageBox::about(nullptr, "提示", message);
			data["play_type"] = "";
			return;
		}

		// > 刷新状态元
		this->drill_COAS_refreshNextState(next_data);
	}

	if (play_type == "随机播放嵌套集合"){

		// > 准备数据
		QJsonArray data_list;
		QJsonArray play_randomNodeSeq = data["play_randomNodeSeq"].toArray();
		for (int i = 0; i < play_randomNodeSeq.count(); i++){
			QJsonObject node_data = Drill_COAS_Init::getInstance()->drill_COAS_getStateNodeData(this->_drill_parentDataId, play_randomNodeSeq[i].toString());
			if (node_data.isEmpty()){ continue; }
			data_list.append(node_data);
		}

		// > 随机抽取数据
		QJsonObject next_data = this->drill_COAS_rollObjData(data_list);
		if (next_data.isEmpty()){	//（空数据时直接报错提示）
			QString message = "【Drill_CoreOfActionSequence.js 系统 - GIF动画序列核心】\n";
			message.append("错误，状态节点\"" + data["name"].toString() + "\"未找到资源名列表。\n");
			message.append("当前为\"" + play_type + "\"，序列数据为：" + TTool::_QJsonArray_AToQString_(play_randomNodeSeq).join(",") + "。");
			QMessageBox::about(nullptr, "提示", message);
			data["play_type"] = "";
			return;
		}

		// > 刷新状态节点
		this->drill_COAS_refreshNextNode(next_data);
	}
	if (play_type == "顺序播放嵌套集合"){

		// > 顺序抽取数据
		QJsonArray play_plainNodeSeq = data["play_plainNodeSeq"].toArray();
		QString next_name = play_plainNodeSeq[this->_drill_curIndex].toString();
		QJsonObject next_data = Drill_COAS_Init::getInstance()->drill_COAS_getStateNodeData(this->_drill_parentDataId, next_name);
		if (next_data.isEmpty()){	//（空数据时直接报错提示）
			QString message = "【Drill_CoreOfActionSequence.js 系统 - GIF动画序列核心】\n";
			message.append("错误，状态节点\"" + data["name"].toString() + "\"未找到资源名列表。\n");
			message.append("当前为\"" + play_type + "\"，序列数据为：" + TTool::_QJsonArray_AToQString_(play_plainNodeSeq).join(",") + "。");
			QMessageBox::about(nullptr, "提示", message);
			data["play_type"] = "";
			return;
		}

		// > 刷新状态节点
		this->drill_COAS_refreshNextNode(next_data);
	}
}
/*-------------------------------------------------
		状态节点 - 子节点 - 根据权重随机抽取
*/
QJsonObject Drill_COAS_StateNodeController::drill_COAS_rollObjData(QJsonArray objData_list){
	if (objData_list.isEmpty()){ return QJsonObject(); }
	if (objData_list.count() == 1){ return objData_list[0].toObject(); }
	//qDebug() << "roll_objData_list:" << objData_list;

	QJsonObject result_data;
	int total_proportion = 0;
	for (int i = 0; i < objData_list.count(); i++){
		QJsonObject objData = objData_list[i].toObject();
		int proportion = objData["proportion"].toInt();
		if (proportion <= 0){ proportion = 1; }
		total_proportion += proportion;
	}
	for (int i = 0; i < objData_list.count(); i++){
		QJsonObject objData = objData_list[i].toObject();
		int proportion = objData["proportion"].toInt();
		if (proportion <= 0){ proportion = 1; }

		// > 概率命中，则返回数据
		if ((rand() % 10000)*0.0001 <= (double)proportion / total_proportion){
			return objData;
		}

		// > 没命中，则减去当前的概率，再进入下一轮抽取
		total_proportion -= proportion;
	}
	return result_data;
}
/*-------------------------------------------------
		状态节点 - 子节点 - 重设数据 状态元
*/
void Drill_COAS_StateNodeController::drill_COAS_refreshNextState(QJsonObject next_data){

	// > 创建状态元
	if (this->_drill_curState == nullptr){
		this->_drill_curState = new Drill_COAS_StateController(next_data);
	}

	// > 重设数据
	this->_drill_curState->drill_COAS_resetData_State(next_data);
	this->_drill_curState->drill_COAS_update();	//（设置数据后，立即强制刷新）
}
/*-------------------------------------------------
		状态节点 - 子节点 - 重设数据 状态节点
*/
void Drill_COAS_StateNodeController::drill_COAS_refreshNextNode(QJsonObject next_data){

	// > 检查层级
	int next_layer = this->_drill_curLayer + 1;
	if (next_layer >= 20){		//（层级溢出，则跳出）
		this->_drill_curNode = nullptr;
		qDebug() << (
			QString() + "【Drill_CoreOfActionSequence.js 系统 - GIF动画序列核心】\n" +
			"错误，状态节点\"" + next_data["name"].toString() + "\"的嵌套出现死循环。"
			);
		return;
	}

	// > 创建状态节点
	if (this->_drill_curNode == nullptr){
		this->_drill_curNode = new Drill_COAS_StateNodeController(next_data);
	}

	// > 重设数据
	this->_drill_curNode->drill_COAS_resetData_Node(next_data);
	this->_drill_curNode->drill_COAS_setParentDataId(this->_drill_parentDataId);
	this->_drill_curNode->drill_COAS_setLayer(next_layer);
	this->_drill_curNode->drill_COAS_refreshNext();
	this->_drill_curNode->drill_COAS_update();	//（设置数据后，立即强制刷新）
}
/*-------------------------------------------------
		状态节点 - 帧刷新状态节点
*/
void Drill_COAS_StateNodeController::drill_COAS_updateNode(){
	QJsonObject data = this->_drill_data;

	// > 帧刷新 状态元类型
	if (this->drill_COAS_isTypeState()){
		this->_drill_curState->drill_COAS_update();

		// > 等待子节点 播放结束
		if (this->_drill_curState->drill_COAS_isStateEnd() == true){
			this->_drill_curIndex += 1;		//（结束后，索引+1）
			if (this->drill_COAS_isNodeEnd() == false){
				this->drill_COAS_refreshNext();
			}
		}
	}

	// > 帧刷新 状态节点类型
	if (this->drill_COAS_isTypeNode()){
		this->_drill_curNode->drill_COAS_update();

		// > 等待子节点 播放结束
		if (this->_drill_curNode->drill_COAS_isNodeEnd() == true){
			this->_drill_curIndex += 1;		//（结束后，索引+1）
			if (this->drill_COAS_isNodeEnd() == false){
				this->drill_COAS_refreshNext();
			}
		}
	}
}


/*-------------------------------------------------
		状态节点 - 数据 - 当前的对象名【开放函数】
*/
QString Drill_COAS_StateNodeController::drill_COAS_curBitmapName(){
	if (this->drill_COAS_isTypeState()){
		return this->_drill_curState->drill_COAS_curBitmapName();
	}
	if (this->drill_COAS_isTypeNode()){
		return this->_drill_curNode->drill_COAS_curBitmapName();
	}
	return "";
}
/*-------------------------------------------------
		状态节点 - 数据 - 当前的路径【开放函数】
*/
QString Drill_COAS_StateNodeController::drill_COAS_curBitmapPath(){
	if (this->drill_COAS_isTypeState()){
		return this->_drill_curState->drill_COAS_curBitmapPath();
	}
	if (this->drill_COAS_isTypeNode()){
		return this->_drill_curNode->drill_COAS_curBitmapPath();
	}
	return "";
}
/*-------------------------------------------------
		状态节点 - 数据 - 当前的色调【开放函数】
*/
int Drill_COAS_StateNodeController::drill_COAS_curBitmapTint(){
	if (this->drill_COAS_isTypeState()){
		return this->_drill_curState->drill_COAS_curBitmapTint();
	}
	if (this->drill_COAS_isTypeNode()){
		return this->_drill_curNode->drill_COAS_curBitmapTint();
	}
	return 0;
}
/*-------------------------------------------------
		状态节点 - 数据 - 当前的模糊【开放函数】
*/
bool Drill_COAS_StateNodeController::drill_COAS_curBitmapSmooth(){
	if (this->drill_COAS_isTypeState()){
		return this->_drill_curState->drill_COAS_curBitmapSmooth();
	}
	if (this->drill_COAS_isTypeNode()){
		return this->_drill_curNode->drill_COAS_curBitmapSmooth();
	}
	return false;
}
/*-------------------------------------------------
		状态节点 - 节点 - 当前是否为 状态元类型【开放函数】
*/
bool Drill_COAS_StateNodeController::drill_COAS_isTypeState(){
	if (this->_drill_curState == nullptr){ return false; }
	return this->_drill_data["play_type"].toString() == "随机播放状态元" ||
		this->_drill_data["play_type"].toString() == "顺序播放状态元";
}
/*-------------------------------------------------
		状态节点 - 节点 - 当前是否为 集合类型【开放函数】
*/
bool Drill_COAS_StateNodeController::drill_COAS_isTypeNode(){
	if (this->_drill_curNode == nullptr){ return false; }
	return this->_drill_data["play_type"].toString() == "随机播放嵌套集合" ||
		this->_drill_data["play_type"].toString() == "顺序播放嵌套集合";
}
/*-------------------------------------------------
		状态节点 - 节点 - 设置父数据id【开放函数】
*/
void Drill_COAS_StateNodeController::drill_COAS_setParentDataId(int data_id){
	this->_drill_parentDataId = data_id;
}
/*-------------------------------------------------
		状态节点 - 节点 - 设置当前层数【开放函数】
*/
void Drill_COAS_StateNodeController::drill_COAS_setLayer(int layer){
	this->_drill_curLayer = layer;
}
/*-------------------------------------------------
		状态节点 - 节点 - 是否结束播放【开放函数】
*/
bool Drill_COAS_StateNodeController::drill_COAS_isNodeEnd(){
	return this->_drill_curIndex >= this->_drill_tarIndex;
}
/*-------------------------------------------------
		状态节点 - 节点 - 当前状态元优先级【开放函数】
*/
int Drill_COAS_StateNodeController::drill_COAS_getCurStatePriority(){
	int priority = this->_drill_data["priority"].toInt();
	if (this->drill_COAS_isTypeState()){
		return qMax(priority, this->_drill_curState->drill_COAS_getCurStatePriority());
	}
	if (this->drill_COAS_isTypeNode()){
		return qMax(priority, this->_drill_curNode->drill_COAS_getCurStatePriority());
	}
	return priority;
}
/*-------------------------------------------------
		状态节点 - 节点 - 可被动作元打断【开放函数】
*/
bool Drill_COAS_StateNodeController::drill_COAS_canBeInterrupted(){
	if (this->_drill_data["canBeInterrupted"].toBool() == true){ return true; }
	int priority = this->_drill_data["priority"].toInt();
	if (this->drill_COAS_isTypeState()){
		return this->_drill_curState->drill_COAS_canBeInterrupted();
	}
	if (this->drill_COAS_isTypeNode()){
		return this->_drill_curNode->drill_COAS_canBeInterrupted();
	}
	return false;
}
/*-------------------------------------------------
		状态节点 - 子节点 - 刷新子节点【开放函数】
*/
void Drill_COAS_StateNodeController::drill_COAS_refreshNext(){
	this->drill_COAS_refreshNext_Private();
}
/*-------------------------------------------------
		状态节点 - 子节点 - 获取当前状态元名称【开放函数】
*/
QString Drill_COAS_StateNodeController::drill_COAS_getCurStateName(){
	if (this->drill_COAS_isTypeState()){
		return this->_drill_curState->drill_COAS_getCurStateName();
	}
	if (this->drill_COAS_isTypeNode()){
		return this->_drill_curNode->drill_COAS_getCurStateName();
	}
	return "";
}
/*-------------------------------------------------
		状态节点 - 子节点 - 获取当前状态元名称（全路径）【开放函数】
*/
QString Drill_COAS_StateNodeController::drill_COAS_getCurStateName_AllRoot(){
	QJsonObject data = this->_drill_data;
	if (this->drill_COAS_isTypeState()){
		return data["name"].toString() + " > " + this->_drill_curState->drill_COAS_getCurStateName();
	}
	if (this->drill_COAS_isTypeNode()){
		return data["name"].toString() + " > " + this->_drill_curNode->drill_COAS_getCurStateName_AllRoot();
	}
	return "";
}
/*-------------------------------------------------
		状态节点 - 操作 - 播放简单状态元集合【开放函数】
*/
void Drill_COAS_StateNodeController::drill_COAS_setNewStateNameList(QStringList state_nameList){
	if (state_nameList.count() == 0){ return; }
	QJsonObject data = this->_drill_data;
	data["play_type"] = "随机播放状态元";
	data["play_randomStateSeq"] = TTool::_QJsonArray_QStringToA_(state_nameList);
	this->_drill_data = data;		//（c++注意此处，data变化后需要强制赋值，因为不是指针修改）
	this->drill_COAS_refreshNext();
}