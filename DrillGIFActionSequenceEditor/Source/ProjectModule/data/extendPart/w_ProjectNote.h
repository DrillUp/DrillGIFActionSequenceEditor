#ifndef W_ProjectNote_H
#define W_ProjectNote_H

#include "ui_w_ProjectNote.h"

/*
-----==========================================================-----
		�ࣺ		��Ŀ��ע �༭����.h
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		��Ŀ��ע�ı��ı༭���ڡ�
-----==========================================================-----
*/
class W_ProjectNote : public QDialog
{
	Q_OBJECT

	public:
		W_ProjectNote(QWidget *parent = 0);
		~W_ProjectNote();
		
	//-----------------------------------
	//----�ؼ�
	public slots:
										//�ؼ� - �༭
		void clickedBtn(QAbstractButton * btn);
		
	//-----------------------------------
	//----����
	public:
		//bool isAddMode;
		QString local_ProjectNote;
	public:
										//���� - ��������
		void setData(QString p);
										//���� - ȡ������
		QString getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();
	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	
	private:
		Ui::W_ProjectNote ui;

};
#endif // W_ProjectNote_H