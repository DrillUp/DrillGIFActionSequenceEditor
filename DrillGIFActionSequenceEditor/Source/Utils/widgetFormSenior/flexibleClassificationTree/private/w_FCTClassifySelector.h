#ifndef W_FCTClassifySelector_H
#define W_FCTClassifySelector_H

#include "ui_W_FCTClassifySelector.h"
#include "C_FCTClassify.h"

/*
-----==========================================================-----
		�ࣺ		���� ѡ�񴰿�.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���͵����������ڸô������á�
-----==========================================================-----
*/

class W_FCTClassifySelector : public QDialog
{
	Q_OBJECT

	public:
		W_FCTClassifySelector(QWidget *parent = nullptr);
		~W_FCTClassifySelector();
		
	//-----------------------------------
	//----�ؼ�
	public:
										//�ؼ� - �򿪴���
		void openWindowAddClassify();
	signals:
										//������ͣ��źţ�
		void classifyAdded(C_FCTClassify classify);

	//-----------------------------------
	//----����
	protected:
		QList<C_FCTClassify>* m_classifyList;
	public:
										//���� - �������ݣ��޸ģ�
		void setDataInModifyMode(QString selected_name, QList<C_FCTClassify>* dataFrom );
										//���� - ȡ������
		C_FCTClassify getSelectedData();

	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_FCTClassifySelector ui;
	//-----------------------------------
};

#endif // W_FCTClassifySelector_H
