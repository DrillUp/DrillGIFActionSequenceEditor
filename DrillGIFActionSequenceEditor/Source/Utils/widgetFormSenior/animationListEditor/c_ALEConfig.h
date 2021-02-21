#pragma once
#include "Source/Utils/widgetForm/pictureSelector/c_PiSConfig.h"

/*
-----==========================================================-----
		�ࣺ		����֡ �����ࣨ���ã�.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		����֡ �������ࡣ
					����ϸ��cpp��
-----==========================================================-----
*/
class C_ALEConfig : public C_PiSConfig {

	public:
		C_ALEConfig();
		~C_ALEConfig();
		
	//-----------------------------------
	//----��������
	protected:
		QString sizeMode;			//�ߴ�ģʽ
		bool isUseFileSys;			//ʹ���ļ��й�����
	public:
									//���óߴ�ģʽ
		void setSizeMode(QString sizeMode);

	//-----------------------------------
	//----������
	public:
									//ʵ���� -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> ʵ����
		virtual void setJsonObject(QJsonObject obj);
};