#include "stdafx.h"
#include "W_QStringEditor.h"

#include "W_QStringListEditor.h"

/*
-----==========================================================-----
		类：		字符串编辑 窗口.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑一条字符串的窗口。

		使用方法：
				> 创建填写窗口
					W_QStringEditor d(this,this);
					d.setParamName("参数名");
					d.setConditionNotNull(true);
					d.setDataInModifyMode("");
					if( d.exec() == QDialog::Accepted ){
						QJsonObject p = d.getData();
						this->local_dataList.append(p);
					}
-----==========================================================-----
*/

W_QStringEditor::W_QStringEditor(QWidget *parent, W_QStringListEditor *windowParent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_paramShowingName = "字符串";
	this->m_paramDescription = "";
	this->m_notNull = false;
	this->m_noRepeat = false;
	this->m_windowParent = windowParent;
	this->setDataInAddMode();

	//-----------------------------------
	//----事件绑定
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_QStringEditor::acceptData);

	//-----------------------------------
	//----初始化ui
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("确定"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("取消"); }
}

W_QStringEditor::~W_QStringEditor(){
}

/*-------------------------------------------------
		回车事件过滤
*/
void W_QStringEditor::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}


/*-------------------------------------------------
		控件 - 设置参数名
*/
void W_QStringEditor::setParamShowingName(QString name){
	this->m_paramShowingName = name;
};
/*-------------------------------------------------
		控件 - 设置参数编辑的描述
*/
void W_QStringEditor::setParamDescription(QString description){
	this->m_paramDescription = description;
};
/*-------------------------------------------------
		控件 - 设置非空
*/
void W_QStringEditor::setConditionNotNull(bool b){
	this->m_notNull = b;
};
/*-------------------------------------------------
		控件 - 设置不可重复
*/
void W_QStringEditor::setConditionNoRepeat(bool b){
	this->m_noRepeat = b;
};



/*-------------------------------------------------
		窗口 - 设置数据（添加）
*/
void W_QStringEditor::setDataInAddMode() {

	// > 标题
	this->setWindowTitle("编辑" + this->m_paramShowingName);
	ui.label->setText(this->m_paramShowingName);

	// > 描述
	ui.label_description->setText(this->m_paramDescription);
	if (this->m_paramDescription == ""){
		ui.label_description->setVisible(false);
	}

	this->m_isAddMode = true;
	this->local_data = "";
	this->m_modifyLastData = "";
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_QStringEditor::setDataInModifyMode(QString data) {

	// > 标题
	this->setWindowTitle("编辑" + this->m_paramShowingName);
	ui.label->setText(this->m_paramShowingName);

	// > 描述
	ui.label_description->setText(this->m_paramDescription);
	if (this->m_paramDescription == ""){
		ui.label_description->setVisible(false);
	}

	this->m_isAddMode = false;
	this->local_data = data;
	this->m_modifyLastData = data;
	this->putDataToUi();
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
QString W_QStringEditor::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_QStringEditor::putDataToUi() {
	ui.lineEdit->setText(this->local_data);
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_QStringEditor::putUiToData() {
	this->local_data = ui.lineEdit->text();
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_QStringEditor::acceptData(){
	this->putUiToData();

	// > 空校验
	if (this->m_notNull == true && this->local_data.isEmpty()) {
		QMessageBox::warning(this, "提示", "必填项不能为空。");
		return;
	}
	// > 重复校验
	if (this->m_noRepeat == true &&
		this->m_windowParent != nullptr &&
		this->m_windowParent->getData().contains(this->local_data) ) {

		// > 添加时
		if (this->m_isAddMode){
			QMessageBox::warning(this, "提示", this->m_paramShowingName + "不能重复。");
			return;

		// > 修改时
		}else{
			if (this->m_modifyLastData == this->local_data){
				// （未编辑直接点确定，不操作）
			}else{
				QMessageBox::warning(this, "提示", this->m_paramShowingName + "不能重复。");
				return;
			}
		}
	}

	emit acceptDataSignal(this->local_data);
	this->accept();
};