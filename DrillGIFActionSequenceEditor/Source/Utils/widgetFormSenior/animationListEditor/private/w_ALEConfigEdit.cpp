#include "stdafx.h"
#include "W_ALEConfigEdit.h"

#include "../c_ALEData.h"
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
	ui.comboBox_unit->clear();
	ui.comboBox_unit->setView(new QListView());
	ui.comboBox_unit->addItems(QStringList() << "֡��λ��1��60֡��" << "�뵥λ��1��100֡��" );
	ui.comboBox_frameUnit->clear();
	ui.comboBox_frameUnit->setView(new QListView());
	ui.comboBox_frameUnit->addItems(QStringList()
		<< "1֡" << "2֡" << "3֡" << "4֡" << "5֡"
		<< "10֡" << "20֡" << "30֡" << "40֡" << "50֡"
		<< "100֡" << "500֡" << "�Զ���");
	ui.comboBox_secondUnit->clear();
	ui.comboBox_secondUnit->setView(new QListView());
	ui.comboBox_secondUnit->addItems(QStringList()
		<< "0.01��" << "0.02��" << "0.03��" << "0.04��" << "0.05��"
		<< "0.10��" << "0.20��" << "0.30��" << "0.40��" << "0.50��"
		<< "1.00��" << "5.00��" << "�Զ���");
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("ȷ��"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("ȡ��"); }

	//-----------------------------------
	//----�¼���
	connect(ui.checkBox, &QCheckBox::toggled, this, &W_ALEConfigEdit::checkBoxChanged);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ALEConfigEdit::acceptData);

	connect(ui.comboBox_frameUnit, &QComboBox::currentTextChanged, this, &W_ALEConfigEdit::timeSelected_frameUnit);
	connect(ui.spinBox_frameUnit, SIGNAL(valueChanged(int)), this, SLOT(timeEdited_frameUnit(int)));
	connect(ui.comboBox_secondUnit, &QComboBox::currentTextChanged, this, &W_ALEConfigEdit::timeSelected_secondUnit);
	connect(ui.doubleSpinBox_secondUnit, SIGNAL(valueChanged(double)), this, SLOT(timeEdited_secondUnit(double)));

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
void W_ALEConfigEdit::timeSelected_frameUnit(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "1֡"){ ui.spinBox_frameUnit->setValue(1); }
	if (text == "2֡"){ ui.spinBox_frameUnit->setValue(2); }
	if (text == "3֡"){ ui.spinBox_frameUnit->setValue(3); }
	if (text == "4֡"){ ui.spinBox_frameUnit->setValue(4); }
	if (text == "5֡"){ ui.spinBox_frameUnit->setValue(5); }
	if (text == "10֡"){ ui.spinBox_frameUnit->setValue(10); }
	if (text == "20֡"){ ui.spinBox_frameUnit->setValue(20); }
	if (text == "30֡"){ ui.spinBox_frameUnit->setValue(30); }
	if (text == "40֡"){ ui.spinBox_frameUnit->setValue(40); }
	if (text == "50֡"){ ui.spinBox_frameUnit->setValue(50); }
	if (text == "100֡"){ ui.spinBox_frameUnit->setValue(100); }
	if (text == "500֡"){ ui.spinBox_frameUnit->setValue(500); }

	this->m_slotBlock = false;
}
/*-------------------------------------------------
		�ؼ� - �༭����
*/
void W_ALEConfigEdit::timeEdited_frameUnit(int value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 1){ ui.comboBox_frameUnit->setCurrentText("1֡"); }
	else if (value == 2){ ui.comboBox_frameUnit->setCurrentText("2֡"); }
	else if (value == 3){ ui.comboBox_frameUnit->setCurrentText("3֡"); }
	else if (value == 4){ ui.comboBox_frameUnit->setCurrentText("4֡"); }
	else if (value == 5){ ui.comboBox_frameUnit->setCurrentText("5֡"); }
	else if (value == 10){ ui.comboBox_frameUnit->setCurrentText("10֡"); }
	else if (value == 20){ ui.comboBox_frameUnit->setCurrentText("20֡"); }
	else if (value == 30){ ui.comboBox_frameUnit->setCurrentText("30֡"); }
	else if (value == 40){ ui.comboBox_frameUnit->setCurrentText("40֡"); }
	else if (value == 50){ ui.comboBox_frameUnit->setCurrentText("50֡"); }
	else if (value == 100){ ui.comboBox_frameUnit->setCurrentText("100֡"); }
	else if (value == 500){ ui.comboBox_frameUnit->setCurrentText("500֡"); }
	else{ ui.comboBox_frameUnit->setCurrentText("�Զ���"); }

	this->m_slotBlock = false;
}
/*-------------------------------------------------
		�ؼ� - ѡ������
*/
void W_ALEConfigEdit::timeSelected_secondUnit(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "0.01��"){ ui.doubleSpinBox_secondUnit->setValue(0.01); }
	if (text == "0.02��"){ ui.doubleSpinBox_secondUnit->setValue(0.02); }
	if (text == "0.03��"){ ui.doubleSpinBox_secondUnit->setValue(0.03); }
	if (text == "0.04��"){ ui.doubleSpinBox_secondUnit->setValue(0.04); }
	if (text == "0.05��"){ ui.doubleSpinBox_secondUnit->setValue(0.05); }
	if (text == "0.10��"){ ui.doubleSpinBox_secondUnit->setValue(0.1); }
	if (text == "0.20��"){ ui.doubleSpinBox_secondUnit->setValue(0.2); }
	if (text == "0.30��"){ ui.doubleSpinBox_secondUnit->setValue(0.3); }
	if (text == "0.40��"){ ui.doubleSpinBox_secondUnit->setValue(0.4); }
	if (text == "0.50��"){ ui.doubleSpinBox_secondUnit->setValue(0.5); }
	if (text == "1.00��"){ ui.doubleSpinBox_secondUnit->setValue(1); }
	if (text == "5.00��"){ ui.doubleSpinBox_secondUnit->setValue(5); }
	
	this->m_slotBlock = false;
}
/*-------------------------------------------------
		�ؼ� - �༭����
*/
void W_ALEConfigEdit::timeEdited_secondUnit(double value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 0.01){ ui.comboBox_secondUnit->setCurrentText("0.01��"); }
	else if (value == 0.02){ ui.comboBox_secondUnit->setCurrentText("0.02��"); }
	else if (value == 0.03){ ui.comboBox_secondUnit->setCurrentText("0.03��"); }
	else if (value == 0.04){ ui.comboBox_secondUnit->setCurrentText("0.04��"); }
	else if (value == 0.05){ ui.comboBox_secondUnit->setCurrentText("0.05��"); }
	else if (value == 0.1){ ui.comboBox_secondUnit->setCurrentText("0.10��"); }
	else if (value == 0.2){ ui.comboBox_secondUnit->setCurrentText("0.20��"); }
	else if (value == 0.3){ ui.comboBox_secondUnit->setCurrentText("0.30��"); }
	else if (value == 0.4){ ui.comboBox_secondUnit->setCurrentText("0.40��"); }
	else if (value == 0.5){ ui.comboBox_secondUnit->setCurrentText("0.50��"); }
	else if (value == 1){ ui.comboBox_secondUnit->setCurrentText("1.00��"); }
	else if (value == 5){ ui.comboBox_secondUnit->setCurrentText("5.00��"); }
	else{ ui.comboBox_secondUnit->setCurrentText("�Զ���"); }

	this->m_slotBlock = false;
}

/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_ALEConfigEdit::setDataInModifyMode(C_ALEConfig config, int unit) {
	this->local_config = config;
	this->local_unit = unit;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
C_ALEConfig W_ALEConfigEdit::getData(){
	return this->local_config;
};
/*-------------------------------------------------
		���� - ȡ�����ݣ���λ��
*/
int W_ALEConfigEdit::getDataUnit(){
	return this->local_unit;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_ALEConfigEdit::putDataToUi() {
	ui.comboBox_sizeMode->setCurrentText(this->local_config.getSizeMode());
	ui.checkBox->setChecked(this->local_config.m_isMaskEnabled);
	this->checkBoxChanged(true);

	ui.comboBox_unit->setCurrentIndex(this->local_unit);
	if (this->local_unit == C_ALEData::FrameUnit){
		ui.stackedWidget->setCurrentIndex(0);
	}
	if (this->local_unit == C_ALEData::SecondUnit){
		ui.stackedWidget->setCurrentIndex(1);
	}

	int value = this->local_config.m_defaultInterval;
	ui.spinBox_frameUnit->setValue(value);
	ui.doubleSpinBox_secondUnit->setValue(value*0.01);
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_ALEConfigEdit::putUiToData() {
	this->local_config.setSizeMode(ui.comboBox_sizeMode->currentText());
	this->local_config.m_isMaskEnabled = ui.checkBox->isChecked();
	this->local_unit = ui.comboBox_unit->currentIndex();

	if (this->local_unit == C_ALEData::FrameUnit){
		this->local_config.m_defaultInterval = ui.spinBox_frameUnit->value();
	}
	if (this->local_unit == C_ALEData::SecondUnit){
		this->local_config.m_defaultInterval = ui.doubleSpinBox_secondUnit->value() * 100;
	}
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_ALEConfigEdit::acceptData(){
	this->putUiToData();

	this->accept();
};