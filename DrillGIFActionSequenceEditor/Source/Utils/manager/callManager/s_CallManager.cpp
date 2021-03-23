#include "stdafx.h"
#include "s_CallManager.h"

#pragma comment(lib, "user32.lib")			//无法解析的外部符号（__imp_FindWindowExW）来自S_CallManager.cpp

/*
-----==========================================================-----
		类：		窗口调取管理器.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		管理窗口与调取的模块。

		目标：		-> 控制软件打开、保存
					-> 获取相同名称的窗口数量
					-> 激活指定窗口

		笔记：		1.窗口还包含进程、线程，它们都需要被激活。
					  并不是发送消息没有效果，而是发错了对象。
					2.HWND不需要引入任何库，可以直接使用，是系统函数。
					  右键查看定义就找到很多介绍，百度里面也有很详细的说明。
					
		使用方法：
				>打开：
					int num = S_CallManager::getInstance()->getWindowNum(rmmv_project_name);
					if (num == 1){
						HWND hwnd = S_CallManager::getInstance()->getWindow(rmmv_project_name);
						int success = S_CallManager::getInstance()->callOpen(hwnd);
					}
					需要注意的是，打开窗口，需要先判断是否有窗口，窗口是否有两个以上。
					只有置顶的窗口，才能成功输入 打开快捷键 。
				>保存：
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
----------CallManager 单例
*/
S_CallManager* S_CallManager::cur_manager = NULL;
S_CallManager* S_CallManager::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_CallManager();
	}
	return cur_manager;
}

/*-------------------------------------------------
		窗体 - 获取rmmv窗体
*/
HWND S_CallManager::getWindow(QString window_name) {
	HWND hWnd;
	while (true){
		hWnd = ::FindWindow(NULL, window_name.toStdWString().c_str());		//(程序名，窗口名)
		if (SUCCEEDED(hWnd)){
			break;
		}
	}
	return hWnd;
}

/*-------------------------------------------------
		窗体 - 获取相同窗体名称的数量
*/
int S_CallManager::getWindowNum(QString window_name) {
	HWND hWnd = nullptr;
	HWND pre_hWnd = nullptr;
	int num = 0;
	while (true){
		hWnd = ::FindWindowEx(nullptr, pre_hWnd, NULL, window_name.toStdWString().c_str());		//(父窗体，子窗体，程序名，窗口名)
		if (SUCCEEDED(hWnd)){
			if (hWnd == 0){
				//没找到
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
		窗体 - 指定窗体是否被置顶
*/
bool S_CallManager::isWindowOnTop(QString window_name){
	return ::GetForegroundWindow() == this->getWindow(window_name);
}
bool S_CallManager::isWindowOnTop(HWND hWnd){
	return ::GetForegroundWindow() == hWnd;
}

/*-------------------------------------------------
		窗体 - 激活/置顶指定窗体
*/
void S_CallManager::activeWindow(QString window_name) {
	this->activeWindow(this->getWindow(window_name));
}
void S_CallManager::activeWindow(HWND hWnd) {
	if (hWnd == 0){ return; };
	DWORD dwForeID = ::GetWindowThreadProcessId(hWnd, NULL);	//激活rmmv窗口
	DWORD dwCurID = ::GetCurrentThreadId();
	::AttachThreadInput(dwCurID, dwForeID, TRUE);
	::ShowWindow(hWnd, SW_SHOWNORMAL);
	::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::SetForegroundWindow(hWnd);
	::AttachThreadInput(dwCurID, dwForeID, FALSE);
}
/*-------------------------------------------------
		窗体 - 激活/置顶自身窗体
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
		调用 - 新建（快捷键）
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
		调用 - 打开（快捷键）
*/
bool S_CallManager::callOpen(HWND hWnd) {
	if (hWnd == 0){ return false; };

	this->activeWindow(hWnd);
	Sleep(200);
	keybd_event(VK_CONTROL, 0, 0, 0);				//按下ctrl键
	keybd_event('O', 0, 0, 0);						//按下S键
	keybd_event('O', 0, KEYEVENTF_KEYUP, 0);		//松开S键
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	//松开ctrl键
	Sleep(400);
	return this->isWindowOnTop(hWnd);
}
/*-------------------------------------------------
		调用 - 保存（快捷键）
*/
bool S_CallManager::callSave(HWND hWnd) {
	if (hWnd == 0){ return false; };

	this->activeWindow(hWnd);
	Sleep(200);
	keybd_event(VK_CONTROL, 0, 0, 0);				//按下ctrl键
	keybd_event('S', 0, 0, 0);						//按下S键
	keybd_event('S', 0, KEYEVENTF_KEYUP, 0);		//松开S键
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	//松开ctrl键
	Sleep(400);
	return this->isWindowOnTop(hWnd);
}
/*-------------------------------------------------
		调用 - 运行（快捷键）
*/
bool S_CallManager::callRun(HWND hWnd) {
	if (hWnd == 0){ return false; };

	this->activeWindow(hWnd);
	Sleep(200);
	keybd_event(VK_CONTROL, 0, 0, 0);				//按下ctrl键
	keybd_event('R', 0, 0, 0);						//按下R键
	keybd_event('R', 0, KEYEVENTF_KEYUP, 0);		//松开R键
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);	//松开ctrl键
	Sleep(400);
	return this->isWindowOnTop(hWnd);
}


/*-------------------------------------------------
		其他 - 笔记脚本碎片
*/
void S_CallManager::unusedScripts() {

	//打开文件
	//ShellExecute(NULL, "open", "abc.txt", NULL, NULL, SW_SHOWNORMAL);

	HWND hWnd;
	QString window_name = "abc.txt - 记事本";

	while (true){
		hWnd = ::FindWindow(NULL, window_name.toStdWString().c_str());
		if (SUCCEEDED(hWnd)){
			if (hWnd == 0){
				//没找到
			}
			else{
				//发送消息
				//SendMessageW(hWnd, WM_KEYDOWN, 's', 0);
				//SendMessageW(hWnd, WM_KEYUP, 's', 0);
			}
			break;
		}
	}
}