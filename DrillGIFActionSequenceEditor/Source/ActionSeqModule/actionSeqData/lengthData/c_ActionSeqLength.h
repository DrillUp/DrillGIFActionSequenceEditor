#pragma once


/*
-----==========================================================-----
		�ࣺ		���ֵ ������.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���ֵ �������ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class C_ActionSeqLength{

	public:
		C_ActionSeqLength();
		~C_ActionSeqLength();
		
	//-----------------------------------
	//----����
	public:
		int realLen_actionSeq;			//���� - ��������
		int realLen_action;				//���� - ����Ԫ
		int realLen_state;				//���� - ״̬Ԫ

	//-----------------------------------
	//----������
	public:
									//���ж�
		bool isNull();
									//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
};