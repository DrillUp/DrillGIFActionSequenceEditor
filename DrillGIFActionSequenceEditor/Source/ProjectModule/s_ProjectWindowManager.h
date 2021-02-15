#pragma once

#include "s_ProjectManager.h"

#include "data/w_ProjectCreate.h"
#include "data/w_ProjectNote.h"

/*
-----==========================================================-----
		类：		项目管理 - 窗口管理.h
		所属模块：	项目管理模块
		功能：		对打开、编辑窗口进行操作，包括响应相关控制。
					【窗口】
-----==========================================================-----
*/


class S_ProjectWindowManager : public QObject
{
	Q_OBJECT

	public:
		S_ProjectWindowManager();
		~S_ProjectWindowManager();
		static S_ProjectWindowManager* project_manager;		//单例
		static S_ProjectWindowManager* getInstance();		//单例，获取自己（必须要拿到全局配置才能进行计算）
		
	//-----------------------------------
	//----全局交互
	signals:
										//全局交互 - 刷新全部设置树
		void reflashAllTree();

	//-----------------------------------
	//----流程
	public slots:
										//流程 - 新建（对话框）
		void newProject();
										//流程 - 打开（对话框）
		void openProject();	
										//流程 - 保存（对话框）
		void saveProject();	
										//流程 - 另存为（对话框）
		void saveAs();

	//-----------------------------------
	//----窗口
	public slots:
										//窗口 - 工程笔记
		void openModifyWindowNote();
										//窗口 - 新建项目
										//		（这个窗口比较特殊，这里直接嵌套到s_ProjectManager中了。）
		//void openModifyWindowCreate();
};

