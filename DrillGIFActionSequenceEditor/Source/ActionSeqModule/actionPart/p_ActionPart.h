#ifndef P_ActionPart_H
#define P_ActionPart_H

#include <QtWidgets>
#include "ui_P_ActionPart.h"

/*
-----==========================================================-----
		�ࣺ		����Ԫ��.h
		���ߣ�		drill_up
		����ģ�飺	����Ԫģ��
		���ܣ�		�ò����ṩ����Ԫ�ı༭��
					����ϸ��cpp��
-----==========================================================-----
*/
class P_ActionPart : public QWidget
{
	Q_OBJECT

	public:
		P_ActionPart(QWidget *parent = 0);
		~P_ActionPart();
		
	//-----------------------------------
	//----�ؼ�
	public:
		

	//-----------------------------------
	//----����
	public:
		QJsonObject local_actionData;
	public:
										//���� - ��������
		void setData(QJsonObject actionData);
										//���� - ȡ������
		QJsonObject getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	private:
		Ui::P_ActionPart ui;

};

#endif // P_ActionPart_H
