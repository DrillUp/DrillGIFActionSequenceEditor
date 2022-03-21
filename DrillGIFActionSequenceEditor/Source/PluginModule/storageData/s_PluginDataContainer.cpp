#include "stdafx.h"
#include "s_PluginDataContainer.h"


/*
-----==========================================================-----
		�ࣺ		������� ����.cpp
		�汾��		v1.04
		���ߣ�		drill_up
		����ģ�飺	���ģ��
		���ܣ�		����plugin.js�е�ȫ������������ݡ�
					����װ�������ṹ��ע����ն�ȡ���źš�
		
		ʹ�÷�����
				> ��ȡ������������
					S_PluginDataContainer::getInstance()->loadPluginData( data_context );
					C_PluginData* data = S_PluginDataContainer::getInstance()->getPluginData( plugin_name );	//������ȡ
					QList<C_PluginData*> data_list = S_PluginDataContainer::getInstance()->getPluginDataTank();	//ȫ����ȡ
				> д�루����������
					S_PluginDataContainer::getInstance()->loadPluginData( data_context );
					S_PluginDataContainer::getInstance()->op_add( data ); //��ӡ��޸ġ�ɾ�� ����
					QString data_context = S_PluginDataContainer::getInstance()->writePluginData();
				> ��ȡ��ֱ�ӷ�����
					QList<C_PluginData*> data_list = S_PluginDataContainer::getInstance()->loadPluginDataDirectly( data_context );
				> д�루ֱ�ӷ�����
					QString data_context = S_PluginDataContainer::getInstance()->writePluginDataDirectly( data_list );
-----==========================================================-----
*/
S_PluginDataContainer::S_PluginDataContainer(){
	this->data_PluginDataTank = QList<C_PluginData*>();
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
		���� - ��ȡȫ���������
*/
QList<C_PluginData*> S_PluginDataContainer::getPluginDataTank() {
	return this->data_PluginDataTank;
}
/*-------------------------------------------------
		���� - ��ȡ��Ӧ�Ĳ������
*/
C_PluginData* S_PluginDataContainer::getPluginData(QString pluginName){
	pluginName = pluginName.replace(".js", "");		//��ȥ����׺��

	for (int i = 0; i < this->data_PluginDataTank.count(); i++){
		C_PluginData* data = this->data_PluginDataTank.at(i);
		if (data->name == pluginName){	
			return data;
		}
	}
	return nullptr;
}
/*-------------------------------------------------
		���� - ���
*/
void S_PluginDataContainer::op_add(C_PluginData* data){
	if (this->data_PluginDataTank.contains(data) == true){ return; }
	this->data_PluginDataTank.append(data);
}
/*-------------------------------------------------
		���� - �滻
*/
void S_PluginDataContainer::op_modify(C_PluginData* data){
	int index = this->data_PluginDataTank.indexOf(data);	//����������� data->name ��������Ѱ��
	if (index == -1){ return; }
	this->data_PluginDataTank.replace(index, data);
}
/*-------------------------------------------------
		���� - ȥ��
*/
void S_PluginDataContainer::op_remove(C_PluginData* data){
	this->data_PluginDataTank.removeOne(data);
}



/*-------------------------------------------------
		��ȡ - ��ȡ���ݣ����̣�
*/
void S_PluginDataContainer::loadPluginData(QString data_context){
	QList<C_PluginData*> data_list = this->readPluginDataPrivate(data_context);
	this->data_PluginDataTank = data_list;
	emit pluginDataReloaded();		//�����Ѷ�ȡ���źţ�
}
/*-------------------------------------------------
		��ȡ - ��ȡ���ݣ����̣������źţ�
*/
void S_PluginDataContainer::loadPluginDataNoSignal(QString data_context){
	QList<C_PluginData*> data_list = this->readPluginDataPrivate(data_context);
	this->data_PluginDataTank = data_list;
}
/*-------------------------------------------------
		��ȡ - һ���Զ�ȡ���ݣ����̣�������������
*/
QList<C_PluginData*> S_PluginDataContainer::loadPluginDataDirectly(QString data_context){
	return this->readPluginDataPrivate(data_context);
}
/*-------------------------------------------------
		��ȡ - ��ȡ���ݣ�˽�з�����
*/
QList<C_PluginData*> S_PluginDataContainer::readPluginDataPrivate(QString data_context){
	QList<C_PluginData*> data_list = QList<C_PluginData*>();

	// > ��ȡ�����ı�
	int start = data_context.indexOf("[");
	int end = data_context.lastIndexOf("]");
	data_context = data_context.mid(start, end - start + 1);

	// > jsonת��
	QJsonDocument jsonDocument = QJsonDocument::fromJson(data_context.toUtf8());
	if (jsonDocument.isNull()){
		QMessageBox::warning(nullptr, "����", "��ȡ�ļ�ʧ�ܡ�", QMessageBox::Yes);
		return QList<C_PluginData*>();
	}

	// > ��ȡ��ֵ
	QJsonArray arr_all = jsonDocument.array();
	for (int i = 0; i < arr_all.count(); i++) {
		QJsonObject obj_plugin = arr_all.at(i).toObject();
		C_PluginData* c_p = new C_PluginData();
		c_p->name = obj_plugin.value("name").toString();
		c_p->status = obj_plugin.value("status").toBool();
		c_p->description = obj_plugin.value("description").toString();
		c_p->parameters = obj_plugin.value("parameters").toObject();
		data_list.push_back(c_p);

		//qDebug() << c_p->parameters;
	}
	return data_list;
}


/*-------------------------------------------------
		д�� - д�����ݣ����̣�
*/
QString S_PluginDataContainer::writePluginData(){
	return this->writePluginDataPrivate(this->data_PluginDataTank);
}
/*-------------------------------------------------
		д�� - һ����д�����ݣ����̣����������������ݣ�
*/
QString S_PluginDataContainer::writePluginDataDirectly(QList<C_PluginData*> data_list){
	return this->writePluginDataPrivate(data_list);
}
/*-------------------------------------------------
		д�� - д�����ݣ�˽�У�
*/
QString S_PluginDataContainer::writePluginDataPrivate(QList<C_PluginData*> data_list){
	QString result = "";
	result = result + "// Generated by RPG Maker With Drill Editor.\r\n";
	result = result + "// Do not edit this file directly.\r\n";
	result = result + "var $plugins =\r\n";
	result = result + "[\r\n";

	for (int i = 0; i < data_list.count(); i++){
		C_PluginData* c_p = data_list.at(i);
		result = result + QJsonDocument(c_p->getJsonObject()).toJson(QJsonDocument::Compact);
		if (i < data_list.count() - 1){
			result = result + ",";
		}
		result = result + "\r\n";
	}

	result = result + "];\r\n";
	return result;
}