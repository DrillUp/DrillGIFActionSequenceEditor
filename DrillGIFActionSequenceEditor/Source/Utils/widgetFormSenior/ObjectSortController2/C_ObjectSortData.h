#pragma once

/*
-----==========================================================-----
		类：		排序数据类.h
		所属模块：	工具模块
		功能：		用于排序的对象数据。
					（该类不含QJsonObject的回转，因为symbol标识不明确）
-----==========================================================-----
*/
class C_ObjectSortData{
	public:
		C_ObjectSortData();
		C_ObjectSortData(int arr_index, int id, QString name, QString type);
		~C_ObjectSortData();

	public:
		int arrIndex;			//数组中所在位置
		int id;					//ID
		QString name;			//名称
		QString type;			//所属分类
	
	public:	
									//运算符重载
		const bool operator== (const C_ObjectSortData& a)const;
									//空判断
		bool isNull();
									//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
		void setJsonObject(QJsonObject obj, QString id_symbol, QString name_symbol, QString type_symbol);
};

