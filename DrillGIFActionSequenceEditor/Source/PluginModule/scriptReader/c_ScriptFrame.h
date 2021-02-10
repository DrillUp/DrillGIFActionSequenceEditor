#pragma once
#include <QList>
#include <QMap>


/*
-----==========================================================-----
		�ࣺ		������ ʵ����.h
		����ģ�飺	���ģ��
		���ܣ�		������ �����ݡ�
					����ϸ��.cpp��
-----==========================================================-----
*/
class C_ScriptFrame_Param;
class C_ScriptFrame{

	public:
		C_ScriptFrame();
		~C_ScriptFrame();
		
	public:
		QString name;								//�����
		QString plugindesc;							//���������@plugindesc��
		QString author;								//���ߣ�@author��
		QString help;								//������Ϣ��@help��
		QList<C_ScriptFrame_Param> param_list;		//�����б�
		
		bool isComplete;							//�����������ݣ����������� ��ǣ�

	//-----------------------------------
	//----������
	public:
										//�Ƿ�Ϊ��������������
		bool isCompleted();
										//���������
		const bool operator== (const C_ScriptFrame& a)const;
										//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
										//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
};

/*
-----==========================================================-----
		�ࣺ		�����ܲ��� ʵ����.h
		����ģ�飺	���ģ��
		���ܣ�		�����Ĳ���ʶ��
-----==========================================================-----
*/
class C_ScriptFrame_Param {
	public:
		C_ScriptFrame_Param();
		~C_ScriptFrame_Param();
	public:
		QString name;			//��������@param��
		QString parent;			//������������@parent��
		QString type;			//�������ͣ�@type��
		QString desc;			//����������@desc��
		QString default_;		//����Ĭ��ֵ��@default��

		int min;				//��������-������Сֵ��@min��
		int max;				//��������-�������ֵ��@max��
		QString on;				//��������-�����ı���@on��
		QString off;			//��������-�����ı���@off��
		QStringList option;		//��������-����������ݣ�@option��
		int require;			//��������-�ļ���@require��
		QString dir;			//��������-�ļ���@dir��
		
	//-----------------------------------
	//----������
	public:
										//���������
		const bool operator== (const C_ScriptFrame_Param& a)const;
										//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
										//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
};