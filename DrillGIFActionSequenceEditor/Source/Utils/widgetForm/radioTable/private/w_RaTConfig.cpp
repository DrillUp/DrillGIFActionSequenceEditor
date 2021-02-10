#include "stdafx.h"
#include "W_RaTConfig.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		��ѡ������� ����.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭ ��ѡ������� ���ڵ�һЩ���á�
					ע�⣬����˽�����ô��ڡ�һ�㲻���á�

		ʹ�÷�����
				>�򿪴���
					W_RaTConfig d;
					d.setDataInModifyMode(this->getConfigParam());
					if (d.exec() == QDialog::Accepted){
						this->setConfigParam( d.getData() );
					}
-----==========================================================-----
*/

W_RaTConfig::W_RaTConfig(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	//-----------------------------------
	//----�¼���
	connect(ui.checkBox_zeroFill, &QCheckBox::toggled, this, &W_RaTConfig::zeroFillChanged);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_RaTConfig::acceptData);
	
	//-----------------------------------
	//----��ʼ��ui
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("ȷ��"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("ȡ��"); }
}

W_RaTConfig::~W_RaTConfig(){
}

/*-------------------------------------------------
		�س��¼�����
*/
void W_RaTConfig::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}


/*-------------------------------------------------
		����乴ѡ�仯
*/
void W_RaTConfig::zeroFillChanged(bool enable) {

	ui.label_zeroFillChar->setEnabled(enable);
	ui.label_zeroFillCount->setEnabled(enable);
	ui.spinBox_zeroFillCount->setEnabled(enable);
	ui.lineEdit_zeroFillChar->setEnabled(enable);

}


/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_RaTConfig::setDataInModifyMode(C_RaTConfig data) {
	this->local_data = data;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
C_RaTConfig W_RaTConfig::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_RaTConfig::putDataToUi() {
	TTool::_int_(ui.spinBox_rowHeight, &this->local_data.rowHeight);
	ui.checkBox_zeroFill->setChecked(this->local_data.zeroFill);
	this->zeroFillChanged(this->local_data.zeroFill);
	TTool::_int_(ui.spinBox_zeroFillCount, &this->local_data.zeroFillCount);
	ui.lineEdit_zeroFillChar->setText(QString(this->local_data.zeroFillChar));

};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_RaTConfig::putUiToData() {
	TTool::_int_(&this->local_data.rowHeight, ui.spinBox_rowHeight);
	this->local_data.zeroFill = ui.checkBox_zeroFill->isChecked();
	TTool::_int_(&this->local_data.zeroFillCount, ui.spinBox_zeroFillCount);
	if (ui.lineEdit_zeroFillChar->text().isEmpty() == false){
		this->local_data.zeroFillChar = ui.lineEdit_zeroFillChar->text().at(0);
	}

};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_RaTConfig::acceptData(){
	this->putUiToData();

	this->accept();
};