#ifndef W_SoftwareAbout_H
#define W_SoftwareAbout_H

#include <QtWidgets/QDialog>
#include "ui_W_SoftwareAbout.h"

/*
-----==========================================================-----
		�ࣺ		���ڴ���.h
		����ģ�飺	������ģ��
		���ܣ�		���ڴ��ڡ�
					����ϸ��cpp��
-----==========================================================-----
*/

class W_SoftwareAbout : public QDialog
{
	Q_OBJECT

	public:
		W_SoftwareAbout(QWidget *parent = 0);		//���캯��
		~W_SoftwareAbout();							//��������
		
	//-----------------------------------
	//----�ؼ�		
	public:		
									//�ؼ� - �����ĵ�
		void openUserManual();
		
	//-----------------------------------
	//----����
									//���� - ��������
									//���� - ��ȡ����
									//���� - �������� -> ui����
									//���� - ui���� -> ��������
									//���� - �ύ���ݣ�У�飩
	private:
		Ui::W_SoftwareAbout ui;
		
};

#endif // W_QExpressionTest_H
