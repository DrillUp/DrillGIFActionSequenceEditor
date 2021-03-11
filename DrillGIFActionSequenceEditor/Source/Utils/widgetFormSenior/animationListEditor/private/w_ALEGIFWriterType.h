#ifndef W_ALEGIFWriterType_H
#define W_ALEGIFWriterType_H

#include <QDialog>
#include "ui_W_ALEGIFWriterType.h"

/*
-----==========================================================-----
		�ࣺ		ѡ��GIF��ȡ�� ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		����ѡ����Ĵ��ڡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class W_ALEGIFWriterType : public QDialog
{
	Q_OBJECT

	public:
		W_ALEGIFWriterType(QWidget *parent = 0 );
		~W_ALEGIFWriterType();

	//-----------------------------------
	//----����
	protected:
		int local_index;
	public:
										//���� - �������ݣ��޸ģ�
		void setDataInModifyMode(int index);
										//���� - ȡ������
		int getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_ALEGIFWriterType ui;
	//-----------------------------------
};

#endif // W_ALEGIFWriterType_H
