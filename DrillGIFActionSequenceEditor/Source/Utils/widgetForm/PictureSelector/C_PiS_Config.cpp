#include "stdafx.h"
#include "C_PiS_Config.h"

/*
-----==========================================================-----
		类：		图片选择块 配置类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		控件块的数据配置类。
					
		说明：		尺寸可以修改，但是由于每次修改config，都需要重建贴图和bitmap，系统速度会变慢。
-----==========================================================-----
*/

C_PiS_Config::C_PiS_Config(){

	// > 零填充
	this->m_zeroFill = false;
	this->m_zeroFillCount = 3;
	this->m_zeroFillChar = '0';

	// > 控件
	this->m_isMaskEnabled = true;
	this->m_isMaskBorderEnabled = true;

	this->m_isMultiSelect = false;

	// > 尺寸
	this->m_size_mode = "中";
	this->m_size_customHeight = 120;
}
C_PiS_Config::~C_PiS_Config(){
}



/*-------------------------------------------------
		尺寸 - 设置模式
*/
void C_PiS_Config::setSize_Mode(QString sizeMode){
	if (sizeMode == ""){ return; }
	if (this->m_size_mode == sizeMode){ return; }
	this->m_size_mode = sizeMode;
}
void C_PiS_Config::setSize_Custom(int customHeight){
	this->m_size_mode = "自定义";
	this->m_size_customHeight = customHeight;
}
/*-------------------------------------------------
		尺寸 - 获取模式
*/
QString C_PiS_Config::getSize_Mode(){
	return this->m_size_mode;
}

/*-------------------------------------------------
		尺寸 - 获取单块宽度
*/
int C_PiS_Config::getSize_ItemWidth(){
	return this->getSize_ItemHeight();
}
/*-------------------------------------------------
		尺寸 - 获取单块高度
*/
int C_PiS_Config::getSize_ItemHeight(){
	int result = 0;
	if (this->m_size_mode == "大"){
		result = 200;
	}else if (this->m_size_mode == "中"){
		result = 120;
	}else if (this->m_size_mode == "小"){
		result = 80;
	}else{
		result = this->m_size_customHeight;
	}
	return result;
}
/*-------------------------------------------------
		尺寸 - 获取父对象高度
*/
int C_PiS_Config::getSize_ParentHeight(){
	return this->getSize_ItemHeight() + 28;		//（固定28像素的距离，用于放滚动条）
}

/*-------------------------------------------------
		尺寸 - 获取换行字符串（\n 撑开选中的 蓝色方块）
*/
QString C_PiS_Config::getSize_LineExpand(){
	QString result = "　";
	int count = qFloor(this->getSize_ItemHeight() / 16);
	for (int i = 0; i < count; i++){
		result += "\n　";
	}
	return result;
}


/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_PiS_Config::getJsonObject(){
	QJsonObject obj;

	// > 零填充
	obj.insert("m_zeroFill", this->m_zeroFill);
	obj.insert("m_zeroFillCount", this->m_zeroFillCount);
	obj.insert("m_zeroFillChar", QString(this->m_zeroFillChar));

	// > 控件
	obj.insert("m_isMaskEnabled", this->m_isMaskEnabled);
	obj.insert("m_isMaskBorderEnabled", this->m_isMaskBorderEnabled);
	//（不存 多选开关）

	// > 尺寸
	obj.insert("m_size_mode", this->m_size_mode);
	obj.insert("m_size_customHeight", this->m_size_customHeight);
	
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_PiS_Config::setJsonObject(QJsonObject obj){

	// > 零填充
	if (obj.value("m_zeroFill").isUndefined() == false){ this->m_zeroFill = obj.value("m_zeroFill").toBool(); }
	if (obj.value("m_zeroFillCount").isUndefined() == false){ this->m_zeroFillCount = obj.value("m_zeroFillCount").toInt(); }
	if (obj.value("m_zeroFillChar").isUndefined() == false){ this->m_zeroFillChar = obj.value("m_zeroFillChar").toString().at(0); }
	
	// > 控件
	if (obj.value("m_isMaskEnabled").isUndefined() == false){ this->m_isMaskEnabled = obj.value("m_isMaskEnabled").toBool(); }
	if (obj.value("m_isMaskBorderEnabled").isUndefined() == false){ this->m_isMaskBorderEnabled = obj.value("m_isMaskBorderEnabled").toBool(); }
	//（不存 多选开关）

	// > 尺寸
	if (obj.value("m_size_mode").isUndefined() == false){ this->m_size_mode = obj.value("m_size_mode").toString(); }
	if (obj.value("m_size_customHeight").isUndefined() == false){ this->m_size_customHeight = obj.value("m_size_customHeight").toInt(); }
}