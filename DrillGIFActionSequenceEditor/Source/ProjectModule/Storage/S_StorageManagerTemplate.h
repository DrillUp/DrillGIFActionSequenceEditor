#pragma once
#include "stdafx.h"

/*
-----==========================================================-----
		�ࣺ		�洢ģ����.h
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		�洢��ģ�壬���ڹ淶����ʵ�ֵ����ݽṹ��
					��ϸ��s_StorageManager.cpp��
-----==========================================================-----
*/

class S_StorageManagerTemplate
{

	public:
		S_StorageManagerTemplate();
		~S_StorageManagerTemplate();
		
	//-----------------------------------
	//----����
	public:
		QFileInfo m_storage_fileInfo;	//�洢/��ȡʱ���ļ���Ϣ
	public:
										//���� - ��ȡ�洢������
		virtual QString getSaveName() = 0;
										//���� - ������ǰ����������
		virtual void clearAllData() = 0;
										//���� - ȫ����Ŀ�������� -> QJsonObject
		virtual QJsonObject getAllDataOfJsonObject() = 0;
										//���� - QJsonObject -> ȫ����Ŀ��������
		virtual void setAllDataFromJsonObject(QJsonObject obj) = 0;
		
	//-----------------------------------
	//----����
	public:
										//���� - ��ȡ�ɰ汾������(��ȡ��)
										//		 ����"getSaveName"�������֣���������������
		virtual QStringList getOldNamesForRead();

};

