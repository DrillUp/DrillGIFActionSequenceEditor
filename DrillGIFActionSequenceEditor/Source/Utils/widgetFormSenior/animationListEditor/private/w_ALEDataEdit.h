#ifndef W_ALEDataEdit_H
#define W_ALEDataEdit_H

#include <QDialog>
#include "../c_ALEData.h"
#include "ui_W_ALEDataEdit.h"

/*
-----==========================================================-----
		�ࣺ		����֡�༭ ����.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭����֡��һЩ���á�
					����ϸ��cpp��
-----==========================================================-----
*/
class W_ALEDataEdit : public QDialog
{
	Q_OBJECT

	public:
		W_ALEDataEdit(QWidget *parent = 0 );
		~W_ALEDataEdit();
		
	//-----------------------------------
	//----�ؼ�		
	public:
		bool m_slotBlock;
	public:
										//�س�����
		void keyPressEvent(QKeyEvent *event);
	public slots:
										//ѡ���ļ������ţ�
		void chooseFile();
										//�ؼ� - ѡ������
		void timeSelected_frameUnit(QString text);
										//�ؼ� - �༭����
		void timeEdited_frameUnit(int value);
										//�ؼ� - ѡ������
		void timeSelected_secondUnit(QString text);
										//�ؼ� - �༭����
		void timeEdited_secondUnit(double value);

	//-----------------------------------
	//----����
	protected:
		C_ALEData local_data;
		int local_index;
		QList<int> local_index_list;
	public:
										//���� - �������ݣ��޸ģ�
		void setDataInModifyMode_One(C_ALEData data, int index);
		void setDataInModifyMode_Multi(C_ALEData data, QList<int> index_list);
										//���� - ȡ������
		C_ALEData getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_ALEDataEdit ui;
	//-----------------------------------
};

#endif // W_ALEDataEdit_H
