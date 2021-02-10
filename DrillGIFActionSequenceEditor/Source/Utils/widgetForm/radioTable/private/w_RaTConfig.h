#ifndef W_RaTConfig_H
#define W_RaTConfig_H

#include "ui_W_RaTConfig.h"
#include "../c_RaTConfig.h"

/*
-----==========================================================-----
		�ࣺ		��ѡ������� ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭ ��ѡ������� ���ڵ�һЩ���á�
					����ϸ��cpp��
-----==========================================================-----
*/
class W_RaTConfig : public QDialog
{
	Q_OBJECT

	public:
		W_RaTConfig(QWidget *parent = 0 );
		~W_RaTConfig();
		
	//-----------------------------------
	//----�ؼ�
	public:
										//�س�����
		void keyPressEvent(QKeyEvent *event);
		
	public slots:
										//����乴ѡ�仯
		void zeroFillChanged(bool enable);	

	//-----------------------------------
	//----����
	protected:
		C_RaTConfig local_data;
	public:
										//���� - �������ݣ��޸ģ�
		void setDataInModifyMode(C_RaTConfig data);
										//���� - ȡ������
		C_RaTConfig getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_RaTConfig ui;
	//-----------------------------------
};

#endif // W_RaTConfig_H
