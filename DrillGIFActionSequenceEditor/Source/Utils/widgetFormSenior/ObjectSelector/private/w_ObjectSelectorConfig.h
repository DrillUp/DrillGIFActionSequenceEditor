#ifndef W_ObjectSelectorConfig_H
#define W_ObjectSelectorConfig_H

#include "ui_W_ObjectSelectorConfig.h"
#include "c_ObjectSelectorConfig.h"

/*
-----==========================================================-----
		�ࣺ		����ѡ�� ����.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭����ѡ�񴰿ڵ�һЩ���á�
					����ϸ��cpp��
-----==========================================================-----
*/
class W_ObjectSelectorConfig : public QDialog
{
	Q_OBJECT

	public:
		W_ObjectSelectorConfig(QWidget *parent = 0 );
		~W_ObjectSelectorConfig();
		
	public:
										//�س�����
		void keyPressEvent(QKeyEvent *event);
		
	public slots:
										//����乴ѡ�仯
		void zeroFillChanged(bool enable);							

	//-----------------------------------
	//----����
	protected:
		C_ObjectSelectorConfig local_data;
	public:
										//���� - �������ݣ��޸ģ�
		void setDataInModifyMode(C_ObjectSelectorConfig data);
										//���� - ȡ������
		C_ObjectSelectorConfig getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_ObjectSelectorConfig ui;
	//-----------------------------------
};

#endif // W_ObjectSelectorConfig_H
