#pragma once

#include "Source/Utils/widgetFastForm/fastForm/c_FastClass.h"

/*
-----==========================================================-----
		�ࣺ		�������� �����ඨ����.h
		����ģ�飺	��������ģ��
		���ܣ�		�������еĿ����ඨ�塣
-----==========================================================-----
*/
class S_ActionSeqDataDefiner : public QObject
{
	Q_OBJECT

	public:
		S_ActionSeqDataDefiner();
		~S_ActionSeqDataDefiner();
		static S_ActionSeqDataDefiner* cur_manager;			//����
		static S_ActionSeqDataDefiner* getInstance();		//��������ȡ�Լ�
		

	//-----------------------------------
	//----�����ඨ��
	public:
										//���� - ����Ԫ ���ݱ�
		C_FastClass getTable_Action();
										//���� - ״̬Ԫ ���ݱ�
		C_FastClass getTable_State();

};

