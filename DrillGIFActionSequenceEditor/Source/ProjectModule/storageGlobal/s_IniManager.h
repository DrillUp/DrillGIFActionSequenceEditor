#pragma once
#include <QObject>
#include <QSettings>


/*
-----==========================================================-----
		�ࣺ		ini�ļ����ù���.cpp
		�汾��		v1.01
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		����QSetting�ļ��Ļ������ù�������ֱ��д��ini�ļ���
					����ϸ��cpp��
-----==========================================================-----
*/
class S_IniManager : public QObject
{
	Q_OBJECT

	//-----------------------------------
	//----�����ṹ
	public:
		S_IniManager();
		~S_IniManager();
		static S_IniManager* cur_instance;			//����
		static S_IniManager* getInstance();			//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩
		void _init();								//��ʼ��
		
	//-----------------------------------
	//----����
	private:
		QSettings* m_qsettings;			//����
		QString m_path;					//�ļ�·��

	public:
										//���� - ��ȡֵ
		QString getConfig(QString key);
										//���� - ����ֵ
										//		��˵����:�������ݼ��洢��
		void setConfig(QString key,QString value);
};

