#pragma once

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
class S_ActionSeqDataContainer : public QObject
{
	Q_OBJECT

	public:
		S_ActionSeqDataContainer();
		~S_ActionSeqDataContainer();
		static S_ActionSeqDataContainer* cur_manager;			//����
		static S_ActionSeqDataContainer* getInstance();			//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩

	//-----------------------------------
	//----����
	private:
		C_PluginData* data_ActionSeq;
	public:
										//���� - ���裨�Զ��Ӳ�����ݶ�ȡ����ȥ�ã�
		void resetPluginData();
										//���� - ��ȡ��������ݣ�
		C_PluginData* getActionSeqData();
										//���� - ��ȡ������ļ���
		QFileInfo getActionSeqPluginFile();
	signals:
										//���� - ���ݱ仯���źţ�
		void actionSeqDataChanged();

};
