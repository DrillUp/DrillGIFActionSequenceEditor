#include "stdafx.h"
#include "W_ALEConfigEdit.h"

#include "../c_ALEData.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动画帧设置 窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑动画帧的整体ui设置。

		使用方法：
				>打开窗口
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
	//----初始化参数
	this->m_slotBlock = false;
	
	//-----------------------------------
	//----初始化ui
	ui.comboBox_sizeMode->clear();
	ui.comboBox_sizeMode->setView(new QListView());
	ui.comboBox_sizeMode->addItems(QStringList() << "大" << "中" << "小");
	ui.comboBox_unit->clear();
	ui.comboBox_unit->setView(new QListView());
	ui.comboBox_unit->addItems(QStringList() << "帧单位（1秒60帧）" << "秒单位（1秒100帧）" );
	ui.comboBox_frameUnit->clear();
	ui.comboBox_frameUnit->setView(new QListView());
	ui.comboBox_frameUnit->addItems(QStringList()
		<< "1帧" << "2帧" << "3帧" << "4帧" << "5帧"
		<< "10帧" << "20帧" << "30帧" << "40帧" << "50帧"
		<< "100帧" << "500帧" << "自定义");
	ui.comboBox_secondUnit->clear();
	ui.comboBox_secondUnit->setView(new QListView());
	ui.comboBox_secondUnit->addItems(QStringList()
		<< "0.01秒" << "0.02秒" << "0.03秒" << "0.04秒" << "0.05秒"
		<< "0.10秒" << "0.20秒" << "0.30秒" << "0.40秒" << "0.50秒"
		<< "1.00秒" << "5.00秒" << "自定义");
	TTool::_chinese_(ui.buttonBox);

	//-----------------------------------
	//----事件绑定
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
		回车事件过滤
*/
void W_ALEConfigEdit::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}

/*-------------------------------------------------
		控件 - 勾选切换
*/
void W_ALEConfigEdit::checkBoxChanged(bool checked){
	if (ui.checkBox->isChecked()){
		ui.checkBox->setText("显示");
	}else{
		ui.checkBox->setText("不显示");
	}
}
/*-------------------------------------------------
		控件 - 选择数字
*/
void W_ALEConfigEdit::timeSelected_frameUnit(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "1帧"){ ui.spinBox_frameUnit->setValue(1); }
	if (text == "2帧"){ ui.spinBox_frameUnit->setValue(2); }
	if (text == "3帧"){ ui.spinBox_frameUnit->setValue(3); }
	if (text == "4帧"){ ui.spinBox_frameUnit->setValue(4); }
	if (text == "5帧"){ ui.spinBox_frameUnit->setValue(5); }
	if (text == "10帧"){ ui.spinBox_frameUnit->setValue(10); }
	if (text == "20帧"){ ui.spinBox_frameUnit->setValue(20); }
	if (text == "30帧"){ ui.spinBox_frameUnit->setValue(30); }
	if (text == "40帧"){ ui.spinBox_frameUnit->setValue(40); }
	if (text == "50帧"){ ui.spinBox_frameUnit->setValue(50); }
	if (text == "100帧"){ ui.spinBox_frameUnit->setValue(100); }
	if (text == "500帧"){ ui.spinBox_frameUnit->setValue(500); }

	this->m_slotBlock = false;
}
/*-------------------------------------------------
		控件 - 编辑数字
*/
void W_ALEConfigEdit::timeEdited_frameUnit(int value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 1){ ui.comboBox_frameUnit->setCurrentText("1帧"); }
	else if (value == 2){ ui.comboBox_frameUnit->setCurrentText("2帧"); }
	else if (value == 3){ ui.comboBox_frameUnit->setCurrentText("3帧"); }
	else if (value == 4){ ui.comboBox_frameUnit->setCurrentText("4帧"); }
	else if (value == 5){ ui.comboBox_frameUnit->setCurrentText("5帧"); }
	else if (value == 10){ ui.comboBox_frameUnit->setCurrentText("10帧"); }
	else if (value == 20){ ui.comboBox_frameUnit->setCurrentText("20帧"); }
	else if (value == 30){ ui.comboBox_frameUnit->setCurrentText("30帧"); }
	else if (value == 40){ ui.comboBox_frameUnit->setCurrentText("40帧"); }
	else if (value == 50){ ui.comboBox_frameUnit->setCurrentText("50帧"); }
	else if (value == 100){ ui.comboBox_frameUnit->setCurrentText("100帧"); }
	else if (value == 500){ ui.comboBox_frameUnit->setCurrentText("500帧"); }
	else{ ui.comboBox_frameUnit->setCurrentText("自定义"); }

	this->m_slotBlock = false;
}
/*-------------------------------------------------
		控件 - 选择数字
*/
void W_ALEConfigEdit::timeSelected_secondUnit(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "0.01秒"){ ui.doubleSpinBox_secondUnit->setValue(0.01); }
	if (text == "0.02秒"){ ui.doubleSpinBox_secondUnit->setValue(0.02); }
	if (text == "0.03秒"){ ui.doubleSpinBox_secondUnit->setValue(0.03); }
	if (text == "0.04秒"){ ui.doubleSpinBox_secondUnit->setValue(0.04); }
	if (text == "0.05秒"){ ui.doubleSpinBox_secondUnit->setValue(0.05); }
	if (text == "0.10秒"){ ui.doubleSpinBox_secondUnit->setValue(0.1); }
	if (text == "0.20秒"){ ui.doubleSpinBox_secondUnit->setValue(0.2); }
	if (text == "0.30秒"){ ui.doubleSpinBox_secondUnit->setValue(0.3); }
	if (text == "0.40秒"){ ui.doubleSpinBox_secondUnit->setValue(0.4); }
	if (text == "0.50秒"){ ui.doubleSpinBox_secondUnit->setValue(0.5); }
	if (text == "1.00秒"){ ui.doubleSpinBox_secondUnit->setValue(1); }
	if (text == "5.00秒"){ ui.doubleSpinBox_secondUnit->setValue(5); }
	
	this->m_slotBlock = false;
}
/*-------------------------------------------------
		控件 - 编辑数字
*/
void W_ALEConfigEdit::timeEdited_secondUnit(double value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 0.01){ ui.comboBox_secondUnit->setCurrentText("0.01秒"); }
	else if (value == 0.02){ ui.comboBox_secondUnit->setCurrentText("0.02秒"); }
	else if (value == 0.03){ ui.comboBox_secondUnit->setCurrentText("0.03秒"); }
	else if (value == 0.04){ ui.comboBox_secondUnit->setCurrentText("0.04秒"); }
	else if (value == 0.05){ ui.comboBox_secondUnit->setCurrentText("0.05秒"); }
	else if (value == 0.1){ ui.comboBox_secondUnit->setCurrentText("0.10秒"); }
	else if (value == 0.2){ ui.comboBox_secondUnit->setCurrentText("0.20秒"); }
	else if (value == 0.3){ ui.comboBox_secondUnit->setCurrentText("0.30秒"); }
	else if (value == 0.4){ ui.comboBox_secondUnit->setCurrentText("0.40秒"); }
	else if (value == 0.5){ ui.comboBox_secondUnit->setCurrentText("0.50秒"); }
	else if (value == 1){ ui.comboBox_secondUnit->setCurrentText("1.00秒"); }
	else if (value == 5){ ui.comboBox_secondUnit->setCurrentText("5.00秒"); }
	else{ ui.comboBox_secondUnit->setCurrentText("自定义"); }

	this->m_slotBlock = false;
}

/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_ALEConfigEdit::setDataInModifyMode(C_ALEConfig config, int unit) {
	this->local_config = config;
	this->local_unit = unit;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
C_ALEConfig W_ALEConfigEdit::getData(){
	return this->local_config;
};
/*-------------------------------------------------
		窗口 - 取出数据（单位）
*/
int W_ALEConfigEdit::getDataUnit(){
	return this->local_unit;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
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
		窗口 - ui数据 -> 本地数据
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
		窗口 - 提交数据（校验）
*/
void W_ALEConfigEdit::acceptData(){
	this->putUiToData();

	this->accept();
};