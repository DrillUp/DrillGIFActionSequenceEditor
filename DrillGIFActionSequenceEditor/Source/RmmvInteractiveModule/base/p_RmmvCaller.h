#pragma once
#include "c_RmmvProjectData.h"

/*
-----==========================================================-----
		�ࣺ		rmmv�����ߣ����ࣩ.h
		�汾��		v1.00
		����ģ�飺	����ģ��
		���ܣ�		��rmmv��������ĵ�����ֻ����һЩ���������Ĳ�����
					����ϸ��.cpp��
-----==========================================================-----
*/

class P_RmmvCaller : public QObject
{
	Q_OBJECT

	public:
		P_RmmvCaller();
		~P_RmmvCaller();
		
	//-----------------------------------
	//----rmmv
	public:
									//rmmv - ��rmmv��ע����ܷ��ؿգ�
		C_RmmvProjectData callRmmvOpen();
									//rmmv - ����rmmv
		void callRmmvSave(C_RmmvProjectData rmmvProjectData);
									//rmmv - ����rmmv
		void callRmmvRun(C_RmmvProjectData rmmvProjectData);

	protected:
									//rmmv - ������������ǵ�rmmv��
		virtual void saveDataToRmmv(C_RmmvProjectData rmmvProjectData) = 0;

};
