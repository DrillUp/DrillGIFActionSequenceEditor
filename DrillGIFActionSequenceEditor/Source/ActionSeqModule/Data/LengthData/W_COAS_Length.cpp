#include "stdafx.h"
#include "W_COAS_Length.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		最大值 窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑 最大值 窗口的一些配置。

		使用方法：
				>打开窗口
					W_COAS_Length d;
					d.setDataInModifyMode(this->getConfigParam());
					if (d.exec() == QDialog::Accepted){
						this->setConfigParam( d.getData() );
					}
-----==========================================================-----
*/

W_COAS_Length::W_COAS_Length(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数

	//-----------------------------------
	//----事件绑定
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_COAS_Length::acceptData);
	
	//-----------------------------------
	//----初始化ui
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("确定"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("取消"); }
}

W_COAS_Length::~W_COAS_Length(){
}

/*-------------------------------------------------
		回车事件过滤
*/
void W_COAS_Length::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}



/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_COAS_Length::setDataInModifyMode(C_COAS_Length data) {
	this->local_data = data;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
C_COAS_Length W_COAS_Length::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_COAS_Length::putDataToUi() {
	
	// > 使用默认值
	if (this->local_data.realLen_actionSeq < 4 ||
		this->local_data.realLen_action < 4 ||
		this->local_data.realLen_state < 4){
		ui.spinBox_actionSeq->setValue(40);
		ui.spinBox_action->setValue(10);
		ui.spinBox_state->setValue(10);
		ui.spinBox_stateNode->setValue(10);
		return;
	}

	TTool::_int_(ui.spinBox_actionSeq, &this->local_data.realLen_actionSeq);
	TTool::_int_(ui.spinBox_action, &this->local_data.realLen_action);
	TTool::_int_(ui.spinBox_state, &this->local_data.realLen_state);
	TTool::_int_(ui.spinBox_stateNode, &this->local_data.realLen_stateNode);
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_COAS_Length::putUiToData() {
	TTool::_int_(&this->local_data.realLen_actionSeq, ui.spinBox_actionSeq);
	TTool::_int_(&this->local_data.realLen_action, ui.spinBox_action);
	TTool::_int_(&this->local_data.realLen_state, ui.spinBox_state);
	TTool::_int_(&this->local_data.realLen_stateNode, ui.spinBox_stateNode);
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_COAS_Length::acceptData(){
	this->putUiToData();

	this->accept();
};