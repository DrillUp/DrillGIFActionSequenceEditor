#ifndef W_FCT_Classify_H
#define W_FCT_Classify_H

#include "ui_W_FCT_Classify.h"
#include "c_FCT_Classify.h"
#include "../p_FlexibleClassificationTree.h"

/*
-----==========================================================-----
		�ࣺ		���� ���ô���.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��������������ڸô������á�
-----==========================================================-----
*/

class W_FCT_Classify : public QDialog
{
	Q_OBJECT

	public:
		W_FCT_Classify(P_FlexibleClassificationTree* p_tree);
		~W_FCT_Classify();
		
	//-----------------------------------
	//----���ؼ�
	protected:
		P_FlexibleClassificationTree* m_parentObj;

	//-----------------------------------
	//----����
	public:
		bool isAddMode;
		C_FCT_Classify* m_dataPtr;
	public:
										//���� - �������ݣ���ӣ�
		void setDataInAddMode();
										//���� - �������ݣ��޸ģ�
		void setDataInModifyMode(C_FCT_Classify* p);
										//���� - ȡ������
		C_FCT_Classify* getData();
										//���� - �������� -> ui����
		virtual void putDataToUi();							
										//���� - ui���� -> ��������
		virtual void putUiToData();

	public slots:
										//���� - У������
		virtual bool checkData();
										//���� - �ύ����
		void acceptData();	

	private:
		Ui::W_FCT_Classify ui;
};

#endif // W_FCT_Classify_H
