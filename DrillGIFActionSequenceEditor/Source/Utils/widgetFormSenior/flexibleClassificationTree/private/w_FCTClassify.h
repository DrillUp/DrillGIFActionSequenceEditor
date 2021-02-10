#ifndef W_FCTClassify_H
#define W_FCTClassify_H

#include "ui_W_FCTClassify.h"
#include "C_FCTClassify.h"

/*
-----==========================================================-----
		�ࣺ		���� ���ô���.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���͵����������ڸô������á�
-----==========================================================-----
*/

class W_FCTClassify : public QDialog
{
	Q_OBJECT

	public:
		W_FCTClassify(QWidget *parent = nullptr);
		~W_FCTClassify();


	//-----------------------------------
	//----����
	public:
		bool isAddMode;
		C_FCTClassify local_FCTClassify;
		QList<C_FCTClassify>* m_checkFrom;
	public:
										//���� - �������ݣ���ӣ�
		void setDataInAddMode(QList<C_FCTClassify>* checkFrom);
										//���� - �������ݣ��޸ģ�
		void setDataInModifyMode(C_FCTClassify p, QList<C_FCTClassify>* checkFrom);
										//���� - ȡ������
		C_FCTClassify getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_FCTClassify ui;
	//-----------------------------------
};

#endif // W_FCTClassify_H
