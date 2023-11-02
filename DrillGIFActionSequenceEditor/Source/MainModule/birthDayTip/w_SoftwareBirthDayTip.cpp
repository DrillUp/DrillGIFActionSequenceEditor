#include "stdafx.h"
#include "W_SoftwareBirthDayTip.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		生日彩蛋 提示框.cpp
		所属模块：	主窗体模块
		功能：		生日彩蛋 提示框。

		使用方法：
				> 打开窗口
					W_SoftwareBirthDayTip d;
					d.exec();
-----==========================================================-----
*/

W_SoftwareBirthDayTip::W_SoftwareBirthDayTip(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----ui初始化
	TTool::_chinese_(ui.buttonBox);

	this->refreshLabel();
}
W_SoftwareBirthDayTip::~W_SoftwareBirthDayTip(){
}


/* --------------------------------------------------------------
		控件 - 刷新文本
*/
void W_SoftwareBirthDayTip::refreshLabel() {
	QDate cur_date = QDate::currentDate();

	QString context;
	context.append("※ ※ ※ ※ ※ D R I L L ※ ※ U P ※ ※ ※ ※ ※");
	context.append("\n");
	context.append("\n");
	context.append("特别提示！1月2日是作者drill_up的生日哦！");
	context.append("\n");
	context.append("\n");
	context.append("直到今天");
	context.append(QString::number(cur_date.year()));
	context.append("年");
	context.append(QString::number(cur_date.month()));
	context.append("月");
	context.append(QString::number(cur_date.day()));
	context.append("日");
	context.append("，作者已经");
	int age = cur_date.year() - 1995;
	context.append(QString::number(age));
	context.append("岁了。");
	if (age > 80){
		context.append("（不知道现在是否还活着…）");
	}
	context.append("\n");
	context.append("\n");
	context.append("庆祝一下吧ヽ(*。>Д<)o゜！");
	context.append("\n");
	context.append("\n");
	context.append("※ ※ ※ ※ ※ D R I L L ※ ※ U P ※ ※ ※ ※ ※");

	//qDebug() << cur_date.year();
	//qDebug() << cur_date.month();
	//qDebug() << cur_date.day();

	ui.label->setText(context);
}
/* --------------------------------------------------------------
		控件 - 时间是否为生日
*/
bool W_SoftwareBirthDayTip::isInBirthDay(){
	QDate cur_date = QDate::currentDate();
	return (cur_date.month() == 1 && cur_date.day() == 2);
}

