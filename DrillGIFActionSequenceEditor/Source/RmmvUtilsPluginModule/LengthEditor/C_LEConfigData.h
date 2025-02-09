#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		类：		待修改的值.h
		作者：		drill_up
		所属模块：	插件模块
		功能：		写入器需要该数据类，填入待修改的值，才能执行修改。
-----==========================================================-----
*/
class C_LEConfigData {

	public:
		C_LEConfigData();
		~C_LEConfigData();
		
	//-----------------------------------
	//----修改值
	private:
		QString pluginName;				//插件名（标识用）
		QString paramKey;				//变量键（标识用）
		int configLen;					//当前配置最大值
	public:
										//修改值 - 初始化
		void initParam(QString pluginName, QString paramKey, int realLen);
										//修改值 - 获取 插件名（英文）
		QString getPluginName();
										//修改值 - 获取 变量键（"阶段-%d"）
		QString getParamKey();
										//修改值 - 获取 变量显示名（"阶段"）
		QString getParamShowingName();
										//修改值 - 获取 当前配置最大值
		int getConfigLen();
		
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
		const bool operator== (const C_LEConfigData& a)const;

};
inline uint qHash(C_LEConfigData &key, uint seed){	//qHasg全局重载
	return qHash(key.getPluginName(), seed) * qHash(key.getParamKey(), seed);
};