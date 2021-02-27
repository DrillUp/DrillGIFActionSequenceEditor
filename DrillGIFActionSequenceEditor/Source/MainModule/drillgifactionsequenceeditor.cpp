#include "stdafx.h"
#include "drillgifactionsequenceeditor.h"

#include "Source/RmmvInteractiveModule/operateBoard/w_RmmvOperateBoard.h"
#include "Source/RmmvInteractiveModule/custom/s_RmmvDataContainer.h"
#include "Source/ActionSeqModule/actionSeqPart/p_ActionSeqPart.h"
#include "Source/ActionSeqModule/actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/ProjectModule/s_ProjectManager.h"

/*
-----==========================================================-----
		�ࣺ		������.cpp
		���ߣ�		drill_up
		����ģ�飺	������ģ��
		���ܣ�		��������������ƴ��ڡ�
					
-----==========================================================-----
*/
DrillGIFActionSequenceEditor::DrillGIFActionSequenceEditor(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->_init();
	DrillGIFActionSequenceEditor::cur_instance = this;

}
DrillGIFActionSequenceEditor::~DrillGIFActionSequenceEditor(){
}

/*-------------------------------------------------
		����
*/
DrillGIFActionSequenceEditor* DrillGIFActionSequenceEditor::cur_instance = NULL;
DrillGIFActionSequenceEditor* DrillGIFActionSequenceEditor::getInstance() {
	if (cur_instance == NULL) {
		cur_instance = new DrillGIFActionSequenceEditor(NULL);
	}
	return cur_instance;
}
/*-------------------------------------------------
		��ʼ��
*/
void DrillGIFActionSequenceEditor::_init() {

	//-----------------------------------
	//----���/��С��
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowMinimizeButtonHint;
	this->setWindowFlags(flags);

	//-----------------------------------
	//----��ʼ��ui
	QHBoxLayout* l = new QHBoxLayout();
	ui.widget_actionSeq->setLayout(l);
	this->m_p_ActionSeqPart = new P_ActionSeqPart(ui.widget_actionSeq);
	l->setMargin(0);
	l->addWidget(this->m_p_ActionSeqPart);
	
	//-----------------------------------
	//----��ʼ������
	this->m_w_RmmvOperateBoard = nullptr;
	S_ProjectManager::getInstance();
	S_RmmvDataContainer::getInstance();
	S_ActionSeqDataContainer::getInstance();

	//-----------------------------------
	//----�¼���
	connect(ui.toolButton_new, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::newProject);
	connect(ui.toolButton_open, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openProject);
	connect(ui.toolButton_save, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::saveProject);
	connect(ui.toolButton_rmmv, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openWindowRmmvInteractive);
	// ��ע��rmmv����������Ҫ�������ӣ������ڴ浵��ȡʱ��������
	connect(S_RmmvDataContainer::getInstance(), &S_RmmvDataContainer::dataAllReloaded, this, &DrillGIFActionSequenceEditor::rmmvInteractiveDataLoaded);
	connect(S_ActionSeqDataContainer::getInstance(), &S_ActionSeqDataContainer::dataAllReloaded, this, &DrillGIFActionSequenceEditor::actionSeqDataLoaded);
	
}

/*-------------------------------------------------
		�ؼ� - ��rmmv��������
*/
void DrillGIFActionSequenceEditor::openWindowRmmvInteractive(){
	if (this->m_w_RmmvOperateBoard == nullptr){
		this->m_w_RmmvOperateBoard = new W_RmmvOperateBoard(this);
		this->m_w_RmmvOperateBoard->setData(S_RmmvDataContainer::getInstance()->getRmmvProjectData());
	}
	this->m_w_RmmvOperateBoard->show();

	//��RmmvProjectData�ڲ���"��rmmv"��ȡ�����ݺ���������������
}
/*-------------------------------------------------
		�ؼ� - rmmv���������Ѷ�ȡ
*/
void DrillGIFActionSequenceEditor::rmmvInteractiveDataLoaded(){
	if (this->m_w_RmmvOperateBoard != nullptr){		//��ˢ�´������ݣ�
		this->m_w_RmmvOperateBoard->setData(S_RmmvDataContainer::getInstance()->getRmmvProjectData());
	}
}


/*-------------------------------------------------
		�ؼ� - ���������Ѷ�ȡ
*/
void DrillGIFActionSequenceEditor::actionSeqDataLoaded(){

	QJsonObject obj_data = S_ActionSeqDataContainer::getInstance()->getActionSeqData();
	if (obj_data.isEmpty()){
		ui.main_widget->setEnabled(false);
	}else{
		ui.main_widget->setEnabled(true);
		this->m_p_ActionSeqPart->setData(obj_data);
	}
}
/*-------------------------------------------------
		�ؼ� - �½���Ŀ
*/
void DrillGIFActionSequenceEditor::newProject(){
	S_ProjectManager::getInstance()->newProject();
}
/*-------------------------------------------------
		�ؼ� - ����Ŀ
*/
void DrillGIFActionSequenceEditor::openProject(){
	S_ProjectManager::getInstance()->openProject();
}
/*-------------------------------------------------
		�ؼ� - ������Ŀ
*/
void DrillGIFActionSequenceEditor::saveProject(){
	
	// > �������ǰ����ҳ������ȫ������
	this->m_p_ActionSeqPart->putUiToData();
	
	// > ����
	S_ProjectManager::getInstance()->saveProject();
}