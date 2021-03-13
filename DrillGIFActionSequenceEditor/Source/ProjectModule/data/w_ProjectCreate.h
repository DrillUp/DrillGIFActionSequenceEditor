#ifndef W_PROJECT_CREATE_H
#define W_PROJECT_CREATE_H

#include "ui_w_ProjectCreate.h"
#include "p_ProjectCreate.h"

/*
-----==========================================================-----
		�ࣺ		�½���Ŀ����.cpp
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		���������̺󣬽��½���Ŀ�ļ��С�
					�����½���Ŀ�����Ϊ��Ŀ��
-----==========================================================-----
*/
class W_ProjectCreate : public QDialog
{
	Q_OBJECT

	public:
		W_ProjectCreate(QWidget* parent = 0);
		~W_ProjectCreate();
		
	//-----------------------------------
	//----�ؼ�
	public:
		P_ProjectCreate* m_P_ProjectCreate;
				
	//-----------------------------------
	//----λ��
	public:
										//λ�� - ���λ��
		void insertPath(QString path);
										//λ�� - ѡ��λ��
		void selectPath(QString path);
										//λ�� - ��ȡѡ���λ��
		QString getCurrentPath();

	//-----------------------------------
	//----����
	public:
										//���� - ��������
		void setDataInModifyMode(C_ProjectData p);
										//���� - ȡ������
		C_ProjectData getData();
	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_ProjectCreate ui;
};

#endif 