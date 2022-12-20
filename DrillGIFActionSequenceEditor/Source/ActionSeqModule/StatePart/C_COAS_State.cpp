#include "stdafx.h"
#include "C_COAS_State.h"

#include "../Data/LengthData/W_COAS_Length.h"
#include "../Data/S_ActionSeqDataContainer.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		状态元 数据类.cpp
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		状态元 的数据类。
-----==========================================================-----
*/
C_COAS_State::C_COAS_State(){
	this->id = -1;

	this->name = "";								//常规 - 状态元名称
	this->tag_tank.clear();							//常规 - 状态元标签
	this->priority = 0;								//常规 - 状态元优先级
	this->proportion = 1;							//常规 - 状态元权重
	this->canBeInterrupted = false;					//常规 - 可被动作元打断

	this->gif_src.clear();							//GIF - 资源
	this->gif_src_file = "img/Special__actionSeq/";	//GIF - 资源文件夹
	this->gif_intervalTank.clear();					//GIF - 帧间隔-明细表
	this->gif_interval = 4;							//GIF - 帧间隔
	this->gif_back_run = false;						//GIF - 是否倒放
	this->gif_preload = false;						//GIF - 是否预加载

	this->tint = 0;									//图像 - 色调值
	this->smooth = false;							//图像 - 模糊边缘

	this->note = "";								//杂项 - 备注
}
C_COAS_State::~C_COAS_State(){
}

/*-------------------------------------------------
		数据 - 清除数据
*/
void C_COAS_State::clearData(){

	this->name = "";								//常规 - 状态元名称
	this->tag_tank.clear();							//常规 - 状态元标签
	this->priority = 0;								//常规 - 状态元优先级
	this->proportion = 1;							//常规 - 状态元权重
	this->canBeInterrupted = false;					//常规 - 可被动作元打断

	this->gif_src.clear();							//GIF - 资源
	this->gif_src_file = "img/Special__actionSeq/";	//GIF - 资源文件夹
	this->gif_intervalTank.clear();					//GIF - 帧间隔-明细表
	this->gif_interval = 4;							//GIF - 帧间隔
	this->gif_back_run = false;						//GIF - 是否倒放
	this->gif_preload = false;						//GIF - 是否预加载

	this->tint = 0;									//图像 - 色调值
	this->smooth = false;							//图像 - 模糊边缘

	this->note = "";								//杂项 - 备注
}

/*-------------------------------------------------
		空判断
*/
bool C_COAS_State::isNull(){
	if (this->id == -1){ return true; }
	if (this->name == ""){ return true; }
	return false;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_COAS_State::getJsonObject(){
	QJsonObject obj_actionSeq = QJsonObject();

	obj_actionSeq.insert("id", this->id);
	obj_actionSeq.insert("name", this->name);

	return obj_actionSeq;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_COAS_State::setJsonObject(QJsonObject obj_actionSeq, int id){

	// > 树数据
	this->id = id;
	this->name = obj_actionSeq.value("name").toString();

}