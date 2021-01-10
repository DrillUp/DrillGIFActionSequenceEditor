#include "stdafx.h"
#include "s_CallManager.h"

#pragma comment(lib, "user32.lib")			//�޷��������ⲿ���ţ�__imp_FindWindowExW������S_CallManager.cpp

/*
-----==========================================================-----
		�ࣺ		���ڵ�ȡ������.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���������ȡ��ģ�顣

		Ŀ�꣺		-> ��������򿪡�����
					-> ��ȡ��ͬ���ƵĴ�������
					-> ����ָ������

		�ʼǣ�		1.���ڻ��������̡��̣߳����Ƕ���Ҫ�����
					  �����Ƿ�����Ϣû��Ч�������Ƿ����˶���
					2.HWND����Ҫ�����κο⣬����ֱ��ʹ�ã���ϵͳ������
					  �Ҽ��鿴������ҵ��ܶ���ܣ��ٶ�����Ҳ�к���ϸ��˵����
					
		ʹ�÷�����
				>�򿪣�
					int num = S_CallManager::getInstance()->getWindowNum(rmmv_project_name);
					if (num == 1){
						HWND hwnd = S_CallManager::getInstance()->getWindow(rmmv_project_name);
						int success = S_CallManager::getInstance()->callOpen(hwnd);
					}
					��Ҫע����ǣ��򿪴��ڣ���Ҫ���ж��Ƿ��д��ڣ������Ƿ����������ϡ�
					ֻ���ö��Ĵ��ڣ����ܳɹ����� �򿪿�ݼ� ��
				>���棺
					int num = S_CallManager::getInstance()->getWindowNum(rmmv_project_name);
					if (num == 1){
						HWND hwnd = S_CallManager::getInstance()->getWindow(rmmv_project_name);
						int success = S_CallManager::getInstance()->callSave(hwnd);
					}

			
-----==========================================================-----
*/

S_CallManager::S_CallManager(){
}
S_CallManager::~S_CallManager() {
}

/* --------------------------------------------------------------
----------CallManager ����
*/
S_CallManager* S_CallManager::cur_manager = NULL;
S_CallManager* S_CallManager::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_CallManager();
	}
	return cur_manager;
}

/*-------------------------------------------------
		���� - ��ȡrmmv����
*/
HWND S_CallManager::getWindow(QString window_name) {
	HWND hWnd;
	while (true){
		hWnd = ::FindWindow(NULL, window_name.toStdWString().c_str());		//(��������������)
		if (SUCCEEDED(hWnd)){
			break;
		}
	}
	return hWnd;
}

/*-------------------------------------------------
		���� - ��ȡ��ͬ�������Ƶ�����
*/
int S_CallManager::getWindowNum(QString window_name) {
	HWND hWnd = nullptr;
	HWND pre_hWnd = nullptr;
	int num = 0;
	while (true){
		hWnd = ::FindWindowEx(nullptr, pre_hWnd, NULL, window_name.toStdWString().c_str());		//(�����壬�Ӵ��壬��������������)
		if (SUCCEEDED(hWnd)){
			if (hWnd == 0){
				//û�ҵ�
				break;
			}else{
				num++;
				pre_hWnd = hWnd;
				continue;
			}
		}
	}
	return num;
}

/*-------------------------------------------------
		���� - ָ�������Ƿ��ö�
*/
bool S_CallManager::isWindowOnTop(HWND hWnd){
	return ::GetForegroundWindow() == hWnd;
}

/*-------------------------------------------------
		���� - ����/�ö�ָ������
*/
void S_CallManager::activeWindow(HWND hWnd) {
	if (hWnd == 0){ return; };
	DWORD dwForeID = ::GetWindowThreadProcessId(hWnd, NULL);	//����rmmv����
	DWORD dwCurID = ::GetCurrentThreadId();
	::AttachThreadInput(dwCurID, dwForeID, TRUE);
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::SetForegroundWindow(hWnd);
	::AttachThreadInput(dwCurID, dwForeID, FALSE);
}
/*-------------------------------------------------
		���� - ����/�ö�������
*/
void S_CallManager::activeLocalWindow(QWidget* widget) {
	HWND hWnd = (HWND)(widget->winId());
	DWORD dwForeID = ::GetWindowThreadProcessId(hWnd, NULL);
	DWORD dwCurID = ::GetCurrentThreadId();
	::AttachThreadInput(dwCurID, dwForeID, TRUE);
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::SetForegroundWindow(hWnd);
	::AttachThreadInput(dwCurID, dwForeID, FALSE);
}

/*-------------------------------------------------
		���� - �½�����ݼ���
*/
bool S_CallManager::callNew(HWND hWnd) {
	if (hWnd == 0){ return false; };

	this->activeWindow(hWnd);
	Sleep(200);
	keybd_event(VK_CONTROL, 0, 0, 0);			
	keybd_event('N', 0, 0, 0);					
	keybd_event('N', 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
	Sleep(400);
	return this->isWindowOnTop(hWnd);
}
/*-------------------------------------------------
		���� - �򿪣���ݼ���
*/
bool S_CallManager::callOpen(HWND hWnd) {
	if (hWnd == 0){ return false; };

	this->activeWindow(hWnd);
	Sleep(200);
	keybd_event(VK_CONTROL, 0, 0, 0);				//����ctrl��
	keybd_event('O', 0, 0, 0);						//����S��
	keybd_event('O', 0, KEYEVENTF_KEYUP, 0);		//�ɿ�S��
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	//�ɿ�ctrl��
	Sleep(400);
	return this->isWindowOnTop(hWnd);
}
/*-------------------------------------------------
		���� - ���棨��ݼ���
*/
bool S_CallManager::callSave(HWND hWnd) {
	if (hWnd == 0){ return false; };

	this->activeWindow(hWnd);
	Sleep(200);
	keybd_event(VK_CONTROL, 0, 0, 0);				//����ctrl��
	keybd_event('S', 0, 0, 0);						//����S��
	keybd_event('S', 0, KEYEVENTF_KEYUP, 0);		//�ɿ�S��
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	//�ɿ�ctrl��
	Sleep(400);
	return this->isWindowOnTop(hWnd);
}
/*-------------------------------------------------
		���� - ���У���ݼ���
*/
bool S_CallManager::callRun(HWND hWnd) {
	if (hWnd == 0){ return false; };

	this->activeWindow(hWnd);
	Sleep(200);
	keybd_event(VK_CONTROL, 0, 0, 0);				//����ctrl��
	keybd_event('R', 0, 0, 0);						//����R��
	keybd_event('R', 0, KEYEVENTF_KEYUP, 0);		//�ɿ�R��
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	//�ɿ�ctrl��
	Sleep(400);
	return this->isWindowOnTop(hWnd);
}


/*-------------------------------------------------
		���� - �ʼǽű���Ƭ
*/
void S_CallManager::unusedScripts() {

	//���ļ�
	//ShellExecute(NULL, "open", "abc.txt", NULL, NULL, SW_SHOWNORMAL);

	HWND hWnd;
	QString window_name = "abc.txt - ���±�";

	while (true){
		hWnd = ::FindWindow(NULL, window_name.toStdWString().c_str());
		if (SUCCEEDED(hWnd)){
			if (hWnd == 0){
				//û�ҵ�
			}
			else{
				//������Ϣ
				//SendMessageW(hWnd, WM_KEYDOWN, 's', 0);
				//SendMessageW(hWnd, WM_KEYUP, 's', 0);
			}
			break;
		}
	}
}