#include "stdafx.h"
#include "c_LEAnnotation.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		���ע�� ����.cpp��ֻ����
		����ģ�飺	����ģ��
		���ܣ�		����Ĺ̶��������ݡ�

		˵����		������ʵֻ�漸����Ҫ�����ݡ�
					�����ṹ���ô� ʵ���� �������ݵĸ�����Ͻ����
					�����⣬�����ܱ�������Ψһ�ԣ�����"�׶�-%d"��"�׶�"���������������
-----==========================================================-----
*/
C_LEAnnotation_Param::C_LEAnnotation_Param(){
	this->paramKey = "";				//��������@Drill_LE_param��
	this->parentKey = "";				//���������@Drill_LE_parentKey��
	this->var = "";						//��������@Drill_LE_var��
	this->varLen = 0;					//�������ֵ
	this->realLen = 0;					//ʵ�����ֵ
}
C_LEAnnotation_Param::~C_LEAnnotation_Param(){
}

/*-------------------------------------------------
		���� - ��ʼ��
*/
void C_LEAnnotation_Param::initParam(QString param, QString parentKey, QString var){
	this->paramKey = param;
	this->parentKey = parentKey;
	this->var = var;
}
/*-------------------------------------------------
		��ȡ - ������
*/
QString C_LEAnnotation_Param::getParamKey(){
	return this->paramKey;
}
/*-------------------------------------------------
		��ȡ - �������
*/
QString C_LEAnnotation_Param::getParentKey(){
	return this->parentKey;
}
/*-------------------------------------------------
		��ȡ - ������
*/
QString C_LEAnnotation_Param::getVarName(){
	return this->var;
}
/*-------------------------------------------------
		��ȡ - �������ֵ
*/
void C_LEAnnotation_Param::setVarLen(int len){
	this->varLen = len;
}
int C_LEAnnotation_Param::getVarLen(){
	return varLen;
}
/*-------------------------------------------------
		��ȡ - ʵ�����ֵ
*/
void C_LEAnnotation_Param::setRealLen(int len){
	this->realLen = len;
}
int C_LEAnnotation_Param::getRealLen(){
	return realLen;
}
/*-------------------------------------------------
		��ȡ - ������ʾ��
*/
QString C_LEAnnotation_Param::getParamShowingName(){
	return this->paramKey.split("-").at(0);
}
/*-------------------------------------------------
		��ȡ - ����������1��ʼ������
*/
QString C_LEAnnotation_Param::getParamName(int index){
	QString i_str = QString::number(index);
	QString result = this->paramKey;
	result = result.replace("%d", i_str);
	return result;
}
/*-------------------------------------------------
		��ȡ - ������������1��ʼ������
*/
QString C_LEAnnotation_Param::getParentName(int index){
	if (this->isParentGrouping() == false){ return ""; }
	
	index--;
	int i = floor(index / 20);
	int start = 20 * i + 1;
	int end = 20 * (i + 1);
	QString str_start = QString::number(start);
	if (start == 1){ str_start = " 1"; };
	QString result = this->getParentKey();
	result.replace(result.lastIndexOf("%d"), 2, QString::number(end));
	result.replace("%d", str_start);

	return result;
}
/*-------------------------------------------------
		��ȡ - ����ע������ȫ��ƥ���ã�
*/
QRegExp C_LEAnnotation_Param::getParamCommentRe(int index){
	QString param_re = "@param[ ]*" + this->getParamName(index) + "$";
	return QRegExp(param_re);
}
/*-------------------------------------------------
		��ȡ - ����ע��������ȫ��ƥ���ã�
*/
QRegExp C_LEAnnotation_Param::getParentCommentRe(int index){
	QString parent_re = "@param[ ]*" + this->getParentName(index) + "$";
	return QRegExp(parent_re);
}
/*-------------------------------------------------
		��ȡ - �Ƿ����
*/
bool C_LEAnnotation_Param::isParentGrouping(){
	return this->parentKey.contains("%d");
}

/*
-----==========================================================-----
		�ࣺ		�����Ϣ.cpp��ֻ����
-----==========================================================-----
*/
C_LEAnnotation::C_LEAnnotation(){
	this->pluginName = "";								//�����
	this->pluginDesc = "";								//�����飨@plugindesc��
	this->pluginAuthor = "";							//������ߣ�@author��
	this->paramList = QList<C_LEAnnotation_Param>();	//�������
	this->paramForbidden = false;						//��ֹ�༭��@Drill_LE_editForbidden��

	this->fullPath = "";				//����ļ�·��
	this->context = "";					//����ı�
	this->message = "";					//�����Ϣ����ȡ����ʾ���ַ�����	
	this->editMessage = "";				//����޸���Ϣ���༭�󣬱༭���ݵ��ַ�����
}
C_LEAnnotation::~C_LEAnnotation(){
}
C_LEAnnotation_Param C_LEAnnotation::getParamByKey(QString paramKey){
	for (int i = 0; i < this->paramList.count(); i++){
		C_LEAnnotation_Param p = this->paramList.at(i);
		if (p.getParamKey() == paramKey){
			return p;
		}
	}
	return C_LEAnnotation_Param();
}
/*-------------------------------------------------
		�ж� - ���ж�
*/
bool C_LEAnnotation::isNull(){
	return this->pluginName == "" || this->pluginDesc == "";
}
/*-------------------------------------------------
		�ж� - �Ƿ񱻽���
*/
bool C_LEAnnotation::isForbidden(){
	return this->paramForbidden;
}
/*-------------------------------------------------
		�ж� - �Ƿ�ɱ༭
*/
bool C_LEAnnotation::isEditable(){
	if (this->paramList.count() == 0){ return false; }
	return true;
}
/*-------------------------------------------------
		�ж� - �����Լ��
*/
bool C_LEAnnotation::isIntegrity(){
	for (int i = 0; i < this->paramList.count(); i++){
		C_LEAnnotation_Param pluginParam = this->paramList.at(i);

		if (pluginParam.getRealLen() == 0){ return false; }
		if (pluginParam.getVarName() == "null"){ continue; }
		if (pluginParam.getVarLen() == 0){ return false; }

		// > �Ա�
		if (pluginParam.getRealLen() != pluginParam.getVarLen()){
			return false;
		}
	}
	return true;
}

