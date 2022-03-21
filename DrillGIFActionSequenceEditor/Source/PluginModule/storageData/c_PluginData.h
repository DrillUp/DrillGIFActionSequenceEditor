#pragma once
#include <QList>
#include <QJsonObject>

/*
-----==========================================================-----
		�ࣺ		������� ʵ����.h
		�汾��		v1.04
		���ߣ�		drill_up
		����ģ�飺	���ģ��
		���ܣ�		�洢�����Ϣ���࣬��Ӧplugin.js��ÿ��������������ݡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class C_PluginData{

	public:
		C_PluginData();
		~C_PluginData();
		
	//-----------------------------------
	//----����
	public:
		QString name;					//�����
		bool status;					//״̬
		QString description;			//�������
		QJsonObject parameters;			//����
	
	//-----------------------------------
	//----��ʱ�ض�����
	protected:
		bool temp_inited;				//���
		QString temp_version;			//�汾(��ȡʱ�ų�ʼ����
		QString temp_type;				//���ͣ���ȡʱ�ų�ʼ����
		QString temp_name;				//����������ȡʱ�ų�ʼ����
	public:
										//���� - ��ʼ����ȡ��
		void initDescription_data();
										//���� - ��ȡ�汾��������� ��ȡ�Σ�
		QString getDescription_version();
		double getDescription_versionNum();
										//���� - ��ȡ���ͣ�������� ��ȡ�Σ�
		QString getDescription_type();
										//���� - ��ȡ��������������� ��ȡ�Σ�
		QString getDescription_name();

	//-----------------------------------
	//----������
	public:
										//���ж�
		bool isNull();	
										//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
										//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
										//���������
		const bool operator== (const C_PluginData& a)const;
};
