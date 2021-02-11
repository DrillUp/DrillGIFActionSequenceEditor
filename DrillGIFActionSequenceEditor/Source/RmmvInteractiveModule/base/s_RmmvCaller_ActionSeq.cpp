#include "stdafx.h"
#include "s_RmmvCaller_ActionSeq.h"

#include "Source/PluginModule/storageData/s_PluginDataContainer.h"

/*
-----==========================================================-----
		�ࣺ		rmmv�����ļ�������.cpp
		����ģ�飺	rmmv��ȡģ��
		���ܣ�		�������������ģ�顣

		Ŀ�꣺		-> ��rmmv�����ļ�
					-> 
					
		ʹ�÷�����
				>�򿪣�
					
			
-----==========================================================-----
*/

S_RmmvCaller_ActionSeq::S_RmmvCaller_ActionSeq(){
	this->init();
}
S_RmmvCaller_ActionSeq::~S_RmmvCaller_ActionSeq() {
}

/* --------------------------------------------------------------
----------RmmvFileManager ����
*/
S_RmmvCaller_ActionSeq* S_RmmvCaller_ActionSeq::cur_manager = NULL;
S_RmmvCaller_ActionSeq* S_RmmvCaller_ActionSeq::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_RmmvCaller_ActionSeq();
	}
	return cur_manager;
}

/* --------------------------------------------------------------
----------��ʼ��
*/
void S_RmmvCaller_ActionSeq::init(){
}

/*-------------------------------------------------
		���� - ��ȡȫ����ع����ļ�
*/
bool S_RmmvCaller_ActionSeq::loadAllRmmvFile(C_RmmvProjectData rmmvProjectData) {
	if (rmmvProjectData.isNull()){ return false; }

	// > ��ȡ����ļ�
	QFile plugin_file(rmmvProjectData.path + "/js/plugins.js");
	if (!plugin_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "����", "δ�ҵ�plugins.js�ļ���", QMessageBox::Yes);
		return false;
	}
	QString plugin_context = plugin_file.readAll();
	this->readPluginData(plugin_context);
	plugin_file.close();

	// > ��ȡϵͳ�ļ�
	QFile system_file(rmmvProjectData.path + "/data/System.json");
	if (!system_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QMessageBox::warning(nullptr, "����", "δ�ҵ�system.js�ļ���", QMessageBox::Yes);
		return false;
	}
	QString system_context = system_file.readAll();
	this->readSystemData(system_context);
	system_file.close();

	return true;
}

/*-------------------------------------------------
		���� - ��ȡ�ļ� - plugin.js
*/
void S_RmmvCaller_ActionSeq::readPluginData(QString context) {

	S_PluginDataContainer::getInstance()->resetPluginDataFromText(context);
}

/*-------------------------------------------------
		���� - ��ȡ�ļ� - System.json
*/
void S_RmmvCaller_ActionSeq::readSystemData(QString context) {
	//...
}


/*-------------------------------------------------
		���� - ������������ǵ�rmmv��
*/
void S_RmmvCaller_ActionSeq::saveDataToRmmv(C_RmmvProjectData rmmvProjectData){
	//���麯��ֱ�Ӹ�д��

}