#pragma once
#include "stdafx.h"


/*
-----==========================================================-----
		�ࣺ		ͼƬ���� ������.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		ͼƬ���� �������ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class C_PiSConfig{

	public:
		C_PiSConfig();
		~C_PiSConfig();
		
	//-----------------------------------
	//----��������
	public:
		bool m_zeroFill;				//������
		int m_zeroFillCount;			//����λ��
		QChar m_zeroFillChar;			//�����ַ�
		bool m_isMaskEnabled;			//�����˱���

		int m_height;					//�߶ȣ��ؼ����ã�
		bool m_isMultiSelect;			//��ѡ����

	//-----------------------------------
	//----������
	public:
									//��ȡͼƬ�߶�
		int getItemHeight();
									//��ȡ�����ַ�����\n �ſ�ѡ�е� ��ɫ���飩
		QString getLineExpand();
									//ʵ���� -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> ʵ����
		virtual void setJsonObject(QJsonObject obj);
};