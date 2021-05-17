#include "stdafx.h"
#include "s_ActionSeqDataDefiner.h"

/*
-----==========================================================-----
		�ࣺ		�������� �����ඨ����.cpp
		����ģ�飺	��������ģ��
		���ܣ�		�������еĿ����ඨ�塣
-----==========================================================-----
*/
S_ActionSeqDataDefiner::S_ActionSeqDataDefiner(){
}
S_ActionSeqDataDefiner::~S_ActionSeqDataDefiner() {
}

/* --------------------------------------------------------------
----------XDS_DatabaseDataDefiner ����
*/
S_ActionSeqDataDefiner* S_ActionSeqDataDefiner::cur_manager = NULL;
S_ActionSeqDataDefiner* S_ActionSeqDataDefiner::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_ActionSeqDataDefiner();
	}
	return cur_manager;
}


/*-----------------------------------
		���� - ����Ԫ ���ݱ�
*/
C_FastClass S_ActionSeqDataDefiner::getTable_Action() {
	C_FastClass result_class;
	
	result_class = C_FastClass("����Ԫ");
	result_class.addQString						("����Ԫ����", "");
	result_class.addInt							("����Ԫ���ȼ�", "20");
	result_class.setInt_UI_ToQSpinBox			("����Ԫ���ȼ�", 0, 999999, 1);
	//	��Դ-״̬Ԫ --> ����֡�༭��
	//	֡��� --> ����֡�༭��
	//	֡���-��ϸ�� --> ����֡�༭��
	result_class.addBoolean						("�Ƿ񵹷�", "false");
	result_class.addInt							("ͼ��-ɫ��ֵ", "0");
	result_class.setInt_UI_ToQSpinBox			("ͼ��-ɫ��ֵ", 0, 360, 1);
	result_class.addBoolean						("ͼ��-ģ����Ե", "false");
	result_class.addQString						("��ע", "");
	result_class.setQString_UI_ToQPlainTextEdit ("��ע",80,true);

	return result_class;
}
/*-----------------------------------
		���� - ״̬Ԫ ���ݱ�
*/
C_FastClass S_ActionSeqDataDefiner::getTable_State() {
	C_FastClass result_class;

	result_class = C_FastClass("״̬Ԫ");
	result_class.addQString						("״̬Ԫ����", "");
	result_class.addInt							("״̬ԪȨ��", "40");
	result_class.setInt_UI_ToQSpinBox			("״̬ԪȨ��", 1, 999999, 1);
	result_class.addInt							("״̬Ԫ���ȼ�", "0");
	result_class.setInt_UI_ToQSpinBox			("״̬Ԫ���ȼ�", 0, 999999, 1);
	//	��Դ-״̬Ԫ --> ����֡�༭��
	//	֡��� --> ����֡�༭��
	//	֡����б� --> ����֡�༭��
	result_class.addBoolean						("�Ƿ񵹷�", "false");
	result_class.addInt							("ͼ��-ɫ��ֵ", "0");
	result_class.setInt_UI_ToQSpinBox			("ͼ��-ɫ��ֵ", 0, 360, 1);
	result_class.addBoolean						("ͼ��-ģ����Ե", "false");
	result_class.addQString						("��ע", "");
	result_class.setQString_UI_ToQPlainTextEdit ("��ע",80,true);

	return result_class;
}
