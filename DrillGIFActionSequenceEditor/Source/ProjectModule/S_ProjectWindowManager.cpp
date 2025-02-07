#include "stdafx.h"
#include "S_ProjectWindowManager.h"

#include PROJECT_INCLUDE

/*
-----==========================================================-----
		类：		项目管理 - 窗口管理.cpp
		作者：		drill_up
		所属模块：	项目管理模块
		功能：		对打开、编辑窗口进行操作，包括响应相关控制。
					【窗口】

		使用方法：
				> 直接调用：
					S_ProjectWindowManager::getInstance()->newProject();
-----==========================================================-----
*/
S_ProjectWindowManager::S_ProjectWindowManager(){
}
S_ProjectWindowManager::~S_ProjectWindowManager(){
}

/* --------------------------------------------------------------
----------ProjectWindowManager 单例
*/
S_ProjectWindowManager* S_ProjectWindowManager::project_manager = NULL;
S_ProjectWindowManager* S_ProjectWindowManager::getInstance() {
	if (project_manager == NULL) {
		project_manager = new S_ProjectWindowManager();
	}
	return project_manager;
}

/*-------------------------------------------------
		流程 - 新建（对话框）
*/
void S_ProjectWindowManager::newProject() {

	bool is_success = S_ProjectManager::getInstance()->newProject();
	// > 工程内容变化时
	if (is_success) { 
		//... 
	}
}
/*-------------------------------------------------
		流程 - 打开（对话框）
*/
void S_ProjectWindowManager::openProject() {

	bool is_success = S_ProjectManager::getInstance()->openProject();

	// > 工程内容变化时
	if (is_success) {
		//... 
	}
}
/*-------------------------------------------------
		流程 - 保存（对话框）
*/
void S_ProjectWindowManager::saveProject() {

	bool is_success = S_ProjectManager::getInstance()->saveProject();
}
/*-------------------------------------------------
		流程 - 另存为（对话框）
*/
void S_ProjectWindowManager::saveAs() {

	bool is_success = S_ProjectManager::getInstance()->saveAs();
}

/*-------------------------------------------------
		窗口 - 打开工程笔记
*/
void S_ProjectWindowManager::openModifyWindowNote(){
	C_ProjectData p_data = S_ProjectManager::getInstance()->data_ProjectData;

	W_ProjectNote d(PROJECT_INSTANCE);
	d.setData(p_data.author, p_data.note);
	if (d.exec() == QDialog::Accepted) {

		// > 直接对工程数据赋值
		S_ProjectManager::getInstance()->data_ProjectData.author = d.getData_author();
		S_ProjectManager::getInstance()->data_ProjectData.note = d.getData_note();
	}
}