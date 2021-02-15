#include "stdafx.h"
#include "s_StorageManager.h"



/*
-----==========================================================-----
		�ࣺ		�洢������.cpp
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		�����м̳�ģ�����������ݣ��洢�ļ�/��ȡ�ļ���
			
		��Ҫ���	1.�洢���������̳�ģ�塣��s_StorageManagerTemplate��
					  ���û�м̳У���û��Ч����

		����˵����	1.ģ����Զ�ʶ����������Ϊ�洢����
					  ��Ҳ�����ֶ��޸�save_name��
					2.������ͨ��ָ��ֱ�Ӳ������࣬û�з���ֵ��
					3.�洢�ṹ���£�
						{
							"S_StorageManager":{����},
							"S_SourceManager":{����},
							"S_RequestManager":{����},
							"C_GlobalSettings":{����},
						}
			
		ʹ�÷�����	
				>�洢��
					S_StorageManager->getInstance()->createSaveFile( �� );
				>��ȡ��
					S_StorageManager->getInstance()->readSaveFile( �� );
				>�����
					S_StorageManager->getInstance()->clearAllApplicationData();
				>�����ȡ�洢�ļ�·����
					this->m_storage_fileInfo
-----==========================================================-----
*/

S_StorageManager::S_StorageManager(){
}
S_StorageManager::~S_StorageManager() {
}

/* --------------------------------------------------------------
----------StorageManager ����
*/
S_StorageManager* S_StorageManager::cur_manager = NULL;
S_StorageManager* S_StorageManager::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_StorageManager();
	}
	return cur_manager;
}


void S_StorageManager::addManager(S_StorageManagerTemplate* s_m) {
	this->m_managers.append(s_m);
}

/* ----------------------------------------------------------------------------------
		���� - �洢�ļ���F:/aaa/bbb.xxx ��
*/
void S_StorageManager::createSaveFile(QString file_name) {
	QJsonObject obj_all = QJsonObject();
	for (int i = 0; i < this->m_managers.count() ;i++) {		//�������ݻ�ȡ
		S_StorageManagerTemplate* s_m = this->m_managers.at(i);
		s_m->m_storage_fileInfo = QFileInfo(file_name);
		QJsonObject obj_manager = s_m->getAllDataOfJsonObject();
		obj_all.insert( s_m->getSaveName() , obj_manager);
	}

	QString context_all = QJsonDocument(obj_all).toJson();
	QFile file(file_name);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate)){
		QMessageBox::warning(this, "����", "�޷������洢�ļ���", QMessageBox::Yes);
		return;
	}
	QTextStream write_stream(&file);
	QTextCodec *codec2 = QTextCodec::codecForName("utf-8");
	write_stream.flush();
	write_stream.setCodec(codec2);
	write_stream.seek(0);						//����0��ʼд��
	write_stream << context_all;
	file.close();
}

/* ----------------------------------------------------------------------------------
		���� - ��ȡ�ļ���F:/aaa/bbb.xxx ��
*/
void S_StorageManager::readSaveFile(QString file_name) {
	QFile file(file_name);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) { 
		QMessageBox::warning(this, "����", "δ�ҵ��ļ���", QMessageBox::Yes);
		return;
	}
	QString context = file.readAll();
	file.close();

	QJsonDocument jsonDocument = QJsonDocument::fromJson(context.toUtf8());
	if (jsonDocument.isNull()){
		QMessageBox::warning(this, "����", "��ȡ�ļ�ʧ�ܡ�", QMessageBox::Yes);
		return;
	}

	QJsonObject obj_all = jsonDocument.object();
	for (int i = 0; i < this->m_managers.count(); i++) {		//�������ݸ�ֵ
		S_StorageManagerTemplate* s_m = this->m_managers.at(i);
		QJsonObject obj_project = obj_all.value(s_m->getSaveName()).toObject();
		s_m->m_storage_fileInfo = QFileInfo(file_name);

		QStringList old_names = s_m->getOldNamesForRead();
		for (int j = 0; j < old_names.count(); j++) {
			QString old_name = old_names.at(j);
			if (obj_project.isEmpty()){
				obj_project = obj_all.value(old_name).toObject();
			}
		}

		s_m->setAllDataFromJsonObject(obj_project);
	}
													
}


/* ----------------------------------------------------------------------------------
		���� - ������г�������
*/
void S_StorageManager::clearAllApplicationData() {

	for (int i = 0; i < this->m_managers.count(); i++) {		//�������ݸ�ֵ
		S_StorageManagerTemplate* s_m = this->m_managers.at(i);
		s_m->clearAllData();
	}
}
