#ifndef W_FPT_Config_H
#define W_FPT_Config_H

#include "ui_W_FPT_Config.h"
#include "c_FPT_Config.h"

/*
-----==========================================================-----
		�ࣺ		������ ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭ ������ ���ڵ�һЩ���á�
					����ϸ��cpp��
-----==========================================================-----
*/
class W_FPT_Config : public QDialog
{
	Q_OBJECT

	public:
		W_FPT_Config( QWidget *parent = 0 );
		~W_FPT_Config();
		
	//-----------------------------------
	//----�ؼ�
	public:
		QStyledItemDelegate* m_delegate;
	public:
										//�ؼ� - �س�����
		void keyPressEvent(QKeyEvent *event);
	public slots:
										//�ؼ� - ����乴ѡ�仯
		void zeroFillChanged(bool enable);

	//-----------------------------------
	//----����
	protected:
		C_FPT_Config* local_data;
	public:
										//���� - ��������
		void setData(C_FPT_Config* data);
										//���� - ȡ������
		C_FPT_Config* getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();
	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	
	private:
		Ui::W_FPT_Config ui;
};

#endif // W_FPT_Config_H
