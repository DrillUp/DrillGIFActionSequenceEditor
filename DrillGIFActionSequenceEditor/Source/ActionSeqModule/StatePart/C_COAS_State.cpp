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
			【所有 列表/结构体 都需要转成JSON字符串，不能直接存QJsonObject/QJsonArray】
*/
QJsonObject C_COAS_State::getJsonObject_Chinese(){
	QJsonObject obj = QJsonObject();
	//obj.insert("id", this->id);

	// > 空对象情况（直接返回，节约存储空间）
	if (this->name == "" && this->gif_src.count() == 0){
		return obj;
	}

	// > 常规
	obj.insert("状态元名称", this->name);
	obj.insert("状态元标签", TTool::_JSON_stringify_(this->tag_tank));
	obj.insert("状态元优先级", QString::number(this->priority));
	obj.insert("状态元权重", QString::number(this->proportion));
	obj.insert("可被动作元打断", this->canBeInterrupted ? "true" : "false");

	// > GIF
	obj.insert("资源-状态元", TTool::_JSON_stringify_(this->gif_src));
	//		（资源文件夹，不需赋值）
	QList<QString> gif_intervalTank_strList = TTool::_QList_IntToQString_(this->gif_intervalTank);
	obj.insert("帧间隔-明细表", TTool::_JSON_stringify_(gif_intervalTank_strList));
	obj.insert("帧间隔", QString::number(this->gif_interval));
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
			【所有 列表/结构体 在读取时，要先转字符串，再解析为可用对象】
*/
void C_COAS_State::setJsonObject_Chinese(QJsonObject obj, int id){
	this->id = id;

	// > 常规
	this->name = obj.value("状态元名称").toString();
	this->tag_tank = TTool::_JSON_parse_To_QListQString_(obj.value("状态元标签").toString());
	this->priority = obj.value("状态元优先级").toString("0").toInt();
	this->proportion = obj.value("状态元权重").toString("40").toInt();
	this->canBeInterrupted = obj.value("可被动作元打断").toString() == "true";

	// > GIF
	QString gif_src_str = obj.value("资源-状态元").toString();
	this->gif_src = TTool::_JSON_parse_To_QListQString_(gif_src_str);			//资源文件名
	//		（资源文件夹，不需赋值）
	QString gif_intervalTank_str = obj.value("帧间隔-明细表").toString();
	QList<QString> gif_intervalTank_strList = TTool::_JSON_parse_To_QListQString_(gif_intervalTank_str);	//帧间隔-明细表
	this->gif_intervalTank = TTool::_QList_QStringToInt_(gif_intervalTank_strList);
	this->gif_interval = obj.value("帧间隔").toString("4").toInt();
	this->gif_back_run = obj.value("是否倒放").toString("false") == "true";
	this->gif_preload = obj.value("是否预加载").toString("false") == "true";

	// > 图像
	this->tint = obj.value("图像-色调值").toString("0").toInt();
	this->smooth = obj.value("图像-模糊边缘").toString("false") == "true";

	// > 杂项
	this->note = obj.value("备注").toString();
}