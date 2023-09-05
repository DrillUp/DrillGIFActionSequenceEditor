#include "stdafx.h"
#include "Drill_COAS_MainController.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动画序列主控制器【复刻类】.cpp
		作者：		drill_up
		所属模块：	动画序列数据
		功能：		> 定义一个专门控制 动画序列 的数据类。
					> 该类可被存到存档中。
					
		说明：		当前复刻版本：[v1.6]
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
	this->drill_controllerMain_initData();								//初始化数据
	this->drill_controllerMain_initChild();								//初始化子功能
	this->drill_controllerMain_resetData(this->_drill_data);
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
	this->drill_controllerMain_initData();								//初始化数据
	this->drill_controllerMain_initChild();								//初始化子功能
	this->drill_controllerMain_resetData(this->_drill_data);
	//$gameTemp._drill_COAS_lastCreatedMainController = this;			//（记录上一个动画序列）
}
Drill_COAS_MainController::~Drill_COAS_MainController(){
}


/*-------------------------------------------------
		动画序列 - 帧刷新【标准函数】
*/
void Drill_COAS_MainController::drill_controllerMain_update(){
	if (this->isNull()){ return; }
	if (this->_drill_data["pause"].toBool(false)){ return; }
	this->drill_controllerMain_updateAttr();			//帧刷新 - A主体
														//帧刷新 - B输出数据（无）
														//帧刷新 - C管理状态元（无）
	this->drill_controllerMain_updateStateAndNode();	//帧刷新 - D管理状态节点
	this->drill_controllerMain_updateAct();				//帧刷新 - E管理动作元
														//帧刷新 - F管理装饰器（无）
}
/*-------------------------------------------------
		动画序列 - 初始化数据【标准默认值】
*/
void Drill_COAS_MainController::drill_controllerMain_initData(){
	QJsonObject data = this->_drill_data;

	// > A主体
	if (data["id"].isUndefined() == true){ data["id"] = -1; }						//A主体 - 标识
	if (data["visible"].isUndefined() == true){ data["visible"] = true; }			//A主体 - 显示情况
	if (data["pause"].isUndefined() == true){ data["pause"] = false; }				//A主体 - 暂停情况

	// > B输出数据（无）

	// > C管理状态元
	if (data["state_tank"].isUndefined() == true){ data["state_tank"] = QJsonArray(); }								//C管理状态元 - 容器

	// > D管理状态节点
	if (data["stateNode_tank"].isUndefined() == true){ data["stateNode_tank"] = QJsonArray(); }						//D管理状态节点 - 容器
	if (data["state_default_randomSeq"].isUndefined() == true){ data["state_default_randomSeq"] = QJsonArray(); }	//D管理状态节点 - 默认的状态元集合

	// > E管理动作元
	if (data["act_tank"].isUndefined() == true){ data["act_tank"] = QJsonArray(); }									//E管理动作元 - 容器

	// > F管理装饰器（子插件用）
	if (data["waitForPreload"].isUndefined() == true){ data["waitForPreload"] = true; }								//F管理装饰器 - 预加载等待
	if (data["bitmapRefreshFrame"].isUndefined() == true){ data["bitmapRefreshFrame"] = true; }						//F管理装饰器 - bitmap刷新框架开关

	this->_drill_data = data;	//（c++中，注意此处的指针，需要重新赋值）
}
/*-------------------------------------------------
		动画序列 - 初始化子功能
*/
void Drill_COAS_MainController::drill_controllerMain_initChild(){
	this->drill_controllerMain_initAttr();			//初始化子功能 - A主体
	this->drill_controllerMain_initBitmapParam();	//初始化子功能 - B输出数据
	this->drill_controllerMain_initState();			//初始化子功能 - C管理状态元
	this->drill_controllerMain_initNode();			//初始化子功能 - D管理状态节点
	this->drill_controllerMain_initAct();			//初始化子功能 - E管理动作元
	this->drill_controllerMain_initDecorator();		//初始化子功能 - F管理装饰器
}
/*-------------------------------------------------
		动画序列 - 重设数据【标准函数】
*/
void Drill_COAS_MainController::drill_controllerMain_resetData(QJsonObject data){

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
	this->drill_controllerMain_initData();									//初始化数据
	this->drill_controllerMain_initChild();									//初始化子功能
}
/*-------------------------------------------------
		动画序列 - 显示/隐藏【标准函数】
*/
void Drill_COAS_MainController::drill_controllerMain_setVisible(bool visible){
	QJsonObject data = this->_drill_data;
	data["visible"] = visible;
	this->_drill_data = data;			//（此函数c++中未使用）
}
/*-------------------------------------------------
		动画序列 - 暂停/继续【标准函数】
*/
void Drill_COAS_MainController::drill_controllerMain_setPause(bool pause){
	QJsonObject data = this->_drill_data;
	data["pause"] = pause;
	this->_drill_data = data;			//（此函数c++中未使用）
}
/*-------------------------------------------------
		动画序列 - 设置销毁【标准函数】
*/
void Drill_COAS_MainController::drill_controllerMain_destroy(){
	this->_drill_needDestroy = true;	//（此函数c++中未使用）
}
/*-------------------------------------------------
		动画序列 - 空判断
*/
bool Drill_COAS_MainController::isNull(){
	return this->_drill_data.isEmpty();
}





	
/*-------------------------------------------------
		A主体 - 初始化子功能
*/
void Drill_COAS_MainController::drill_controllerMain_initAttr(){
	this->_drill_curTime = 0;						//A主体 - 当前时间
	this->_drill_needDestroy = false;				//A主体 - 销毁
	this->_drill_checkArrayEnabled = true;			//A主体 - 校验数据是否为数组
}
/*-------------------------------------------------
		A主体 - 校验数据是否为数组
*/
void Drill_COAS_MainController::drill_controllerMain_checkArray(QJsonValue arr){
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
		A主体 - 帧刷新
*/
void Drill_COAS_MainController::drill_controllerMain_updateAttr(){

	// > 时间流逝
	this->_drill_curTime += 1;
}


/*-------------------------------------------------
		B输出数据 - 初始化子功能
*/
void Drill_COAS_MainController::drill_controllerMain_initBitmapParam(){
	this->_drill_curBitmapName = "";				//B输出数据 - 当前的资源名
	this->_drill_curBitmapPath = "";				//B输出数据 - 当前的路径
	this->_drill_curBitmapTint = 0;					//B输出数据 - 当前的色调
	this->_drill_curBitmapSmooth = false;			//B输出数据 - 当前的模糊
}


/*-------------------------------------------------
		C管理状态元 - 初始化子功能
*/
void Drill_COAS_MainController::drill_controllerMain_initState(){
	//（无）
}
/*-------------------------------------------------
		C管理状态元 - 获取数据 - 全部【开放函数】
*/
QJsonArray Drill_COAS_MainController::drill_controllerMain_getStateData_All(){
	return this->_drill_data["state_tank"].toArray();
}
/*-------------------------------------------------
		C管理状态元 - 获取数据 - 根据名称【开放函数】
*/
QJsonObject Drill_COAS_MainController::drill_controllerMain_getStateData_ByName(QString state_name){
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
		C管理状态元 - 获取数据 - 全部名称【开放函数】
*/
QStringList Drill_COAS_MainController::drill_controllerMain_getStateData_AllName(){
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
		D管理状态节点 - 初始化子功能
*/
void Drill_COAS_MainController::drill_controllerMain_initNode(){
	QJsonObject data = this->_drill_data;

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
	this->_drill_data = data;	//（c++中，注意此处的指针，需要重新赋值）

	// > 状态节点
	//this._drill_node_curName = "";	//（立刻变化，没有缓冲设置）
	this->_drill_node_curSerial = "";	//（注意，序列号在c++中为字符串）
	this->_drill_node_curController = new Drill_COAS_StateNodeController(node_data);
	this->_drill_node_curController->drill_controllerNode_setParentDataId(data["id"].toInt());
	this->_drill_node_curController->drill_controllerNode_setLayer(0);

	// > 数据量查看
	qDebug() << node_data;
}
/*-------------------------------------------------
		D管理状态节点 - 帧刷新
*/
void Drill_COAS_MainController::drill_controllerMain_updateStateAndNode(){
	if (this->drill_controllerMain_Act_isPlayingAct() == true){ return; }		//动作播放时，不操作
	
	// > 动作元打断
	if (this->_drill_act_interrupt == true){
		this->_drill_act_interrupt = false;
		if (this->_drill_node_curController->drill_controllerNode_canBeInterrupted()){
			this->_drill_node_curController->drill_controllerNode_resetTimer();
			this->_drill_node_curController->drill_controllerNode_refreshNext();
		}
	}

	// > 状态节点 数据刷新情况
	if (this->_drill_node_curSerial != this->_drill_node_curController->_drill_controllerSerial){
		this->_drill_node_curController->drill_controllerNode_resetTimer();
		this->_drill_node_curController->drill_controllerNode_refreshNext();
		this->_drill_node_curSerial = this->_drill_node_curController->_drill_controllerSerial;
	}

	// > 状态节点 播放完毕情况
	if (this->_drill_node_curController->drill_controllerNode_isEnd() == true){
		this->_drill_node_curController->drill_controllerNode_resetTimer();
		this->_drill_node_curController->drill_controllerNode_refreshNext();
	}

	// > 状态节点 - 帧刷新
	this->_drill_node_curController->drill_controllerNode_update();

	// > 状态节点 - B输出数据
	this->_drill_curBitmapName = this->_drill_node_curController->drill_controllerNode_curBitmapName();
	this->_drill_curBitmapPath = this->_drill_node_curController->drill_controllerNode_curBitmapPath();
	this->_drill_curBitmapTint = this->_drill_node_curController->drill_controllerNode_curBitmapTint();
	this->_drill_curBitmapSmooth = this->_drill_node_curController->drill_controllerNode_curBitmapSmooth();
}
/*-------------------------------------------------
		D管理状态节点 - 获取数据 - 全部【开放函数】
*/
QJsonArray Drill_COAS_MainController::drill_controllerMain_getNodeData_All(){
	return this->_drill_data["stateNode_tank"].toArray();
}
/*-------------------------------------------------
		D管理状态节点 - 获取数据 - 根据名称【开放函数】
*/
QJsonObject Drill_COAS_MainController::drill_controllerMain_getNodeData_ByName(QString stateNode_name){
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
		D管理状态节点 - 获取数据 - 全部名称【开放函数】
*/
QStringList Drill_COAS_MainController::drill_controllerMain_getNodeData_AllName(){
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
		D管理状态节点 - 获取当前状态元名称【开放函数】
*/
QString Drill_COAS_MainController::drill_controllerMain_Node_getCurStateName(){
	return this->_drill_node_curController->drill_controllerNode_getStateName();
}
/*-------------------------------------------------
		D管理状态节点 - 获取当前状态元名称（全路径）【开放函数】
*/
QString Drill_COAS_MainController::drill_controllerMain_Node_getCurStateName_AllRoot(){
	return this->_drill_node_curController->drill_controllerNode_getStateName_AllRoot();
}
/*-------------------------------------------------
		D管理状态节点 - 获取当前优先级【开放函数】
*/
int Drill_COAS_MainController::drill_controllerMain_Node_getCurPriority(){
	return this->_drill_node_curController->drill_controllerNode_getPriority();
}
/*-------------------------------------------------
		D管理状态节点 - 操作 - 播放默认的状态元集合【开放函数】
*/
void Drill_COAS_MainController::drill_controllerMain_setStateNodeDefault(){
	this->drill_controllerMain_setStateNode("默认的状态元集合");
}
/*-------------------------------------------------
		D管理状态节点 - 操作 - 播放状态节点【开放函数】
*/
void Drill_COAS_MainController::drill_controllerMain_setStateNode(QString node_name){
	QJsonObject data = this->_drill_data;
	QJsonObject node_data = this->drill_controllerMain_getNodeData_ByName(node_name);
	if (node_data.isEmpty()){ return; }
	this->_drill_node_curController->drill_controllerNode_resetData(node_data);					//集合 - 重设数据
	this->_drill_node_curController->drill_controllerNode_setParentDataId(data["id"].toInt());	//集合 - 设置父数据id
	this->_drill_node_curController->drill_controllerNode_setLayer(0);							//集合 - 设置当前层数
	this->_drill_node_curController->drill_controllerNode_refreshNext();						//集合 - 刷新子节点
}
/*-------------------------------------------------
		D管理状态节点 - 操作 - 播放简单状态元集合【开放函数】
*/
void Drill_COAS_MainController::drill_controllerMain_setSimpleStateNode(QStringList state_nameList){
	//this->drill_controllerMain_checkArray(state_nameList);	//（不需要检查）
	this->_drill_node_curController->drill_COAS_setNewStateNameList(state_nameList);
}
/*-------------------------------------------------
		D管理状态节点 - 操作 - 播放状态元/状态节点 根据标签【开放函数】
*/
bool Drill_COAS_MainController::drill_controllerMain_setAnnotation(QString annotation){
	//（此函数在c++中未使用）
	return false;
}
/*-------------------------------------------------
		D管理状态节点 - 操作 - 播放状态元 根据标签列表【开放函数】
bool Drill_COAS_MainController::drill_controllerMain_setAnnotation(QStringList annotation_list){

	// > 找到符合注解数量最多的状态元名
	int max_fit_count = 0;			//（最大符合数量）
	QList<QJsonObject> fit_seq;		//（最大符合的索引列表）
	QJsonArray stateData_list = this->drill_controllerMain_getStateData_All();
	for( int i = 0; i < stateData_list.count(); i++){
		QJsonObject stateData = stateData_list[i].toObject();
		QStringList tag_tank = TTool::_QJsonArray_AToQString_(stateData["tag_tank"].toArray());
		if (tag_tank.count() == 0){ continue; }

		// > 记录注解符合数量
		int fit_count = 0;
		for( int j = 0; j < annotation_list.count(); j++){
			QString annotation = annotation_list[j];
			if (tag_tank.contains(annotation) == true){
				fit_count += 1;
			}
		}

		// > 符合数量更大时，清空序列，重新添加
		if (fit_count > max_fit_count){
			fit_seq.clear();
			max_fit_count = fit_count;

			QJsonObject fit;
			fit["index"] = i;
			fit["count"] = fit_count;
			fit["name"] = stateData["name"].toString();
			fit_seq.append(fit);

			// > 符合数量相等，累计
		}
		else if (fit_count == max_fit_count){
			QJsonObject fit = {};
			fit["index"] = i;
			fit["count"] = fit_count;
			fit["name"] = stateData["name"].toString();
			fit_seq.append(fit);

			// > 符合数量少了，跳过
		}else{
			continue;
		}
	}
	if (fit_seq.count() == 0){ return false; }

	// > 根据最大值的下标取出符合的名称
	QStringList stateName_list;
	for (int i = 0; i < fit_seq.count(); i++){
		stateName_list.append(fit_seq[i]["name"].toString());
	}

	// > 播放简单状态元集合
	this->drill_controllerMain_setSimpleStateNode(stateName_list);
	return true;
}
*/


/*-------------------------------------------------
		E管理动作元 - 初始化子功能
*/
void Drill_COAS_MainController::drill_controllerMain_initAct(){

	// > 动作元
	this->_drill_act_curName = "";
	this->_drill_act_curSerial = "";	//（注意，序列号在c++中为字符串）
	this->_drill_act_curController = new Drill_COAS_ActController();
	this->_drill_act_interrupt = false;
}
/*-------------------------------------------------
		E管理动作元 - 帧刷新
*/
void Drill_COAS_MainController::drill_controllerMain_updateAct(){
	if (this->drill_controllerMain_Act_isPlayingAct() == false){ return; }	//动作未播放时，不操作

	// > 动作元打断 锁
	this->_drill_act_interrupt = true;

	// > 动作元 数据刷新情况
	if (this->_drill_act_curSerial != this->_drill_act_curController->_drill_controllerSerial){
		QJsonObject data_act = this->drill_controllerMain_getActData_ByName(this->_drill_act_curName);
		if (data_act.isEmpty() == false){
			this->_drill_act_curController->drill_controllerAct_resetData(data_act);
		}
		this->_drill_act_curSerial = this->_drill_act_curController->_drill_controllerSerial;
	}

	// > 动作元 播放完毕情况
	if (this->_drill_act_curController->drill_controllerAct_isEnd() == true){
		this->_drill_act_curName = "";
		this->_drill_act_curSerial = "";
	}

	// > 动作元 - 帧刷新
	this->_drill_act_curController->drill_controllerAct_update();

	// > 动作元 - B输出数据
	this->_drill_curBitmapName = this->_drill_act_curController->drill_controllerAct_curBitmapName();
	this->_drill_curBitmapPath = this->_drill_act_curController->drill_controllerAct_curBitmapPath();
	this->_drill_curBitmapTint = this->_drill_act_curController->drill_controllerAct_curBitmapTint();
	this->_drill_curBitmapSmooth = this->_drill_act_curController->drill_controllerAct_curBitmapSmooth();
}
/*-------------------------------------------------
		E管理动作元 - 获取数据 - 全部【开放函数】
*/
QJsonArray Drill_COAS_MainController::drill_controllerMain_getActData_All(){
	return this->_drill_data["act_tank"].toArray();
}
/*-------------------------------------------------
		E管理动作元 - 获取数据 - 根据名称【开放函数】
*/
QJsonObject Drill_COAS_MainController::drill_controllerMain_getActData_ByName(QString stateNode_name){
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
		E管理动作元 - 获取数据 - 全部名称【开放函数】
*/
QStringList Drill_COAS_MainController::drill_controllerMain_getActData_AllName(){
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
		E管理动作元 - 获取当前动作元名称【开放函数】
*/
QString Drill_COAS_MainController::drill_controllerMain_Act_getCurName(){
	return this->_drill_act_curName;
}
/*-------------------------------------------------
		E管理动作元 - 操作 - 播放动作元【开放函数】
*/
void Drill_COAS_MainController::drill_controllerMain_Act_setAct(QString act_name){
	if (this->_drill_act_curName == act_name){ return; }

	// > 检查高优先级状态元
	if (this->_drill_act_curName == ""){
		QJsonObject data_act = this->drill_controllerMain_getActData_ByName(act_name);
		int state_priority = this->drill_controllerMain_Node_getCurPriority();
		if (state_priority > data_act["priority"].toInt()){	//（同级的动作元可以覆盖状态元）
			return;
		}
	}

	// > 动作正在播放时
	if (this->_drill_act_curName != ""){
		QJsonObject data_act = this->drill_controllerMain_getActData_ByName(act_name);
		QJsonObject cur_act = this->drill_controllerMain_getActData_ByName(this->_drill_act_curName);

		if (cur_act["priority"].toInt() >= data_act["priority"].toInt()){	//（只能覆盖小的优先级，不包括同级）
			return;
		}
	}

	this->_drill_act_curName = act_name;
}
/*-------------------------------------------------
		E管理动作元 - 操作 - 立刻终止动作【开放函数】
*/
void Drill_COAS_MainController::drill_controllerMain_Act_stopAct(){
	this->_drill_act_curName = "";
	this->_drill_act_curSerial = "";
}
/*-------------------------------------------------
		E管理动作元 - 操作 - 是否正在播放动作元【开放函数】
*/
bool Drill_COAS_MainController::drill_controllerMain_Act_isPlayingAct(){
	return this->_drill_act_curName != "";
}


/*-------------------------------------------------
		F管理装饰器 - 初始化子功能
*/
void Drill_COAS_MainController::drill_controllerMain_initDecorator(){
	//（无）
}


