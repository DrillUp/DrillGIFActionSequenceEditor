#include "stdafx.h"
#include "C_ProjectData.h"

#include "Source/ProjectModule/S_ProjectManager_Custom.h"

/*
-----==========================================================-----
		类：		项目数据 实体类.cpp
		作者：		drill_up
		所属模块：	项目管理模块
		功能：		存储项目的基本内容的数据类。
-----==========================================================-----
*/

C_ProjectData::C_ProjectData(){
	this->clear();
}
C_ProjectData::~C_ProjectData(){
}


/*-------------------------------------------------
		软件数据 - 获取软件全名
*/
QString C_ProjectData::getSoftname(){
	return S_ProjectManager_Custom::getInstance()->getSoftname();
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
	Q_ASSERT(this->name != "");  //（该断言可以注释掉）（这里不允许项目名为空字符串，如果有报错，注意检查一下）
	QString file_path = this->getProjectRootPath() + this->name + "Files/";
	QDir temp_dir;
	temp_dir.mkpath(file_path);
	return file_path;
}
/*-------------------------------------------------
		项目数据 - 获取项目文件（D:/aaa/vvv/项目名/项目名.xxx）
*/
QString C_ProjectData::getProjectFile() {
	//	（必须返回一个非空字符串）

	// > 打开时 - 检查文件
	QString file_path = this->getProjectRootPath() + this->name + "." + PROJECT_SUFFIX;
	if (QFileInfo(file_path).exists()) { return file_path; }

	// > 打开时 - 检查旧项目文件
	QStringList old_suffix_list = QString(PROJECT_SUFFIX_OLD).split(",");
	for (int i = 0; i < old_suffix_list.count(); i++) {
		QString old_suffix = old_suffix_list.at(i);
		if (old_suffix.isEmpty()) { continue; }
		file_path = this->getProjectRootPath() + this->name + "." + old_suffix;
		if (QFileInfo(file_path).exists()) { return file_path; }
	}


	// > 保存时 - 固定字符串
	return this->getProjectRootPath() + this->name + "." + PROJECT_SUFFIX;
}
/*-------------------------------------------------
		项目数据 - 获取后缀筛选器
*/
QString C_ProjectData::getProjectFileFilter() {
	QString result;
	result.append(QObject::tr("工程文件(*."));
	result.append(PROJECT_SUFFIX);
	result.append(")");
	return result;
}
/*-------------------------------------------------
		项目数据 - 获取后缀筛选器（含旧后缀）
*/
QString C_ProjectData::getProjectFileFilterWithOld() {
	QString result;
	result.append(QObject::tr("工程文件(*."));
	result.append(PROJECT_SUFFIX);

	QStringList old_suffix_list = QString(PROJECT_SUFFIX_OLD).split(",");
	for (int i = 0; i < old_suffix_list.count(); i++) {
		QString old_suffix = old_suffix_list.at(i);
		if (old_suffix.isEmpty()) { continue; }
		result.append(";");
		result.append("*.");
		result.append(old_suffix);
	}

	result.append(")");
	return result;
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
		项目数据 - 原生对话框窗口获取
*/
C_ProjectData C_ProjectData::openQDialogForProjectCreate(){

	C_ProjectData data;
	QFileDialog fd;
	fd.setWindowTitle( QObject::tr("保存工程文件") );
	fd.setAcceptMode(QFileDialog::AcceptSave);		//对话框类型（打开/保存）（保存会有文件覆盖提示）
	fd.setDirectory(data.getProjectRootPath());		//默认目录
	fd.setNameFilters(QStringList() << data.getProjectFileFilter());
	fd.setViewMode(QFileDialog::Detail);
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()) {
			return C_ProjectData();
		}
		QString file_path = fd.selectedFiles().first();
		QFileInfo file_info(file_path);
		if (file_info.exists()){	//（文件已存在，则覆盖项目文件）
			data.setName(file_info.completeBaseName());
			data.setPath(file_info.absolutePath());
		}else{						//（文件不存在，则创建项目文件夹+项目文件）
			data.setName(file_info.completeBaseName());
			data.setPath(file_info.absolutePath() + "/" + file_info.completeBaseName());
		}
		return data;
	}else {
		return C_ProjectData();
	}
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