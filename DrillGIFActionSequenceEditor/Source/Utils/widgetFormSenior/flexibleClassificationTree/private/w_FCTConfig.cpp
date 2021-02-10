#include "stdafx.h"
#include "W_FCTConfig.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		������ ����.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭ ������ ���ڵ�һЩ���á�
					ע�⣬����˽�����ô��ڡ�һ�㲻���á�

		ʹ�÷�����
				>�򿪴���
					W_FCTConfig d;
					d.setDataInModifyMode(this->getConfigParam());
					if (d.exec() == QDialog::Accepted){
						this->setConfigParam( d.getData() );
					}
-----==========================================================-----
*/

W_FCTConfig::W_FCTConfig(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	//-----------------------------------
	//----�¼���
	connect(ui.checkBox_zeroFill, &QCheckBox::toggled, this, &W_FCTConfig::zeroFillChanged);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_FCTConfig::acceptData);
	
	//-----------------------------------
	//----��ʼ��ui
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("ȷ��"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("ȡ��"); }
}

W_FCTConfig::~W_FCTConfig(){
}

/*-------------------------------------------------
		�س��¼�����
*/
void W_FCTConfig::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}


/*-------------------------------------------------
		����乴ѡ�仯
*/
void W_FCTConfig::zeroFillChanged(bool enable) {

	ui.label_zeroFillChar->setEnabled(enable);
	ui.label_zeroFillCount->setEnabled(enable);
	ui.spinBox_zeroFillCount->setEnabled(enable);
	ui.lineEdit_zeroFillChar->setEnabled(enable);

}
/*-------------------------------------------------
		�Զ����֧ģʽ����
*/
void W_FCTConfig::setModeType(QString cur_type, QStringList type_list) {
	ui.comboBox_sortType->clear();
	ui.comboBox_sortType->addItems(type_list);
	ui.comboBox_sortType->setCurrentText(cur_type);
}


/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_FCTConfig::setDataInModifyMode(C_FCTConfig data) {
	this->local_data = data;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
C_FCTConfig W_FCTConfig::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_FCTConfig::putDataToUi() {
	TTool::_int_(ui.spinBox_rowHeight, &this->local_data.rowHeight);
	ui.checkBox_zeroFill->setChecked(this->local_data.zeroFill);
	this->zeroFillChanged(this->local_data.zeroFill);
	TTool::_int_(ui.spinBox_zeroFillCount, &this->local_data.zeroFillCount);
	ui.lineEdit_zeroFillChar->setText(QString(this->local_data.zeroFillChar));

	TTool::_int_(ui.spinBox_pageContainCount, &this->local_data.pagePerNum);
	TTool::_QString_(ui.comboBox_sortType, &this->local_data.sortType);

};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_FCTConfig::putUiToData() {
	TTool::_int_(&this->local_data.rowHeight, ui.spinBox_rowHeight);
	this->local_data.zeroFill = ui.checkBox_zeroFill->isChecked();
	TTool::_int_(&this->local_data.zeroFillCount, ui.spinBox_zeroFillCount);
	if (ui.lineEdit_zeroFillChar->text().isEmpty() == false){
		this->local_data.zeroFillChar = ui.lineEdit_zeroFillChar->text().at(0);
	}

	TTool::_int_(&this->local_data.pagePerNum, ui.spinBox_pageContainCount);
	TTool::_QString_(&this->local_data.sortType, ui.comboBox_sortType);

};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_FCTConfig::acceptData(){
	this->putUiToData();

	this->accept();
};