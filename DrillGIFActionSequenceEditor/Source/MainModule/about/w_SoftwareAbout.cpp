#include "stdafx.h"
#include "W_SoftwareAbout.h"

#include "../versionLog/w_SoftwareVersionLog.h"
#include "Source/ProjectModule/S_ProjectManager_Custom.h"
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
	connect(ui.toolButton, &QToolButton::clicked, this, &W_SoftwareAbout::openUserManual_menu);
	connect(ui.toolButton_2, &QToolButton::clicked, this, &W_SoftwareAbout::openVersionLog);

	//-----------------------------------
	//----ui初始化

	// > 标题块文本
	ui.label_title->setText("GIF动画序列编辑器（" + S_ProjectManager_Custom::getInstance()->version + "）");

	// > 按钮组
	TTool::_chinese_(ui.buttonBox);

}
W_SoftwareAbout::~W_SoftwareAbout(){
}


/* --------------------------------------------------------------
		控件 - 帮助文档
*/
void W_SoftwareAbout::openUserManual_menu() {
	QToolButton* cur_btn = qobject_cast<QToolButton*>(this->sender());		//从action里面取出数据指针
	QMenu *menu = new QMenu();
	QAction *action;

	action = new QAction("关于GIF动画序列核心编辑器（入门篇）.docx", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/nav/Help.png"));
	connect(action, &QAction::triggered, this, &W_SoftwareAbout::openUserManual_1);
	menu->addAction(action);

	action = new QAction("关于GIF动画序列核心编辑器（高级篇）.docx", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/nav/Help.png"));
	connect(action, &QAction::triggered, this, &W_SoftwareAbout::openUserManual_2);
	menu->addAction(action);

	QPoint p = cur_btn->mapToGlobal(QPoint(0, 0));
	p.setY(p.y() + cur_btn->height());
	menu->exec(p);
}
void W_SoftwareAbout::openUserManual_1() {
	QString sPath = qApp->applicationDirPath();
	QString docx_name = "关于GIF动画序列核心编辑器（入门篇）.docx";

	QString docx = sPath + "/help/" + docx_name;
	if (QFileInfo(docx).exists()){
		QDesktopServices::openUrl(QUrl("file:/" + docx)); 
	}else{
		QMessageBox::warning(this, "错误", "文档\"" + docx_name + "\"不见了。", QMessageBox::Yes);
	}
}
void W_SoftwareAbout::openUserManual_2() {
	QString sPath = qApp->applicationDirPath();
	QString docx_name = "关于GIF动画序列核心编辑器（高级篇）.docx";

	QString docx = sPath + "/help/" + docx_name;
	if (QFileInfo(docx).exists()){
		QDesktopServices::openUrl(QUrl("file:/" + docx));
	}else{
		QMessageBox::warning(this, "错误", "文档\"" + docx_name + "\"不见了。", QMessageBox::Yes);
	}
}
/* --------------------------------------------------------------
		控件 - 更新日志
*/
void W_SoftwareAbout::openVersionLog(){
	W_SoftwareVersionLog d(this);
	d.exec();
}