#include "stdafx.h"
#include "w_FPT_Config.h"

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
W_FPT_Config::W_FPT_Config(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������

	//-----------------------------------
	//----�¼���
	this->m_delegate = TTool::_ChangeCombox_itemHeight_(ui.comboBox_sortType, 30);
	connect(ui.checkBox_zeroFill, &QCheckBox::toggled, this, &W_FPT_Config::zeroFillChanged);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_FPT_Config::acceptData);
	
	//-----------------------------------
	//----��ʼ��ui
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("ȷ��"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("ȡ��"); }
}

W_FPT_Config::~W_FPT_Config(){
}

/*-------------------------------------------------
		�ؼ� - �س��¼�����
*/
void W_FPT_Config::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}
/*-------------------------------------------------
		�ؼ� - ����乴ѡ�仯
*/
void W_FPT_Config::zeroFillChanged(bool enable) {

	ui.label_zeroFillChar->setEnabled(enable);
	ui.label_zeroFillCount->setEnabled(enable);
	ui.spinBox_zeroFillCount->setEnabled(enable);
	ui.lineEdit_zeroFillChar->setEnabled(enable);
}


/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_FPT_Config::setData(C_FPT_Config* data) {
	this->local_data = data;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
C_FPT_Config* W_FPT_Config::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_FPT_Config::putDataToUi() {

	// > ��ʾ
	TTool::_int_(ui.spinBox_rowHeight, &this->local_data->rowHeight);
	ui.checkBox_zeroFill->setChecked(this->local_data->zeroFill);
	this->zeroFillChanged(this->local_data->zeroFill);
	TTool::_int_(ui.spinBox_zeroFillCount, &this->local_data->zeroFillCount);
	ui.lineEdit_zeroFillChar->setText(QString(this->local_data->zeroFillChar));

	// > ��֧
	ui.comboBox_sortType->clear();
	ui.comboBox_sortType->addItems(this->local_data->getModeList());
	ui.comboBox_sortType->setCurrentText(this->local_data->getCurrentMode());
	TTool::_int_(ui.spinBox_pageContainCount, &this->local_data->pagePerNum);
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_FPT_Config::putUiToData() {

	// > ��ʾ
	TTool::_int_(&this->local_data->rowHeight, ui.spinBox_rowHeight);
	this->local_data->zeroFill = ui.checkBox_zeroFill->isChecked();
	TTool::_int_(&this->local_data->zeroFillCount, ui.spinBox_zeroFillCount);
	if (ui.lineEdit_zeroFillChar->text().isEmpty() == false){
		this->local_data->zeroFillChar = ui.lineEdit_zeroFillChar->text().at(0);
	}

	// > ��֧
	this->local_data->setCurrentMode(ui.comboBox_sortType->currentText());
	TTool::_int_(&this->local_data->pagePerNum, ui.spinBox_pageContainCount);
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_FPT_Config::acceptData(){
	this->putUiToData();

	this->accept();
};