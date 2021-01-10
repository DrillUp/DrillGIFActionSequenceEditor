#pragma once


/*
-----==========================================================-----
		�ࣺ		���ڵ�ȡ������.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���������ȡ��ģ�顣
					����ϸ��.cpp��
-----==========================================================-----
*/

class S_CallManager : public QWidget
{
	Q_OBJECT

	public:
		S_CallManager();
		~S_CallManager();
		static S_CallManager* cur_manager;			//����
		static S_CallManager* getInstance();		//��������ȡ�Լ�
		
	//-----------------------------------
	//----����
	public:
										//���� - ��ȡ��ͬ�������Ƶ�����
		int getWindowNum(QString window_name);
										//���� - ��ȡrmmv����
										//		��û�ҵ����ڣ����ص�ֵ == 0��
		HWND getWindow(QString window_name);
										//���� - ָ�������Ƿ��ö�
		bool isWindowOnTop(HWND hWnd);
										//���� - ����/�ö�ָ������
										//		������ô������Ӵ����ö�����ô�ô����Ǽ���ȴ�����ö��ģ�
		void activeWindow(HWND hWnd);
										//���� - ����/�ö�������
		void activeLocalWindow(QWidget* widget);

	//-----------------------------------
	//----����
	public:
										//���� - �½�����ݼ���
										//		��δ�ɹ��򷵻�false��
		bool callNew(HWND hWnd);
										//���� - �򿪣���ݼ���
										//		��δ�ɹ��򷵻�false��
		bool callOpen(HWND hWnd);
										//���� - ���棨��ݼ���
										//		��δ�ɹ��򷵻�false��
		bool callSave(HWND hWnd);
										//���� - ���У���ݼ���
										//		��δ�ɹ��򷵻�false��
		bool callRun(HWND hWnd);

	//-----------------------------------
	//----����
										//���� - �ʼǽű���Ƭ
		void unusedScripts();

};
