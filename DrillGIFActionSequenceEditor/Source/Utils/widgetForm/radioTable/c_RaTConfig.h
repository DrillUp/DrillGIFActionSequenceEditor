#pragma once


/*
-----==========================================================-----
		类：		单选表格配置 数据类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		单选表格配置 的数据类。
					（详细见cpp）
-----==========================================================-----
*/
class C_RaTConfig{

	public:
		C_RaTConfig();
		~C_RaTConfig();
		
	//-----------------------------------
	//----常规配置
	public:
		int rowHeight;				//行高
		bool zeroFill;				//零填充
		int zeroFillCount;			//填充位数
		QChar zeroFillChar;			//填充字符
		
	//-----------------------------------
	//----类属性
	public:
									//实体类 -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
};