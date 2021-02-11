#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		�ࣺ		rmmv�������� ʵ����.h
		�汾��		v1.00
		����ģ�飺	����ģ��
		���ܣ�		���̻�����Ϣ���ݡ�
					����ϸ��.cpp��
-----==========================================================-----
*/
class C_RmmvProjectData{

	public:
		C_RmmvProjectData();
		~C_RmmvProjectData();
		
	//-----------------------------------
	//----����
	public:
		QString name;				//������
		QString version;			//���̰汾
		QString path;				//���̸�·��
	
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
		const bool operator== (const C_RmmvProjectData& a)const;
};
