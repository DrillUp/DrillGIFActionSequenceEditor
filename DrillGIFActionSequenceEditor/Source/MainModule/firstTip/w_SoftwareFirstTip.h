#ifndef W_SoftwareFirstTip_H
#define W_SoftwareFirstTip_H

#include <QtWidgets/QDialog>
#include "ui_W_SoftwareFirstTip.h"

/*
-----==========================================================-----
		�ࣺ		��ҳ��ʾ��.h
		����ģ�飺	������ģ��
		���ܣ�		��ҳ��ʾ��
					����ϸ��cpp��
-----==========================================================-----
*/

class W_SoftwareFirstTip : public QDialog
{
	Q_OBJECT

	public:
		W_SoftwareFirstTip(QWidget *parent = 0);		//���캯��
		~W_SoftwareFirstTip();							//��������
		static W_SoftwareFirstTip* cur_instance;		//����
		static W_SoftwareFirstTip* getInstance();		//��������ȡ�Լ�
		void _init();									//��ʼ��
		
	//-----------------------------------
	//----�ؼ�		
	public:		
									//�ؼ� - �����ĵ�
		void openUserManual();
		
	//-----------------------------------
	//----����
	public slots:		
								//��ť���¼�
		void acceptData();
		void rejectData();
								//���ڹر��¼�������رհ�ť��
		void closeEvent(QCloseEvent *event);

	private:
		Ui::W_SoftwareFirstTip ui;
		
};

#endif // W_QExpressionTest_H
