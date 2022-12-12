#include "stdafx.h"
#include "Drill_COAS_ActController.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动作元控制器【复刻类】.cpp
		作者：		drill_up
		所属模块：	动画序列数据
		功能：		> 定义一个专门控制 动作元 的数据类。
					> 该类可被存到存档中。
					
		说明：		当前复刻版本：[v1.5]
-----==========================================================-----
*/
Drill_COAS_ActController::Drill_COAS_ActController(){
	this->_drill_data = QJsonObject();									//深拷贝数据
	this->_drill_controllerSerial = QUuid::createUuid().toString();		//（生成一个不重复的序列号）
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("{", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("}", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("-", "");
	this->drill_initData_Act();											//初始化数据
	this->drill_initPrivateData_Act();									//私有数据初始化
	this->drill_COAS_resetData_Act(this->_drill_data);
}
Drill_COAS_ActController::Drill_COAS_ActController(QJsonObject data){
	this->_drill_data = data;											//深拷贝数据
	this->_drill_controllerSerial = QUuid::createUuid().toString();		//（生成一个不重复的序列号）
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("{", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("}", "");
	this->_drill_controllerSerial = this->_drill_controllerSerial.replace("-", "");
	this->drill_initData_Act();											//初始化数据
	this->drill_initPrivateData_Act();									//私有数据初始化
	this->drill_COAS_resetData_Act(this->_drill_data);
}
Drill_COAS_ActController::~Drill_COAS_ActController(){
}


/*-------------------------------------------------
		动作元 - 帧刷新【标准函数】
*/
void Drill_COAS_ActController::drill_COAS_update(){
	if (this->isNull()){ return; }
	this->_drill_curTime += 1;				//时间+1
	this->drill_COAS_updateAct();			//刷新动作元
}
/*-------------------------------------------------
		动作元 - 空判断
*/
bool Drill_COAS_ActController::isNull(){
	return this->_drill_data.isEmpty();
}


/*-------------------------------------------------
		动作元 - 初始化数据【标准默认值】
*/
void Drill_COAS_ActController::drill_initData_Act(){
	QJsonObject data = this->_drill_data;

	// > 常规
	if (data["name"].isUndefined() == true){ data["name"] = ""; }							//动作元名称
	if (data["tag_tank"].isUndefined() == true){ data["tag_tank"] = QJsonArray(); }			//动作元标签
	if (data["priority"].isUndefined() == true){ data["priority"] = 0; }					//动作元优先级
	
	// > GIF
	if (data["gif_src"].isUndefined() == true){ data["gif_src"] = QJsonArray(); }							//GIF - 资源
	if (data["gif_src_file"].isUndefined() == true){ data["gif_src_file"] = "img/Special__actionSeq/"; }	//GIF - 资源文件夹
	if (data["gif_intervalTank"].isUndefined() == true){ data["gif_intervalTank"] = QJsonArray(); }			//GIF - 帧间隔-明细表
	if (data["gif_interval"].isUndefined() == true){ data["gif_interval"] = 4; }							//GIF - 帧间隔
	if (data["gif_back_run"].isUndefined() == true){ data["gif_back_run"] = false; }						//GIF - 是否倒放
	if (data["gif_preload"].isUndefined() == true){ data["gif_preload"] = false; }							//GIF - 是否预加载

	// > 图像
	if (data["tint"].isUndefined() == true){ data["tint"] = 0; }					//图像 - 色调值
	if (data["smooth"].isUndefined() == true){ data["smooth"] = false; }			//图像 - 模糊边缘

	// > 杂项
	if (data["note"].isUndefined() == true){ data["note"] = ""; }					//杂项 - 备注
}
/*-------------------------------------------------
		动作元 - 私有数据初始化
*/
void Drill_COAS_ActController::drill_initPrivateData_Act(){
	QJsonObject data = this->_drill_data;

	// > 常规
	this->_drill_curTime = 0;										//常规 - 当前时间
	this->_drill_needDestroy = false;								//常规 - 销毁

	// > GIF - 输出数据
	this->_drill_curBitmapName = "";								//输出数据 - 当前的对象名
	this->_drill_curBitmapPath = data["gif_src_file"].toString();	//输出数据 - 当前的路径
	this->_drill_curBitmapTint = data["tint"].toInt();				//输出数据 - 当前的色调
	this->_drill_curBitmapSmooth = data["smooth"].toBool();			//输出数据 - 当前的模糊

	// > GIF - 播放
	this->_drill_curTickTime = 0;									//播放 - 当前累计时间
	this->_drill_curIndex = 0;										//播放 - 当前索引
	this->_drill_tarIndex = 0;										//播放 - 索引结束位置

	// > GIF - 帧间隔列表 计算
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
		动作元 - 重设数据（私有）
*/
void Drill_COAS_ActController::drill_COAS_resetData_Act(QJsonObject data){

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
	this->drill_initData_Act();												//初始化数据
	this->drill_initPrivateData_Act();										//私有数据初始化
}
/*-------------------------------------------------
		动作元 - 帧刷新动作元
*/
void Drill_COAS_ActController::drill_COAS_updateAct(){
	QJsonObject data = this->_drill_data;

	// > 当前索引
	int cur_index = this->_drill_curIndex;
	if (data["gif_back_run"].toBool() == true){		//（倒放情况）
		cur_index = this->_drill_curIntervalTank.count() - 1 - this->_drill_curIndex;
	}
	if (cur_index < 0){ cur_index = 0; }	//【动作元 播放完毕后，保持在最后一帧，等待 状态节点 控制新的一轮。】
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

	// > 输出数据
	this->_drill_curBitmapName = data["gif_src"].toArray()[cur_index].toString();
	this->_drill_curBitmapPath = data["gif_src_file"].toString();
	this->_drill_curBitmapTint = data["tint"].toInt();
	this->_drill_curBitmapSmooth = data["smooth"].toBool();

	// > 当前累计时间+1
	this->_drill_curTickTime += 1;
}


/*-------------------------------------------------
		动作元 - 输出数据 - 当前的对象名【开放函数】
*/
QString Drill_COAS_ActController::drill_COAS_curBitmapName(){
	return this->_drill_curBitmapName;
}
/*-------------------------------------------------
		动作元 - 输出数据 - 当前的路径【开放函数】
*/
QString Drill_COAS_ActController::drill_COAS_curBitmapPath(){
	return this->_drill_curBitmapPath;
}
/*-------------------------------------------------
		动作元 - 输出数据 - 当前的色调【开放函数】
*/
int Drill_COAS_ActController::drill_COAS_curBitmapTint(){
	return this->_drill_curBitmapTint;
}
/*-------------------------------------------------
		动作元 - 输出数据 - 当前的模糊【开放函数】
*/
bool Drill_COAS_ActController::drill_COAS_curBitmapSmooth(){
	return this->_drill_curBitmapSmooth;
}
/*-------------------------------------------------
		动作元 - 节点 - 是否结束播放【开放函数】
*/
bool Drill_COAS_ActController::drill_COAS_isActEnd(){
	return this->_drill_curIndex >= this->_drill_tarIndex;
}
