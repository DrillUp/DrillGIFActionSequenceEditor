#include "stdafx.h"
#include "w_ALEGIFReaderType.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		选择GIF读取器 窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		单独选择项的窗口。

		使用方法：
				>打开窗口
					W_ALELoadGIFType d;
					if (d.exec() == QDialog::Accepted){
						this->setConfigParam( d.getData() );
					}
-----==========================================================-----
*/

W_ALEGIFReaderType::W_ALEGIFReaderType(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->local_index = 0;

	//-----------------------------------
	//----初始化ui
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ALEGIFReaderType::acceptData);
	TTool::_chinese_(ui.buttonBox);

}

W_ALEGIFReaderType::~W_ALEGIFReaderType(){
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void W_ALEGIFReaderType::setDataInModifyMode(int index){
	this->local_index = index;
	this->putDataToUi();
};
/*-------------------------------------------------
		窗口 - 取出数据
*/
int W_ALEGIFReaderType::getData(){
	this->putUiToData();
	return this->local_index;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_ALEGIFReaderType::putDataToUi() {

	if (this->local_index == 0){ ui.radioButton->setChecked(true); }
	if (this->local_index == 1){ ui.radioButton_2->setChecked(true); }
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_ALEGIFReaderType::putUiToData() {
	
	if (ui.radioButton->isChecked()){ this->local_index = 0; }
	if (ui.radioButton_2->isChecked()){ this->local_index = 1; }
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_ALEGIFReaderType::acceptData(){
	this->putUiToData();

	this->accept();
};