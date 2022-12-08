#pragma once
#include "stdafx.h"

#include "S_StorageManagerTemplate.h"

/*
-----==========================================================-----
		类：		存储管理器.h
		所属模块：	项目管理模块
		功能：		将所有继承模板的子类的数据，存储文件/读取文件。
					详细见.cpp。
-----==========================================================-----
*/

class S_StorageManager : public QWidget
{
	Q_OBJECT

	public:
		S_StorageManager();
		~S_StorageManager();
		static S_StorageManager* cur_manager;		//单例
		static S_StorageManager* getInstance();		//单例，获取自己（必须要拿到全局配置才能进行计算）
		
	//-----------------------------------
	//----数据
	private:
		QList<S_StorageManagerTemplate*> m_managers;
	public:
										//数据 - 添加管理器（模板中自动添加，不需要重复添加）
		void addManager(S_StorageManagerTemplate* s_m);
		

	//-----------------------------------
	//----管理
	public:
										//管理 - 清除所有程序数据
		void clearAllApplicationData();
										//管理 - 存储文件（F:/aaa/bbb.xxx ）
		void createSaveFile(QString file_name);
										//管理 - 读取文件（F:/aaa/bbb.xxx ）
		void readSaveFile(QString file_name);

};

