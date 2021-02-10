#pragma once


/*
-----==========================================================-----
		类：		类型 数据类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		灵活分类树的 类型 数据类。
					（详细见cpp）
-----==========================================================-----
*/
class C_FCTConfig{

	public:
		C_FCTConfig();
		~C_FCTConfig();
		
	//-----------------------------------
	//----常规配置
	public:
		int rowHeight;				//行高
		bool zeroFill;				//零填充
		int zeroFillCount;			//填充位数
		QChar zeroFillChar;			//填充字符

		int pagePerNum;				//按id排序
		QString sortType;			//排序 - 对象排序方式
		
	//-----------------------------------
	//----类属性
	public:
									//实体类 -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
};