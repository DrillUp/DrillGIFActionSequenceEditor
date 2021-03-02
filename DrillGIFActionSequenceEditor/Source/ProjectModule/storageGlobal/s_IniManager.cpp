#include "stdafx.h"
#include "S_IniManager.h"


/*
-----==========================================================-----
		类：		ini文件配置管理.cpp
		版本：		v1.01
		所属模块：	项目管理模块
		功能：		基于QSetting文件的基本配置管理器，直接写成ini文件。
					（通常用于存储UI拖放窗口、记录最大最小化等文件信息）

		子功能：	->ini文件
						->自动创建
						->设置键
						->获取键

-----==========================================================-----
*/

S_IniManager::S_IniManager(){
	S_IniManager::cur_instance = this;
	this->_init();
}
S_IniManager::~S_IniManager() {
}
/*-------------------------------------------------
		单例
*/
S_IniManager* S_IniManager::cur_instance = NULL;
S_IniManager* S_IniManager::getInstance() {
	if (cur_instance == NULL) {
		cur_instance = new S_IniManager();
	}
	return cur_instance;
}
/*-------------------------------------------------
		初始化
*/
void S_IniManager::_init() {

	// > 文件路径
	this->m_path = QCoreApplication::applicationDirPath() + "/config.ini";
	QFile f(this->m_path);
	if ( !f.exists() ){
		if (!f.open(QIODevice::WriteOnly | QIODevice::Text)){
			//（测试文件打开）
		}
		f.close();
	}
	
	// > 配置
	this->m_qsettings = new QSettings(this->m_path, QSettings::IniFormat);
	
}

/*-------------------------------------------------
		操作 - 获取值
*/
QString S_IniManager::getConfig(QString key){
	return this->m_qsettings->value(key).toString();
}
/*-------------------------------------------------
		操作 - 插入值
*/
void S_IniManager::setConfig(QString key, QString value){
	this->m_qsettings->setValue(key, value);
}
