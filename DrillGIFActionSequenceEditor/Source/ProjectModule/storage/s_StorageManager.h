#pragma once

#include "s_StorageManagerTemplate.h"

/*
-----==========================================================-----
		�ࣺ		�洢������.h
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		�����м̳�ģ�����������ݣ��洢�ļ�/��ȡ�ļ���
					��ϸ��.cpp��
-----==========================================================-----
*/

class S_StorageManager : public QWidget
{
	Q_OBJECT

	public:
		S_StorageManager();
		~S_StorageManager();
		static S_StorageManager* cur_manager;		//����
		static S_StorageManager* getInstance();		//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩
		
	//-----------------------------------
	//----����
	private:
		QList<S_StorageManagerTemplate*> m_managers;
	public:
										//���� - ��ӹ�������ģ�����Զ���ӣ�����Ҫ�ظ���ӣ�
		void addManager(S_StorageManagerTemplate* s_m);
		

	//-----------------------------------
	//----����
	public:
										//���� - ������г�������
		void clearAllApplicationData();
										//���� - �洢�ļ���F:/aaa/bbb.xxx ��
		void createSaveFile(QString file_name);
										//���� - ��ȡ�ļ���F:/aaa/bbb.xxx ��
		void readSaveFile(QString file_name);

};

