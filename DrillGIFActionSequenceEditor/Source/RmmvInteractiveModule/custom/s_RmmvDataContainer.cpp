#include "stdafx.h"
#include "s_RmmvDataContainer.h"

/*
-----==========================================================-----
		�ࣺ		rmmv���� ��������.cpp
		����ģ�飺	����ģ��
		���ܣ�		�洢������ݵ�������	
-----==========================================================-----
*/
S_RmmvDataContainer::S_RmmvDataContainer(){
	this->clearAllData();
}
S_RmmvDataContainer::~S_RmmvDataContainer() {
}

/* --------------------------------------------------------------
----------RmmvFileManager ����
*/
S_RmmvDataContainer* S_RmmvDataContainer::cur_container = NULL;
S_RmmvDataContainer* S_RmmvDataContainer::getInstance() {
	if (cur_container == NULL) {
		cur_container = new S_RmmvDataContainer();
	}
	return cur_container;
}

/*-------------------------------------------------
		���� - �޸�
*/
void S_RmmvDataContainer::modify(C_RmmvProjectData data){
	this->m_RmmvProjectData = data;
}
/*-------------------------------------------------
		���� - ��ȡ
*/
C_RmmvProjectData S_RmmvDataContainer::getRmmvProjectData(){
	return this->m_RmmvProjectData;
}

/*-------------------------------------------------
		���� - ��ȡ����ļ�
*/
QFileInfo S_RmmvDataContainer::getRmmvFile_Plugin(QString plugin_name){
	QString name = plugin_name.replace(".js","");
	return QFileInfo(this->m_RmmvProjectData.path + "/js/plugins/" + plugin_name+ ".js");
}

/*-----------------------------------
		���� - ��ȡ�洢������
*/
QString S_RmmvDataContainer::getSaveName() {
	return "S_RmmvDataContainer";
}
/*-----------------------------------
		���� - �����ǰ����������
*/
void S_RmmvDataContainer::clearAllData() {
	this->m_RmmvProjectData = C_RmmvProjectData();
}
/*-----------------------------------
		���� - ȫ������Դ���� -> QJsonObject
*/
QJsonObject S_RmmvDataContainer::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	obj_all.insert("C_RmmvProjectData", this->m_RmmvProjectData.getJsonObject());	//rmmv��������
	
	return obj_all;
}

/*-----------------------------------
		���� - QJsonObject -> ȫ������Դ����
*/
void S_RmmvDataContainer::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	QJsonObject obj_RmmvProjectData = obj_all.value("C_RmmvProjectData").toObject();	//rmmv��������
	this->m_RmmvProjectData.setJsonObject(obj_RmmvProjectData);

	emit dataAllReloaded();
}
