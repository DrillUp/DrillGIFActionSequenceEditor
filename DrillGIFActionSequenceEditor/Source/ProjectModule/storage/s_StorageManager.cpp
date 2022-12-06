#include "stdafx.h"
#include "S_StorageManager.h"



/*
-----==========================================================-----
		类：		存储管理器.cpp
		所属模块：	项目管理模块
		功能：		将所有继承模板的子类的数据，存储文件/读取文件。
			
		必要事项：	1.存储的子类必须继承模板。（s_StorageManagerTemplate）
					  如果没有继承，则没有效果。

		其它说明：	1.模板会自动识别子类名作为存储名。
					  你也可以手动修改save_name。
					2.管理器通过指针直接操作子类，没有返回值。
					3.存储结构如下：
						{
							"S_StorageManager":{……},
							"S_SourceManager":{……},
							"S_RequestManager":{……},
							"C_GlobalSettings":{……},
						}
			
		使用方法：	
				>存储：
					S_StorageManager->getInstance()->createSaveFile( … );
				>读取：
					S_StorageManager->getInstance()->readSaveFile( … );
				>清除：
					S_StorageManager->getInstance()->clearAllApplicationData();
				>子类获取存储文件路径：
					this->m_storage_fileInfo
-----==========================================================-----
*/

S_StorageManager::S_StorageManager(){
}
S_StorageManager::~S_StorageManager() {
}

/* --------------------------------------------------------------
----------StorageManager 单例
*/
S_StorageManager* S_StorageManager::cur_manager = NULL;
S_StorageManager* S_StorageManager::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_StorageManager();
	}
	return cur_manager;
}


void S_StorageManager::addManager(S_StorageManagerTemplate* s_m) {
	this->m_managers.append(s_m);
}

/* ----------------------------------------------------------------------------------
		管理 - 存储文件（F:/aaa/bbb.xxx ）
*/
void S_StorageManager::createSaveFile(QString file_name) {
	QJsonObject obj_all = QJsonObject();
	for (int i = 0; i < this->m_managers.count() ;i++) {		//依次数据获取
		S_StorageManagerTemplate* s_m = this->m_managers.at(i);
		s_m->m_storage_fileInfo = QFileInfo(file_name);
		QJsonObject obj_manager = s_m->getAllDataOfJsonObject();
		obj_all.insert( s_m->getSaveName() , obj_manager);
	}

	QString context_all = QJsonDocument(obj_all).toJson();
	QFile file(file_name);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Truncate)){
		QMessageBox::warning(this, "错误", "无法创建存储文件。", QMessageBox::Yes);
		return;
	}
	QTextStream write_stream(&file);
	QTextCodec *codec2 = QTextCodec::codecForName("utf-8");
	write_stream.flush();
	write_stream.setCodec(codec2);
	write_stream.seek(0);						//光标从0开始写入
	write_stream << context_all;
	file.close();
}

/* ----------------------------------------------------------------------------------
		管理 - 读取文件（F:/aaa/bbb.xxx ）
*/
void S_StorageManager::readSaveFile(QString file_name) {
	QFile file(file_name);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) { 
		QMessageBox::warning(this, "错误", "未找到文件。", QMessageBox::Yes);
		return;
	}
	QString context = file.readAll();
	file.close();

	QJsonDocument jsonDocument = QJsonDocument::fromJson(context.toUtf8());
	if (jsonDocument.isNull()){
		QMessageBox::warning(this, "错误", "读取文件失败。", QMessageBox::Yes);
		return;
	}

	QJsonObject obj_all = jsonDocument.object();
	for (int i = 0; i < this->m_managers.count(); i++) {		//依次数据赋值
		S_StorageManagerTemplate* s_m = this->m_managers.at(i);
		QJsonObject obj_project = obj_all.value(s_m->getSaveName()).toObject();
		s_m->m_storage_fileInfo = QFileInfo(file_name);

		QStringList old_names = s_m->getOldNamesForRead();
		for (int j = 0; j < old_names.count(); j++) {
			QString old_name = old_names.at(j);
			if (obj_project.isEmpty()){
				obj_project = obj_all.value(old_name).toObject();
			}
		}

		s_m->setAllDataFromJsonObject(obj_project);
	}
													
}


/* ----------------------------------------------------------------------------------
		管理 - 清除所有程序数据
*/
void S_StorageManager::clearAllApplicationData() {

	for (int i = 0; i < this->m_managers.count(); i++) {		//依次数据赋值
		S_StorageManagerTemplate* s_m = this->m_managers.at(i);
		s_m->clearAllData();
	}
}
