#include "stdafx.h"
#include "s_PluginDataContainer.h"


/*
-----==========================================================-----
		类：		插件 存储数据容器.cpp
		所属模块：	插件模块
		功能：		插件编辑并存储数据的容器。
					【封装的容器结构，通过信号连接】
		
		使用方法：
				>初始化：
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
----------PluginDataManager 单例
*/
S_PluginDataContainer* S_PluginDataContainer::cur_manager = NULL;
S_PluginDataContainer* S_PluginDataContainer::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_PluginDataContainer();
	}
	return cur_manager;
}


/*-------------------------------------------------
		数据 - 重设
*/
void S_PluginDataContainer::resetPluginData(QList<C_PluginData*> data_list) {
	this->data_PluginDatas = data_list;
	emit pluginDataChanged();
}
void S_PluginDataContainer::resetPluginDataFromText(QString data_context){
	QList<C_PluginData*> data_list = QList<C_PluginData*>();

	// > 截取数据文本
	int start = data_context.indexOf("[");
	int end = data_context.lastIndexOf("]");
	data_context = data_context.mid(start, end - start + 1);

	// > json转换
	QJsonDocument jsonDocument = QJsonDocument::fromJson(data_context.toUtf8());
	if (jsonDocument.isNull()){
		QMessageBox::warning(nullptr, "错误", "读取文件失败。", QMessageBox::Yes);
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

		//qDebug() << c_p->parameters;
	}

	this->resetPluginData(data_list);
}

/*-------------------------------------------------
		数据 - 获取
*/
QList<C_PluginData*> S_PluginDataContainer::getPluginData() {
	return this->data_PluginDatas;
}