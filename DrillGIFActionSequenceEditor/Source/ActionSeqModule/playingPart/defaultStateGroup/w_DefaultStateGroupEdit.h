#ifndef W_DefaultStateGroupEdit_H
#define W_DefaultStateGroupEdit_H

#include <QtWidgets/QDialog>
#include "ui_W_DefaultStateGroupEdit.h"
#include "Source/Utils/widgetForm/radioTable/p_RadioTable.h"

/*
-----==========================================================-----
		�ࣺ		Ĭ��״̬Ԫ���� �༭.h
		����ģ�飺	��������ģ��
		���ܣ�		Ĭ��״̬Ԫ���ϵĴ��ڡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class W_DefaultStateGroupEdit : public QDialog
{
	Q_OBJECT

	public:
		W_DefaultStateGroupEdit(QWidget *parent = 0);	//���캯��
		~W_DefaultStateGroupEdit();						//��������
		
	//-----------------------------------
	//----�ؼ�
	public:
		P_RadioTable* m_p_RadioTable;
	public:
									//�ؼ� - ѡ�б仯
		void selectedChanged(QList<int> index_list);
		
	//-----------------------------------
	//----����
	public slots:
									//���� - ��������
		void setData(QStringList data,QStringList selected);
									//���� - ��ȡ����
		QStringList getData();
									//���� - �������� -> ui����
									//���� - ui���� -> ��������
									//���� - �ύ���ݣ�У�飩
	private:
		Ui::W_DefaultStateGroupEdit ui;
		
};

#endif // W_QExpressionTest_H
