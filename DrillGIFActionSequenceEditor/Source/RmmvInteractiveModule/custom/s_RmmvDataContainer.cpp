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
	this->data_RmmvProjectData = data;
}
/*-------------------------------------------------
		���� - ��ȡ
*/
C_RmmvProjectData S_RmmvDataContainer::getRmmvProjectData(){
	return this->data_RmmvProjectData;
}

/*-------------------------------------------------
		���� - ��ȡ�ļ�
*/
QFileInfo S_RmmvDataContainer::getRmmvFile_Plugin(QString plugin_name){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	QString name = plugin_name.replace(".js","");
	return QFileInfo(this->data_RmmvProjectData.path + "/js/plugins/" + plugin_name+ ".js");
}
//���� - ��ȡ�ļ� - ��ͼ
QFileInfo S_RmmvDataContainer::getRmmvFile_Map(int i){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	QString name = QString("Map%1").arg(i, 3, 10, QLatin1Char('0'));
	return QFileInfo(this->data_RmmvProjectData.path + "/data/" + name + ".json");
}
QFileInfo S_RmmvDataContainer::getRmmvFile_MapInfos(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/MapInfos.json");
}
//���� - ��ȡ�ļ� - ��ɫ
QFileInfo S_RmmvDataContainer::getRmmvFile_Actors(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Actors.json");
}
//���� - ��ȡ�ļ� - ְҵ
QFileInfo S_RmmvDataContainer::getRmmvFile_Classes(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Classes.json");
}
//���� - ��ȡ�ļ� - ��Ʒ
QFileInfo S_RmmvDataContainer::getRmmvFile_Items(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Items.json");
}
//���� - ��ȡ�ļ� - ����
QFileInfo S_RmmvDataContainer::getRmmvFile_Skills(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Skills.json");
}
//���� - ��ȡ�ļ� - ����
QFileInfo S_RmmvDataContainer::getRmmvFile_Weapons(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Weapons.json");
}
//���� - ��ȡ�ļ� - ����
QFileInfo S_RmmvDataContainer::getRmmvFile_Armors(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Armors.json");
}
//���� - ��ȡ�ļ� - ״̬
QFileInfo S_RmmvDataContainer::getRmmvFile_States(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/States.json");
}
//���� - ��ȡ�ļ� - ����
QFileInfo S_RmmvDataContainer::getRmmvFile_Enemies(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Enemies.json");
}
//���� - ��ȡ�ļ� - ��Ⱥ
QFileInfo S_RmmvDataContainer::getRmmvFile_Troops(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Troops.json");
}
//���� - ��ȡ�ļ� - �����¼�
QFileInfo S_RmmvDataContainer::getRmmvFile_CommonEvents(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/CommonEvents.json");
}
//���� - ��ȡ�ļ� - ����
QFileInfo S_RmmvDataContainer::getRmmvFile_Animations(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Animations.json");
}
//���� - ��ȡ�ļ� - ͼ��
QFileInfo S_RmmvDataContainer::getRmmvFile_Tilesets(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/Tilesets.json");
}
//���� - ��ȡ�ļ� - ϵͳ
QFileInfo S_RmmvDataContainer::getRmmvFile_System(){
	if (this->data_RmmvProjectData.isNull()){ return QFileInfo(); }
	return QFileInfo(this->data_RmmvProjectData.path + "/data/System.json");
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
	this->data_RmmvProjectData = C_RmmvProjectData();
}
/*-----------------------------------
		���� - ȫ������Դ���� -> QJsonObject
*/
QJsonObject S_RmmvDataContainer::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	obj_all.insert("C_RmmvProjectData", this->data_RmmvProjectData.getJsonObject());	//rmmv��������
	
	return obj_all;
}

/*-----------------------------------
		���� - QJsonObject -> ȫ������Դ����
*/
void S_RmmvDataContainer::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	QJsonObject obj_RmmvProjectData = obj_all.value("C_RmmvProjectData").toObject();	//rmmv��������
	this->data_RmmvProjectData.setJsonObject(obj_RmmvProjectData);

	emit dataAllReloaded();
}
