#include "stdafx.h"
#include "W_ALEConfigEdit.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		����֡���� ����.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭����֡������ui���á�

		ʹ�÷�����
				>�򿪴���
					W_ALEConfigEdit d;
					d.setDataInModifyMode(this->getConfigParam());
					if (d.exec() == QDialog::Accepted){
						this->setConfigParam( d.getData() );
					}
-----==========================================================-----
*/
W_ALEConfigEdit::W_ALEConfigEdit(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->m_slotBlock = false;
	
	//-----------------------------------
	//----��ʼ��ui
	ui.comboBox_sizeMode->clear();
	ui.comboBox_sizeMode->setView(new QListView());
	ui.comboBox_sizeMode->addItems(QStringList() << "��" << "��" << "С");
	ui.comboBox->clear();
	ui.comboBox->setView(new QListView());
	ui.comboBox->addItems(QStringList()
		<< "0.01��" << "0.02��" << "0.03��" << "0.04��" << "0.05��"
		<< "0.10��" << "0.20��" << "0.30��" << "0.40��" << "0.50��"
		<< "1.00��" << "5.00��" << "�Զ���");
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("ȷ��"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("ȡ��"); }

	//-----------------------------------
	//----�¼���
	connect(ui.checkBox, &QCheckBox::toggled, this, &W_ALEConfigEdit::checkBoxChanged);
	connect(ui.comboBox, &QComboBox::currentTextChanged, this, &W_ALEConfigEdit::timeSelected);
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(timeEdited(double)));
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ALEConfigEdit::acceptData);

	this->checkBoxChanged(true);
}

W_ALEConfigEdit::~W_ALEConfigEdit(){
}

/*-------------------------------------------------
		�س��¼�����
*/
void W_ALEConfigEdit::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}

/*-------------------------------------------------
		�ؼ� - ��ѡ�л�
*/
void W_ALEConfigEdit::checkBoxChanged(bool checked){
	if (ui.checkBox->isChecked()){
		ui.checkBox->setText("��ʾ");
	}else{
		ui.checkBox->setText("����ʾ");
	}
}
/*-------------------------------------------------
		�ؼ� - ѡ������
*/
void W_ALEConfigEdit::timeSelected(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "0.01��"){ ui.doubleSpinBox->setValue(0.01); }
	if (text == "0.02��"){ ui.doubleSpinBox->setValue(0.02); }
	if (text == "0.03��"){ ui.doubleSpinBox->setValue(0.03); }
	if (text == "0.04��"){ ui.doubleSpinBox->setValue(0.04); }
	if (text == "0.05��"){ ui.doubleSpinBox->setValue(0.05); }
	if (text == "0.10��"){ ui.doubleSpinBox->setValue(0.1); }
	if (text == "0.20��"){ ui.doubleSpinBox->setValue(0.2); }
	if (text == "0.30��"){ ui.doubleSpinBox->setValue(0.3); }
	if (text == "0.40��"){ ui.doubleSpinBox->setValue(0.4); }
	if (text == "0.50��"){ ui.doubleSpinBox->setValue(0.5); }
	if (text == "1.00��"){ ui.doubleSpinBox->setValue(1); }
	if (text == "5.00��"){ ui.doubleSpinBox->setValue(5); }
	
	this->m_slotBlock = false;
}
/*-------------------------------------------------
		�ؼ� - �༭����
*/
void W_ALEConfigEdit::timeEdited(double value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 0.01){ ui.comboBox->setCurrentText("0.01��"); }
	else if (value == 0.02){ ui.comboBox->setCurrentText("0.02��"); }
	else if (value == 0.03){ ui.comboBox->setCurrentText("0.03��"); }
	else if (value == 0.04){ ui.comboBox->setCurrentText("0.04��"); }
	else if (value == 0.05){ ui.comboBox->setCurrentText("0.05��"); }
	else if (value == 0.1){ ui.comboBox->setCurrentText("0.10��"); }
	else if (value == 0.2){ ui.comboBox->setCurrentText("0.20��"); }
	else if (value == 0.3){ ui.comboBox->setCurrentText("0.30��"); }
	else if (value == 0.4){ ui.comboBox->setCurrentText("0.40��"); }
	else if (value == 0.5){ ui.comboBox->setCurrentText("0.50��"); }
	else if (value == 0.1){ ui.comboBox->setCurrentText("1.00��"); }
	else if (value == 0.5){ ui.comboBox->setCurrentText("5.00��"); }
	else{ ui.comboBox->setCurrentText("�Զ���"); }

	this->m_slotBlock = false;
}

/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_ALEConfigEdit::setDataInModifyMode(C_ALEConfig config) {
	this->local_config = config;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
C_ALEConfig W_ALEConfigEdit::getData(){
	return this->local_config;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_ALEConfigEdit::putDataToUi() {
	ui.comboBox_sizeMode->setCurrentText(this->local_config.getSizeMode());
	ui.checkBox->setChecked(this->local_config.m_isMaskEnabled);
	this->checkBoxChanged(true);

	double value = this->local_config.m_defaultInterval*0.01;
	ui.doubleSpinBox->setValue(value);
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_ALEConfigEdit::putUiToData() {
	this->local_config.setSizeMode(ui.comboBox_sizeMode->currentText());
	this->local_config.m_isMaskEnabled = ui.checkBox->isChecked();

	this->local_config.m_defaultInterval = ui.doubleSpinBox->value() * 100;
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_ALEConfigEdit::acceptData(){
	this->putUiToData();

	this->accept();
};