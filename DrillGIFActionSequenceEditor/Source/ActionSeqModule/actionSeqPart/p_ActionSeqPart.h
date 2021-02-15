#ifndef P_ActionSeqPart_H
#define P_ActionSeqPart_H

#include <QtWidgets>
#include "ui_P_ActionSeqPart.h"

#include "Source/ActionSeqModule/actionPart/p_ActionPart.h"
#include "Source/ActionSeqModule/statePart/p_StatePart.h"

/*
-----==========================================================-----
		�ࣺ		�������п�.h
		���ߣ�		drill_up
		����ģ�飺	��������ģ��
		���ܣ�		�ò����ṩ�������з�ӳ�����ܣ���Ƕ��״̬Ԫ������Ԫ�Ŀ顣
					����ϸ��cpp��
-----==========================================================-----
*/
class P_ActionSeqPart : public QWidget
{
	Q_OBJECT

	public:
		P_ActionSeqPart(QWidget *parent = 0);
		~P_ActionSeqPart();
		
	//-----------------------------------
	//----�ؼ�
	protected:
		P_ActionPart* m_actionPart;
		P_StatePart* m_statePart;
	public:
										//�ؼ� - ��ʼ����
		void startPlay();
										//�ؼ� - ��������
		void endPlay();

	//-----------------------------------
	//----����
	public:
		QJsonObject local_actionSeq;
	public:
										//���� - ��������
		void setData(QJsonObject actionSeq);
										//���� - ȡ������
		QJsonObject getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	private:
		Ui::P_ActionSeqPart ui;

};

#endif // P_ActionSeqPart_H
