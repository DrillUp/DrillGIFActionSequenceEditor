#include "stdafx.h"
#include "c_FCT_Config.h"

#include "../p_FlexibleClassificationTree.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		树设置 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		灵活分类树的 树设置 数据类。
					
		子功能：
					->空对象
					
-----==========================================================-----
*/
C_FCT_Config::C_FCT_Config() : C_FPT_Config(){

	// > 分支模式
	this->m_mode = "种类分支（按id递增排序）";
	this->m_modeList << "种类分支（按id递增排序）" << "种类分支（按名称递增排序）";

	// > 种类页
	this->m_emptyClassify = new C_FCT_Classify();		//未分类
	this->m_classifyList = QList<C_FCT_Classify*>();
	this->checkClassifyList();
}
C_FCT_Config::~C_FCT_Config(){
}


/*-------------------------------------------------
		分支模式 - 判断 种类分支_ID递增 模式
*/
bool C_FCT_Config::is_classify_idInc_Mode(){
	return this->m_mode == "种类分支（按id递增排序）";
}
/*-------------------------------------------------
		分支模式 - 判断 种类分支_名称递增 模式
*/
bool C_FCT_Config::is_classify_nameInc_Mode(){
	return this->m_mode == "种类分支（按名称递增排序）";
}

/*-------------------------------------------------
		种类页 - 获取
*/
QList<C_FCT_Classify*> C_FCT_Config::get_classify_DataList(){
	return this->m_classifyList;
}
/*-------------------------------------------------
		种类页 - 清理列表
*/
void C_FCT_Config::clear_classify_DataList(){
	this->m_classifyList.clear();
	this->checkClassifyList();
}

/*-------------------------------------------------
		种类页 - 添加数据（基函数）
*/
void C_FCT_Config::add_classify(C_FCT_Classify* data){
	if (data->getName() == ""){ return; }
	if (this->m_classifyList.contains(data)){ return; }
	this->checkClassifyList();

	int index = this->m_classifyList.count() - 1;	//（数据列表中至少有一个）
	this->m_classifyList.insert(index, data);		//（在倒数第二个位置添加种类）
}
/*-------------------------------------------------
		种类页 - 删除数据（基函数）
*/
void C_FCT_Config::remove_classify(QString classify_name){
	if (classify_name == ""){ return; }

	int index = this->get_classify_Index(classify_name);
	if (index != -1){
		this->m_classifyList.removeAt(index);
	}
}
/*-------------------------------------------------
		种类页 - 数据上移
*/
void C_FCT_Config::set_classify_moveUp(QString classify_name){

	int index = this->get_classify_Index(classify_name);
	if (index == -1){ return; }
	if (index == 0){ return; }
	if (index == this->get_classify_Count() - 1){ return; }
	this->m_classifyList.swap(index, index - 1);
}
/*-------------------------------------------------
		种类页 - 数据下移
*/
void C_FCT_Config::set_classify_moveDown(QString classify_name){

	int index = this->get_classify_Index(classify_name);
	if (index == -1){ return; }
	if (index >= this->get_classify_Count() - 2){ return; }
	this->m_classifyList.swap(index, index + 1);
}

/*-------------------------------------------------
		种类页 - 获取
*/
C_FCT_Classify* C_FCT_Config::get_classify(QString classify_name){
	for (int i = 0; i < this->m_classifyList.count(); i++){
		C_FCT_Classify* c_c = this->m_classifyList.at(i);
		if (c_c->getName() == classify_name){
			return c_c;
		}
	}
	return nullptr;
}
/*-------------------------------------------------
		种类页 - 获取 - 种类页索引
*/
int C_FCT_Config::get_classify_Index(QString classify_name){
	if (classify_name == "未分类"){ classify_name = ""; }	//（未分类与空页性质一样）
	for (int i = 0; i < this->m_classifyList.count(); i++){
		if (this->m_classifyList.at(i)->getName() == classify_name){
			return i;
		}
	}
	return -1;
}
/*-------------------------------------------------
		种类页 - 获取 - 数量
*/
int C_FCT_Config::get_classify_Count(){
	return this->m_classifyList.count();
}
/*-------------------------------------------------
		种类页 - 获取 - 判断种类存在
*/
bool C_FCT_Config::has_classify(QString classify_name){
	return this->get_classify_Index(classify_name) != -1;
}
/*-------------------------------------------------
		种类页 - 获取 - 全部种类
*/
QStringList C_FCT_Config::getAll_classify_NameList(){
	QStringList result_list = QStringList();
	for (int i = 0; i < this->m_classifyList.count(); i++){
		result_list.append(this->m_classifyList.at(i)->getName());
	}
	return result_list;
}
/*-------------------------------------------------
		私有 - 检查列表
*/
void C_FCT_Config::checkClassifyList(){
	if (this->m_classifyList.contains(this->m_emptyClassify) != true){
		this->m_classifyList.append(this->m_emptyClassify);
	}
}


/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_FCT_Config::getJsonObject(){

	// > 种类数据（存储在 this->data 中）
	QJsonArray arr = QJsonArray();
	for (int i = 0; i < this->m_classifyList.count(); i++){
		C_FCT_Classify* c_c = this->m_classifyList.at(i);
		if (c_c->isNull() == false){		//（排除"未分类"）
			arr.append(c_c->getJsonObject());
		}
	}
	this->data.insert("FCT_ClassifyList", arr);

	// > 对象
	QJsonObject obj = C_FPT_Config::getJsonObject();
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_FCT_Config::setJsonObject(QJsonObject obj, P_FlexiblePageTree* parent_obj){
	C_FPT_Config::setJsonObject(obj, parent_obj);

	// > 父对象强转
	P_FlexibleClassificationTree* parent_obj_ex = dynamic_cast<P_FlexibleClassificationTree*>(parent_obj);

	// > 种类数据（存储在 this->data 中）
	this->m_classifyList.clear();
	if (this->data.value("FCT_ClassifyList").isUndefined() == false){
		QJsonArray arr = this->data.value("FCT_ClassifyList").toArray();
		for (int i = 0; i < arr.count(); i++){
			C_FCT_Classify* c_c = parent_obj_ex->createClassifyData();
			c_c->setJsonObject(arr.at(i).toObject());
			this->m_classifyList.append(c_c);
		}
	}
	// > 旧数据情况
	if (obj.value("FCTClassifyList").isUndefined() == false){
		QJsonArray arr = obj.value("FCTClassifyList").toArray();
		for (int i = 0; i < arr.count(); i++){
			C_FCT_Classify* c_c = parent_obj_ex->createClassifyData();
			c_c->setJsonObject(arr.at(i).toObject());
			this->m_classifyList.append(c_c);
		}
	}

	this->checkClassifyList();
}