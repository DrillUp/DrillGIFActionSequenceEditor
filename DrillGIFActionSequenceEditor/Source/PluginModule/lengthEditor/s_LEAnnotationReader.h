#pragma once
#include <QObject>
#include "c_LEAnnotation.h"

#include "Source/Utils/common/TTool.h"
#include "Source/Utils/manager/txtRead/p_TxtFastReader.h"

/*
-----==========================================================-----
		�ࣺ		���ֵ ע�� ��ȡ��.h
		����ģ�飺	���ģ��
		���ܣ�		ֻɨ�����ֵ�����ע�⡣
					����ϸ��cpp��
-----==========================================================-----
*/
class S_LEAnnotationReader : public QObject
{
	Q_OBJECT

	public:
		S_LEAnnotationReader();
		~S_LEAnnotationReader();
		static S_LEAnnotationReader* cur_manager;			//����
		static S_LEAnnotationReader* getInstance();			//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩
		void init();										//��ʼ��
		
	//-----------------------------------
	//----��ȡ
	public:
												//��ȡ - ��ȡע������
												//		��˵������C_LEAnnotation�к��ļ�����
		C_LEAnnotation* readPlugin(QFileInfo file);
												//��ȡ - ˢ�²������ֵ
												//		��˵����������ı����޸ĺ���Ҫˢ�¡�
		void refreshPluginAnnotation(C_LEAnnotation* plugin);

												//��ȡ - ����ı��Ƿ����"xxx-10"���и�ʽ
		bool isPluginIncludedLengthParam(QString plugin_context);

};

