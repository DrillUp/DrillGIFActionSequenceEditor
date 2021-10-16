#include "stdafx.h"
#include "c_FPT_Config.h"

#include "../p_FlexiblePageTree.h"
#include "Source/Utils/manager/chineseManager/S_ChineseManager.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		树设置 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		灵活分类树的 树设置 数据类。
					
		子功能：
					->空对象
					
-----==========================================================-----
*/
C_FPT_Config::C_FPT_Config(){

	// > 显示设置
	this->rowHeight = 30;
	this->zeroFill = true;
	this->zeroFillCount = 4;
	this->zeroFillChar = '0';

	// > 分支模式
	this->m_mode = "ID分支（按id递增排序）";
	this->m_modeList = QStringList() << "ID分支（按id递增排序）" << "名称分支（按名称递增排序）";

	// > ID页
	this->pagePerNum = 30;

	// > 自定义数据
	this->data = QJsonObject();

	// > 私有临时对象
	this->m_id_maxCount = -1;

	//  （名称列表 - s_ChineseManager首字母定义）
	this->m_name_textList = QStringList() << "- A -" << "- B -" << "- C -" << "- D -" << "- E -" << "- F -" << "- G -" << "- H -" << "- I -" << "- J -" << "- K -" << "- L -" << "- M -" << "- N -" << "- O -" << "- P -" << "- Q -" << "- R -" << "- S -" << "- T -" << "- U -" << "- V -" << "- W -" << "- X -" << "- Y -" << "- Z -" << "生僻字";
	this->m_name_symbolList = QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z" << "@";
	//  （名称列表 - 特殊符号情况）
	this->m_name_textList << "其他符号" << "空名称";
	this->m_name_symbolList << "其他符号" << "空名称";
}
C_FPT_Config::~C_FPT_Config(){
}


/*-------------------------------------------------
		分支模式 - 设置当前模式
*/
void C_FPT_Config::setCurrentMode(QString sortMode){
	this->m_mode = sortMode;
}
/*-------------------------------------------------
		分支模式 - 获取当前模式
*/
QString C_FPT_Config::getCurrentMode(){
	return this->m_mode;
}
bool C_FPT_Config::is_id_Mode(){
	return this->m_mode == "ID分支（按id递增排序）";
}
bool C_FPT_Config::is_name_Mode(){
	return this->m_mode == "名称分支（按名称递增排序）";
}
/*-------------------------------------------------
		分支模式 - 设置模式列表
*/
void C_FPT_Config::setModeList(QStringList sortModeList){
	this->m_modeList = sortModeList;
}
/*-------------------------------------------------
		分支模式 - 获取模式列表
*/
QStringList C_FPT_Config::getModeList(){
	return this->m_modeList;
}



/*-------------------------------------------------
		ID页 - 设置最大值
*/
void C_FPT_Config::set_id_MaxCount(int count){
	this->m_id_maxCount = count;
}
/*-------------------------------------------------
		ID页 - 获取最大值
*/
int C_FPT_Config::get_id_MaxCount(){
	return this->m_id_maxCount;
}
/*-------------------------------------------------
		ID页 - 获取页数
*/
int C_FPT_Config::get_id_PageCount(){
	if (this->m_id_maxCount <= 0){ return -1; }
	return qCeil((double)this->m_id_maxCount / this->pagePerNum);
}
/*-------------------------------------------------
		ID页 - 获取底
*/
int C_FPT_Config::get_id_Bottom(int page_index){
	int bottom = page_index * this->pagePerNum + 1;
	if (this->m_id_maxCount < 0){ return -1; }
	return bottom;
}
/*-------------------------------------------------
		ID页 - 获取顶
*/
int C_FPT_Config::get_id_Top(int page_index){
	int top = (page_index + 1) * this->pagePerNum;
	if (this->m_id_maxCount < 0){ return -1; }

	if (top >= this->m_id_maxCount){
		top = this->m_id_maxCount - 1;
	}
	return top;
}
/*-------------------------------------------------
		ID页 - 获取树枝名称（根据页索引）
*/
QString C_FPT_Config::get_id_PageNameByIndex(int page_index){
	int bottom = this->get_id_Bottom(page_index);
	int top = this->get_id_Top(page_index);
	if (bottom == -1){ return ""; }
	if (top == -1){ return ""; }

	QString page_name = "";
	page_name = "第" + S_ChineseManager::getInstance()->toChineseNumLower(page_index + 1) + "页 ";

	// > id编号
	if (this->zeroFill){
		page_name += TTool::_zeroFill_(bottom, this->zeroFillCount, QLatin1Char(this->zeroFillChar.toLatin1()))
			+ "至" + TTool::_zeroFill_(top, this->zeroFillCount, QLatin1Char(this->zeroFillChar.toLatin1()));
	}else{
		page_name += QString::number(bottom) + "至" + QString::number(top);
	}
	return page_name;
}
/*-------------------------------------------------
		ID页 - 获取ID格式化名称
*/
QString C_FPT_Config::get_id_FormatedId(int id){
	QString result = "";
	if (this->zeroFill){
		result = TTool::_zeroFill_(id, this->zeroFillCount, QLatin1Char(this->zeroFillChar.toLatin1()));
	}else{
		result = QString::number(id);
	}
	return result;
}


/*-------------------------------------------------
		名称页 - 获取页数
*/
int C_FPT_Config::get_name_PageCount(){
	return this->m_name_textList.count();
}
/*-------------------------------------------------
		名称页 - 获取名称列表
*/
QStringList C_FPT_Config::get_name_PageNameList(){
	return this->m_name_textList;
}
/*-------------------------------------------------
		名称页 - 获取标识列表
*/
QStringList C_FPT_Config::get_name_PageSymbolList(){
	return this->m_name_symbolList;
}


/*-------------------------------------------------
		自定义数据 - 获取数据
*/
QJsonObject C_FPT_Config::getCustomData(){
	return this->data;
}

/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_FPT_Config::getJsonObject(){
	QJsonObject obj = QJsonObject();

	// > 显示设置
	obj.insert("rowHeight", this->rowHeight);
	obj.insert("zeroFill", this->zeroFill);
	obj.insert("zeroFillCount", this->zeroFillCount);
	obj.insert("zeroFillChar", QString(this->zeroFillChar));

	// > 分支模式
	obj.insert("sortType", this->m_mode);
	//（m_sortModeList用于继承控制，不存）

	// > ID页
	obj.insert("pagePerNum", this->pagePerNum);

	// > 自定义数据
	obj.insert("data", this->data);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_FPT_Config::setJsonObject(QJsonObject obj, P_FlexiblePageTree* parent_obj){

	// > 显示设置
	if (obj.value("rowHeight").isUndefined() == false){ this->rowHeight = obj.value("rowHeight").toInt(); }
	if (obj.value("zeroFill").isUndefined() == false){ this->zeroFill = obj.value("zeroFill").toBool(); }
	if (obj.value("zeroFillCount").isUndefined() == false){ this->zeroFillCount = obj.value("zeroFillCount").toInt(); }
	if (obj.value("zeroFillChar").isUndefined() == false){ this->zeroFillChar = obj.value("zeroFillChar").toString().at(0); }

	// > 分支模式
	if (obj.value("sortType").isUndefined() == false){ this->m_mode = obj.value("sortType").toString(); }
	//（m_sortModeList用于继承控制，不存）

	// > ID页
	if (obj.value("pagePerNum").isUndefined() == false){ this->pagePerNum = obj.value("pagePerNum").toInt(); }

	// > 自定义数据
	if (obj.value("data").isUndefined() == false){ this->data = obj.value("data").toObject(); }
}