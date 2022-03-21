#pragma once

#include "c_PluginData.h"

/*
-----==========================================================-----
		�ࣺ		������� ����.h
		�汾��		v1.04
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
										//��ȡ - һ���Զ�ȡ���ݣ�plugins.js�ı�����ȡ��ֱ�ӷ����б�������������
		QList<C_PluginData*> loadPluginDataDirectly(QString data_context);
	signals:
										//��ȡ - �����Ѷ�ȡ���źţ�
		void pluginDataReloaded();
	private:
										//��ȡ - ��ȡ���ݣ�˽�У�
		QList<C_PluginData*> readPluginDataPrivate(QString data_context);
		

	//-----------------------------------
	//----д��
	public:
										//д�� - д�����ݣ�plugins.js�ı���
		QString writePluginData();
										//д�� - һ����д�����ݣ�plugins.js�ı������������������ݣ�
		QString writePluginDataDirectly(QList<C_PluginData*> data_list);
	private:
										//д�� - д�����ݣ�˽�У�
		QString writePluginDataPrivate(QList<C_PluginData*> data_list);


};
