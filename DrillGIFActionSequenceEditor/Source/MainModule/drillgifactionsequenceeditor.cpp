#include "stdafx.h"
#include "drillgifactionsequenceeditor.h"

#include "Source/RmmvInteractiveModule/operateBoard/w_RmmvOperateBoard.h"
#include "Source/RmmvInteractiveModule/custom/s_RmmvDataContainer.h"
#include "Source/ActionSeqModule/actionSeqPart/p_ActionSeqPart.h"
#include "Source/ActionSeqModule/actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/ProjectModule/s_ProjectManager.h"

/*
-----==========================================================-----
		类：		主窗体.cpp
		作者：		drill_up
		所属模块：	主窗体模块
		功能：		程序进入后的主控制窗口。
					
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
		单例
*/
DrillGIFActionSequenceEditor* DrillGIFActionSequenceEditor::cur_instance = NULL;
DrillGIFActionSequenceEditor* DrillGIFActionSequenceEditor::getInstance() {
	if (cur_instance == NULL) {
		cur_instance = new DrillGIFActionSequenceEditor(NULL);
	}
	return cur_instance;
}
/*-------------------------------------------------
		初始化
*/
void DrillGIFActionSequenceEditor::_init() {

	//-----------------------------------
	//----最大化/最小化
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowMinimizeButtonHint;
	this->setWindowFlags(flags);

	//-----------------------------------
	//----初始化ui
	QHBoxLayout* l = new QHBoxLayout();
	ui.widget_actionSeq->setLayout(l);
	this->m_p_ActionSeqPart = new P_ActionSeqPart(ui.widget_actionSeq);
	l->setMargin(0);
	l->addWidget(this->m_p_ActionSeqPart);
	
	//-----------------------------------
	//----初始化参数
	this->m_w_RmmvOperateBoard = nullptr;
	S_ProjectManager::getInstance();
	S_RmmvDataContainer::getInstance();
	S_ActionSeqDataContainer::getInstance();

	//-----------------------------------
	//----事件绑定
	connect(ui.toolButton_new, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::newProject);
	connect(ui.toolButton_open, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openProject);
	connect(ui.toolButton_save, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::saveProject);
	connect(ui.toolButton_rmmv, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openWindowRmmvInteractive);
	// （注意rmmv交互的数据要最先连接，这样在存档读取时不会乱序）
	connect(S_RmmvDataContainer::getInstance(), &S_RmmvDataContainer::dataAllReloaded, this, &DrillGIFActionSequenceEditor::rmmvInteractiveDataLoaded);
	connect(S_ActionSeqDataContainer::getInstance(), &S_ActionSeqDataContainer::dataAllReloaded, this, &DrillGIFActionSequenceEditor::actionSeqDataLoaded);
	
}

/*-------------------------------------------------
		控件 - 打开rmmv交互窗口
*/
void DrillGIFActionSequenceEditor::openWindowRmmvInteractive(){
	if (this->m_w_RmmvOperateBoard == nullptr){
		this->m_w_RmmvOperateBoard = new W_RmmvOperateBoard(this);
		this->m_w_RmmvOperateBoard->setData(S_RmmvDataContainer::getInstance()->getRmmvProjectData());
	}
	this->m_w_RmmvOperateBoard->show();

	//（RmmvProjectData在操作"打开rmmv"获取到数据后，立即存入容器）
}
/*-------------------------------------------------
		控件 - rmmv交互数据已读取
*/
void DrillGIFActionSequenceEditor::rmmvInteractiveDataLoaded(){
	if (this->m_w_RmmvOperateBoard != nullptr){		//（刷新窗口数据）
		this->m_w_RmmvOperateBoard->setData(S_RmmvDataContainer::getInstance()->getRmmvProjectData());
	}
}


/*-------------------------------------------------
		控件 - 动作序列已读取
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
		控件 - 新建项目
*/
void DrillGIFActionSequenceEditor::newProject(){
	S_ProjectManager::getInstance()->newProject();
}
/*-------------------------------------------------
		控件 - 打开项目
*/
void DrillGIFActionSequenceEditor::openProject(){
	S_ProjectManager::getInstance()->openProject();
}
/*-------------------------------------------------
		控件 - 保存项目
*/
void DrillGIFActionSequenceEditor::saveProject(){
	
	// > 点击保存前，将页面数据全部导出
	this->m_p_ActionSeqPart->putUiToData();
	
	// > 保存
	S_ProjectManager::getInstance()->saveProject();
}