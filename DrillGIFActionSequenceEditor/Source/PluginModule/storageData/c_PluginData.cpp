#include "stdafx.h"
#include "C_PluginData.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		������� ʵ����.cpp
		�汾��		v1.04
		���ߣ�		drill_up
		����ģ�飺	���ģ��
		���ܣ�		�洢�����Ϣ���࣬��Ӧplugin.js��ÿ��������������ݡ�

		˵����		1.���ڵײ����QJsonDocument����д��˳��һ�����ҡ�
					2.parameters��ֵ������ �ַ���object ��
-----==========================================================-----
*/
C_PluginData::C_PluginData(){
	this->name = "";
	this->status = false;
	this->description = "";
	this->parameters = QJsonObject();

	this->temp_inited = false;
	this->temp_version = "";			//�汾(��ȡʱ�ų�ʼ����
	this->temp_type = "";				//���ͣ���ȡʱ�ų�ʼ����
	this->temp_name = "";				//����������ȡʱ�ų�ʼ����
}
C_PluginData::~C_PluginData(){
}



//���� - ��ʼ����ȡ��
void C_PluginData::initDescription_data(){
	this->temp_inited = true;

	// > �汾
	if (this->description.isEmpty() == false){			//�����ݷ����Ž�ȡ���ݣ�
		QString data = this->description.trimmed();
		QChar ch = data.at(0);
		if (ch == '[' || ch == '(' || ch == '��' || data.at(0) == 'v' || data.at(0) == 'V'){
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

	// > ����
	QStringList data_list = this->description.split(" - ");	//��ȡ��ܵ�������ݣ�
	if (data_list.count() >= 2){
		QStringList data_list2 = data_list.at(0).trimmed().split(QRegExp("[ \\]\\)]+"));
		this->temp_type = data_list2.last();
		if (this->temp_type.contains(".")){ this->temp_type = ""; }		//���������ַ�������ȥ����
		if (this->temp_type.contains("[")){ this->temp_type = ""; }
		if (this->temp_type.contains("]")){ this->temp_type = ""; }
		if (this->temp_type.contains("-")){ this->temp_type = ""; }
	}

	// > ������
	QStringList data_list2 = this->description.split(" ");
	this->temp_name = data_list2.last();
}
//���� - ��ȡ�汾��������� ��ȡ�Σ�
QString C_PluginData::getDescription_version(){
	if (this->temp_inited == false){ this->initDescription_data(); }
	return this->temp_version;
}
double C_PluginData::getDescription_versionNum(){
	if (this->temp_inited == false){ this->initDescription_data(); }
	if (this->temp_version.isEmpty()){ return -1; }
	return TTool::_to_double_(this->temp_version);
}
//���� - ��ȡ���ͣ�������� ��ȡ�Σ�
QString C_PluginData::getDescription_type(){
	if (this->temp_inited == false){ this->initDescription_data(); }
	return this->temp_type;
}
//���� - ��ȡ��������������� ��ȡ�Σ�
QString C_PluginData::getDescription_name(){
	if (this->temp_inited == false){ this->initDescription_data(); }
	return this->temp_name;
}


/*-------------------------------------------------
		���ж�
*/
bool C_PluginData::isNull(){
	return this->name == "";
}
/*-------------------------------------------------
		���������
*/
const bool C_PluginData::operator== (const C_PluginData& a)const {
	return this->name == a.name;
}
/*-------------------------------------------------
		ʵ���� -> QJsonObject
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
		QJsonObject -> ʵ����
*/
void C_PluginData::setJsonObject(QJsonObject obj){
	this->name = obj.value("name").toString();
	this->status = obj.value("status").toBool();
	this->description = obj.value("description").toString();
	this->parameters = obj.value("parameters").toObject();
}
