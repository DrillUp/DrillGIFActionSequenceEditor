#ifndef W_ALEGIFReaderType_H
#define W_ALEGIFReaderType_H

#include <QDialog>
#include "ui_W_ALEGIFReaderType.h"

/*
-----==========================================================-----
		�ࣺ		ѡ��GIF��ȡ�� ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		����ѡ����Ĵ��ڡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class W_ALEGIFReaderType : public QDialog
{
	Q_OBJECT

	public:
		W_ALEGIFReaderType(QWidget *parent = 0 );
		~W_ALEGIFReaderType();

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
		Ui::W_ALEGIFReaderType ui;
	//-----------------------------------
};

#endif // W_ALEGIFReaderType_H
