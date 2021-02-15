#include "stdafx.h"
#include "W_ProjectNote.h"

#include <QtWidgets>
#include "Source/Utils/common/TTool.h"

W_ProjectNote::W_ProjectNote(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.buttonBox, &QDialogButtonBox::clicked, this, &W_ProjectNote::clickedBtn);

	ui.buttonBox->button(QDialogButtonBox::Ok)->setText(("确定"));
	ui.buttonBox->button(QDialogButtonBox::Retry)->setText(("重新编辑"));
	ui.buttonBox->button(QDialogButtonBox::Cancel)->setText(("取消"));

	this->putUiToData();	//本地文本初始化
}
W_ProjectNote::~W_ProjectNote(){
}

/*-------------------------------------------------
		窗口 - 设置数据
*/
void W_ProjectNote::setData(QString p) {
	if (p == ""){ return; }
	this->local_ProjectNote = p;
	this->putDataToUi();
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
QString W_ProjectNote::getData(){
	return this->local_ProjectNote;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_ProjectNote::putDataToUi() {

	TTool::_QString_(ui.plainTextEdit, &this->local_ProjectNote);
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_ProjectNote::putUiToData() {

	TTool::_QString_(&this->local_ProjectNote, ui.plainTextEdit);

};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_ProjectNote::acceptData(){
	this->putUiToData();

	//无校验

	this->accept();
};
/*-------------------------------------------------
		窗口 - 按钮设置
*/
void W_ProjectNote::clickedBtn(QAbstractButton * btn){
	if (btn == ui.buttonBox->button(QDialogButtonBox::Ok)) {
		this->acceptData();
	}
	else if (btn == ui.buttonBox->button(QDialogButtonBox::Retry)) {
		this->putDataToUi();
	}

};