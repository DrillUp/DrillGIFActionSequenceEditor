#ifndef W_FCT_ClassifySelector_H
#define W_FCT_ClassifySelector_H

#include "ui_W_FCT_ClassifySelector.h"
#include "c_FCT_Classify.h"
#include "../p_FlexibleClassificationTree.h"

/*
-----==========================================================-----
		�ࣺ		���� ѡ�񴰿�.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��������������ڸô������á�
-----==========================================================-----
*/

class W_FCT_ClassifySelector : public QDialog
{
	Q_OBJECT

	public:
		W_FCT_ClassifySelector(P_FlexibleClassificationTree* p_obj);
		~W_FCT_ClassifySelector();
		
	//-----------------------------------
	//----���ؼ�
	protected:
		P_FlexibleClassificationTree* m_parentObj;

	//-----------------------------------
	//----����
	protected:
		C_FCT_Config* m_configPtr;
	public:
										//���� - �������ݣ��޸ģ�
		void setData( C_FCT_Config* config, QString last_selectedName = "");
										//���� - ȡ������
		QString getSelectedData();

	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_FCT_ClassifySelector ui;
	//-----------------------------------
};

#endif // W_FCT_ClassifySelector_H
