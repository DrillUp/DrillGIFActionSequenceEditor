#ifndef P_PlayingPart_H
#define P_PlayingPart_H

#include <QtWidgets>
#include "ui_P_PlayingPart.h"

/*
-----==========================================================-----
		�ࣺ		��ӳ��.h
		���ߣ�		drill_up
		����ģ�飺	��������ģ��
		���ܣ�		�ò����ṩ�������еķ�ӳ��
					����ϸ��cpp��
-----==========================================================-----
*/
class P_PlayingPart : public QWidget
{
	Q_OBJECT

	public:
		P_PlayingPart(QWidget *parent = 0);
		~P_PlayingPart();
		
	//-----------------------------------
	//----�ؼ�
	public:
										//�ؼ� - ��ʼ����
		void startPlay();
										//�ؼ� - ��������
		void endPlay();

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
		Ui::P_PlayingPart ui;

};

#endif // P_PlayingPart_H
