#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		�ࣺ		ѡ�������� ����.h
		����ģ�飺	����ģ��
		���ܣ�		����ѡ�����Ĵ������ò�����
-----==========================================================-----
*/
class C_ObjectSelectorConfig{

	public:
		C_ObjectSelectorConfig();
		~C_ObjectSelectorConfig();
		
	//-----------------------------------
	//----��������
	public:
		int rowHeight;				//�и�
		bool zeroFill;				//�����
		int zeroFillCount;			//���λ��
		QChar zeroFillChar;			//����ַ�

		int pagePerNum;					//ID - ÿҳ����
		bool showHavingDataInitials;	//���� - ��ʾ����ĸ
		
	//-----------------------------------
	//----��������
	public:
		bool excludeFirstData;		//�ų���һ��
		bool addNullValue;			//��һ�����ݱ�ʾ ��
		
	//-----------------------------------
	//----������
	public:
									//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
};
