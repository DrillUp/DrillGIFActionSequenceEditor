#include "stdafx.h"
#include "W_ObjectSelectorConfig.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		����ѡ�� ����.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭����ѡ�񴰿ڵ�һЩ���á�
					ע�⣬���Ƕ���ѡ������˽�����ô��ڡ�һ�㲻���á�

		ʹ�÷�����
				>�򿪴���
					W_ObjectSelectorConfig d;
					d.setDataInModifyMode(this->getConfigParam());
					if (d.exec() == QDialog::Accepted){
						this->setConfigParam( d.getData() );
					}
-----==========================================================-----
*/

W_ObjectSelectorConfig::W_ObjectSelectorConfig(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	

	//-----------------------------------
	//----�¼���
	connect(ui.checkBox_zeroFill, &QCheckBox::toggled, this, &W_ObjectSelectorConfig::zeroFillChanged);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ObjectSelectorConfig::acceptData);
	
	//-----------------------------------
	//----��ʼ��ui
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("ȷ��"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("ȡ��"); }
}

W_ObjectSelectorConfig::~W_ObjectSelectorConfig(){
}

/*-------------------------------------------------
		�س��¼�����
*/
void W_ObjectSelectorConfig::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}


/*-------------------------------------------------
		����乴ѡ�仯
*/
void W_ObjectSelectorConfig::zeroFillChanged(bool enable) {

	ui.label_zeroFillChar->setEnabled(enable);
	ui.label_zeroFillCount->setEnabled(enable);
	ui.spinBox_zeroFillCount->setEnabled(enable);
	ui.lineEdit_zeroFillChar->setEnabled(enable);

}


/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_ObjectSelectorConfig::setDataInModifyMode(C_ObjectSelectorConfig data) {
	this->local_data = data;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
C_ObjectSelectorConfig W_ObjectSelectorConfig::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_ObjectSelectorConfig::putDataToUi() {
	TTool::_int_(ui.spinBox_rowHeight, &this->local_data.rowHeight);
	ui.checkBox_zeroFill->setChecked(this->local_data.zeroFill);
	this->zeroFillChanged(this->local_data.zeroFill);
	TTool::_int_(ui.spinBox_zeroFillCount, &this->local_data.zeroFillCount);
	ui.lineEdit_zeroFillChar->setText(QString(this->local_data.zeroFillChar));

	TTool::_int_(ui.spinBox_pageContainCount, &this->local_data.pagePerNum);
	ui.checkBox_showHavingDataInitials->setChecked(this->local_data.showHavingDataInitials);


};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_ObjectSelectorConfig::putUiToData() {
	TTool::_int_(&this->local_data.rowHeight, ui.spinBox_rowHeight);
	this->local_data.zeroFill = ui.checkBox_zeroFill->isChecked();
	TTool::_int_(&this->local_data.zeroFillCount, ui.spinBox_zeroFillCount);
	if (ui.lineEdit_zeroFillChar->text().isEmpty() == false){
		this->local_data.zeroFillChar = ui.lineEdit_zeroFillChar->text().at(0);
	}

	TTool::_int_(&this->local_data.pagePerNum, ui.spinBox_pageContainCount);
	this->local_data.showHavingDataInitials = ui.checkBox_showHavingDataInitials->isChecked();


};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_ObjectSelectorConfig::acceptData(){
	this->putUiToData();

	this->accept();
};