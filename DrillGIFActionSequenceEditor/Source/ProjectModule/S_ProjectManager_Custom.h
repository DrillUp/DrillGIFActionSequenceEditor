#pragma once
#include "stdafx.h"

#include "Data/C_ProjectData.h"
#include "Storage/S_StorageManagerTemplate.h"

/*
-----==========================================================-----
		类：		项目管理（自定义部分）.h
		作者：		drill_up
		所属模块：	项目管理模块
		功能：		项目管理自定义相关数据、相关操作都在这里控制。
					（详细见cpp）
-----==========================================================-----
*/

#define PROJECT_SUFFIX		"drillas"									//项目文件后缀
#define PROJECT_SUFFIX_OLD	",,"										//旧的项目文件后缀（填入多个，逗号分隔）
#define PROJECT_INCLUDE		"DrillGIFActionSequenceEditor.h"			//引用头文件
#define PROJECT_INSTANCE	DrillGIFActionSequenceEditor::getInstance()	//父窗口关联
#define PROJECT_USE_W_ProjectCreate true								//是否使用 新建项目窗口
//#define PROJECT_USE_S_UniqueTempFileFactory ""						//是否使用 唯一文件管理工厂（不用就把这一行注释掉）

class S_ProjectManager_Custom : public QObject
{
	Q_OBJECT

	public:
		S_ProjectManager_Custom();
		~S_ProjectManager_Custom();
		static S_ProjectManager_Custom* project_manager;		//单例
		static S_ProjectManager_Custom* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）
		
	
	//-----------------------------------
	//----软件信息
	public:
		static QString softname;				//软件名
		static QString version;					//软件版本
		static QString version_serial;			//软件版本号
	public:
										//软件信息 - 获取软件全名
		QString getSoftname();
										//软件信息 - 获取机器名
		QString getMachineName();


	//-----------------------------------
	//----项目操作
	public:
										//项目操作 - 修改名称前
		void changeTitle_Before();
										//项目操作 - 修改名称后
		void changeTitle_After(QString new_title);
										//项目操作 - 清除项目数据前
		void clearProject_Before();
										//项目操作 - 清除项目数据后
		void clearProject_After();
										//项目操作 - 保存文件前
										//		【说明】：如果是另存为情况，则 saveAs_last_fileDir 必然会有值。
		void createSaveFile_Before(QString tar_fileDir, QString saveAs_last_fileDir);
										//项目操作 - 保存文件后
										//		【说明】：如果是另存为情况，则 saveAs_last_fileDir 必然会有值。
		void createSaveFile_After(QString tar_fileDir, QString saveAs_last_fileDir);
										//项目操作 - 读取文件前
		void readSaveFile_Before();
										//项目操作 - 读取文件后
		void readSaveFile_After();
		
};

