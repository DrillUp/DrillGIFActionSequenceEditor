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
class C_FCTClassify{

	public:
		C_FCTClassify();		//空对象
		C_FCTClassify(QString name, QString description);
		~C_FCTClassify();

	public:
		QString name;			//名称（不能重复）
		QString description;	//描述
		
	//-----------------------------------
	//----类属性
	public:
									//运算符重载
		const bool operator== (const C_FCTClassify& a)const;
									//实体类 -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
									//空判断
		bool isNull();
};