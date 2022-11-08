#include "stdafx.h"
#include "Drill_COAS_StateController.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		状态元控制器【复刻类】.cpp
		作者：		drill_up
		所属模块：	动画序列数据
		功能：		> 定义一个专门控制 状态元 的数据类。
					> 该类可被存到存档中。
					
		说明：		当前复刻版本：[v1.3]
-----==========================================================-----
*/
Drill_COAS_StateController::Drill_COAS_StateController(){
	this->_drill_data = QJsonObject();		//深拷贝数据
	this->drill_initData_State();			//初始化数据
	this->drill_initPrivateData_State();	//私有数据初始化
}
Drill_COAS_StateController::Drill_COAS_StateController(QJsonObject data){
	this->_drill_data = data;				//深拷贝数据
	this->drill_initData_State();			//初始化数据
	this->drill_initPrivateData_State();	//私有数据初始化
}
Drill_COAS_StateController::~Drill_COAS_StateController(){
}

/*-------------------------------------------------
		数据 - 空判断
*/
bool Drill_COAS_StateController::isNull(){
	return this->_drill_data.isEmpty();
}
/*-------------------------------------------------
		数据 - 初始化数据
*/
void Drill_COAS_StateController::drill_initData_State(){
	QJsonObject data = this->_drill_data;

	// > 常规
	if (data["name"].isUndefined() == true){ data["name"] = ""; }					//状态元名称
	if (data["priority"].isUndefined() == true){ data["priority"] = 0; }			//状态元优先级
	if (data["proportion"].isUndefined() == true){ data["proportion"] = 40; }		//状态元权重

	// > GIF
	if (data["gif_src"].isUndefined() == true){ data["gif_src"] = QJsonArray(); }							//GIF - 资源
	if (data["gif_src_file"].isUndefined() == true){ data["gif_src_file"] = "img/Special__actionSeq/"; }	//GIF - 资源
	if (data["gif_intervalTank"].isUndefined() == true){ data["gif_intervalTank"] = QJsonArray(); }			//GIF - 帧间隔-明细表
	if (data["gif_interval"].isUndefined() == true){ data["gif_interval"] = 4; }							//GIF - 帧间隔
	if (data["gif_back_run"].isUndefined() == true){ data["gif_back_run"] = false; }						//GIF - 是否倒放

	// > 图像
	if (data["tint"].isUndefined() == true){ data["tint"] = 0; }					//图像 - 色调值
	if (data["smooth"].isUndefined() == true){ data["smooth"] = false; }			//图像 - 模糊边缘

	// > 杂项
	if (data["note"].isUndefined() == true){ data["note"] = ""; }					//杂项 - 备注
}
/*-------------------------------------------------
		数据 - 私有数据初始化
*/
void Drill_COAS_StateController::drill_initPrivateData_State(){
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
		this->_drill_curIntervalTank[i] = interval;
	}
}

/*-------------------------------------------------
		帧刷新（需要父类手动执行）
*/
void Drill_COAS_StateController::drill_COAS_update(){
	if (this->isNull()){ return; }
	this->_drill_curTime += 1;				//时间+1
	this->drill_COAS_updateState();			//刷新状态元
}
/*-------------------------------------------------
		帧刷新 - 刷新状态元
*/
void Drill_COAS_StateController::drill_COAS_updateState(){
	

}


/*-------------------------------------------------
		数据 - 状态元 - 设置序列（接口）

				说明：	序列中存放 状态名称 的数组，状态元会跳过不识别的对象。
*/
void Drill_COAS_StateController::drill_COAS_setSequence(QStringList seq){
	
}