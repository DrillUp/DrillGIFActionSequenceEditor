#include "stdafx.h"
#include "W_ALE_Config.h"

#include "../C_ALE_DataSet.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 配置类 窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧编辑块 配置类 的编辑窗口。

		使用方法：
				> 打开窗口
					W_ALE_Config d;
					d.setDataInModifyMode(this->getConfigParam());
					if (d.exec() == QDialog::Accepted){
						this->setConfigParam( d.getData() );
					}
-----==========================================================-----
*/

W_ALE_Config::W_ALE_Config(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_slotBlock = false;
	
	//-----------------------------------
	//----初始化ui
	
	// > 显示尺寸
	ui.comboBox_sizeMode->clear();
	ui.comboBox_sizeMode->setView(new QListView());
	ui.comboBox_sizeMode->addItems(QStringList() << "小" << "中" << "大");


	//-----------------------------------
	//----事件绑定
	connect(ui.checkBox_MaskEnabled, &QCheckBox::toggled, this, &W_ALE_Config::checkBoxChanged_MaskEnabled);
	connect(ui.checkBox_MaskBorderEnabled, &QCheckBox::toggled, this, &W_ALE_Config::checkBoxChanged_MaskBorderEnabled);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ALE_Config::acceptData);

	TTool::_chinese_(ui.buttonBox);
	this->checkBoxChanged_MaskEnabled(true);
	this->checkBoxChanged_MaskBorderEnabled(true);
}

W_ALE_Config::~W_ALE_Config(){
}

/*-------------------------------------------------
		控件 - 回车过滤
*/
void W_ALE_Config::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}

/*-------------------------------------------------
		控件 - 勾选切换
*/
void W_ALE_Config::checkBoxChanged_MaskEnabled(bool checked){
	if (ui.checkBox_MaskEnabled->isChecked()){
		ui.checkBox_MaskEnabled->setText("显示");
	}else{
		ui.checkBox_MaskEnabled->setText("不显示");
	}
}
void W_ALE_Config::checkBoxChanged_MaskBorderEnabled(bool checked){
	if (ui.checkBox_MaskBorderEnabled->isChecked()){
		ui.checkBox_MaskBorderEnabled->setText("显示");
	}else{
		ui.checkBox_MaskBorderEnabled->setText("不显示");
	}
}


/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_ALE_Config::setDataInModifyMode(C_ALE_Config config) {
	this->local_config = config;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
C_ALE_Config W_ALE_Config::getData(){
	return this->local_config;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_ALE_Config::putDataToUi() {
	
	ui.comboBox_sizeMode->setCurrentText(this->local_config.getSize_Mode());
	ui.checkBox_MaskEnabled->setChecked(this->local_config.m_isMaskEnabled);
	ui.checkBox_MaskBorderEnabled->setChecked(this->local_config.m_isMaskBorderEnabled);
	this->checkBoxChanged_MaskEnabled(true);
	this->checkBoxChanged_MaskBorderEnabled(true);
	ui.checkBox_TextBold->setChecked(this->local_config.m_frameText_isBold);

};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_ALE_Config::putUiToData() {
	
	this->local_config.setSize_Mode(ui.comboBox_sizeMode->currentText());
	this->local_config.m_isMaskEnabled = ui.checkBox_MaskEnabled->isChecked();
	this->local_config.m_isMaskBorderEnabled = ui.checkBox_MaskBorderEnabled->isChecked();
	this->local_config.m_frameText_isBold = ui.checkBox_TextBold->isChecked();

};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_ALE_Config::acceptData(){
	this->putUiToData();

	this->accept();
};