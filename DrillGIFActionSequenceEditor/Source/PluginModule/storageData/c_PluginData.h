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
	//----����
	public:
		QString name;					//�����
		bool status;					//״̬
		QString description;			//�������
		QJsonObject parameters;			//����
	
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
