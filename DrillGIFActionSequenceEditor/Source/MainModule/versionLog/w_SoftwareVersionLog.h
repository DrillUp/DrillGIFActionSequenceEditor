#ifndef W_SoftwareVersionLog_H
#define W_SoftwareVersionLog_H

#include <QtWidgets/QDialog>
#include "ui_W_SoftwareVersionLog.h"

/*
-----==========================================================-----
		类：		版本日志窗口.h
		所属模块：	主窗体模块
		功能：		版本日志窗口。
					（详细见cpp）
-----==========================================================-----
*/

class W_SoftwareVersionLog : public QDialog
{
	Q_OBJECT

	public:
		W_SoftwareVersionLog(QWidget *parent = 0);		//构造函数
		~W_SoftwareVersionLog();						//析构函数
		
	//-----------------------------------
	//----窗口
									//窗口 - 设置数据
									//窗口 - 获取数据
									//窗口 - 本地数据 -> ui数据
									//窗口 - ui数据 -> 本地数据
									//窗口 - 提交数据（校验）
	private:
		Ui::W_SoftwareVersionLog ui;
		
};

#endif // W_SoftwareVersionLog_H
