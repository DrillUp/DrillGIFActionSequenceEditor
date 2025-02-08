#include "stdafx.h"
#include "W_SoftwareVersionLog.h"

#include "Source/ProjectModule/S_ProjectManager_Custom.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		版本日志窗口.cpp
		所属模块：	主窗体模块
		功能：		版本日志窗口。

		使用方法：
				> 打开窗口
					W_SoftwareVersionLog d;
					d.exec();
-----==========================================================-----
*/

W_SoftwareVersionLog::W_SoftwareVersionLog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----ui初始化

	// > 标题块文本
	ui.label_title->setText(QString("GIF动画序列编辑器（") + S_ProjectManager_Custom::getInstance()->version + "）");

	// > 按钮组
	TTool::_chinese_(ui.buttonBox);

}
W_SoftwareVersionLog::~W_SoftwareVersionLog(){
}

