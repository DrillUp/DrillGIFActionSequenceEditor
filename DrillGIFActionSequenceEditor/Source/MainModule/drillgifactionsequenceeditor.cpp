#include "stdafx.h"
#include "drillgifactionsequenceeditor.h"

#include "Source/ActionSeqModule/actionSeqPart/p_ActionSeqPart.h"

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

	//-----------------------------------
	//----��ʼ��ui
	QHBoxLayout* l = new QHBoxLayout();
	ui.widget_actionSeq->setLayout(l);
	P_ActionSeqPart* actionSeqPart = new P_ActionSeqPart(parent);
	l->addWidget(actionSeqPart);

}

DrillGIFActionSequenceEditor::~DrillGIFActionSequenceEditor()
{

}
