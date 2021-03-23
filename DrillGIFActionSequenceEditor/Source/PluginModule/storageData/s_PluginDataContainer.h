#pragma once

#include "c_PluginData.h"

/*
-----==========================================================-----
		�ࣺ		������� ����.h
		�汾��		v1.02
		���ߣ�		drill_up
		����ģ�飺	���ģ��
		���ܣ�		����plugin.js�е�ȫ������������ݡ�
					����װ�������ṹ��ע����ն�ȡ���źš�
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
		QList<C_PluginData*> data_PluginDataTank;
	public:
										//���� - ��ȡȫ���������
										//		��˵��������ȡ����ǰ��Ҫȷ����֮ǰ �ֶ� ִ����read��ȡ����Ȼ��ȡ�������ݡ�
		QList<C_PluginData*> getPluginDataTank();
										//���� - ��ȡ��Ӧ�Ĳ������
										//		��˵��������ȡ����ǰ��Ҫȷ����֮ǰ �ֶ� ִ����read��ȡ����Ȼ��ȡ�������ݡ�
										//		��˵������û���򷵻ؿ�ָ�롣
		C_PluginData* getPluginData(QString pluginName);
	signals:
										//���� - ���ݱ仯���źţ�
		void pluginDataReloaded();

	//-----------------------------------
	//----����
	public:
										//���� - ���
		void op_add(C_PluginData* data);
										//���� - �޸�
		void op_modify(C_PluginData* data);
										//���� - ɾ��
		void op_remove(C_PluginData* data);


	//-----------------------------------
	//----��ȡ
	public:
										//��ȡ - ��ȡ���ݣ�plugins.js�ı���
		void loadPluginData(QString data_context);
										//��ȡ - ��ȡ���ݣ�plugins.js�ı��������źţ�
		void loadPluginDataNoSignal(QString data_context);
	private:
										//��ȡ - ��ȡ���ݣ�˽�У�
		QList<C_PluginData*> readPluginData(QString data_context);
		
	//-----------------------------------
	//----д��
	public:
										//д�� - д�����ݣ�plugins.js�ı���
		QString writePluginData();



};
