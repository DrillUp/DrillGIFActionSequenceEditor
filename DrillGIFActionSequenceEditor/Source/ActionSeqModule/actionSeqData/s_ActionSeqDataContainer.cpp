#include "stdafx.h"
#include "s_ActionSeqDataContainer.h"

#include "Source/PluginModule/storageData/s_PluginDataContainer.h"
#include "Source/RmmvInteractiveModule/custom/s_RmmvDataContainer.h"

/*
-----==========================================================-----
		�ࣺ		�������� ��������.cpp
		����ģ�飺	��������ģ��
		���ܣ�		�������е�����������
					������ʹ�ò�������е����ݡ�
			
		ʹ�÷�����
				>��ʼ����
					S_ActionSeqDataContainer::getInstance();
-----==========================================================-----
*/
S_ActionSeqDataContainer::S_ActionSeqDataContainer(){
	this->data_ActionSeq = nullptr;

	//-----------------------------------
	//----�¼���
	connect(S_PluginDataContainer::getInstance(), &S_PluginDataContainer::pluginDataChanged, this, &S_ActionSeqDataContainer::resetPluginData);

}
S_ActionSeqDataContainer::~S_ActionSeqDataContainer() {
}

/* --------------------------------------------------------------
----------PluginDataManager ����
*/
S_ActionSeqDataContainer* S_ActionSeqDataContainer::cur_manager = NULL;
S_ActionSeqDataContainer* S_ActionSeqDataContainer::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_ActionSeqDataContainer();
	}
	return cur_manager;
}


/*-------------------------------------------------
		���� - ����
*/
void S_ActionSeqDataContainer::resetPluginData() {
	this->data_ActionSeq = nullptr;

	QList<C_PluginData*> data_list = S_PluginDataContainer::getInstance()->getPluginData();
	for (int i = 0; i < data_list.count(); i++){
		C_PluginData* data = data_list.at(i);
		if (data->name == "Drill_CoreOfActionSequence"){		//��GIF�������к��ģ�
			this->data_ActionSeq = data;
		}
	}

	if (this->data_ActionSeq == nullptr){
		if (QMessageBox::information(nullptr, "��ʾ", "�ù����У�û���ҵ�\"GIF�������к���\"�Ĳ�����ã��Ƿ���ӣ�", "���", "ȡ��", 0, 1) == 0){
			//...
		}
	}

	emit actionSeqDataChanged();
}

/*-------------------------------------------------
		���� - ��ȡ��������ݣ�
*/
C_PluginData* S_ActionSeqDataContainer::getActionSeqData() {
	return this->data_ActionSeq;
}
/*-------------------------------------------------
		���� - ��ȡ������ļ���
*/
QFileInfo S_ActionSeqDataContainer::getActionSeqPluginFile() {
	return S_RmmvDataContainer::getInstance()->getRmmvFile_Plugin("Drill_CoreOfActionSequence");
}