#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		类：		修改的参数.h（存储）
		所属模块：	插件模块
		功能：		插件的配置参数数据。
-----==========================================================-----
*/
class C_LEConfigData {
	public:
		C_LEConfigData();
		~C_LEConfigData();
	private:
		QString pluginName;			//插件名
		QString paramKey;			//变量键
		int configLen;				//配置最大值
	public:
											//参数 - 初始化
		void initParam(QString pluginName, QString paramKey, int realLen);
											//获取 - 插件名（英文）
		QString getPluginName();
											//获取 - 变量键 "阶段-%d"
		QString getParamKey();
											//获取 - 变量显示名 "阶段"
		QString getParamShowingName();
											//获取 - 配置最大值
		int getConfigLen();

											//实体类 -> QJsonObject
		QJsonObject getJsonObject();
											//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
											//空判断
		bool isNull();
											//运算符重载
		const bool operator== (const C_LEConfigData& a)const;
};
inline uint qHash(C_LEConfigData &key, uint seed){	//qHasg全局重载
	return qHash(key.getPluginName(), seed) * qHash(key.getParamKey(), seed);
};