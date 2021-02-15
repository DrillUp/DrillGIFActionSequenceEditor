#pragma once

/*
-----==========================================================-----
		类：		存储模板块.h
		所属模块：	项目管理模块
		功能：		存储的模板，用于规范子类实现的数据结构。
					详细见s_StorageManager.cpp。
-----==========================================================-----
*/

class S_StorageManagerTemplate
{

	public:
		S_StorageManagerTemplate();
		~S_StorageManagerTemplate();
		
	//-----------------------------------
	//----数据
	public:
		QFileInfo m_storage_fileInfo;	//存储/读取时的文件信息
	public:
										//数据 - 获取存储的名称
		virtual QString getSaveName() = 0;
										//数据 - 清除当前管理器数据
		virtual void clearAllData() = 0;
										//数据 - 全部项目管理数据 -> QJsonObject
		virtual QJsonObject getAllDataOfJsonObject() = 0;
										//数据 - QJsonObject -> 全部项目管理数据
		virtual void setAllDataFromJsonObject(QJsonObject obj) = 0;
		
	//-----------------------------------
	//----特殊
	public:
										//特殊 - 获取旧版本的名称(读取用)
										//		 如果"getSaveName"改了名字，这里用来作兼容
		virtual QStringList getOldNamesForRead();

};

