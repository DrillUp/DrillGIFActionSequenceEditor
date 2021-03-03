#include "stdafx.h"
#include "W_ActionSeqLength.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		���ֵ ����.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭ ���ֵ ���ڵ�һЩ���á�

		ʹ�÷�����
				>�򿪴���
					W_ActionSeqLength d;
					d.setDataInModifyMode(this->getConfigParam());
					if (d.exec() == QDialog::Accepted){
						this->setConfigParam( d.getData() );
					}
-----==========================================================-----
*/

W_ActionSeqLength::W_ActionSeqLength(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	//-----------------------------------
	//----�¼���
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ActionSeqLength::acceptData);
	
	//-----------------------------------
	//----��ʼ��ui
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("ȷ��"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("ȡ��"); }
}

W_ActionSeqLength::~W_ActionSeqLength(){
}

/*-------------------------------------------------
		�س��¼�����
*/
void W_ActionSeqLength::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}



/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_ActionSeqLength::setDataInModifyMode(C_ActionSeqLength data) {
	this->local_data = data;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
C_ActionSeqLength W_ActionSeqLength::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_ActionSeqLength::putDataToUi() {
	
	// > ʹ��Ĭ��ֵ
	if (this->local_data.realLen_actionSeq < 4 ||
		this->local_data.realLen_action < 4 ||
		this->local_data.realLen_state < 4){
		ui.spinBox_actionSeq->setValue(40);
		ui.spinBox_action->setValue(10);
		ui.spinBox_state->setValue(10);
		return;
	}

	TTool::_int_(ui.spinBox_actionSeq, &this->local_data.realLen_actionSeq);
	TTool::_int_(ui.spinBox_action, &this->local_data.realLen_action);
	TTool::_int_(ui.spinBox_state, &this->local_data.realLen_state);
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_ActionSeqLength::putUiToData() {
	TTool::_int_(&this->local_data.realLen_actionSeq, ui.spinBox_actionSeq);
	TTool::_int_(&this->local_data.realLen_action, ui.spinBox_action);
	TTool::_int_(&this->local_data.realLen_state, ui.spinBox_state);
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_ActionSeqLength::acceptData(){
	this->putUiToData();

	this->accept();
};