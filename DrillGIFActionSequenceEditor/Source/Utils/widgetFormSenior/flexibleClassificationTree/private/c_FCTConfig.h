#pragma once


/*
-----==========================================================-----
		�ࣺ		���� ������.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���������� ���� �����ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class C_FCTConfig{

	public:
		C_FCTConfig();
		~C_FCTConfig();
		
	//-----------------------------------
	//----��������
	public:
		int rowHeight;				//�и�
		bool zeroFill;				//�����
		int zeroFillCount;			//���λ��
		QChar zeroFillChar;			//����ַ�

		int pagePerNum;				//��id����
		QString sortType;			//���� - ��������ʽ
		
	//-----------------------------------
	//----������
	public:
									//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
};