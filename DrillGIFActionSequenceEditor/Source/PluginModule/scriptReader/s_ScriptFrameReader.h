#pragma once

#include "c_ScriptFrame.h"

/*
-----==========================================================-----
		�ࣺ		�ű�����������.h
		����ģ�飺	���ģ��
		���ܣ�		��ȡjs�ļ��ڵ��������ݿ�����ݡ�
					����ϸ��.cpp��
-----==========================================================-----
*/

class S_ScriptFrameReader : public QObject
{
	Q_OBJECT

	public:
		S_ScriptFrameReader();
		~S_ScriptFrameReader();
		static S_ScriptFrameReader* cur_manager;			//����
		static S_ScriptFrameReader* getInstance();			//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩

	//-----------------------------------
	//----��ȡ
	public:
										//��ȡ - �����������Ҫ��
		C_ScriptFrame readPlugin_simple(QString plugin_context);
										//��ȡ - ���������������
		C_ScriptFrame readPlugin_complete(QString plugin_context);

		
	//-----------------------------------
	//----����
	public:
										//���� - ������
		//QString getContentField(QString plugin_helpContext);
										//���� - �����ļ�
		//QString getContentFiles(QString plugin_helpContext);
};
