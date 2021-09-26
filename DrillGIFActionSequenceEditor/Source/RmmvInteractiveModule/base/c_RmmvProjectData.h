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
	protected:
		QString name;				//工程名
		QString version;			//工程版本
		QString path;				//工程根路径
	public:
		bool optional_backup;
	public:
												//数据 - 解析工程文件
		void initFromFile(QString full_filePath);
												//数据 - 获取工程名称
												//		【说明】：index.html中的完整工程名称。
		QString getName();
												//数据 - 获取工程版本
		QString getVersion();
												//数据 - 获取工程根路径
												//		【说明】：工程根路径，返回"C:/aaa/插件集合示例"。
		QString getRootPath();
												//数据 - 判断工程是否存在
		bool isProjectExist();
	
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
