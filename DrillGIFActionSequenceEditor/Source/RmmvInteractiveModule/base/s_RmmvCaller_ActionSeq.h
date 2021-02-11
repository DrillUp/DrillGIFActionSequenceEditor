#pragma once
#include "p_RmmvCaller.h"

/*
-----==========================================================-----
		�ࣺ		rmmv�����ļ�������.h
		����ģ�飺	rmmv��ȡģ��
		���ܣ�		�������������ģ�顣
					����ϸ��.cpp��
-----==========================================================-----
*/

class S_RmmvCaller_ActionSeq : public P_RmmvCaller
{
	Q_OBJECT

	public:
		S_RmmvCaller_ActionSeq();
		~S_RmmvCaller_ActionSeq();
		static S_RmmvCaller_ActionSeq* cur_manager;			//����
		static S_RmmvCaller_ActionSeq* getInstance();		//��������ȡ�Լ�
		void init();										//��ʼ��
		
	//-----------------------------------
	//----����
	public:
										//���� - ��ȡȫ����ع����ļ�
										//		��˵��������ȡʧ�ܷ���false
		bool loadAllRmmvFile(C_RmmvProjectData rmmvProjectData);
										//���� - ��ȡ�ļ� - plugin.js
		void readPluginData(QString context);
										//���� - ��ȡ�ļ� - System.json
		void readSystemData(QString context);

	protected:
										//���� - ������������ǵ�rmmv��
		virtual void saveDataToRmmv(C_RmmvProjectData rmmvProjectData);

};
