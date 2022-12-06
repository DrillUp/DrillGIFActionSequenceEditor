#pragma once
#include "stdafx.h"
#include <QObject>
#include <QSettings>


/*
-----==========================================================-----
		类：		ini文件配置管理.cpp
		版本：		v1.02
		所属模块：	项目管理模块
		功能：		基于QSetting文件的基本配置管理器，直接写成ini文件。
					（详细见cpp）
-----==========================================================-----
*/
class S_IniManager : public QObject
{
	Q_OBJECT

	//-----------------------------------
	//----基础结构
	public:
		S_IniManager();
		~S_IniManager();
		static S_IniManager* cur_instance;			//单例
		static S_IniManager* getInstance();			//单例，获取自己（必须要拿到全局配置才能进行计算）
		void _init();								//初始化
		
	//-----------------------------------
	//----操作
	private:
		QSettings* m_qsettings;			//配置
		QString m_path;					//文件路径

	public:
										//操作 - 获取值
		QString getConfig(QString key);
										//操作 - 插入值
										//		【说明】:插入数据即存储。
		void setConfig(QString key,QString value);
};

