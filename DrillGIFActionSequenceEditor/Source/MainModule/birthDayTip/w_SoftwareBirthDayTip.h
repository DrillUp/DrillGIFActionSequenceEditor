#ifndef W_SoftwareBirthDayTip_H
#define W_SoftwareBirthDayTip_H

#include <QtWidgets/QDialog>
#include "ui_W_SoftwareBirthDayTip.h"

/*
-----==========================================================-----
		类：		生日彩蛋 提示框.h
		所属模块：	主窗体模块
		功能：		生日彩蛋 提示框。
					（详细见cpp）
-----==========================================================-----
*/

class W_SoftwareBirthDayTip : public QDialog
{
	Q_OBJECT

	public:
		W_SoftwareBirthDayTip(QWidget *parent = 0);			//构造函数
		~W_SoftwareBirthDayTip();							//析构函数
		
	//-----------------------------------
	//----控件		
	public:		
									//控件 - 刷新文本
		void refreshLabel();
									//控件 - 时间是否为生日
		bool isInBirthDay();
		
	//-----------------------------------
	//----窗口
									//窗口 - 设置数据
									//窗口 - 获取数据
									//窗口 - 本地数据 -> ui数据
									//窗口 - ui数据 -> 本地数据
									//窗口 - 提交数据（校验）
	private:
		Ui::W_SoftwareBirthDayTip ui;
		
};

#endif // W_SoftwareBirthDayTip_H
