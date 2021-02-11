#pragma once

#include "c_PluginData.h"

/*
-----==========================================================-----
		�ࣺ		��� �洢��������.h
		����ģ�飺	���ģ��
		���ܣ�		����༭���洢���ݵ�������
					����ϸ��.cpp��
-----==========================================================-----
*/
class S_PluginDataContainer : public QObject
{
	Q_OBJECT

	public:
		S_PluginDataContainer();
		~S_PluginDataContainer();
		static S_PluginDataContainer* cur_manager;			//����
		static S_PluginDataContainer* getInstance();			//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩

	//-----------------------------------
	//----����
	private:
		QList<C_PluginData*> data_PluginDatas;
	public:
										//���� - ���裨�����б�
		void resetPluginData(QList<C_PluginData*> data_list);
										//���� - ���裨plugins.js�ı���
		void resetPluginDataFromText(QString data_context);
										//���� - ��ȡ
		QList<C_PluginData*> getPluginData();
	signals:
										//���� - ���ݱ仯���źţ�
		void pluginDataChanged();

};
