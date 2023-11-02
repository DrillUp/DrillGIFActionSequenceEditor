#include "stdafx.h"
#include "W_SoftwareAbout.h"

#include "../versionLog/w_SoftwareVersionLog.h"
#include "Source/ProjectModule/Data/C_ProjectData.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		关于窗口.cpp
		所属模块：	主窗体模块
		功能：		关于窗口。

		使用方法：
				> 打开窗口
					W_SoftwareAbout d;
					d.exec();
-----==========================================================-----
*/

W_SoftwareAbout::W_SoftwareAbout(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----事件绑定
	connect(ui.toolButton, &QToolButton::clicked, this, &W_SoftwareAbout::openUserManual);
	connect(ui.toolButton_2, &QToolButton::clicked, this, &W_SoftwareAbout::openVersionLog);

	//-----------------------------------
	//----ui初始化

	// > 标题块文本
	ui.label_title->setText("GIF动画序列编辑器（" + C_ProjectData::version + "）");

	// > 按钮组
	TTool::_chinese_(ui.buttonBox);

}
W_SoftwareAbout::~W_SoftwareAbout(){
}


/* --------------------------------------------------------------
		控件 - 用户手册
*/
void W_SoftwareAbout::openUserManual() {
	QString sPath = qApp->applicationDirPath();

	QString docx = sPath + "/help/关于GIF动画序列核心编辑器.docx";
	if (QFileInfo(docx).exists()){
		QDesktopServices::openUrl(QUrl("file:/" + docx)); 
	}else{
		QMessageBox::warning(this, "错误", "文档\"关于GIF动画序列核心编辑器.docx\"不见了。", QMessageBox::Yes);
	}

}
/* --------------------------------------------------------------
		控件 - 更新日志
*/
void W_SoftwareAbout::openVersionLog(){
	W_SoftwareVersionLog d(this);
	d.exec();
}