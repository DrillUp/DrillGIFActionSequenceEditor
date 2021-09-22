#pragma once
#include <QList>
#include <QJsonObject>

/*
-----==========================================================-----
		�ࣺ		������� ʵ����.h
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
	//----�洢����
	public:
		QString name;					//�洢���� - �����
		bool status;					//�洢���� - ״̬
		QString description;			//�洢���� - �������
		QJsonObject parameters;			//�洢���� - ����
	
	//-----------------------------------
	//----������
	public:
												//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
												//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
												//���������
		const bool operator== (const C_PluginData& a)const;
};
