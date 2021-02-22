#include "stdafx.h"
#include "p_PlayingPart.h"

#include "Source/Utils/manager/GIFManager/s_GIFManager.h"

/*
-----==========================================================-----
		类：		放映区.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供动作元编辑功能。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_PlayingPart::P_PlayingPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数

	// > 编辑表格
	this->m_table_action = new P_RadioTable(ui.tableWidget_action);
	this->m_table_state = new P_RadioTable(ui.tableWidget_state);
	QJsonObject obj_config = QJsonObject();
	obj_config.insert("zeroFillCount", 2);
	obj_config.insert("rowHeight", 22);
	this->m_table_action->setConfigParam(obj_config);	//固定参数
	this->m_table_state->setConfigParam(obj_config);	

	//-----------------------------------
	//----初始化ui


	//-----------------------------------
	//----事件绑定


}

P_PlayingPart::~P_PlayingPart(){
}

/*-------------------------------------------------
		放映区 - 开始播放
*/
void P_PlayingPart::openPlay() {
	//S_GIFManager::getInstance()->dismantlingGIF_testApi();
	
}

/*-------------------------------------------------
		控件 - 获取状态元名称
*/
QStringList P_PlayingPart::getStateNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		result.append(this->local_stateDataList.at(i).value("状态元名称").toString());
	}
	return result;
}
/*-------------------------------------------------
		控件 - 获取动作元名称
*/
QStringList P_PlayingPart::getActionNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		result.append(this->local_actionDataList.at(i).value("动作元名称").toString());
	}
	return result;
}

/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_PlayingPart::setSource(QList<QJsonObject> stateDataList, QList<QJsonObject> actionDataList){
	this->local_stateDataList = stateDataList;
	this->local_actionDataList = actionDataList;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_PlayingPart::setDefaultStateData(QStringList defaultStateList) {
	this->local_defaultStateList = defaultStateList;
	ui.label_defaultState->setText(this->local_defaultStateList.join("\n"));
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QStringList P_PlayingPart::getDefaultStateData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_defaultStateList;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_PlayingPart::putDataToUi() {

	this->m_table_action->setSource(this->getActionNameList());
	this->m_table_state->setSource(this->getStateNameList());
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_PlayingPart::putUiToData() {
	

}
