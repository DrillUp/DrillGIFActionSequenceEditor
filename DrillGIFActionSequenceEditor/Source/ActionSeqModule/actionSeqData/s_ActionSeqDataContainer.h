#pragma once
#include "Source/ProjectModule/storage/s_StorageManagerTemplate.h"

#include "Source/PluginModule/storageData/c_PluginData.h"

/*
-----==========================================================-----
		�ࣺ		�������� ��������.h
		����ģ�飺	��������ģ��
		���ܣ�		�������е�����������
					������ʹ�ò�������е����ݡ�
					����ϸ��.cpp��
-----==========================================================-----
*/
class S_ActionSeqDataContainer : public QObject, public S_StorageManagerTemplate
{
	Q_OBJECT

	public:
		S_ActionSeqDataContainer();
		~S_ActionSeqDataContainer();
		static S_ActionSeqDataContainer* cur_manager;			//����
		static S_ActionSeqDataContainer* getInstance();			//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩

	//-----------------------------------
	//----������ݣ�ֻ�������棩
	private:
		C_PluginData* data_ActionSeq;
	public:
										//������� - ���裨�Զ��Ӳ�����ݶ�ȡ����ȥ�ã�
		void resetPluginData();
										//������� - ��ȡ��������ݣ�
		C_PluginData* getActionSeqData();
										//������� - ��ȡ������ļ���
		QFileInfo getActionSeqPluginFile();
	signals:
										//���� - ���ݱ仯���źţ�
		void actionSeqDataChanged();
		
	//-----------------------------------
	//----������
	private:
		QJsonObject data_treeConfig;
	public:
										//������ - �޸�
		void modifyTreeData(QJsonObject tree);
										//������ - ��ȡ
		QJsonObject getTreeData();

	//-----------------------------------
	//----����
	public:
										//���� - ��ȡ�������ͼƬ·����F:/xxx/xxx/Special__actionSeq��
										//		��˵����������rmmv���ļ�·����rmmv������ݶ��ڽ���ģ�顣
		QString getActionSeqDir();
		
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
