﻿#include "stdafx.h"
#include "Drill_COAS_StateController.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		状态元控制器【复刻类】.cpp
		作者：		drill_up
		所属模块：	动画序列数据
		功能：		> 定义一个专门控制 状态元 的数据类。
					> 该类可被存到存档中。
					
		说明：		当前复刻版本：[v1.6]
-----==========================================================-----
*/
Drill_COAS_StateController::Drill_COAS_StateController(){
	this->_drill_data = QJsonObject();									//深拷贝数据
	this->_drill_controllerSerial = QUuid::createUuid().toString();		//（生成一个不重复的序列号）
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("{", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("}", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("-", "");
	this->drill_controllerState_initData();								//初始化数据
	this->drill_controllerState_initChild();							//初始化子功能
	this->drill_controllerState_resetData(this->_drill_data);
}
Drill_COAS_StateController::Drill_COAS_StateController(QJsonObject data){
	this->_drill_data = data;											//深拷贝数据
	this->_drill_controllerSerial = QUuid::createUuid().toString();		//（生成一个不重复的序列号）
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("{", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("}", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("-", "");
	this->drill_controllerState_initData();								//初始化数据
	this->drill_controllerState_initChild();							//初始化子功能
	this->drill_controllerState_resetData(this->_drill_data);
}
Drill_COAS_StateController::~Drill_COAS_StateController(){
}


/*-------------------------------------------------
		状态元 - 帧刷新【标准函数】
*/
void Drill_COAS_StateController::drill_controllerState_update(){
	if (this->isNull()){ return; }
	this->drill_controllerState_updateAttr();		//帧刷新 - A主体
													//帧刷新 - B输出数据（无）
	this->drill_controllerState_updateGIF();		//帧刷新 - C播放GIF
}
/*-------------------------------------------------
		状态元 - 初始化数据【标准默认值】
*/
void Drill_COAS_StateController::drill_controllerState_initData(){
	QJsonObject data = this->_drill_data;

	// > A主体
	if (data["name"].isUndefined() == true){ data["name"] = ""; }								//A主体 - 名称
	if (data["tag_tank"].isUndefined() == true){ data["tag_tank"] = QJsonArray(); }				//A主体 - 标签
	if (data["priority"].isUndefined() == true){ data["priority"] = 0; }						//A主体 - 优先级
	if (data["proportion"].isUndefined() == true){ data["proportion"] = 40; }					//A主体 - 权重
	if (data["canBeInterrupted"].isUndefined() == true){ data["canBeInterrupted"] = false; }	//A主体 - 可被动作元打断
	if (data["note"].isUndefined() == true){ data["note"] = ""; }								//A主体 - 备注

	// > B输出数据
	if (data["tint"].isUndefined() == true){ data["tint"] = 0; }								//B输出数据 - 色调值
	if (data["smooth"].isUndefined() == true){ data["smooth"] = false; }						//B输出数据 - 模糊边缘

	// > C播放GIF
	if (data["gif_src"].isUndefined() == true){ data["gif_src"] = QJsonArray(); }							//C播放GIF - 资源
	if (data["gif_src_file"].isUndefined() == true){ data["gif_src_file"] = "img/Special__actionSeq/"; }	//C播放GIF - 资源文件夹
	if (data["gif_intervalTank"].isUndefined() == true){ data["gif_intervalTank"] = QJsonArray(); }			//C播放GIF - 帧间隔-明细表
	if (data["gif_interval"].isUndefined() == true){ data["gif_interval"] = 4; }							//C播放GIF - 帧间隔
	if (data["gif_back_run"].isUndefined() == true){ data["gif_back_run"] = false; }						//C播放GIF - 是否倒放
	if (data["gif_preload"].isUndefined() == true){ data["gif_preload"] = false; }							//C播放GIF - 是否预加载

	this->_drill_data = data;	//（c++中，注意此处的指针，需要重新赋值）
}
/*-------------------------------------------------
		状态元 - 初始化子功能
*/
void Drill_COAS_StateController::drill_controllerState_initChild(){
	this->drill_controllerState_initAttr();			//初始化子功能 - A主体
	this->drill_controllerState_initBitmapParam();	//初始化子功能 - B输出数据
	this->drill_controllerState_initGIF();			//初始化子功能 - C播放GIF
}
/*-------------------------------------------------
		状态元 - 重设数据（私有）
*/
void Drill_COAS_StateController::drill_controllerState_resetData(QJsonObject data){

	// > C播放GIF - 重置播放
	this->drill_controllerState_setCurIndex_Private(0);

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
	this->drill_controllerState_initData();									//初始化数据
	this->drill_controllerState_initChild();								//初始化子功能
}
/*-------------------------------------------------
		状态元 - 空判断
*/
bool Drill_COAS_StateController::isNull(){
	return this->_drill_data.isEmpty();
}


/*-------------------------------------------------
		A主体 - 当前名称【开放函数】
*/
QString Drill_COAS_StateController::drill_controllerState_getName(){
	return this->_drill_data["name"].toString();
}
/*-------------------------------------------------
		A主体 - 当前优先级【开放函数】
*/
int Drill_COAS_StateController::drill_controllerState_getPriority(){
	return this->_drill_data["priority"].toInt();
}
/*-------------------------------------------------
		A主体 - 可被动作元打断【开放函数】
*/
int Drill_COAS_StateController::drill_controllerState_canBeInterrupted(){
	return this->_drill_data["canBeInterrupted"].toBool();
}

/*-------------------------------------------------
		B输出数据 - 当前的资源名【开放函数】
*/
QString Drill_COAS_StateController::drill_controllerState_curBitmapName(){
	return this->_drill_curBitmapName;
}
/*-------------------------------------------------
		B输出数据 - 当前的路径【开放函数】
*/
QString Drill_COAS_StateController::drill_controllerState_curBitmapPath(){
	return this->_drill_data["gif_src_file"].toString();
}
/*-------------------------------------------------
		B输出数据 - 当前的色调【开放函数】
*/
int Drill_COAS_StateController::drill_controllerState_curBitmapTint(){
	return this->_drill_data["tint"].toInt();
}
/*-------------------------------------------------
		B输出数据 - 当前的模糊【开放函数】
*/
bool Drill_COAS_StateController::drill_controllerState_curBitmapSmooth(){
	return this->_drill_data["smooth"].toBool();
}

/*-------------------------------------------------
		C播放GIF - 是否结束播放【开放函数】
*/
bool Drill_COAS_StateController::drill_controllerState_isEnd(){
	return this->_drill_curIndex >= this->_drill_tarIndex;
}
/*-------------------------------------------------
		C播放GIF - 重置播放【开放函数】
*/
void Drill_COAS_StateController::drill_controllerState_resetTimer(){
	this->drill_controllerState_setCurIndex_Private(0);
}
/*-------------------------------------------------
		C播放GIF - 设置指定帧【开放函数】
*/
void Drill_COAS_StateController::drill_controllerState_setCurIndex(int index){
	this->drill_controllerState_setCurIndex_Private(index);
}


/*-------------------------------------------------
		A主体 - 初始化子功能
*/
void Drill_COAS_StateController::drill_controllerState_initAttr(){
	this->_drill_curTime = 0;						//A主体 - 当前时间
	this->_drill_needDestroy = false;				//A主体 - 销毁
}
/*-------------------------------------------------
		A主体 - 帧刷新
*/
void Drill_COAS_StateController::drill_controllerState_updateAttr(){

	// > 时间流逝
	this->_drill_curTime += 1;
}


/*-------------------------------------------------
		B输出数据 - 初始化子功能
*/
void Drill_COAS_StateController::drill_controllerState_initBitmapParam(){
	this->_drill_curBitmapName = "";		//B输出数据 - 当前的资源名
											//B输出数据 - 当前的路径（从数据中直接获取）
											//B输出数据 - 当前的色调（从数据中直接获取）
											//B输出数据 - 当前的模糊（从数据中直接获取）
}


/*-------------------------------------------------
		C播放GIF - 初始化子功能
*/
void Drill_COAS_StateController::drill_controllerState_initGIF(){
	QJsonObject data = this->_drill_data;

	// > 播放参数
	this->_drill_curTickTime = 0;			//播放参数 - 当前累计时间
	this->_drill_curIndex = 0;				//播放参数 - 当前索引
	this->_drill_tarIndex = 0;				//播放参数 - 索引结束位置
	if (data["gif_src"].isUndefined() == false){
		this->_drill_tarIndex = data["gif_src"].toArray().count();
	}

	// > 帧间隔列表 计算
	this->_drill_curIntervalTank = QJsonArray();
	for (int i = 0; i < data["gif_src"].toArray().count(); i++){
		int interval = data["gif_interval"].toInt();
		QJsonArray gif_intervalTank = data["gif_intervalTank"].toArray();
		if (i < gif_intervalTank.count()){
			interval = gif_intervalTank[i].toInt();
		}
		this->_drill_curIntervalTank.append(interval);
	}
}
/*-------------------------------------------------
		C播放GIF - 设置指定帧
*/
void Drill_COAS_StateController::drill_controllerState_setCurIndex_Private(int index){
	if (index >= this->_drill_tarIndex){
		index = this->_drill_tarIndex - 1;
	}

	// > B输出数据 - 重设对象名
	this->_drill_curBitmapName = "";

	// > 播放参数
	this->_drill_curTickTime = 0;				//播放参数 - 当前累计时间
	this->_drill_curIndex = index;				//播放参数 - 当前索引
}
/*-------------------------------------------------
		C播放GIF - 帧刷新（状态元）
*/
void Drill_COAS_StateController::drill_controllerState_updateGIF(){
	QJsonObject data = this->_drill_data;

	// > 当前索引
	int cur_index = this->_drill_curIndex;
	if (data["gif_back_run"].toBool() == true){		//（倒放情况）
		cur_index = this->_drill_curIntervalTank.count() - 1 - this->_drill_curIndex;
	}
	if (cur_index < 0){ cur_index = 0; }	//【状态元 播放完毕后，保持在最后一帧，等待 状态节点 控制新的一轮。】
	if (cur_index >= this->_drill_curIntervalTank.count()){ cur_index = this->_drill_curIntervalTank.count() - 1; }
	if (this->_drill_curIntervalTank.count() == 0){ return; }

	// > 帧间隔列表
	int cur_time = this->_drill_curTickTime;
	int tar_time = this->_drill_curIntervalTank[cur_index].toInt();
	if (cur_time >= tar_time){
		// > 当前索引+1
		this->_drill_curIndex += 1;		//（达到帧间隔后，索引+1）
		this->_drill_curTickTime = 0;
	}

	// > B输出数据 - 记录对象名
	this->_drill_curBitmapName = data["gif_src"].toArray()[cur_index].toString();

	// > 当前累计时间+1
	this->_drill_curTickTime += 1;
}