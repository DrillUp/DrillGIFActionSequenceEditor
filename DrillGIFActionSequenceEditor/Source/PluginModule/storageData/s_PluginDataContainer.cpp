#include "stdafx.h"
#include "s_PluginDataContainer.h"


/*
-----==========================================================-----
		�ࣺ		��� �洢��������.cpp
		����ģ�飺	���ģ��
		���ܣ�		����༭���洢���ݵ�������
					����װ�������ṹ��ͨ���ź����ӡ�
		
		ʹ�÷�����
				>��ʼ����
					S_PluginDataContainer::getInstance()->resetPluginDataFromText( data_context );
					S_PluginDataContainer::getInstance()->getPluginData();
				
-----==========================================================-----
*/
S_PluginDataContainer::S_PluginDataContainer(){
	this->data_PluginDatas = QList<C_PluginData*>();
}
S_PluginDataContainer::~S_PluginDataContainer() {
}

/* --------------------------------------------------------------
----------PluginDataManager ����
*/
S_PluginDataContainer* S_PluginDataContainer::cur_manager = NULL;
S_PluginDataContainer* S_PluginDataContainer::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_PluginDataContainer();
	}
	return cur_manager;
}


/*-------------------------------------------------
		���� - ����
*/
void S_PluginDataContainer::resetPluginData(QList<C_PluginData*> data_list) {
	this->data_PluginDatas = data_list;
	emit pluginDataChanged();
}
void S_PluginDataContainer::resetPluginDataFromText(QString data_context){
	QList<C_PluginData*> data_list = QList<C_PluginData*>();

	// > ��ȡ�����ı�
	int start = data_context.indexOf("[");
	int end = data_context.lastIndexOf("]");
	data_context = data_context.mid(start, end - start + 1);

	// > jsonת��
	QJsonDocument jsonDocument = QJsonDocument::fromJson(data_context.toUtf8());
	if (jsonDocument.isNull()){
		QMessageBox::warning(nullptr, "����", "��ȡ�ļ�ʧ�ܡ�", QMessageBox::Yes);
		return;
	}

	QJsonArray arr_all = jsonDocument.array();
	for (int i = 0; i < arr_all.count(); i++) {
		QJsonObject obj_plugin = arr_all.at(i).toObject();
		C_PluginData* c_p = new C_PluginData();
		c_p->name = obj_plugin.value("name").toString();
		c_p->status = obj_plugin.value("status").toBool();
		c_p->description = obj_plugin.value("description").toString();
		c_p->parameters = obj_plugin.value("parameters").toObject();
		data_list.push_back(c_p);
	}

	this->resetPluginData(data_list);
}

/*-------------------------------------------------
		���� - ��ȡ
*/
QList<C_PluginData*> S_PluginDataContainer::getPluginData() {
	return this->data_PluginDatas;
}