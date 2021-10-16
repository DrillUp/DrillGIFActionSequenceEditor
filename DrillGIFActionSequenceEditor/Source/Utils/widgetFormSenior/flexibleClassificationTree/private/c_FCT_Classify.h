#pragma once

#include <QJsonObject>
#include "Source/Utils/widgetFastForm/fastForm/c_FastClass.h"

/*
-----==========================================================-----
		�ࣺ		���� ������.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���������� ���� �����ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class C_FCT_Classify{

	public:
		C_FCT_Classify();
		~C_FCT_Classify();
		
	//-----------------------------------
	//----��������
	protected:
		QString name;			//���ƣ������ظ���
		QString description;	//����
	public:
									//�������� - ������
		QString getName();
		void setName(QString name);
		QString getDescription();
		void setDescription(QString description);

	//-----------------------------------
	//----�Զ�������
	public:
		QJsonObject data;
	public:
									//�Զ������� - ��ȡ����
		QJsonObject getCustomData();
									//�Զ������� - ��ȡģ��
		virtual C_FastClass getCustomClass();	
									//�Զ������� - ��ȡ����
		virtual QString getCustomClassName();	
									//�Զ������� - ��ȡ��������
		virtual QString getCustomObjectName();	

	//-----------------------------------
	//----������
	public:
									//ģ�帴��
		void copyBaseDataFrom(C_FCT_Classify* base);
									//ʵ���� -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> ʵ����
		virtual void setJsonObject(QJsonObject obj);
									//���ж�
		virtual bool isNull();

};