#pragma once


/*
-----==========================================================-----
		�ࣺ		��ѡ������� ������.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ѡ������� �������ࡣ
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
		int rowHeight;				//�и�
		bool zeroFill;				//�����
		int zeroFillCount;			//���λ��
		QChar zeroFillChar;			//����ַ�
		
	//-----------------------------------
	//----������
	public:
									//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
};