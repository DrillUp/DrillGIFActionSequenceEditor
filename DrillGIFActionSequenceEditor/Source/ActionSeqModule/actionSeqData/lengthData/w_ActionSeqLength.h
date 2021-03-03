#ifndef W_ActionSeqLength_H
#define W_ActionSeqLength_H

#include "ui_W_ActionSeqLength.h"
#include "c_ActionSeqLength.h"

/*
-----==========================================================-----
		�ࣺ		���ֵ ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭ ���ֵ ���ڵ�һЩ���á�
					����ϸ��cpp��
-----==========================================================-----
*/
class W_ActionSeqLength : public QDialog
{
	Q_OBJECT

	public:
		W_ActionSeqLength(QWidget *parent = 0 );
		~W_ActionSeqLength();
		
	//-----------------------------------
	//----�ؼ�
	public:
										//�س�����
		void keyPressEvent(QKeyEvent *event);
		

	//-----------------------------------
	//----����
	protected:
		C_ActionSeqLength local_data;
	public:
										//���� - �������ݣ��޸ģ�
		void setDataInModifyMode(C_ActionSeqLength data);
										//���� - ȡ������
		C_ActionSeqLength getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_ActionSeqLength ui;
	//-----------------------------------
};

#endif // W_ActionSeqLength_H
