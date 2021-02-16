#include "stdafx.h"
#include "p_ActionPart.h"

/*
-----==========================================================-----
		类：		动作元块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供动作元编辑功能。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_ActionPart::P_ActionPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数

	//-----------------------------------
	//----初始化ui
	this->m_table = new P_RadioTable(ui.tableWidget);
	QJsonObject obj_config = QJsonObject();
	obj_config.insert("zeroFillCount", 2);
	obj_config.insert("rowHeight", 22);
	this->m_table->setConfigParam(obj_config);	//固定参数

	//-----------------------------------
	//----事件绑定
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_ActionPart::currentIndexChanged);

}

P_ActionPart::~P_ActionPart(){
}

/*-------------------------------------------------
		控件 - 获取动作元名称
*/
QStringList P_ActionPart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		result.append(this->local_actionDataList.at(i).value("动作元名称").toString());
	}
	return result;
}

/*-------------------------------------------------
		控件 - 动作元切换
*/
void P_ActionPart::currentIndexChanged(int index){

	// > 旧的内容存储


	// > 填入新的内容
	ui.lineEdit_name->setText(this->getNameList().at(index));
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_ActionPart::setData(QList<QJsonObject> actionData) {
	this->local_actionDataList = actionData;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QList<QJsonObject> P_ActionPart::getData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_actionDataList;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_ActionPart::putDataToUi() {
	
	this->m_table->setSource(this->getNameList());

}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_ActionPart::putUiToData() {
	

}
