#pragma once

/*
-----==========================================================-----
		类：		排序数据类.h
		所属模块：	工具模块
		功能：		用于排序的对象数据。
					（该类不含QJsonObject的回转，因为不知道obj的 key-value 中的key值）
-----==========================================================-----
*/
class C_ObjectSortData{

	public:
		C_ObjectSortData();
		~C_ObjectSortData();

	//-----------------------------------
	//----原数据
	protected:
		int m_org_arrIndex;			//数组中所在位置
		int m_org_id;				//ID
		QString m_org_name;			//名称
		QString m_org_type;			//所属分类
	public:
									//原数据 - 执行初始化
		void initData(int arrIndex, int id, QString name, QString type);
	public:
									//原数据 - 设置ID
		void setID(int id);
									//原数据 - 设置名称
		void setName(QString name);
									//原数据 - 设置所属分类
		void setType(QString type);

	//-----------------------------------
	//----排序用数据
	public:
		int m_sort_id;				//排序用ID
		QString m_sort_name;		//排序用名称
		QString m_sort_type;		//排序用所属分类

	//-----------------------------------
	//----获取
	public:
									//获取 - 数组中所在位置
		int getArrIndex();
									//获取 - ID
		int getID_Sort();
		int getID_Org();
									//获取 - 名称
		QString getName_Sort();
		QString getName_Org();
									//获取 - 所属分类
		QString getType_Sort();
		QString getType_Org();

	//-----------------------------------
	//----类属性
	public:	
									//运算符重载
		const bool operator== (const C_ObjectSortData& a)const;
									//空判断
		bool isNull();
									//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
									//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj, QString key_id, QString key_name, QString key_type);

									//实体类 -> QJsonObject（无） 
};

