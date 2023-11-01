#include "stdafx.h"
#include "W_ALE_GIFReader_TypeChoose.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		GIF读取器 类型选择窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		GIF导入 时，选择导入方式的窗口。

		使用方法：
				> 打开窗口
					W_ALE_GIFReader_TypeChoose d;
					d.setDataInModifyMode(0);
					if (d.exec() == QDialog::Accepted){
						int typeIndex = d.getData();
					}
-----==========================================================-----
*/

W_ALE_GIFReader_TypeChoose::W_ALE_GIFReader_TypeChoose(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_typeIndex = 0;

	//-----------------------------------
	//----初始化ui
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ALE_GIFReader_TypeChoose::acceptData);
	TTool::_chinese_(ui.buttonBox);

}
W_ALE_GIFReader_TypeChoose::~W_ALE_GIFReader_TypeChoose(){
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void W_ALE_GIFReader_TypeChoose::setDataInModifyMode(int typeIndex){
	this->m_typeIndex = typeIndex;
	this->putDataToUi();
};
/*-------------------------------------------------
		窗口 - 取出数据
*/
int W_ALE_GIFReader_TypeChoose::getData(){
	this->putUiToData();
	return this->m_typeIndex;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_ALE_GIFReader_TypeChoose::putDataToUi() {

	if (this->m_typeIndex == 0){ ui.radioButton->setChecked(true); }
	if (this->m_typeIndex == 1){ ui.radioButton_2->setChecked(true); }
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_ALE_GIFReader_TypeChoose::putUiToData() {
	
	if (ui.radioButton->isChecked()){ this->m_typeIndex = 0; }
	if (ui.radioButton_2->isChecked()){ this->m_typeIndex = 1; }
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_ALE_GIFReader_TypeChoose::acceptData(){
	this->putUiToData();

	this->accept();
};