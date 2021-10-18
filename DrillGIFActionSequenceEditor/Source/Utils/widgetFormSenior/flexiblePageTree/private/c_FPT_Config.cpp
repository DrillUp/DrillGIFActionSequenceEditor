#include "stdafx.h"
#include "c_FPT_Config.h"

#include "../p_FlexiblePageTree.h"
#include "Source/Utils/manager/chineseManager/S_ChineseManager.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		������ ������.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���������� ������ �����ࡣ
					
		�ӹ��ܣ�
					->�ն���
					
-----==========================================================-----
*/
C_FPT_Config::C_FPT_Config(){

	// > ��ʾ����
	this->rowHeight = 30;
	this->zeroFill = true;
	this->zeroFillCount = 4;
	this->zeroFillChar = '0';

	// > ��֧ģʽ
	this->m_mode = "ID��֧����id��������";
	this->m_modeList = QStringList() << "ID��֧����id��������" << "���Ʒ�֧�������Ƶ�������";

	// > IDҳ
	this->pagePerNum = 30;

	// > �Զ�������
	this->data = QJsonObject();

	// > ˽����ʱ����
	this->m_id_maxCount = -1;

	//  �������б� - s_ChineseManager����ĸ���壩
	this->m_name_textList = QStringList() << "- A -" << "- B -" << "- C -" << "- D -" << "- E -" << "- F -" << "- G -" << "- H -" << "- I -" << "- J -" << "- K -" << "- L -" << "- M -" << "- N -" << "- O -" << "- P -" << "- Q -" << "- R -" << "- S -" << "- T -" << "- U -" << "- V -" << "- W -" << "- X -" << "- Y -" << "- Z -" << "��Ƨ��";
	this->m_name_symbolList = QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z" << "@";
	//  �������б� - ������������
	this->m_name_textList << "��������" << "������";
	this->m_name_symbolList << "��������" << "������";
}
C_FPT_Config::~C_FPT_Config(){
}


/*-------------------------------------------------
		��֧ģʽ - ���õ�ǰģʽ
*/
void C_FPT_Config::setCurrentMode(QString sortMode){
	this->m_mode = sortMode;
}
/*-------------------------------------------------
		��֧ģʽ - ��ȡ��ǰģʽ
*/
QString C_FPT_Config::getCurrentMode(){
	return this->m_mode;
}
bool C_FPT_Config::is_id_Mode(){
	return this->m_mode == "ID��֧����id��������";
}
bool C_FPT_Config::is_name_Mode(){
	return this->m_mode == "���Ʒ�֧�������Ƶ�������";
}
/*-------------------------------------------------
		��֧ģʽ - ����ģʽ�б�
*/
void C_FPT_Config::setModeList(QStringList sortModeList){
	this->m_modeList = sortModeList;
}
/*-------------------------------------------------
		��֧ģʽ - ��ȡģʽ�б�
*/
QStringList C_FPT_Config::getModeList(){
	return this->m_modeList;
}



/*-------------------------------------------------
		IDҳ - �������ֵ
*/
void C_FPT_Config::set_id_MaxCount(int count){
	this->m_id_maxCount = count;
}
/*-------------------------------------------------
		IDҳ - ��ȡ���ֵ
*/
int C_FPT_Config::get_id_MaxCount(){
	return this->m_id_maxCount;
}
/*-------------------------------------------------
		IDҳ - ��ȡҳ��
*/
int C_FPT_Config::get_id_PageCount(){
	if (this->m_id_maxCount <= 0){ return -1; }
	return qCeil((double)this->m_id_maxCount / this->pagePerNum);
}
/*-------------------------------------------------
		IDҳ - ��ȡ��
*/
int C_FPT_Config::get_id_Bottom(int page_index){
	int bottom = page_index * this->pagePerNum + 1;
	if (this->m_id_maxCount < 0){ return -1; }
	return bottom;
}
/*-------------------------------------------------
		IDҳ - ��ȡ��
*/
int C_FPT_Config::get_id_Top(int page_index){
	int top = (page_index + 1) * this->pagePerNum;
	if (this->m_id_maxCount < 0){ return -1; }

	if (top >= this->m_id_maxCount){
		top = this->m_id_maxCount;
	}
	return top;
}
/*-------------------------------------------------
		IDҳ - ��ȡ��֦���ƣ�����ҳ������
*/
QString C_FPT_Config::get_id_PageNameByIndex(int page_index){
	int bottom = this->get_id_Bottom(page_index);
	int top = this->get_id_Top(page_index);
	if (bottom == -1){ return ""; }
	if (top == -1){ return ""; }

	QString page_name = "";
	page_name = "��" + S_ChineseManager::getInstance()->toChineseNumLower(page_index + 1) + "ҳ ";

	// > id���
	if (this->zeroFill){
		page_name += TTool::_zeroFill_(bottom, this->zeroFillCount, QLatin1Char(this->zeroFillChar.toLatin1()))
			+ "��" + TTool::_zeroFill_(top, this->zeroFillCount, QLatin1Char(this->zeroFillChar.toLatin1()));
	}else{
		page_name += QString::number(bottom) + "��" + QString::number(top);
	}
	return page_name;
}
/*-------------------------------------------------
		IDҳ - ��ȡID��ʽ������
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
		����ҳ - ��ȡҳ��
*/
int C_FPT_Config::get_name_PageCount(){
	return this->m_name_textList.count();
}
/*-------------------------------------------------
		����ҳ - ��ȡ�����б�
*/
QStringList C_FPT_Config::get_name_PageNameList(){
	return this->m_name_textList;
}
/*-------------------------------------------------
		����ҳ - ��ȡ��ʶ�б�
*/
QStringList C_FPT_Config::get_name_PageSymbolList(){
	return this->m_name_symbolList;
}


/*-------------------------------------------------
		�Զ������� - ��ȡ����
*/
QJsonObject C_FPT_Config::getCustomData(){
	return this->data;
}

/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_FPT_Config::getJsonObject(){
	QJsonObject obj = QJsonObject();

	// > ��ʾ����
	obj.insert("rowHeight", this->rowHeight);
	obj.insert("zeroFill", this->zeroFill);
	obj.insert("zeroFillCount", this->zeroFillCount);
	obj.insert("zeroFillChar", QString(this->zeroFillChar));

	// > ��֧ģʽ
	obj.insert("sortType", this->m_mode);
	//��m_sortModeList���ڼ̳п��ƣ����棩

	// > IDҳ
	obj.insert("pagePerNum", this->pagePerNum);

	// > �Զ�������
	obj.insert("data", this->data);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_FPT_Config::setJsonObject(QJsonObject obj, P_FlexiblePageTree* parent_obj){

	// > ��ʾ����
	if (obj.value("rowHeight").isUndefined() == false){ this->rowHeight = obj.value("rowHeight").toInt(); }
	if (obj.value("zeroFill").isUndefined() == false){ this->zeroFill = obj.value("zeroFill").toBool(); }
	if (obj.value("zeroFillCount").isUndefined() == false){ this->zeroFillCount = obj.value("zeroFillCount").toInt(); }
	if (obj.value("zeroFillChar").isUndefined() == false){ this->zeroFillChar = obj.value("zeroFillChar").toString().at(0); }

	// > ��֧ģʽ
	if (obj.value("sortType").isUndefined() == false){ this->m_mode = obj.value("sortType").toString(); }
	//��m_sortModeList���ڼ̳п��ƣ����棩

	// > IDҳ
	if (obj.value("pagePerNum").isUndefined() == false){ this->pagePerNum = obj.value("pagePerNum").toInt(); }

	// > �Զ�������
	if (obj.value("data").isUndefined() == false){ this->data = obj.value("data").toObject(); }
}