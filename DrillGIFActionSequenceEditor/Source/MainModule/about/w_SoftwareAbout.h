#ifndef W_SoftwareAbout_H
#define W_SoftwareAbout_H

#include <QtWidgets/QDialog>
#include "ui_W_SoftwareAbout.h"

/*
-----==========================================================-----
		类：		关于窗口.h
		所属模块：	主窗体模块
		功能：		关于窗口。
					（详细见cpp）
-----==========================================================-----
*/

class W_SoftwareAbout : public QDialog
{
	Q_OBJECT

	public:
		W_SoftwareAbout(QWidget *parent = 0);		//构造函数
		~W_SoftwareAbout();							//析构函数
		
	//-----------------------------------
	//----控件		
	public:		
									//控件 - 帮助文档
		void openUserManual();
									//控件 - 更新日志
		void openVersionLog();
		
	//-----------------------------------
	//----窗口
									//窗口 - 设置数据
									//窗口 - 获取数据
									//窗口 - 本地数据 -> ui数据
									//窗口 - ui数据 -> 本地数据
									//窗口 - 提交数据（校验）
	private:
		Ui::W_SoftwareAbout ui;
		
};

#endif // W_SoftwareAbout_H
