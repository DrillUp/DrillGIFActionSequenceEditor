﻿#include "stdafx.h"
#include "C_COAS_Action.h"

#include "../Data/LengthData/W_COAS_Length.h"
#include "../Data/S_ActionSeqDataContainer.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		动作元 数据类.cpp
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		动作元 的数据类。
-----==========================================================-----
*/
C_COAS_Action::C_COAS_Action(){
	this->id = -1;

	this->name = "";								//常规 - 动作元名称
	this->tag_tank.clear();							//常规 - 动作元标签
	this->priority = 0;								//常规 - 动作元优先级

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
C_COAS_Action::~C_COAS_Action(){
}

/*-------------------------------------------------
		数据 - 清除数据
*/
void C_COAS_Action::clearData(){

	this->name = "";								//常规 - 动作元名称
	this->tag_tank.clear();							//常规 - 动作元标签
	this->priority = 0;								//常规 - 动作元优先级

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
bool C_COAS_Action::isNull(){
	if (this->id == -1){ return true; }
	if (this->name == ""){ return true; }
	return false;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_COAS_Action::getJsonObject_Chinese(){
	QJsonObject obj = QJsonObject();
	//obj.insert("id", this->id);

	// > 常规
	obj.insert("动作元名称", this->name);
	obj.insert("动作元标签", TTool::_JSON_stringify_(this->tag_tank));
	obj.insert("动作元优先级", QString::number(this->priority));

	// > GIF
	obj.insert("资源-动作元", TTool::_JSON_stringify_(this->gif_src));
	//		（资源文件夹，不需赋值）
	QList<QString> gif_intervalTank_strList = TTool::_QList_IntToQString_(this->gif_intervalTank);
	obj.insert("帧间隔-明细表", TTool::_JSON_stringify_(gif_intervalTank_strList));
	obj.insert("帧间隔", this->gif_interval);
	obj.insert("是否倒放", this->gif_back_run ? "true" : "false");
	obj.insert("是否预加载", this->gif_preload ? "true" : "false");

	// > 图像
	obj.insert("图像-色调值", QString::number(this->tint));
	obj.insert("图像-模糊边缘", this->smooth ? "true" : "false");

	// > 杂项
	obj.insert("备注", this->note);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_COAS_Action::setJsonObject_Chinese(QJsonObject obj, int i){
	this->id = i;

	// > 常规
	this->name = obj.value("动作元名称").toString();
	this->tag_tank = TTool::_JSON_parse_To_QListQString_(obj.value("动作元标签").toString());
	this->priority = obj.value("动作元优先级").toString().toInt();

	// > GIF
	QString gif_src_str = obj.value("资源-动作元").toString();
	this->gif_src = TTool::_JSON_parse_To_QListQString_(gif_src_str);			//资源文件名
	//		（资源文件夹，不需赋值）
	QString gif_intervalTank_str = obj.value("帧间隔-明细表").toString();
	QList<QString> gif_intervalTank_strList = TTool::_JSON_parse_To_QListQString_(gif_intervalTank_str);	//帧间隔-明细表
	this->gif_intervalTank = TTool::_QList_QStringToInt_(gif_intervalTank_strList);
	this->gif_interval = obj.value("帧间隔").toString().toInt();
	this->gif_back_run = obj.value("是否倒放").toString() == "true";
	this->gif_preload = obj.value("是否预加载").toString() == "true";

	// > 图像
	this->tint = obj.value("图像-色调值").toString().toInt();
	this->smooth = obj.value("图像-模糊边缘").toString() == "true";

	// > 杂项
	this->note = obj.value("备注").toString();

}