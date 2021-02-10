#include "stdafx.h"
#include "C_ScriptFrame.h"


/*
-----==========================================================-----
		类：		插件框架 实体类.cpp
		所属模块：	插件模块
		功能：		插件框架 的数据。

		必要事项：	> 框架数据为从js脚本中读取到的数据。
					  由于js脚本随时可变，所以不建议存储。
-----==========================================================-----
*/
C_ScriptFrame::C_ScriptFrame(){
	this->name = "";								  	//插件名
	this->plugindesc = "";							 	//插件描述（@plugindesc）
	this->author = "";									//作者（@author）
	this->help = "";								 	//帮助信息（@help）
	this->param_list = QList<C_ScriptFrame_Param>(); 	//参数列表

	this->isComplete = false;							//完整解析内容（参数解析器 标记）
}
C_ScriptFrame::~C_ScriptFrame(){
}
/*-------------------------------------------------
		是否为完整解析的内容
*/
bool C_ScriptFrame::isCompleted(){
	return this->isComplete;
}
/*-------------------------------------------------
		运算符重载
*/
const bool C_ScriptFrame::operator== (const C_ScriptFrame& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_ScriptFrame::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("plugindesc", this->plugindesc);
	obj.insert("author", this->author);
	obj.insert("help", this->help);
	QJsonArray arr_param_list = QJsonArray();
	for (int i = 0; i < this->param_list.count(); i++){ 
		C_ScriptFrame_Param c_p = this->param_list.at(i);
		arr_param_list.append(c_p.getJsonObject());
	}
	obj.insert("param_list", arr_param_list);
	obj.insert("isComplete", isComplete);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ScriptFrame::setJsonObject(QJsonObject obj){

	if (obj.value("name").isUndefined() == false){ this->name = obj.value("name").toString(); }
	if (obj.value("plugindesc").isUndefined() == false){ this->plugindesc = obj.value("plugindesc").toString(); }
	if (obj.value("author").isUndefined() == false){ this->author = obj.value("author").toString(); }
	if (obj.value("help").isUndefined() == false){ this->help = obj.value("help").toString(); }
	if (obj.value("param_list").isUndefined() == false){
		QJsonArray arr_option = QJsonArray();
		this->param_list = QList<C_ScriptFrame_Param>();
		for (int i = 0; i < arr_option.count(); i++){ 
			C_ScriptFrame_Param c_p = C_ScriptFrame_Param();
			c_p.setJsonObject(arr_option.at(i).toObject());
			this->param_list.append(c_p);
		}
	}
	if (obj.value("isComplete").isUndefined() == false){ this->isComplete = obj.value("isComplete").toBool(); }
}


/*
-----==========================================================-----
		类：		插件框架参数 实体类.h
		所属模块：	插件模块
		功能：		单独的参数识别。
-----==========================================================-----
*/
C_ScriptFrame_Param::C_ScriptFrame_Param(){
	this->name = "";				//参数名（@param）
	this->parent = "";				//参数父类名（@parent）
	this->type = "";				//参数类型（@type）
	this->desc = "";				//参数描述（@desc）
	this->default_ = "";			//参数默认值（@default）

	this->min = -1;					//参数类型-数字最小值（@min）
	this->max = -1;					//参数类型-数字最大值（@max）
	this->on = "";					//参数类型-布尔文本（@on）
	this->off = "";					//参数类型-布尔文本（@off）
	this->option = QStringList(); 	//参数类型-下拉框的内容（@option）
	this->require = -1;				//参数类型-文件（@require）
	this->dir = "";					//参数类型-文件（@dir）
}
C_ScriptFrame_Param::~C_ScriptFrame_Param(){
}
/*-------------------------------------------------
		运算符重载
*/
const bool C_ScriptFrame_Param::operator== (const C_ScriptFrame_Param& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_ScriptFrame_Param::getJsonObject(){
	QJsonObject obj = QJsonObject();
	obj.insert("name", this->name);
	obj.insert("parent", this->parent);
	obj.insert("type", this->type);
	obj.insert("desc", this->desc);
	obj.insert("default_", this->default_);

	obj.insert("min", this->min);
	obj.insert("max", this->max);
	obj.insert("on", this->on);
	obj.insert("off", this->off);
	QJsonArray arr_option = QJsonArray();
	for (int i = 0; i < this->option.count(); i++){ 
		arr_option.append(this->option.at(i)); 
	}
	obj.insert("option", arr_option);
	obj.insert("require", this->require);
	obj.insert("dir", this->dir);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_ScriptFrame_Param::setJsonObject(QJsonObject obj){

	if (obj.value("name").isUndefined() == false){ this->name = obj.value("name").toString(); }
	if (obj.value("parent").isUndefined() == false){ this->parent = obj.value("parent").toString(); }
	if (obj.value("type").isUndefined() == false){ this->type = obj.value("type").toString(); }
	if (obj.value("desc").isUndefined() == false){ this->desc = obj.value("desc").toString(); }
	if (obj.value("default_").isUndefined() == false){ this->default_ = obj.value("default_").toString(); }

	if (obj.value("min").isUndefined() == false){ this->min = obj.value("min").toInt(); }
	if (obj.value("max").isUndefined() == false){ this->max = obj.value("max").toInt(); }
	if (obj.value("on").isUndefined() == false){ this->on = obj.value("on").toString(); }
	if (obj.value("off").isUndefined() == false){ this->off = obj.value("off").toString(); }
	if (obj.value("option").isUndefined() == false){
		QJsonArray arr_option = QJsonArray();
		this->option = QStringList();
		for (int i = 0; i < arr_option.count(); i++){ this->option.append(arr_option.at(i).toString()); }
	}
	if (obj.value("require").isUndefined() == false){ this->require = obj.value("require").toInt(); }
	if (obj.value("dir").isUndefined() == false){ this->dir = obj.value("dir").toString(); }
}
