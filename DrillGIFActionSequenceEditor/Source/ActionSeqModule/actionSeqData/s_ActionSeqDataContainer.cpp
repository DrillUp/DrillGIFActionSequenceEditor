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
	this->data_ActionSeq = nullptr;
	this->data_treeConfig = QJsonObject();

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
	this->data_ActionSeq = nullptr;

	QList<C_PluginData*> data_list = S_PluginDataContainer::getInstance()->getPluginData();
	for (int i = 0; i < data_list.count(); i++){
		C_PluginData* data = data_list.at(i);
		if (data->name == "Drill_CoreOfActionSequence"){		//��GIF�������к��ģ�
			this->data_ActionSeq = data;
		}
	}

	if (this->data_ActionSeq == nullptr){
		if (QMessageBox::information(nullptr, "��ʾ", "�ù����У�û���ҵ�\"GIF�������к���\"�Ĳ�����ã��Ƿ���ӣ�", "���", "ȡ��", 0, 1) == 0){
			//...
		}
	}

	emit actionSeqDataChanged();
}

/*-------------------------------------------------
		������� - ��ȡ��������ݣ�
*/
C_PluginData* S_ActionSeqDataContainer::getActionSeqData() {
	return this->data_ActionSeq;
}
/*-------------------------------------------------
		������� - ��ȡ������ļ���
*/
QFileInfo S_ActionSeqDataContainer::getActionSeqPluginFile() {
	return S_RmmvDataContainer::getInstance()->getRmmvFile_Plugin("Drill_CoreOfActionSequence");
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
	this->data_treeConfig = QJsonObject();
}
/*-----------------------------------
		���� - ȫ������Դ���� -> QJsonObject
*/
QJsonObject S_ActionSeqDataContainer::getAllDataOfJsonObject(){
	QJsonObject obj_all = QJsonObject();

	obj_all.insert("data_treeConfig", this->data_treeConfig);	//����������
	
	return obj_all;
}

/*-----------------------------------
		���� - QJsonObject -> ȫ������Դ����
*/
void S_ActionSeqDataContainer::setAllDataFromJsonObject(QJsonObject obj_all){
	this->clearAllData();

	this->data_treeConfig = obj_all.value("data_treeConfig").toObject();	//����������

	emit dataAllReloaded();
}
