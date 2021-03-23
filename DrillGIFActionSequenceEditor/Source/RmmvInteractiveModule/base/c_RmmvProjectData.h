#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		类：		rmmv工程数据 实体类.h
		版本：		v1.01
		所属模块：	交互模块
		功能：		工程基本信息数据。
					（详细见.cpp）
-----==========================================================-----
*/
class C_RmmvProjectData{

	public:
		C_RmmvProjectData();
		~C_RmmvProjectData();
		
	//-----------------------------------
	//----数据
	public:
		QString name;				//工程名
		QString version;			//工程版本
		QString path;				//工程根路径

		bool optional_backup;		//备份标记
	
	//-----------------------------------
	//----类属性
	public:
												//空判断
		bool isNull();
												//实体类 -> QJsonObject
		QJsonObject getJsonObject();
												//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
												//运算符重载
		const bool operator== (const C_RmmvProjectData& a)const;
};
