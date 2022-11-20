#pragma once
#include "stdafx.h"


/*
-----==========================================================-----
		�ࣺ		�������� ������.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ѡ�������� �������ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class C_RaTConfig{

	public:
		C_RaTConfig();
		~C_RaTConfig();
		
	//-----------------------------------
	//----��������
	public:
		bool showNumber;			//��ʾ����
		bool zeroFill;				//������
		int zeroFillCount;			//����λ��
		QChar zeroFillChar;			//�����ַ�

		int rowHeight;				//�и�
		bool isMultiSelect;			//��ѡ����

	//-----------------------------------
	//----������
	public:
									//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
};