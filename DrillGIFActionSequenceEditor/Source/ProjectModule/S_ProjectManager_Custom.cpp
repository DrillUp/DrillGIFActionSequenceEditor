#include "stdafx.h"
#include "S_ProjectManager_Custom.h"
#include "S_ProjectManager.h"

#include "Data/C_ProjectData.h"
#include "Data/W_ProjectCreate.h"
#include "Storage/S_StorageManager.h"
#include "StorageGlobal/S_IniManager.h"
#include "File/S_TempFileManager.h"
#include PROJECT_INCLUDE

#ifdef QT_NETWORK_LIB
#include <QHostInfo>
#endif // QT_NETWORK_LIB

/*
-----==========================================================-----
		类：		项目管理（自定义部分）.cpp
		作者：		drill_up
		所属模块：	项目管理模块
		功能：		项目管理自定义相关数据、相关操作都在这里控制。
-----==========================================================-----
*/

S_ProjectManager_Custom::S_ProjectManager_Custom(){
}
S_ProjectManager_Custom::~S_ProjectManager_Custom() {
}

/* --------------------------------------------------------------
----------单例
*/
S_ProjectManager_Custom* S_ProjectManager_Custom::project_manager = NULL;
S_ProjectManager_Custom* S_ProjectManager_Custom::getInstance() {
	if (project_manager == NULL) {
		project_manager = new S_ProjectManager_Custom();
	}
	return project_manager;
}


QString S_ProjectManager_Custom::softname = "GIF动画序列编辑器";	//软件名
QString S_ProjectManager_Custom::version = "v1.32";					//软件版本
QString S_ProjectManager_Custom::version_serial = "";				//软件版本号

/*-------------------------------------------------
		软件信息 - 获取软件全名
*/
QString S_ProjectManager_Custom::getSoftname() {
	//return this->softname + "(" + this->version + "." + this->version_serial + ")";
	return this->softname + "(" + this->version + ")";
	//return this->softname;
}
/*-------------------------------------------------
		软件信息 - 获取机器名
*/
QString S_ProjectManager_Custom::getMachineName() {
	#ifdef QT_NETWORK_LIB
		return QHostInfo::localHostName();
	#else
		return "";
	#endif // QT_NETWORK_LIB
}



/* --------------------------------------------------------------
		项目操作 - 修改名称前
*/
void S_ProjectManager_Custom::changeTitle_Before() {
	//（无）
}
/* --------------------------------------------------------------
		项目操作 - 修改名称后
*/
void S_ProjectManager_Custom::changeTitle_After(QString new_title) {
	//（无）
}

/* --------------------------------------------------------------
		项目操作 - 清除项目数据前
*/
void S_ProjectManager_Custom::clearProject_Before() {
	//（无）
}
/* --------------------------------------------------------------
		项目操作 - 清除项目数据后
*/
void S_ProjectManager_Custom::clearProject_After() {
	//（无）
}


/* --------------------------------------------------------------
		项目操作 - 保存文件前
*/
void S_ProjectManager_Custom::createSaveFile_Before(QString tar_fileDir, QString saveAs_last_fileDir) {
	//（无）
}
/* --------------------------------------------------------------
		项目操作 - 保存文件后
*/
void S_ProjectManager_Custom::createSaveFile_After(QString tar_fileDir, QString saveAs_last_fileDir) {
	//（无）
}


/* --------------------------------------------------------------
		项目操作 - 读取文件前
*/
void S_ProjectManager_Custom::readSaveFile_Before() {
	//（无）
}
/* --------------------------------------------------------------
		项目操作 - 读取文件后
*/
void S_ProjectManager_Custom::readSaveFile_After() {
	//（无）
}
