#pragma once
#include "Source/ProjectModule/storage/s_StorageManagerTemplate.h"

#include "lengthData/c_ActionSeqLength.h"
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
	//----��ȡ����ֻ�������棩
	private:
		C_PluginData* data_ActionSeqPlugin;
	public:
										//��ȡ�� - ���������ݣ��Զ��Ӳ�����ݶ�ȡ����ȥ�ã�
		void resetPluginData();
										//��ȡ�� - ��ȡ�������
		C_PluginData* getActionSeqPlugin();
										//��ȡ�� - ��ȡ����ļ�
										//		��˵���������δ�򿪹����ļ����򷵻ص�info�ļ������ڡ�
		QFileInfo getActionSeqPluginFile();
										//��ȡ�� - ��ȡ���ȣ�����ģ�
										//		��˵���������δ�򿪹����ļ����򷵻ص�Ϊnull��
		C_ActionSeqLength getActionSeqPluginLength();
		
	//-----------------------------------
	//----������������
	private:
		C_ActionSeqLength data_ActionSeqLength;		//����
		QJsonObject data_ActionSeqData;				//��������
	public:
										//���� - ��������
		void setActionSeqData(QJsonObject data);
										//���� - ��ȡ����
		QJsonObject getActionSeqData();
										//���� - ���ó���
		void setActionSeqLength(C_ActionSeqLength data);
										//���� - ��ȡ����
		C_ActionSeqLength getActionSeqLength();
										//���� - ���ݳ��Ƚ���һ���հ׵Ķ��������б�
										//		��˵������QJsonObjectΪ�� �� �������е�ֵΪ�� ��������������ﴴ��������Ϊ���ߡ�
		QJsonObject buildEmptyActionSeqData(C_ActionSeqLength data);
										//���� - ��ȡȫ���������ļ�
		QList<QFileInfo> getAllRelatedFile();

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
		
	//-----------------------------------
	//----�ź�
	signals:
										//�ź� - �洢���ݱ���ȡ
		void dataAllReloaded();
};
