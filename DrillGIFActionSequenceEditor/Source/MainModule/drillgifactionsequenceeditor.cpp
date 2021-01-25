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
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化ui
	QHBoxLayout* l = new QHBoxLayout();
	ui.widget_actionSeq->setLayout(l);
	P_ActionSeqPart* actionSeqPart = new P_ActionSeqPart(parent);
	l->addWidget(actionSeqPart);

}

DrillGIFActionSequenceEditor::~DrillGIFActionSequenceEditor()
{

}
