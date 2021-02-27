#ifndef W_ALEConfigEdit_H
#define W_ALEConfigEdit_H

#include <QDialog>
#include "../c_ALEConfig.h"
#include "ui_W_ALEConfigEdit.h"

/*
-----==========================================================-----
		�ࣺ		����֡���� ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭����֡������ui���á�
					����ϸ��cpp��
-----==========================================================-----
*/
class W_ALEConfigEdit : public QDialog
{
	Q_OBJECT

	public:
		W_ALEConfigEdit(QWidget *parent = 0 );
		~W_ALEConfigEdit();
		
	//-----------------------------------
	//----�ؼ�		
	public:
		bool m_slotBlock;
	public:
										//�س�����
		void keyPressEvent(QKeyEvent *event);
	public slots:
										//�ؼ� - ��ѡ�л�
		void checkBoxChanged(bool checked);
										//�ؼ� - ѡ������
		void timeSelected(QString text);
										//�ؼ� - �༭����
		void timeEdited(double value);

	//-----------------------------------
	//----����
	protected:
		C_ALEConfig local_config;		//
	public:
										//���� - �������ݣ��޸ģ�
		void setDataInModifyMode(C_ALEConfig config);
										//���� - ȡ������
		C_ALEConfig getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_ALEConfigEdit ui;
	//-----------------------------------
};

#endif // W_ALEConfigEdit_H