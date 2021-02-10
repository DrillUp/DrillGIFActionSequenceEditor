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
class C_FCTClassify{

	public:
		C_FCTClassify();		//�ն���
		C_FCTClassify(QString name, QString description);
		~C_FCTClassify();

	public:
		QString name;			//���ƣ������ظ���
		QString description;	//����
		
	//-----------------------------------
	//----������
	public:
									//���������
		const bool operator== (const C_FCTClassify& a)const;
									//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
									//���ж�
		bool isNull();
};