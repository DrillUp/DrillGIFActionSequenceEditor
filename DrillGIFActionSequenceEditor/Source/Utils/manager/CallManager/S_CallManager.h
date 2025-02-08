#pragma once


/*
-----==========================================================-----
		类：		窗口调取管理器.h
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		管理窗口与调取的模块。
					（详细见.cpp）
-----==========================================================-----
*/

class S_CallManager : public QWidget
{
	Q_OBJECT

	public:
		S_CallManager();
		~S_CallManager();
		static S_CallManager* cur_manager;			//单例
		static S_CallManager* getInstance();		//单例，获取自己
		
	//-----------------------------------
	//----窗体
	public:
										//窗体 - 获取rmmv窗体
										//		（没找到窗口，返回的值 == 0）
		HWND getWindow(QString window_name);
										//窗体 - 获取相同窗体名称的数量
		int getWindowNum(QString window_name);
										//窗体 - 指定窗体是否被置顶
		bool isWindowOnTop(HWND hWnd);
		bool isWindowOnTop(QString window_name);
										//窗体 - 激活/置顶指定窗体
										//		（如果该窗体有子窗体置顶，那么该窗体是激活却不是置顶的）
		void activeWindow(HWND hWnd);
		void activeWindow(QString window_name);
										//窗体 - 激活/置顶自身窗体
		void activeLocalWindow(QWidget* widget);

	//-----------------------------------
	//----调用
	public:
										//调用 - 新建（快捷键）
										//		（未成功则返回false）
		bool callNew(HWND hWnd);
										//调用 - 打开（快捷键）
										//		（未成功则返回false）
		bool callOpen(HWND hWnd);
										//调用 - 保存（快捷键）
										//		（未成功则返回false）
		bool callSave(HWND hWnd);
										//调用 - 运行（快捷键）
										//		（未成功则返回false）
		bool callRun(HWND hWnd);

	//-----------------------------------
	//----其他
										//其他 - 笔记脚本碎片
		void unusedScripts();

};
