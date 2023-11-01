#include "stdafx.h"
#include "C_PLE_DataSet.h"

/*
-----==========================================================-----
		类：		图片编辑块 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		图片编辑块 的数据类。
					【该数据类只用于传值，所以不需要 ID或名称 的标识】
-----==========================================================-----
*/

C_PLE_DataSet::C_PLE_DataSet(){

	// > 资源文件
	this->picPath_list.clear();

	// > 父路径（全局数据）
	this->pic_parentDir = "";
}
C_PLE_DataSet::~C_PLE_DataSet(){
}


/*-------------------------------------------------
		主流程 - 设置资源
*/
void C_PLE_DataSet::setPicList(QString pic_file, QStringList pic_name_list){
	pic_file = QDir(pic_file).absolutePath();	//（去掉末尾的"/"）

	// > 设置父路径
	this->setData_ParentDir(pic_file);

	// > 设置资源
	this->picPath_list.clear();
	for (int i = 0; i < pic_name_list.count(); i++){
		QString path;
		path.append(pic_file);
		path.append("/");
		path.append(pic_name_list.at(i));
		path.append(".png");
		this->picPath_list.append(path);
	}
}


/*-------------------------------------------------
		资源文件 - 设置
*/
void C_PLE_DataSet::setData_PicList(QList<QString> picPath_list){
	this->picPath_list = picPath_list;
}
/*-------------------------------------------------
		资源文件 - 获取
*/
QList<QString> C_PLE_DataSet::getData_PicList(){
	return this->picPath_list;
}
int C_PLE_DataSet::getData_PicListCount(){
	return this->picPath_list.count();
}
/*-------------------------------------------------
		资源文件 - 获取 - 根据索引
*/
QString C_PLE_DataSet::getPic_ByIndex(int index){
	if (index < 0){ return ""; }
	if (index >= this->picPath_list.count()){ return ""; }
	return this->picPath_list.at(index);
}
/*-------------------------------------------------
		资源文件 - 获取 - 根据索引列表
*/
QList<QString> C_PLE_DataSet::getPic_ByIndexList(QList<int> index_list){
	QList<QString> result_list;
	for (int i = 0; i < index_list.count(); i++){
		int index = index_list.at(i);
		QString pic = this->getPic_ByIndex(index);
		result_list.append(pic);
	}
	return result_list;
}
/*-------------------------------------------------
		资源文件 - 获取 - 全部文件是否在同一文件夹
*/
bool C_PLE_DataSet::isPicList_AllInSameDir(){
	if (this->picPath_list.count() == 0){ return false; }

	QFileInfo fileinfo = QFileInfo(this->picPath_list.first());
	QString dir = fileinfo.absolutePath();
	for (int i = 1; i < this->picPath_list.count(); i++){
		QFileInfo fileinfo2 = QFileInfo(this->picPath_list.first());
		QString dir2 = fileinfo2.absolutePath();
		if (dir != dir2){
			return false;
		}
	}
	return true;
}
/*-------------------------------------------------
		资源文件 - 获取 - 父文件目录
*/
QString C_PLE_DataSet::getPicList_ParentDir(){

	// > 不在同一文件夹，返回空字符串
	if (this->isPicList_AllInSameDir() == false){ return ""; }
	
	// > 父文件目录
	QFileInfo fileinfo = QFileInfo(this->picPath_list.first());
	QString dir = fileinfo.absolutePath();
	return dir;
}


/*-------------------------------------------------
		父路径 - 设置
*/
void C_PLE_DataSet::setData_ParentDir(QString parentDir){
	this->pic_parentDir = QDir(parentDir).absolutePath();
}
/*-------------------------------------------------
		父路径 - 获取
*/
QString C_PLE_DataSet::getData_ParentDir(){
	return this->pic_parentDir;
}
/*-------------------------------------------------
		父路径 - 资源中是否有文件名
*/
bool C_PLE_DataSet::hasPicName(QString pic_filename){
	for (int i = 0; i < this->picPath_list.count(); i++){
		QFileInfo fileinfo = QFileInfo(this->picPath_list.first());
		if (fileinfo.completeBaseName() == pic_filename){
			return true;
		}
	}
	return false;
}


/*-------------------------------------------------
		空判断
*/
bool C_PLE_DataSet::isNull(){
	return this->picPath_list.count() == 0;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_PLE_DataSet::getJsonObject(){
	QJsonObject obj;

	// > 资源文件
	QJsonArray arr;
	for (int i = 0; i < this->picPath_list.count(); i++){
		arr.append(this->picPath_list.at(i));
	}
	obj.insert("picPath_list", arr);

	// > 父路径（全局数据）
	obj.insert("pic_parentDir", this->pic_parentDir);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_PLE_DataSet::setJsonObject(QJsonObject obj){

	// > 资源文件
	if (obj.value("picPath_list").isUndefined() == false){
		this->picPath_list.clear();
		QJsonArray arr = obj.value("picPath_list").toArray();
		for (int i = 0; i < arr.count(); i++){
			this->picPath_list.append(arr.at(i).toString());
		}
	}

	// > 父路径（全局数据）
	this->pic_parentDir = obj.value("pic_parentDir").toString();

}