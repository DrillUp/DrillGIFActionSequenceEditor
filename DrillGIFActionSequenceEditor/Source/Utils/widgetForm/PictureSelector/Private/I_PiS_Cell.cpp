#include "stdafx.h"
#include "I_PiS_Cell.h"

/*
-----==========================================================-----
		类：		图片选择块 单元胞.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		图片选择块的元胞对象。
-----==========================================================-----
*/

I_PiS_Cell::I_PiS_Cell(){

	// > 数据
	this->index_Title = -1;
	this->index_OrgBitmap = -1;

	// > 控件
	this->item = nullptr;
	this->widget = nullptr;
}
I_PiS_Cell::~I_PiS_Cell(){
}


/*-------------------------------------------------
		控件 - 获取贴图
*/
QPixmap I_PiS_Cell::getQPixmap(QList<QPixmap>* org_bitmapList){
	if (org_bitmapList->count() == 0){ return QPixmap(); }
	if (org_bitmapList->count() <= this->index_OrgBitmap ){ return QPixmap(); }
	return org_bitmapList->at(this->index_OrgBitmap);
}


/*-------------------------------------------------
		空判断
*/
bool I_PiS_Cell::isNull(){
	if (this->index_Title == -1){ return true; }
	if (this->index_OrgBitmap == -1){ return true; }
	return false;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject I_PiS_Cell::getJsonObject(){
	QJsonObject obj;

	// > 数据
	obj.insert("index_Title", this->index_Title);
	obj.insert("index_OrgBitmap", this->index_OrgBitmap);
	
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void I_PiS_Cell::setJsonObject(QJsonObject obj){

	// > 数据
	this->index_Title = obj.value("index_Title").toInt(-1);
	this->index_OrgBitmap = obj.value("index_OrgBitmap").toInt(-1);

}