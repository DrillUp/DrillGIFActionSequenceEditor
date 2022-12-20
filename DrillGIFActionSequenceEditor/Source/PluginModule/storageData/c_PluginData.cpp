#include "stdafx.h"
#include "C_PluginData.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		插件数据 实体类.cpp
		版本：		v1.04
		作者：		drill_up
		所属模块：	插件模块
		功能：		存储插件信息的类，对应plugin.js中每条插件的配置数据。

		说明：		1.由于底层基于QJsonDocument，重写后，顺序一定会乱。
					2.parameters的值，不是 字符串object 。
-----==========================================================-----
*/
C_PluginData::C_PluginData(){
	this->name = "";
	this->status = false;
	this->description = "";
	this->parameters = QJsonObject();

	this->temp_inited = false;
	this->temp_version = "";			//版本(调取时才初始化）
	this->temp_type = "";				//类型（调取时才初始化）
	this->temp_name = "";				//中文名（调取时才初始化）
}
C_PluginData::~C_PluginData(){
}



//数据 - 初始化截取段
void C_PluginData::initDescription_data(){
	this->temp_inited = true;

	// > 版本
	if (this->description.isEmpty() == false){			//（根据方括号截取内容）
		QString data = this->description.trimmed();
		QChar ch = data.at(0);
		if (ch == '[' || ch == '(' || ch == '（' || data.at(0) == 'v' || data.at(0) == 'V'){
			QStringList data_list = this->description.split("]");
			if (data_list.count() >= 2){
				this->temp_version = data_list.at(0) + "]";
			}
			else{
				data_list = this->description.split(" ");
				if (data_list.count() >= 2){
					this->temp_version = data_list.at(0);
				}
			}
		}
	}

	// > 类型
	QStringList data_list = this->description.split(" - ");	//（取横杠的左侧内容）
	if (data_list.count() >= 2){
		QStringList data_list2 = data_list.at(0).trimmed().split(QRegExp("[ \\]\\)]+"));
		this->temp_type = data_list2.last();
		if (this->temp_type.contains(".")){ this->temp_type = ""; }		//（含特殊字符的类型去掉）
		if (this->temp_type.contains("[")){ this->temp_type = ""; }
		if (this->temp_type.contains("]")){ this->temp_type = ""; }
		if (this->temp_type.contains("-")){ this->temp_type = ""; }
	}

	// > 中文名
	QStringList data_list2 = this->description.split(" ");
	this->temp_name = data_list2.last();
}
//数据 - 获取版本（插件描述 截取段）
QString C_PluginData::getDescription_version(){
	if (this->temp_inited == false){ this->initDescription_data(); }
	return this->temp_version;
}
double C_PluginData::getDescription_versionNum(){
	if (this->temp_inited == false){ this->initDescription_data(); }
	if (this->temp_version.isEmpty()){ return -1; }
	return TTool::_to_double_(this->temp_version);
}
//数据 - 获取类型（插件描述 截取段）
QString C_PluginData::getDescription_type(){
	if (this->temp_inited == false){ this->initDescription_data(); }
	return this->temp_type;
}
//数据 - 获取中文名（插件描述 截取段）
QString C_PluginData::getDescription_name(){
	if (this->temp_inited == false){ this->initDescription_data(); }
	return this->temp_name;
}


/*-------------------------------------------------
		空判断
*/
bool C_PluginData::isNull(){
	return this->name == "";
}
/*-------------------------------------------------
		运算符重载
*/
const bool C_PluginData::operator== (const C_PluginData& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_PluginData::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("status", this->status);
	obj.insert("description", this->description);
	obj.insert("parameters", this->parameters);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_PluginData::setJsonObject(QJsonObject obj){
	this->name = obj.value("name").toString();
	this->status = obj.value("status").toBool();
	this->description = obj.value("description").toString();
	this->parameters = obj.value("parameters").toObject();
}
