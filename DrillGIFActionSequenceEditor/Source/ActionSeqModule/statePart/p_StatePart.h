#ifndef P_StatePart_H
#define P_StatePart_H

#include <QtWidgets>
#include "ui_P_StatePart.h"

/*
-----==========================================================-----
		�ࣺ		״̬Ԫ��.h
		���ߣ�		drill_up
		����ģ�飺	״̬Ԫģ��
		���ܣ�		�ò����ṩ״̬Ԫ�༭���ܡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class P_StatePart : public QWidget
{
	Q_OBJECT

	public:
		P_StatePart(QWidget *parent = 0);
		~P_StatePart();
		
	//-----------------------------------
	//----�ؼ�
	public:
		

	//-----------------------------------
	//----����
	public:
		QJsonObject local_state;
	public:
										//���� - ��������
		void setData(QJsonObject state);
										//���� - ȡ������
		QJsonObject getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	private:
		Ui::P_StatePart ui;

};

#endif // P_StatePart_H
