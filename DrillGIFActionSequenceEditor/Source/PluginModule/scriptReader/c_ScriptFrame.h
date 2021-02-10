#pragma once
#include <QList>
#include <QMap>


/*
-----==========================================================-----
		类：		插件框架 实体类.h
		所属模块：	插件模块
		功能：		插件框架 的数据。
					（详细见.cpp）
-----==========================================================-----
*/
class C_ScriptFrame_Param;
class C_ScriptFrame{

	public:
		C_ScriptFrame();
		~C_ScriptFrame();
		
	public:
		QString name;								//插件名
		QString plugindesc;							//插件描述（@plugindesc）
		QString author;								//作者（@author）
		QString help;								//帮助信息（@help）
		QList<C_ScriptFrame_Param> param_list;		//参数列表
		
		bool isComplete;							//完整解析内容（参数解析器 标记）

	//-----------------------------------
	//----类属性
	public:
										//是否为完整解析的内容
		bool isCompleted();
										//运算符重载
		const bool operator== (const C_ScriptFrame& a)const;
										//实体类 -> QJsonObject
		QJsonObject getJsonObject();
										//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
};

/*
-----==========================================================-----
		类：		插件框架参数 实体类.h
		所属模块：	插件模块
		功能：		单独的参数识别。
-----==========================================================-----
*/
class C_ScriptFrame_Param {
	public:
		C_ScriptFrame_Param();
		~C_ScriptFrame_Param();
	public:
		QString name;			//参数名（@param）
		QString parent;			//参数父类名（@parent）
		QString type;			//参数类型（@type）
		QString desc;			//参数描述（@desc）
		QString default_;		//参数默认值（@default）

		int min;				//参数类型-数字最小值（@min）
		int max;				//参数类型-数字最大值（@max）
		QString on;				//参数类型-布尔文本（@on）
		QString off;			//参数类型-布尔文本（@off）
		QStringList option;		//参数类型-下拉框的内容（@option）
		int require;			//参数类型-文件（@require）
		QString dir;			//参数类型-文件（@dir）
		
	//-----------------------------------
	//----类属性
	public:
										//运算符重载
		const bool operator== (const C_ScriptFrame_Param& a)const;
										//实体类 -> QJsonObject
		QJsonObject getJsonObject();
										//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
};