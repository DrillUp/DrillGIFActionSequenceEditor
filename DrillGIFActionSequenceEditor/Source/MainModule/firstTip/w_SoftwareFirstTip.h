#ifndef W_SoftwareFirstTip_H
#define W_SoftwareFirstTip_H

#include <QtWidgets/QDialog>
#include "ui_W_SoftwareFirstTip.h"

/*
-----==========================================================-----
		类：		首页提示框.h
		所属模块：	主窗体模块
		功能：		首页提示框。
					（详细见cpp）
-----==========================================================-----
*/

class W_SoftwareFirstTip : public QDialog
{
	Q_OBJECT

	public:
		W_SoftwareFirstTip(QWidget *parent = 0);		//构造函数
		~W_SoftwareFirstTip();							//析构函数
		static W_SoftwareFirstTip* cur_instance;		//单例
		static W_SoftwareFirstTip* getInstance();		//单例，获取自己
		void _init();									//初始化
		
	//-----------------------------------
	//----控件		
	public:		
									//控件 - 帮助文档
		void openUserManual();
		
	//-----------------------------------
	//----窗口
	public slots:		
									//窗口 - 提交数据
		void acceptData();
									//窗口 - 取消数据
		void rejectData();
									//窗口 - 窗口关闭事件（点击关闭按钮）
		void closeEvent(QCloseEvent *event);

	private:
		Ui::W_SoftwareFirstTip ui;
		
};

#endif // W_SoftwareFirstTip_H
