#ifndef W_FCTConfig_H
#define W_FCTConfig_H

#include "ui_W_FCTConfig.h"
#include "c_FCTConfig.h"

/*
-----==========================================================-----
		�ࣺ		������ ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭ ������ ���ڵ�һЩ���á�
					����ϸ��cpp��
-----==========================================================-----
*/
class W_FCTConfig : public QDialog
{
	Q_OBJECT

	public:
		W_FCTConfig(QWidget *parent = 0 );
		~W_FCTConfig();
		
	//-----------------------------------
	//----�ؼ�
	public:
										//�س�����
		void keyPressEvent(QKeyEvent *event);
		
	public slots:
										//����乴ѡ�仯
		void zeroFillChanged(bool enable);				
										//�Զ����֧ģʽ����
		void setModeType(QString cur_type, QStringList type_list);

	//-----------------------------------
	//----����
	protected:
		C_FCTConfig local_data;
	public:
										//���� - �������ݣ��޸ģ�
		void setDataInModifyMode(C_FCTConfig data);
										//���� - ȡ������
		C_FCTConfig getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_FCTConfig ui;
	//-----------------------------------
};

#endif // W_FCTConfig_H
