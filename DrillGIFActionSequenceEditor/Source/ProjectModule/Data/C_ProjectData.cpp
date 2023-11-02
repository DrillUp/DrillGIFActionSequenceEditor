#include "stdafx.h"
#include "C_ProjectData.h"

#ifdef QT_NETWORK_LIB
#include <QHostInfo>
#endif // QT_NETWORK_LIB

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
QString C_ProjectData::version = "v1.30";				//软件版本
QString C_ProjectData::version_serial = "";				//软件版本号


/*-------------------------------------------------
		软件数据 - 获取软件全名
*/
QString C_ProjectData::getSoftname(){
	//return this->softname + "(" + this->version + "." + this->version_serial + ")";
	return this->softname + "(" + this->version + ")";
}


/*-------------------------------------------------
		清空数据
*/
void C_ProjectData::clear(){

	// > 项目数据
	this->name = "";														//项目名
	this->path = QCoreApplication::applicationDirPath() + "/workspace/";	//项目路径（含项目名 D:/aaa/vvv/项目名/）

	// > 软件数据
	this->author = this->getUserName();					//项目作者
	this->createDate = QDateTime::currentDateTime();	//创建时间
	this->lastSaveDate = QDateTime::currentDateTime();	//上一次修改时间

	// > 软件附加数据
	this->note = "";									//项目笔记（备注）
}


/*-------------------------------------------------
		项目数据 - 获取项目名
*/
QString C_ProjectData::getName() {
	return this->name;
}
/*-------------------------------------------------
		项目数据 - 获取项目后缀
*/
QString C_ProjectData::getSuffix() {
	return PROJECT_SUFFIX;
}
/*-------------------------------------------------
		项目数据 - 设置项目名
*/
void C_ProjectData::setName(QString name) {
	this->name = name;
}
/*-------------------------------------------------
		项目数据 - 设置项目路径
*/
void C_ProjectData::setPath(QString path) {
	this->path = path;
}
/*-------------------------------------------------
		项目数据 - 获取项目父目录（D:/aaa/vvv/）
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
		项目数据 - 获取项目路径（D:/aaa/vvv/项目名/）
*/
QString C_ProjectData::getProjectRootPath() {
	QString root_path = this->path;
	if (root_path.at(root_path.size() - 1) != '/' ){ root_path += "/"; }		//加斜杠
	return root_path;
}
/*-------------------------------------------------
		项目数据 - 获取项目文件夹（D:/aaa/vvv/项目名/项目名Files/）
*/
QString C_ProjectData::getProjectFilePath() {
	QString file_path = this->getProjectRootPath() + this->name + "Files/";
	QDir temp_dir;
	temp_dir.mkpath(file_path);
	return file_path;
}
/*-------------------------------------------------
		项目数据 - 获取项目文件（D:/aaa/vvv/项目名/项目名.xxx）
*/
QString C_ProjectData::getProjectFile() {
	return this->getProjectRootPath() + this->name + "." + PROJECT_SUFFIX;
}

/*-------------------------------------------------
		项目数据 - 获取用户名
*/
QString C_ProjectData::getUserName(){
	QString userName = QStandardPaths::writableLocation( QStandardPaths::HomeLocation );
	userName = userName.section("/", -1, -1);
	return userName;
}
/*-------------------------------------------------
		项目数据 - 获取机器名
*/
QString C_ProjectData::getMachineName(){
	#ifdef QT_NETWORK_LIB
	return QHostInfo::localHostName();
	#else
	return "";
	#endif // QT_NETWORK_LIB
}


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
	QJsonObject obj;

	// > 项目数据
	obj.insert("name", this->name);
	obj.insert("url", this->path);

	// > 软件数据
	obj.insert("author", this->author);
	obj.insert("createDate", this->createDate.toString("yyyy/MM/dd hh:mm:ss"));
	obj.insert("lastSaveDate", this->lastSaveDate.toString("yyyy/MM/dd hh:mm:ss"));

	// > 软件附加数据
	obj.insert("note", this->note);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ProjectData::setJsonObject(QJsonObject obj){

	// > 项目数据
	this->name = obj.value("name").toString();
	this->path = obj.value("url").toString();

	// > 软件数据
	if (obj.value("author").isUndefined() == false){ this->author = obj.value("author").toString(); }
	if (this->author == ""){ this->author = this->getUserName(); }
	if (obj.value("createDate").isUndefined() == false){ this->createDate = QDateTime::fromString( obj.value("createDate").toString(), "yyyy/MM/dd hh:mm:ss"); }
	if (obj.value("lastSaveDate").isUndefined() == false){ this->lastSaveDate = QDateTime::fromString(obj.value("lastSaveDate").toString(), "yyyy/MM/dd hh:mm:ss"); }

	// > 软件附加数据
	if (obj.value("note").isUndefined() == false){ this->note = obj.value("note").toString(); }
}