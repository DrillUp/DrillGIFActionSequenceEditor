#include "stdafx.h"
#include "c_LEConfigData.h"

#include "Source/Utils/common/TTool.h"


/*
-----==========================================================-----
		�ࣺ		���ò���.cpp���洢��
-----==========================================================-----
*/
C_LEConfigData::C_LEConfigData(){
	this->pluginName = "";
	this->paramKey = "";
	this->configLen = -1;
}
C_LEConfigData::~C_LEConfigData(){
}

/*-------------------------------------------------
		���� - ��ʼ��
*/
void C_LEConfigData::initParam(QString pluginName, QString paramKey, int realLen){
	this->pluginName = pluginName;
	this->paramKey = paramKey;
	this->configLen = realLen;
}
/*-------------------------------------------------
		��ȡ - �������Ӣ�ģ�
*/
QString C_LEConfigData::getPluginName(){
	return this->pluginName;
}
/*-------------------------------------------------
		��ȡ - ������ "�׶�-%d"
*/
QString C_LEConfigData::getParamKey(){
	return this->paramKey;
}
/*-------------------------------------------------
		��ȡ - ������ʾ�� "�׶�"
*/
QString C_LEConfigData::getParamShowingName(){
	return this->paramKey.split("-").at(0);
}
/*-------------------------------------------------
		��ȡ - �������ֵ
*/
int C_LEConfigData::getConfigLen(){
	return this->configLen;
}
/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_LEConfigData::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("pluginName", this->pluginName);
	obj.insert("paramKey", this->paramKey);
	obj.insert("realLen", this->configLen);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_LEConfigData::setJsonObject(QJsonObject obj){
	this->pluginName = obj.value("pluginName").toString();
	this->paramKey = obj.value("paramKey").toString();
	this->configLen = obj.value("realLen").toInt();
}
/*-------------------------------------------------
		���ж�
*/
bool C_LEConfigData::isNull(){
	return this->paramKey == "" || this->pluginName == "";
}
/*-------------------------------------------------
		���������
*/
const bool C_LEConfigData::operator== (const C_LEConfigData& a)const {
	return this->pluginName == a.pluginName && this->paramKey == a.paramKey;
}