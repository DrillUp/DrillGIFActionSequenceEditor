#include "stdafx.h"
#include "W_FoldableTabChildWindow.h"

#include "../P_FoldableTabRelater.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		折叠子窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		快速填写表单参数的窗口。

		使用方法：	该类不可单用，见P_FoldableTabManager。
-----==========================================================-----
*/
W_FoldableTabChildWindow::W_FoldableTabChildWindow(P_FoldableTabRelater* parentManager, C_FoldableTabPrivate* part, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowMinimizeButtonHint;
	this->setWindowFlags(flags);
	

	//-----------------------------------
	//----事件绑定
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(rejectData()));

	//-----------------------------------
	//----初始化ui
	this->m_parentManager = parentManager;
	this->m_part = part;
	this->m_layout = new QHBoxLayout(ui.widget_context);
	ui.widget_context->setLayout(this->m_layout);
	TTool::_chinese_(ui.buttonBox);

}
W_FoldableTabChildWindow::~W_FoldableTabChildWindow(){
}

/*-------------------------------------------------
		控件 - 刷新子块控件
*/
void W_FoldableTabChildWindow::refreshPart(){
	this->setWindowTitle(this->m_part->name);
	this->m_part->partWidget;

	if (this->m_part->isInChildWindow == true){
		this->m_layout->addWidget(this->m_part->partWidget);
		this->m_part->partWidget->show();
	}
}

/*-------------------------------------------------
		控件 - 清理控件
*/
void W_FoldableTabChildWindow::clearPart(){
	this->m_layout->removeWidget(this->m_part->partWidget);
}


/* --------------------------------------------------------------
		窗口 - 显示事件（最小化还原）
*/
void W_FoldableTabChildWindow::showEvent(QShowEvent *event){
	this->m_part->isInChildWindow = true;
	this->m_parentManager->refreshAllPart();

	event->accept();
}
/* --------------------------------------------------------------
		窗口 - 关闭事件（按钮）
*/
void W_FoldableTabChildWindow::closeEvent(QCloseEvent *event){
	this->m_part->isInChildWindow = false;
	this->m_parentManager->refreshAllPart();

	event->accept();
}
/* --------------------------------------------------------------
		窗口 - 关闭事件（按钮）
*/
void W_FoldableTabChildWindow::acceptData(){

	this->m_part->isInChildWindow = false;
	this->m_parentManager->refreshAllPart();
}
/* --------------------------------------------------------------
		窗口 - 关闭事件（按钮）
*/
void W_FoldableTabChildWindow::rejectData(){

	this->m_part->isInChildWindow = false;
	this->m_parentManager->refreshAllPart();
}