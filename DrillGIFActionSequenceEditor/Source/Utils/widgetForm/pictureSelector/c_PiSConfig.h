#pragma once


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
		bool zeroFill;				//�����
		int zeroFillCount;			//���λ��
		QChar zeroFillChar;			//����ַ�

		int height;					//�߶ȣ��ؼ����ã�
		bool isMultiSelect;			//��ѡ����

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