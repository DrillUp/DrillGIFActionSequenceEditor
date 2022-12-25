#include "stdafx.h"
#include "C_COAS_StateNode.h"

#include "../Data/LengthData/W_COAS_Length.h"
#include "../Data/S_ActionSeqDataContainer.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		状态节点 数据类.cpp
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		状态节点 的数据类。
-----==========================================================-----
*/
C_COAS_StateNode::C_COAS_StateNode(){
	this->id = -1;

	this->name = "";								//常规 - 节点名称
	this->tag_tank.clear();							//常规 - 节点标签
	this->priority = 0;								//常规 - 节点优先级
	this->proportion = 1;							//常规 - 节点权重
	this->canBeInterrupted = false;					//常规 - 可被动作元打断

	this->play_type = "随机播放状态元";				//播放列表 - 播放方式
	this->play_randomStateSeq.clear();				//播放列表 - 随机播放状态元
	this->play_plainStateSeq.clear();				//播放列表 - 顺序播放状态元
	this->play_randomNodeSeq.clear();				//播放列表 - 随机播放嵌套集合
	this->play_plainNodeSeq.clear();				//播放列表 - 顺序播放嵌套集合
	this->play_randomMax = 1;						//播放列表 - 随机播放的次数上限

	this->note = "";								//杂项 - 备注
}
C_COAS_StateNode::~C_COAS_StateNode(){
}

/*-------------------------------------------------
		数据 - 清除数据
*/
void C_COAS_StateNode::clearData(){

	this->name = "";								//常规 - 状态元名称
	this->tag_tank.clear();							//常规 - 状态元标签
	this->priority = 0;								//常规 - 状态元优先级
	this->proportion = 1;							//常规 - 状态元权重
	this->canBeInterrupted = false;					//常规 - 可被动作元打断

	this->play_type = "随机播放状态元";				//播放列表 - 播放方式
	this->play_randomStateSeq.clear();				//播放列表 - 随机播放状态元
	this->play_plainStateSeq.clear();				//播放列表 - 顺序播放状态元
	this->play_randomNodeSeq.clear();				//播放列表 - 随机播放嵌套集合
	this->play_plainNodeSeq.clear();				//播放列表 - 顺序播放嵌套集合
	this->play_randomMax = 1;						//播放列表 - 随机播放的次数上限

	this->note = "";								//杂项 - 备注
}
/*-------------------------------------------------
		数据 - 判断空节点
*/
bool C_COAS_StateNode::isEmptyNode(){
	if (this->name == ""){ return true; }
	if (this->play_type == ""){ return true; }

	if (this->play_type == "随机播放状态元"){
		if (this->play_randomStateSeq.count() == 0){ return true; }
		return false;
	}
	if (this->play_type == "顺序播放状态元"){
		if (this->play_plainStateSeq.count() == 0){ return true; }
		return false;
	}
	if (this->play_type == "随机播放嵌套集合"){
		if (this->play_randomNodeSeq.count() == 0){ return true; }
		return false;
	}
	if (this->play_type == "顺序播放嵌套集合"){
		if (this->play_plainNodeSeq.count() == 0){ return true; }
		return false;
	}
	return true;
}



/*-------------------------------------------------
		空判断
*/
bool C_COAS_StateNode::isNull(){
	if (this->id == -1){ return true; }
	if (this->name == ""){ return true; }
	return false;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
			【所有 列表/结构体 都需要转成JSON字符串，不能直接存QJsonObject/QJsonArray】
*/
QJsonObject C_COAS_StateNode::getJsonObject_Chinese(){
	QJsonObject obj = QJsonObject();
	//obj.insert("id", this->id);

	// > 空对象情况（直接返回，节约存储空间）
	if (this->isEmptyNode()){
		return obj;
	}

	// > 常规
	obj.insert("节点名称", this->name);
	obj.insert("节点标签", TTool::_JSON_stringify_(this->tag_tank));
	obj.insert("节点优先级", QString::number(this->priority));
	obj.insert("节点权重", QString::number(this->proportion));
	obj.insert("可被动作元打断", this->canBeInterrupted ? "true" : "false");

	// > 播放列表
	obj.insert("播放方式", this->play_type);
	obj.insert("随机播放状态元", TTool::_JSON_stringify_(this->play_randomStateSeq));
	obj.insert("顺序播放状态元", TTool::_JSON_stringify_(this->play_plainStateSeq));
	obj.insert("随机播放嵌套集合", TTool::_JSON_stringify_(this->play_randomNodeSeq));
	obj.insert("顺序播放嵌套集合", TTool::_JSON_stringify_(this->play_plainNodeSeq));
	obj.insert("随机播放的次数上限", QString::number(this->play_randomMax));

	// > 杂项
	obj.insert("备注", this->note);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
			【所有 列表/结构体 在读取时，要先转字符串，再解析为可用对象】
*/
void C_COAS_StateNode::setJsonObject_Chinese(QJsonObject obj, int id){
	this->id = id;

	// > 常规
	this->name = obj.value("节点名称").toString();

	this->tag_tank = TTool::_JSON_parse_To_QListQString_(obj.value("节点标签").toString());
	this->priority = obj.value("节点优先级").toString("0").toInt();
	this->proportion = obj.value("节点权重").toString("40").toInt();
	this->canBeInterrupted = obj.value("可被动作元打断").toString() == "true";

	// > 播放列表
	this->play_type = obj.value("播放方式").toString("随机播放状态元");
	this->play_randomStateSeq = TTool::_JSON_parse_To_QListQString_(obj.value("随机播放状态元").toString());
	this->play_plainStateSeq = TTool::_JSON_parse_To_QListQString_(obj.value("顺序播放状态元").toString());
	this->play_randomNodeSeq = TTool::_JSON_parse_To_QListQString_(obj.value("随机播放嵌套集合").toString());
	this->play_plainNodeSeq = TTool::_JSON_parse_To_QListQString_(obj.value("顺序播放嵌套集合").toString());
	this->play_randomMax = obj.value("随机播放的次数上限").toString("1").toInt();

	// > 杂项
	this->note = obj.value("备注").toString();
}