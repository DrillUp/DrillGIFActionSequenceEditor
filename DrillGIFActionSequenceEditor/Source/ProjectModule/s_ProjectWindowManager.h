#pragma once

#include "s_ProjectManager.h"

#include "data/w_ProjectCreate.h"
#include "data/w_ProjectNote.h"

/*
-----==========================================================-----
		�ࣺ		��Ŀ���� - ���ڹ���.h
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		�Դ򿪡��༭���ڽ��в�����������Ӧ��ؿ��ơ�
					�����ڡ�
-----==========================================================-----
*/


class S_ProjectWindowManager : public QObject
{
	Q_OBJECT

	public:
		S_ProjectWindowManager();
		~S_ProjectWindowManager();
		static S_ProjectWindowManager* project_manager;		//����
		static S_ProjectWindowManager* getInstance();		//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩
		
	//-----------------------------------
	//----ȫ�ֽ���
	signals:
										//ȫ�ֽ��� - ˢ��ȫ��������
		void reflashAllTree();

	//-----------------------------------
	//----����
	public slots:
										//���� - �½����Ի���
		void newProject();
										//���� - �򿪣��Ի���
		void openProject();	
										//���� - ���棨�Ի���
		void saveProject();	
										//���� - ���Ϊ���Ի���
		void saveAs();

	//-----------------------------------
	//----����
	public slots:
										//���� - ���̱ʼ�
		void openModifyWindowNote();
										//���� - �½���Ŀ
										//		��������ڱȽ����⣬����ֱ��Ƕ�׵�s_ProjectManager���ˡ���
		//void openModifyWindowCreate();
};

