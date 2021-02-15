#pragma once
#include "Source/ProjectModule/storage/s_StorageManagerTemplate.h"

#include "../base/c_RmmvProjectData.h"

/*
-----==========================================================-----
		�ࣺ		rmmv���� ��������.h
		����ģ�飺	����ģ��
		���ܣ�		�洢������ݵ�������
					����ϸ��.cpp��
-----==========================================================-----
*/

class S_RmmvDataContainer : public QObject, public S_StorageManagerTemplate
{
	Q_OBJECT

	public:
		S_RmmvDataContainer();
		~S_RmmvDataContainer();
		static S_RmmvDataContainer* cur_container;		//����
		static S_RmmvDataContainer* getInstance();		//��������ȡ�Լ�
		
	//-----------------------------------
	//----����
	private:
		C_RmmvProjectData m_RmmvProjectData;
	public:
										//���� - �޸�
		void modify(C_RmmvProjectData data);
										//���� - ��ȡ
		C_RmmvProjectData getRmmvProjectData();
	
	//-----------------------------------
	//----�����ļ�
	public:
										//���� - ��ȡ����ļ�
		QFileInfo getRmmvFile_Plugin(QString plugin_name);
		

	//-----------------------------------
	//----�ź�
	signals:
										//�ź� - �洢���ݱ���ȡ
		void dataAllReloaded();

	//-----------------------------------
	//----�洢���ݣ��̳У�
	public:
										//���� - ��ȡ�洢������
		QString getSaveName();
										//���� - �����ǰ����������
		void clearAllData();
										//���� - ȫ������Դ���� -> QJsonObject
		QJsonObject getAllDataOfJsonObject();
										//���� - QJsonObject -> ȫ������Դ����
		void setAllDataFromJsonObject(QJsonObject obj);
};
