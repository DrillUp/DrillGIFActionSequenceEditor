#pragma once
#include "../base/p_RmmvCaller.h"

/*
-----==========================================================-----
		�ࣺ		rmmv�����ߣ��������� ר�ã�.h
		����ģ�飺	����ģ��
		���ܣ�		��rmmv������ģ�飬ֻ���� �������� ����ļ���
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
										//���� - ��rmmv���̳У�
		virtual C_RmmvProjectData callRmmvSelect();

										//���� - ��ȡȫ����ع����ļ�
										//		��˵��������ȡʧ�ܷ���false
		bool loadAllRmmvFile(C_RmmvProjectData rmmvProjectData);
										//���� - ������Դ�ļ�
		void coverSourceFileToTemp(C_RmmvProjectData rmmvProjectData);


	protected:
										//���� - ������������ǵ�rmmv��
		virtual void saveDataToRmmv(C_RmmvProjectData rmmvProjectData);

};
