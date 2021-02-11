#include "stdafx.h"
#include "drillgifactionsequenceeditor.h"

#include "Source/ActionSeqModule/actionSeqPart/p_ActionSeqPart.h"

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
	//----初始化ui
	QHBoxLayout* l = new QHBoxLayout();
	ui.widget_actionSeq->setLayout(l);
	P_ActionSeqPart* actionSeqPart = new P_ActionSeqPart(this);
	l->addWidget(actionSeqPart);

}