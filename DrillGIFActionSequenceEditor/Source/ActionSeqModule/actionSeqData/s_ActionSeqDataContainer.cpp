#include "stdafx.h"
#include "s_ActionSeqDataContainer.h"

#include "Source/ProjectModule/file/s_TempFileManager.h"
#include "Source/PluginModule/storageData/s_PluginDataContainer.h"
#include "Source/RmmvInteractiveModule/custom/s_RmmvDataContainer.h"

/*
-----==========================================================-----
		�ࣺ		�������� ��������.cpp
		����ģ�飺	��������ģ��
		���ܣ�		�������е�����������
					������ʹ�ò�������е����ݡ�
			
		ʹ�÷�����
				>��ʼ����
					S_ActionSeqDataContainer::getInstance();
-----==========================================================-----
*/
S_ActionSeqDataContainer::S_ActionSeqDataContainer(){
	this->clearAllData();

	//-----------------------------------
	//----�¼���
	connect(S_PluginDataContainer::getInstance(), &S_PluginDataContainer::pluginDataChanged, this, &S_ActionSeqDataContainer::resetPluginData);

}
S_ActionSeqDataContainer::~S_ActionSeqDataContainer() {
}

/* --------------------------------------------------------------
----------PluginDataManager ����
*/
S_ActionSeqDataContainer* S_ActionSeqDataContainer::cur_manager = NULL;
S_ActionSeqDataContainer* S_ActionSeqDataContainer::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_ActionSeqDataContainer();
	}
	return cur_manager;
}


/*-------------------------------------------------
		������� - ����
*/
void S_ActionSeqDataContainer::resetPluginData() {
	this->clearAllData();

	QList<C_PluginData*> data_list = S_PluginDataContainer::getInstance()->getPluginData();
	for (int i = 0; i < data_list.count(); i++){
		C_PluginData* data = data_list.at(i);
		if (data->name == "Drill_CoreOfActionSequence"){		//��GIF�������к��ģ�
			this->data_ActionSeqPlugin = data;
		}
	}

	if (this->data_ActionSeqPlugin == nullptr){
		//if (QMessageBox::information(nullptr, "��ʾ", "�ù����У�û���ҵ�\"GIF�������к���\"�Ĳ�����ã��Ƿ���ӣ�", "���", "ȡ��", 0, 1) == 0){
		//	//...
		//}
	}

	emit dataAllReloaded();
}

/*-------------------------------------------------
		������� - ��ȡ��������ݣ�
*/
C_PluginData* S_ActionSeqDataContainer::getActionSeqPlugin() {
	return this->data_ActionSeqPlugin;
}
/*-------------------------------------------------
		������� - ��ȡ������ļ���
*/
QFileInfo S_ActionSeqDataContainer::getActionSeqPluginFile() {
	return S_RmmvDataContainer::getInstance()->getRmmvFile_Plugin("Drill_CoreOfActionSequence");
}

/*-------------------------------------------------
		������������ - ����
*/
void S_ActionSeqDataContainer::setActionSeqData(QJsonObject obj){
	this->data_ActionSeqData = obj;
}
/*-------------------------------------------------
		������������ - ��ȡ
*/
QJsonObject S_ActionSeqDataContainer::getActionSeqData(){

	// > ���Ϊ��rmmv�ж�ȡ�����ݣ�������
	if (this->data_ActionSeqData.isEmpty()){
		if (this->data_ActionSeqPlugin != nullptr){
			this->data_ActionSeqData = this->data_ActionSeqPlugin->parameters;
		}
	}

	// > Ĭ������浵�ڵ�����
	return this->data_ActionSeqData;
}

/*-------------------------------------------------
		���� - ��ȡ�������ͼƬ·��
*/
QString S_ActionSeqDataContainer::getActionSeqDir(){
	QString dir_path = S_TempFileManager::getInstance()->getTempFileUrl() +"/Special__actionSeq";
	QDir dir(dir_path);
	if (dir.exists() == false){ dir.mkdir(dir_path); }
	return dir_path;
}


/*-------------------------------------------------
		������ - �޸�
*/
void S_ActionSeqDataContainer::modifyTreeData(QJsonObject tree){
	this->data_treeConfig = tree;
}
/*-------------------------------------------------
		������ - ��ȡ
*/
QJsonObject S_ActionSeqDataContainer::getTreeData(){
	return this->data_treeConfig;
}

/*-----------------------------------
		���� - ��ȡ�洢������
*/
QString S_ActionSeqDataContainer::getSaveName() {
	return "S_ActionSeqDataContainer";
}
/*-----------------------------------
		���� - �����ǰ����������
*/
void S_ActionSeqDataContainer::clearAllData() {
	this->data_ActionSeqPlugin = nullptr;
	this->data_treeConfig = QJsonObject();
	this->data_ActionSeqData = QJsonObject();
}
/*-----------------------------------
		���� - ȫ������Դ���� -> QJsonObject
*/
QJsonObject S_ActionSeqDataContainer::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	obj_all.insert("data_treeConfig", this->data_treeConfig);			//����������
	obj_all.insert("data_ActionSeqData", this->data_ActionSeqData);		//������������
	
	return obj_all;
}

/*-----------------------------------
		���� - QJsonObject -> ȫ������Դ����
*/
void S_ActionSeqDataContainer::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	this->data_treeConfig = obj_all.value("data_treeConfig").toObject();		//����������
	this->data_ActionSeqData = obj_all.value("data_ActionSeqData").toObject();	//������������

	emit dataAllReloaded();
}
