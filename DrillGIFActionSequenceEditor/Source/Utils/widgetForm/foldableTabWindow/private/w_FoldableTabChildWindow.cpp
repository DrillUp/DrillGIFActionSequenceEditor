#include "stdafx.h"
#include "W_FoldableTabChildWindow.h"

#include "../p_FoldableTabRelater.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		�۵��Ӵ���.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		������д�������Ĵ��ڡ�

		ʹ�÷�����	���಻�ɵ��ã���P_FoldableTabManager��
-----==========================================================-----
*/
W_FoldableTabChildWindow::W_FoldableTabChildWindow(P_FoldableTabRelater* parentManager, C_FoldableTabPrivate* part, QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowMinimizeButtonHint;
	this->setWindowFlags(flags);
	

	//-----------------------------------
	//----�¼���
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(rejectData()));

	//-----------------------------------
	//----��ʼ��ui
	this->m_parentManager = parentManager;
	this->m_part = part;
	this->m_layout = new QHBoxLayout(ui.widget_context);
	ui.widget_context->setLayout(this->m_layout);
	TTool::_chinese_(ui.buttonBox);

}
W_FoldableTabChildWindow::~W_FoldableTabChildWindow(){
}

/*-------------------------------------------------
		�ؼ� - ˢ���ӿ�ؼ�
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
		�ؼ� - ����ؼ�
*/
void W_FoldableTabChildWindow::clearPart(){
	this->m_layout->removeWidget(this->m_part->partWidget);
}


/* --------------------------------------------------------------
		���� - ��ʾ�¼�����С����ԭ��
*/
void W_FoldableTabChildWindow::showEvent(QShowEvent *event){
	this->m_part->isInChildWindow = true;
	this->m_parentManager->refreshAllPart();

	event->accept();
}
/* --------------------------------------------------------------
		���� - �ر��¼�����ť��
*/
void W_FoldableTabChildWindow::closeEvent(QCloseEvent *event){
	this->m_part->isInChildWindow = false;
	this->m_parentManager->refreshAllPart();

	event->accept();
}
/* --------------------------------------------------------------
		���� - �ر��¼�����ť��
*/
void W_FoldableTabChildWindow::acceptData(){

	this->m_part->isInChildWindow = false;
	this->m_parentManager->refreshAllPart();
}
/* --------------------------------------------------------------
		���� - �ر��¼�����ť��
*/
void W_FoldableTabChildWindow::rejectData(){

	this->m_part->isInChildWindow = false;
	this->m_parentManager->refreshAllPart();
}