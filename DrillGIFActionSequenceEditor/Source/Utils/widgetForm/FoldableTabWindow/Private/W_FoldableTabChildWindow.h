#ifndef W_FoldableTabChildWindow_H
#define W_FoldableTabChildWindow_H
#include "stdafx.h"

#include "ui_W_FoldableTabChildWindow.h"
#include "C_FoldableTabPrivate.h"

/*
-----==========================================================-----
		类：		折叠子窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		快速填写表单参数的窗口。
					（详细见cpp）
-----==========================================================-----
*/

class P_FoldableTabRelater;
class W_FoldableTabChildWindow : public QDialog
{
	Q_OBJECT

	public:
		W_FoldableTabChildWindow(P_FoldableTabRelater* parentManager, C_FoldableTabPrivate* part, QWidget *parent = 0);
		~W_FoldableTabChildWindow();
		
	//-----------------------------------
	//----控件
	private:
		P_FoldableTabRelater* m_parentManager;
		C_FoldableTabPrivate* m_part;
		QHBoxLayout* m_layout;
	public slots:
										//控件 - 刷新子块控件
		void refreshPart();
										//控件 - 清理控件
		void clearPart();
		
	//-----------------------------------
	//----窗口
	private:
										//窗口 - 窗口显示事件
		void showEvent(QShowEvent *event);
										//窗口 - 关闭事件（点击关闭按钮）
		void closeEvent(QCloseEvent *event);
	private slots:
										//窗口 - 提交数据
		void acceptData();
		void rejectData();

	private:
		Ui::W_FoldableTabChildWindow ui;
	//-----------------------------------
};

#endif // W_FoldableTabChildWindow_H
