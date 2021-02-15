#pragma once
#include <QObject>

#include "c_LEConfigData.h"
#include "c_LEAnnotation.h"

#include "Source/Utils/common/TTool.h"
#include "Source/Utils/manager/txtRead/p_TxtFastReader.h"

/*
-----==========================================================-----
		�ࣺ		�ı�ɨ����.h
		����ģ�飺	����ģ��
		���ܣ�		�ṩɨ�衢��дֱ�ӹ��ܡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class S_LEConfigWriter : public QObject
{
	Q_OBJECT

	public:
		S_LEConfigWriter();
		~S_LEConfigWriter();
		static S_LEConfigWriter* cur_manager;			//����
		static S_LEConfigWriter* getInstance();			//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩
		void init();									//��ʼ��

	//-----------------------------------
	//----��д
	public:
												//��д - ���ֵ�޸�
												//		��˵��1��:ÿ��ֻ�ܽ�һ��ע���������д�루���ע����Ҫ����޸ģ�
												//		��˵��2��:ʧ�ܷ��ؿ��ַ���
		QString doOverwritePlugin(QString context, C_LEAnnotation_Param param, C_LEConfigData config);

	private:
												//��д - ���� "�׶�-1" ��ȡ�� "@param �׶�-1" ��ȫ�������ַ���
		QStringList getParamStringList(QStringList contextList, QRegExp re);
		
};

