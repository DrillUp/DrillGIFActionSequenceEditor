#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		类：		选择器设置 参数.h
		所属模块：	工具模块
		功能：		对象选择器的窗口配置参数。
-----==========================================================-----
*/
class C_ObjectSelectorConfig{

	public:
		C_ObjectSelectorConfig();
		~C_ObjectSelectorConfig();
		
	//-----------------------------------
	//----常规配置
	public:
		int rowHeight;				//行高
		bool zeroFill;				//零填充
		int zeroFillCount;			//填充位数
		QChar zeroFillChar;			//填充字符

		int pagePerNum;					//ID - 每页容量
		bool showHavingDataInitials;	//名称 - 显示首字母
		
	//-----------------------------------
	//----特殊配置
	public:
		bool excludeFirstData;		//排除第一个
		bool addNullValue;			//第一个数据表示 无
		
	//-----------------------------------
	//----类属性
	public:
									//实体类 -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
};
