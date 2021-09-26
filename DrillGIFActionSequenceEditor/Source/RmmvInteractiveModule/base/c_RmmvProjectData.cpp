#include "stdafx.h"
#include "C_RmmvProjectData.h"

/*
-----==========================================================-----
		类：		rmmv工程数据 实体类.cpp
		版本：		v1.01
		所属模块：	交互模块
		功能：		工程基本信息数据。
-----==========================================================-----
*/
C_RmmvProjectData::C_RmmvProjectData(){
	this->name = "";			//工程名
	this->version = "";			//工程版本
	this->path = "";			//工程根路径

	this->optional_backup = true;
}
C_RmmvProjectData::~C_RmmvProjectData(){
}


/*-------------------------------------------------
		数据 - 解析工程文件
*/
void C_RmmvProjectData::initFromFile(QString full_filePath){
	QString dir_path = QFileInfo(full_filePath).absolutePath();		//获取根目录

	// > 读取工程文件
	QFile project_file(full_filePath);
	if (!project_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "未找到Game.rpgproject文件。", QMessageBox::Yes);
		return;
	}
	QString version = project_file.readAll();
	project_file.close();

	// > 读取index文件
	QFile index_file(dir_path + "/index.html");
	if (!index_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "错误", "未找到index.html文件。", QMessageBox::Yes);
		return;
	}
	QString index_context = index_file.readAll();
	index_file.close();

	// > 解析出名称
	int i_a = index_context.indexOf("<title>") + 7;
	int i_b = index_context.indexOf("</title>", i_a);
	this->name = index_context.mid(i_a, i_b - i_a);
	this->version = version;
	this->path = dir_path;
}
/*-------------------------------------------------
		数据 - 获取工程名称
*/
QString C_RmmvProjectData::getName(){
	return this->name;
}
/*-------------------------------------------------
		数据 - 获取工程版本
*/
QString C_RmmvProjectData::getVersion(){
	return this->version;
}
/*-------------------------------------------------
		数据 - 获取工程根路径
*/
QString C_RmmvProjectData::getRootPath(){
	return this->path;
}
/*-------------------------------------------------
		数据 - 判断工程是否存在
*/
bool C_RmmvProjectData::isProjectExist(){
	if (QFileInfo(this->path).exists() == false){ return false; }
	if (QFileInfo(this->path + "/index.html").exists() == false){ return false; }
	return true;
}

/*-------------------------------------------------
		空判断
*/
bool C_RmmvProjectData::isNull(){
	return this->name == "" || this->path == "";
}
/*-------------------------------------------------
		运算符重载
*/
const bool C_RmmvProjectData::operator== (const C_RmmvProjectData& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_RmmvProjectData::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("version", this->version); 
	obj.insert("path", this->path);
	obj.insert("optional_backup", this->optional_backup);
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_RmmvProjectData::setJsonObject(QJsonObject obj){
	this->name = obj.value("name").toString();
	this->version = obj.value("version").toString();
	this->path = obj.value("path").toString();
	this->optional_backup = obj.value("optional_backup").toBool();
}
