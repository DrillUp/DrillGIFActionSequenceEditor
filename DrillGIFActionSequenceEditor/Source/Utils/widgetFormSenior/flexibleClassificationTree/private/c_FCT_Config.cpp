#include "stdafx.h"
#include "c_FCT_Config.h"

#include "../p_FlexibleClassificationTree.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		������ ������.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���������� ������ �����ࡣ
					
		�ӹ��ܣ�
					->�ն���
					
-----==========================================================-----
*/
C_FCT_Config::C_FCT_Config() : C_FPT_Config(){

	// > ��֧ģʽ
	this->m_modeList << "�����֧����id��������" << "�����֧�������Ƶ�������";

	// > ����ҳ
	this->m_emptyClassify = new C_FCT_Classify();		//δ����
	this->m_classifyList = QList<C_FCT_Classify*>();
	this->checkClassifyList();
}
C_FCT_Config::~C_FCT_Config(){
}


/*-------------------------------------------------
		��֧ģʽ - �ж� �����֧_ID���� ģʽ
*/
bool C_FCT_Config::is_classify_idInc_Mode(){
	return this->m_mode == "�����֧����id��������";
}
/*-------------------------------------------------
		��֧ģʽ - �ж� �����֧_���Ƶ��� ģʽ
*/
bool C_FCT_Config::is_classify_nameInc_Mode(){
	return this->m_mode == "�����֧�������Ƶ�������";
}

/*-------------------------------------------------
		����ҳ - ��ȡ
*/
QList<C_FCT_Classify*> C_FCT_Config::get_classify_DataList(){
	return this->m_classifyList;
}
/*-------------------------------------------------
		����ҳ - �����б�
*/
void C_FCT_Config::clear_classify_DataList(){
	this->m_classifyList.clear();
	this->checkClassifyList();
}

/*-------------------------------------------------
		����ҳ - ������ݣ���������
*/
void C_FCT_Config::add_classify(C_FCT_Classify* data){
	if (data->getName() == ""){ return; }
	if (this->m_classifyList.contains(data)){ return; }
	this->checkClassifyList();

	int index = this->m_classifyList.count() - 1;	//�������б���������һ����
	this->m_classifyList.insert(index, data);		//���ڵ����ڶ���λ��������ࣩ
}
/*-------------------------------------------------
		����ҳ - ɾ�����ݣ���������
*/
void C_FCT_Config::remove_classify(QString classify_name){
	if (classify_name == ""){ return; }

	int index = this->get_classify_Index(classify_name);
	if (index != -1){
		this->m_classifyList.removeAt(index);
	}
}
/*-------------------------------------------------
		����ҳ - ��������
*/
void C_FCT_Config::set_classify_moveUp(QString classify_name){

	int index = this->get_classify_Index(classify_name);
	if (index == -1){ return; }
	if (index == 0){ return; }
	if (index == this->get_classify_Count() - 1){ return; }
	this->m_classifyList.swap(index, index - 1);
}
/*-------------------------------------------------
		����ҳ - ��������
*/
void C_FCT_Config::set_classify_moveDown(QString classify_name){

	int index = this->get_classify_Index(classify_name);
	if (index == -1){ return; }
	if (index >= this->get_classify_Count() - 2){ return; }
	this->m_classifyList.swap(index, index + 1);
}

/*-------------------------------------------------
		����ҳ - ��ȡ
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
		����ҳ - ��ȡ - ����ҳ����
*/
int C_FCT_Config::get_classify_Index(QString classify_name){
	if (classify_name == "δ����"){ classify_name = ""; }	//��δ�������ҳ����һ����
	for (int i = 0; i < this->m_classifyList.count(); i++){
		if (this->m_classifyList.at(i)->getName() == classify_name){
			return i;
		}
	}
	return -1;
}
/*-------------------------------------------------
		����ҳ - ��ȡ - ����
*/
int C_FCT_Config::get_classify_Count(){
	return this->m_classifyList.count();
}
/*-------------------------------------------------
		����ҳ - ��ȡ - �ж��������
*/
bool C_FCT_Config::has_classify(QString classify_name){
	return this->get_classify_Index(classify_name) != -1;
}
/*-------------------------------------------------
		����ҳ - ��ȡ - ȫ������
*/
QStringList C_FCT_Config::getAll_classify_NameList(){
	QStringList result_list = QStringList();
	for (int i = 0; i < this->m_classifyList.count(); i++){
		result_list.append(this->m_classifyList.at(i)->getName());
	}
	return result_list;
}
/*-------------------------------------------------
		˽�� - ����б�
*/
void C_FCT_Config::checkClassifyList(){
	if (this->m_classifyList.contains(this->m_emptyClassify) != true){
		this->m_classifyList.append(this->m_emptyClassify);
	}
}


/*-------------------------------------------------
		ʵ���� -> QJsonObject
*/
QJsonObject C_FCT_Config::getJsonObject(){

	// > �������ݣ��洢�� this->data �У�
	QJsonArray arr = QJsonArray();
	for (int i = 0; i < this->m_classifyList.count(); i++){
		C_FCT_Classify* c_c = this->m_classifyList.at(i);
		if (c_c->isNull() == false){		//���ų�"δ����"��
			arr.append(c_c->getJsonObject());
		}
	}
	this->data.insert("FCT_ClassifyList", arr);

	// > ����
	QJsonObject obj = C_FPT_Config::getJsonObject();
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> ʵ����
*/
void C_FCT_Config::setJsonObject(QJsonObject obj, P_FlexiblePageTree* parent_obj){
	C_FPT_Config::setJsonObject(obj, parent_obj);

	// > ������ǿת
	P_FlexibleClassificationTree* parent_obj_ex = dynamic_cast<P_FlexibleClassificationTree*>(parent_obj);

	// > �������ݣ��洢�� this->data �У�
	this->m_classifyList.clear();
	if (this->data.value("FCT_ClassifyList").isUndefined() == false){
		QJsonArray arr = this->data.value("FCT_ClassifyList").toArray();
		for (int i = 0; i < arr.count(); i++){
			C_FCT_Classify* c_c = parent_obj_ex->createClassifyData();
			c_c->setJsonObject(arr.at(i).toObject());
			this->m_classifyList.append(c_c);
		}
	}
	// > ���������
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