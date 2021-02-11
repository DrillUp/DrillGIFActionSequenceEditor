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
	//----��ʼ��ui
	QHBoxLayout* l = new QHBoxLayout();
	ui.widget_actionSeq->setLayout(l);
	P_ActionSeqPart* actionSeqPart = new P_ActionSeqPart(this);
	l->addWidget(actionSeqPart);

}