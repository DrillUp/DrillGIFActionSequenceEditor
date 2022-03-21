#pragma once
#include "Source/ProjectModule/storage/s_StorageManagerTemplate.h"

#include "../base/c_RmmvProjectData.h"

/*
-----==========================================================-----
		�ࣺ		rmmv���� ��������.h
		�汾��		v1.01	�����಻�ٸ��¡�
		����ģ�飺	����ģ��
		���ܣ�		��ֻ�洢��rmmv�������ݵ�������
					���ṩһϵ�й����ļ��Ļ�ȡ���ܡ�
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
		C_RmmvProjectData data_RmmvProjectData;
	public:
										//���� - �޸�
		void modify(C_RmmvProjectData data);
										//���� - ��ȡ
		C_RmmvProjectData getRmmvProjectData();
	
	//-----------------------------------
	//----�����ļ�
	public:
										//���� - ��ȡ�ļ� - ���
		QFileInfo getRmmvFile_Plugin(QString plugin_name);
										//���� - ��ȡ�ļ� - �������
		QFileInfo getRmmvFile_PluginsData();
										//���� - ��ȡ�ļ� - ��ͼ
		QFileInfo getRmmvFile_Map(int i);
		QFileInfo getRmmvFile_MapInfos();
										//���� - ��ȡ�ļ� - ��ɫ
		QFileInfo getRmmvFile_Actors();
										//���� - ��ȡ�ļ� - ְҵ
		QFileInfo getRmmvFile_Classes();
										//���� - ��ȡ�ļ� - ��Ʒ
		QFileInfo getRmmvFile_Items();
										//���� - ��ȡ�ļ� - ����
		QFileInfo getRmmvFile_Skills();
										//���� - ��ȡ�ļ� - ����
		QFileInfo getRmmvFile_Weapons();
										//���� - ��ȡ�ļ� - ����
		QFileInfo getRmmvFile_Armors();
										//���� - ��ȡ�ļ� - ״̬
		QFileInfo getRmmvFile_States();
										//���� - ��ȡ�ļ� - ����
		QFileInfo getRmmvFile_Enemies();
										//���� - ��ȡ�ļ� - ��Ⱥ
		QFileInfo getRmmvFile_Troops();
										//���� - ��ȡ�ļ� - �����¼�
		QFileInfo getRmmvFile_CommonEvents();
										//���� - ��ȡ�ļ� - ����
		QFileInfo getRmmvFile_Animations();
										//���� - ��ȡ�ļ� - ͼ��
		QFileInfo getRmmvFile_Tilesets();
										//���� - ��ȡ�ļ� - ϵͳ
		QFileInfo getRmmvFile_System();
		

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
