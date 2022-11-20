#include "stdafx.h"
#include "W_RaTConfig.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		单选表格配置 窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑 单选表格配置 窗口的一些配置。
					注意，这是私有配置窗口。一般不外用。

		使用方法：
				>打开窗口
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
	//----初始化参数

	//-----------------------------------
	//----事件绑定
	connect(ui.checkBox_zeroFill, &QCheckBox::toggled, this, &W_RaTConfig::zeroFillChanged);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_RaTConfig::acceptData);
	
	//-----------------------------------
	//----初始化ui
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("确定"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("取消"); }
}

W_RaTConfig::~W_RaTConfig(){
}

/*-------------------------------------------------
		回车事件过滤
*/
void W_RaTConfig::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}


/*-------------------------------------------------
		零填充勾选变化
*/
void W_RaTConfig::zeroFillChanged(bool enable) {

	ui.label_zeroFillChar->setEnabled(enable);
	ui.label_zeroFillCount->setEnabled(enable);
	ui.spinBox_zeroFillCount->setEnabled(enable);
	ui.lineEdit_zeroFillChar->setEnabled(enable);

}


/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_RaTConfig::setDataInModifyMode(C_RaTConfig data) {
	this->local_data = data;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
C_RaTConfig W_RaTConfig::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_RaTConfig::putDataToUi() {
	TTool::_int_(ui.spinBox_rowHeight, &this->local_data.rowHeight);
	ui.checkBox_zeroFill->setChecked(this->local_data.zeroFill);
	this->zeroFillChanged(this->local_data.zeroFill);
	TTool::_int_(ui.spinBox_zeroFillCount, &this->local_data.zeroFillCount);
	ui.lineEdit_zeroFillChar->setText(QString(this->local_data.zeroFillChar));

};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
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
		窗口 - 提交数据（校验）
*/
void W_RaTConfig::acceptData(){
	this->putUiToData();

	this->accept();
};