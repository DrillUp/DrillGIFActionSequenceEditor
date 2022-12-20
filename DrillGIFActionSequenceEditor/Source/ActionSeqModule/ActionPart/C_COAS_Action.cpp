#include "stdafx.h"
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
void C_COAS_Action::clearTankData(){

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
QJsonObject C_COAS_Action::getJsonObject(){
	QJsonObject obj = QJsonObject();

	//obj.insert("id", this->id);

	obj.insert("动作元名称", this->name);
	obj.insert("动作元标签", TTool::_JSON_stringify_(this->m_curTagTank));
	obj.insert("动作元优先级", QString::number(ui.spinBox_priority->value()));
	obj.insert("是否倒放", ui.checkBox_gif_back_run->isChecked() ? "true" : "false");
	obj.insert("是否预加载", ui.checkBox_gif_preload->isChecked() ? "true" : "false");
	obj.insert("图像-色调值", QString::number(ui.horizontalSlider_tint->value()));
	obj.insert("图像-模糊边缘", ui.checkBox_smooth->isChecked() ? "true" : "false");
	obj.insert("备注", ui.plainTextEdit_note->toPlainText());

	obj.insert("帧间隔", this->gif_interval);
	//（资源文件夹不需要）
	obj.insert("资源-动作元", TTool::_JSON_stringify_(this->gif_src));
	QList<QString> gif_intervalTank_strList = TTool::_QList_IntToQString_(this->gif_intervalTank);
	obj.insert("帧间隔-明细表", TTool::_JSON_stringify_(gif_intervalTank_strList));

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_COAS_Action::setJsonObject(QJsonObject obj, int i){

	this->id = i;
	this->name = obj.value("name").toString();

	ui.lineEdit_name->setText(obj_data.value("动作元名称").toString());
	this->m_curTagTank = TTool::_JSON_parse_To_QListQString_(obj_data.value("动作元标签").toString());
	this->refreshTagTank();
	ui.spinBox_priority->setValue(obj_data.value("动作元优先级").toString().toInt());
	ui.checkBox_gif_back_run->setChecked(obj_data.value("是否倒放").toString() == "true");
	ui.checkBox_gif_preload->setChecked(obj_data.value("是否预加载").toString() == "true");
	ui.horizontalSlider_tint->setValue(obj_data.value("图像-色调值").toString().toInt());
	ui.checkBox_smooth->setChecked(obj_data.value("图像-模糊边缘").toString() == "true");
	ui.plainTextEdit_note->setPlainText(obj_data.value("备注").toString());

	// > 图片数据
	int gif_interval = obj_data.value("帧间隔").toString().toInt();								//帧间隔
	QString gif_src_file = S_ActionSeqDataContainer::getInstance()->getActionSeqDir();			//资源文件夹

	QString gif_src_str = obj_data.value("资源-动作元").toString();
	QList<QString> gif_src = TTool::_JSON_parse_To_QListQString_(gif_src_str);			//资源文件名
	QString gif_intervalTank_str = obj_data.value("帧间隔-明细表").toString();
	QList<QString> gif_intervalTank_strList = TTool::_JSON_parse_To_QListQString_(gif_intervalTank_str);	//帧间隔-明细表
	QList<int> gif_intervalTank = TTool::_QList_QStringToInt_(gif_intervalTank_strList);
	
}