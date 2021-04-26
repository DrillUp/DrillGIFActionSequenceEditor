#ifndef P_MouseResizeButtonPart_H
#define P_MouseResizeButtonPart_H

#include <QtWidgets>
#include "ui_P_MouseResizeButtonPart.h"
#include "../p_MouseResizeController.h"

/*
-----==========================================================-----
		�ࣺ		������� ��ť��.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	ͼ��ģ��
		���ܣ�		�ṩ������ŵ� ��ť�ؼ��顣
					����ϸ��cpp��
-----==========================================================-----
*/
class P_MouseResizeButtonPart : public QWidget
{
	Q_OBJECT

	public:
		P_MouseResizeButtonPart(QWidget *parent = 0);
		~P_MouseResizeButtonPart();

	//-----------------------------------
	//----�ؼ�
	public slots:
										//�ؼ� - �仯
		void zoomValueChanged(double value);
		
	//-----------------------------------
	//----������
	protected:
		P_MouseResizeController* m_p_MouseResizeController;
	public slots:
										//������ - ��ʼ��
		void initWidget(P_MouseResizeController* controller);
		
	//-----------------------------------
	//----��
	private:
		Ui::P_MouseResizeButtonPart ui;

};

#endif // P_MouseResizeButtonPart_H
