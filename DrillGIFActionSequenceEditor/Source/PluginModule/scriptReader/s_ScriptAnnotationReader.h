#pragma once

#include "c_ScriptAnnotation.h"

/*
-----==========================================================-----
		�ࣺ		�ű�ע�������.h
		����ģ�飺	���ģ��
		���ܣ�		��ȡjs�ļ��ڵ��������� @��ע�� ���ݡ�
					����ϸ��.cpp��
-----==========================================================-----
*/

class S_ScriptAnnotationReader : public QObject
{
	Q_OBJECT

	public:
		S_ScriptAnnotationReader();
		~S_ScriptAnnotationReader();
		static S_ScriptAnnotationReader* cur_manager;			//����
		static S_ScriptAnnotationReader* getInstance();			//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩

	//-----------------------------------
	//----��ȡ
	public:
										//��ȡ - �����������Ҫ��
		C_ScriptAnnotation readPlugin_simple(QString plugin_context);
										//��ȡ - ���������������
		C_ScriptAnnotation readPlugin_complete(QString plugin_context);

		
	//-----------------------------------
	//----����
	public:
										//���� - ������
		//QString getContentField(QString plugin_helpContext);
										//���� - �����ļ�
		//QString getContentFiles(QString plugin_helpContext);
};
