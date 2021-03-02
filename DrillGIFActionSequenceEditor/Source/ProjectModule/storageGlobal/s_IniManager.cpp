#include "stdafx.h"
#include "S_IniManager.h"


/*
-----==========================================================-----
		�ࣺ		ini�ļ����ù���.cpp
		�汾��		v1.01
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		����QSetting�ļ��Ļ������ù�������ֱ��д��ini�ļ���
					��ͨ�����ڴ洢UI�ϷŴ��ڡ���¼�����С�����ļ���Ϣ��

		�ӹ��ܣ�	->ini�ļ�
						->�Զ�����
						->���ü�
						->��ȡ��

-----==========================================================-----
*/

S_IniManager::S_IniManager(){
	S_IniManager::cur_instance = this;
	this->_init();
}
S_IniManager::~S_IniManager() {
}
/*-------------------------------------------------
		����
*/
S_IniManager* S_IniManager::cur_instance = NULL;
S_IniManager* S_IniManager::getInstance() {
	if (cur_instance == NULL) {
		cur_instance = new S_IniManager();
	}
	return cur_instance;
}
/*-------------------------------------------------
		��ʼ��
*/
void S_IniManager::_init() {

	// > �ļ�·��
	this->m_path = QCoreApplication::applicationDirPath() + "/config.ini";
	QFile f(this->m_path);
	if ( !f.exists() ){
		if (!f.open(QIODevice::WriteOnly | QIODevice::Text)){
			//�������ļ��򿪣�
		}
		f.close();
	}
	
	// > ����
	this->m_qsettings = new QSettings(this->m_path, QSettings::IniFormat);
	
}

/*-------------------------------------------------
		���� - ��ȡֵ
*/
QString S_IniManager::getConfig(QString key){
	return this->m_qsettings->value(key).toString();
}
/*-------------------------------------------------
		���� - ����ֵ
*/
void S_IniManager::setConfig(QString key, QString value){
	this->m_qsettings->setValue(key, value);
}
