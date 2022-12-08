#include "stdafx.h"
#include "C_ProjectData.h"

/*
-----==========================================================-----
		类：		项目数据 实体类.cpp
		所属模块：	项目管理模块
		功能：		存储项目的基本内容的数据类。
-----==========================================================-----
*/
C_ProjectData::C_ProjectData(){
	this->name = "";														//项目名
	this->path = QCoreApplication::applicationDirPath() + "/workspace/";	//项目路径（含项目名 D:/aaa/vvv/项目名/）

	this->note = "";									//项目笔记（备注）
	this->createDate = QDateTime::currentDateTime();	//创建时间
	this->lastSaveDate = QDateTime::currentDateTime();	//上一次修改时间
}

C_ProjectData::~C_ProjectData(){
}

QString C_ProjectData::softname = "GIF动画序列编辑器";	//软件名
QString C_ProjectData::version = "v1.20";				//软件版本
QString C_ProjectData::version_serial = "";				//软件版本号

/*-------------------------------------------------
		空判断
*/
bool C_ProjectData::isNull(){
	return this->name == "";
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_ProjectData::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("url", this->path);
	obj.insert("note", this->note);
	obj.insert("createDate", this->createDate.toString("yyyy/MM/dd hh:mm:ss"));
	obj.insert("lastSaveDate", this->lastSaveDate.toString("yyyy/MM/dd hh:mm:ss"));
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ProjectData::setJsonObject(QJsonObject obj){

	this->name = obj.value("name").toString();
	this->path = obj.value("url").toString();
	if (obj.value("note").isUndefined() == false){ this->note = obj.value("note").toString(); }
	if (obj.value("createDate").isUndefined() == false){ this->createDate = QDateTime::fromString( obj.value("createDate").toString(), "yyyy/MM/dd hh:mm:ss"); }
	if (obj.value("lastSaveDate").isUndefined() == false){ this->lastSaveDate = QDateTime::fromString(obj.value("lastSaveDate").toString(), "yyyy/MM/dd hh:mm:ss"); }

}
/*-------------------------------------------------
		获取项目全名
*/
QString C_ProjectData::getSoftname(){
	//return this->softname + "(" + this->version + "." + this->version_serial + ")";
	return this->softname + "(" + this->version + ")";
}

/*-------------------------------------------------
		获取项目名
*/
QString C_ProjectData::getName() {
	return this->name;
}
/*-------------------------------------------------
		获取项目后缀
*/
QString C_ProjectData::getSuffix() {
	return PROJECT_SUFFIX;
}
/*-------------------------------------------------
		设置项目名
*/
void C_ProjectData::setName(QString name) {
	this->name = name;
}
/*-------------------------------------------------
		设置项目路径
*/
void C_ProjectData::setPath(QString path) {
	this->path = path;
}
/*-------------------------------------------------
		获取项目父目录（D:/aaa/vvv/）
*/
QString C_ProjectData::getParentPath(){
	if (this->name == ""){
		return this->path;
	}
	else{
		if (this->path.at(this->path.length()-1) == '/'){
			int pos = this->path.mid(0, this->path.length() - 1).lastIndexOf("/");
			return this->path.mid(0, pos);
		}
		else{
			int pos = this->path.lastIndexOf("/");
			return this->path.mid(0, pos);
		}
	}
}
/*-------------------------------------------------
		获取项目路径（D:/aaa/vvv/项目名/）
*/
QString C_ProjectData::getProjectRootPath() {
	QString root_path = this->path;
	if (root_path.at(root_path.size() - 1) != '/' ){ root_path += "/"; }		//加斜杠
	return root_path;
}
/*-------------------------------------------------
		获取项目文件夹（D:/aaa/vvv/项目名/项目名Files/）
*/
QString C_ProjectData::getProjectFilePath() {
	QString file_path = this->getProjectRootPath() + this->name + "Files/";
	QDir temp_dir;
	temp_dir.mkpath(file_path);
	return file_path;
}
/*-------------------------------------------------
		获取项目文件（D:/aaa/vvv/项目名/项目名.xxx）
*/
QString C_ProjectData::getProjectFile() {
	return this->getProjectRootPath() + this->name + "." + PROJECT_SUFFIX;
}

