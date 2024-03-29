﻿#pragma once
#include "stdafx.h"
#include <QList>
#include <QMap>

#define PROJECT_SUFFIX		"drillas"										//项目文件后缀
#define PROJECT_INCLUDE		"DrillGIFActionSequenceEditor.h"				//引用头文件
#define PROJECT_INSTANCE	DrillGIFActionSequenceEditor::getInstance()		//父窗口关联

/*
-----==========================================================-----
		类：		项目数据 实体类.h
		所属模块：	项目管理模块
		功能：		存储项目的基本内容的数据类。
-----==========================================================-----
*/
class C_ProjectData{

	public:
		C_ProjectData();
		~C_ProjectData();
		
	//-----------------------------------
	//----项目数据
	private:
		QString name;					//项目名（强制私有，项目名）
		QString path;					//项目路径（强制私有，D:/aaa/vvv/项目名/）
	public:
										//项目数据 - 获取项目名
		QString getName();
										//项目数据 - 获取项目后缀
		QString getSuffix();
										//项目数据 - 设置项目名
		void setName(QString name);
										//项目数据 - 设置项目路径
		void setPath(QString path);
										//项目数据 - 获取项目父目录（D:/aaa/vvv/）
		QString getParentPath();
										//项目数据 - 获取项目路径（D:/aaa/vvv/项目名/）
		QString getProjectRootPath();
										//项目数据 - 获取项目文件夹（D:/aaa/vvv/项目名/项目名Files/）
		QString getProjectFilePath();
										//项目数据 - 获取项目文件（D:/aaa/vvv/项目名/项目名.xxx）
		QString getProjectFile();
										//项目数据 - 获取用户名
		QString getUserName();
										//项目数据 - 获取机器名
		QString getMachineName();
		
	//-----------------------------------
	//----软件数据
	public:
		static QString softname;				//软件名
		static QString version;					//软件版本
		static QString version_serial;			//软件版本号
		QString author;							//项目作者
		QDateTime createDate;					//创建时间
		QDateTime lastSaveDate;					//上一次修改时间
	public:
										//软件数据 - 获取软件全名
		QString getSoftname();
		
	//-----------------------------------
	//----软件附加数据
	public:
		QString note;							//项目笔记（备注）

	//-----------------------------------
	//----类属性
	public:
										//清空数据
		void clear();
										//空判断
		bool isNull();
										//实体类 -> QJsonObject
		QJsonObject getJsonObject();
										//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);

};
