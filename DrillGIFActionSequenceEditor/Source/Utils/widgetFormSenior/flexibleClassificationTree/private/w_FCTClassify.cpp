#include "stdafx.h"
#include "w_FCTClassify.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		类型 配置窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		类型的设置内容在该窗口设置。
-----==========================================================-----
*/

W_FCTClassify::W_FCTClassify(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->isAddMode = true;

	//-----------------------------------
	//----事件绑定
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));

	//-----------------------------------
	//----初始化ui
	TTool::_chinese_(ui.buttonBox);

}
W_FCTClassify::~W_FCTClassify(){
}




/*-------------------------------------------------
		窗口 - 设置数据（添加）
*/
void W_FCTClassify::setDataInAddMode(QList<C_FCTClassify>* checkFrom) {
	this->isAddMode = true;
	this->local_FCTClassify = C_FCTClassify();
	this->local_FCTClassify.name = "新的分类";
	this->local_FCTClassify.description = "没有描述。";
	this->m_checkFrom = checkFrom;
	this->setWindowTitle("添加分类");
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_FCTClassify::setDataInModifyMode(C_FCTClassify p, QList<C_FCTClassify>* checkFrom) {
	this->isAddMode = false;
	this->local_FCTClassify = p;
	this->m_checkFrom = checkFrom;
	this->setWindowTitle("编辑分类");
	this->putDataToUi();
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
C_FCTClassify W_FCTClassify::getData(){
	return this->local_FCTClassify;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_FCTClassify::putDataToUi() {
	TTool::_QString_(ui.lineEdit_tagName, &local_FCTClassify.name);
	TTool::_QString_(ui.plainTextEdit, &local_FCTClassify.description);
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_FCTClassify::putUiToData() {
	TTool::_QString_(&local_FCTClassify.name, ui.lineEdit_tagName);
	TTool::_QString_(&local_FCTClassify.description, ui.plainTextEdit);
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_FCTClassify::acceptData(){
	QString old_name = local_FCTClassify.name;
	this->putUiToData();

	// > 空校验
	if (ui.lineEdit_tagName->text() == "") {
		QMessageBox::warning(this,"提示","分类名称不能为空。");
		return;
	}
	if (ui.lineEdit_tagName->text() == "未分类") {
		QMessageBox::warning(this, "提示", "分类名称不能为\"未分类\"。");
		return;
	}

	// > 新增校验
	if (this->isAddMode == true && this->m_checkFrom->contains(local_FCTClassify)){
		QMessageBox::warning(this, "提示", "分类名称\"" + local_FCTClassify.name + "\"已存在。");
		return;
	}
	// > 编辑校验
	if (this->isAddMode == false &&
		old_name != local_FCTClassify.name &&
		this->m_checkFrom->contains(local_FCTClassify) ){
		QMessageBox::warning(this, "提示", "分类名称\"" + local_FCTClassify.name + "\"已存在。");
		return;
	}

	this->accept();
};