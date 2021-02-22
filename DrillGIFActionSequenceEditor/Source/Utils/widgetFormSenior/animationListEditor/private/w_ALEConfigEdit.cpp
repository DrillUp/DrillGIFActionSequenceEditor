#include "stdafx.h"
#include "W_ALEConfigEdit.h"

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
	ui.comboBox->clear();
	ui.comboBox->setView(new QListView());
	ui.comboBox->addItems(QStringList()
		<< "0.01秒" << "0.02秒" << "0.03秒" << "0.04秒" << "0.05秒"
		<< "0.10秒" << "0.20秒" << "0.30秒" << "0.40秒" << "0.50秒"
		<< "1.00秒" << "5.00秒" << "自定义");
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("确定"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("取消"); }

	//-----------------------------------
	//----事件绑定
	connect(ui.checkBox, &QCheckBox::toggled, this, &W_ALEConfigEdit::checkBoxChanged);
	connect(ui.comboBox, &QComboBox::currentTextChanged, this, &W_ALEConfigEdit::timeSelected);
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(timeEdited(double)));
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ALEConfigEdit::acceptData);

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
void W_ALEConfigEdit::timeSelected(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "0.01秒"){ ui.doubleSpinBox->setValue(0.01); }
	if (text == "0.02秒"){ ui.doubleSpinBox->setValue(0.02); }
	if (text == "0.03秒"){ ui.doubleSpinBox->setValue(0.03); }
	if (text == "0.04秒"){ ui.doubleSpinBox->setValue(0.04); }
	if (text == "0.05秒"){ ui.doubleSpinBox->setValue(0.05); }
	if (text == "0.10秒"){ ui.doubleSpinBox->setValue(0.1); }
	if (text == "0.20秒"){ ui.doubleSpinBox->setValue(0.2); }
	if (text == "0.30秒"){ ui.doubleSpinBox->setValue(0.3); }
	if (text == "0.40秒"){ ui.doubleSpinBox->setValue(0.4); }
	if (text == "0.50秒"){ ui.doubleSpinBox->setValue(0.5); }
	if (text == "1.00秒"){ ui.doubleSpinBox->setValue(1); }
	if (text == "5.00秒"){ ui.doubleSpinBox->setValue(5); }
	
	this->m_slotBlock = false;
}
/*-------------------------------------------------
		控件 - 编辑数字
*/
void W_ALEConfigEdit::timeEdited(double value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 0.01){ ui.comboBox->setCurrentText("0.01秒"); }
	else if (value == 0.02){ ui.comboBox->setCurrentText("0.02秒"); }
	else if (value == 0.03){ ui.comboBox->setCurrentText("0.03秒"); }
	else if (value == 0.04){ ui.comboBox->setCurrentText("0.04秒"); }
	else if (value == 0.05){ ui.comboBox->setCurrentText("0.05秒"); }
	else if (value == 0.1){ ui.comboBox->setCurrentText("0.10秒"); }
	else if (value == 0.2){ ui.comboBox->setCurrentText("0.20秒"); }
	else if (value == 0.3){ ui.comboBox->setCurrentText("0.30秒"); }
	else if (value == 0.4){ ui.comboBox->setCurrentText("0.40秒"); }
	else if (value == 0.5){ ui.comboBox->setCurrentText("0.50秒"); }
	else if (value == 0.1){ ui.comboBox->setCurrentText("1.00秒"); }
	else if (value == 0.5){ ui.comboBox->setCurrentText("5.00秒"); }
	else{ ui.comboBox->setCurrentText("自定义"); }

	this->m_slotBlock = false;
}

/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_ALEConfigEdit::setDataInModifyMode(C_ALEConfig config) {
	this->local_config = config;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
C_ALEConfig W_ALEConfigEdit::getData(){
	return this->local_config;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_ALEConfigEdit::putDataToUi() {
	ui.comboBox_sizeMode->setCurrentText(this->local_config.getSizeMode());
	ui.checkBox->setChecked(this->local_config.m_isMaskEnabled);
	this->checkBoxChanged(true);

	double value = this->local_config.m_defaultInterval*0.01;
	ui.doubleSpinBox->setValue(value);
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_ALEConfigEdit::putUiToData() {
	this->local_config.setSizeMode(ui.comboBox_sizeMode->currentText());
	this->local_config.m_isMaskEnabled = ui.checkBox->isChecked();

	this->local_config.m_defaultInterval = ui.doubleSpinBox->value() * 100;
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_ALEConfigEdit::acceptData(){
	this->putUiToData();

	this->accept();
};