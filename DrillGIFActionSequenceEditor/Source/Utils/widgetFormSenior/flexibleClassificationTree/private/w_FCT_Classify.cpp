#include "stdafx.h"
#include "W_FCT_Classify.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		种类 配置窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		种类的设置内容在该窗口设置。
-----==========================================================-----
*/

W_FCT_Classify::W_FCT_Classify(P_FlexibleClassificationTree* p_obj)
	: QDialog(p_obj->getTree())
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->isAddMode = true;
	this->m_parentObj = p_obj;

	//-----------------------------------
	//----事件绑定
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));

	//-----------------------------------
	//----初始化ui
	TTool::_chinese_(ui.buttonBox);

}
W_FCT_Classify::~W_FCT_Classify(){
}




/*-------------------------------------------------
		窗口 - 设置数据（添加）
*/
void W_FCT_Classify::setDataInAddMode() {
	this->isAddMode = true;
	this->m_dataPtr = this->m_parentObj->createClassifyData();
	this->m_dataPtr->setName("新的种类");
	this->m_dataPtr->setDescription("没有描述。");
	this->setWindowTitle("添加种类");
	this->putDataToUi();
};
/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_FCT_Classify::setDataInModifyMode(C_FCT_Classify* p) {
	this->isAddMode = false;
	this->m_dataPtr = p;
	this->setWindowTitle("编辑种类");
	this->putDataToUi();
};
/*-------------------------------------------------
		窗口 - 取出数据
*/
C_FCT_Classify* W_FCT_Classify::getData(){
	return this->m_dataPtr;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_FCT_Classify::putDataToUi() {
	ui.lineEdit_tagName->setText(m_dataPtr->getName());
	ui.plainTextEdit->setPlainText(m_dataPtr->getDescription());
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_FCT_Classify::putUiToData() {
	//（注意，由于传的指针，赋值要最后决定了才执行）
	m_dataPtr->setName(ui.lineEdit_tagName->text());
	m_dataPtr->setDescription(ui.plainTextEdit->toPlainText());
};
/*-------------------------------------------------
		窗口 - 校验数据
*/
bool W_FCT_Classify::checkData(){
	QString old_name = m_dataPtr->getName();
	QString tag_name = ui.lineEdit_tagName->text();
	QStringList all_name = this->m_parentObj->getAllClassifyName();

	// > 空校验
	if (tag_name == "") {
		QMessageBox::warning(this, "提示", "种类名称不能为空。");
		return false;
	}
	if (tag_name == "未分类") {
		QMessageBox::warning(this, "提示", "种类名称不能为\"未分类\"。");
		return false;
	}

	// > 新增校验
	if (this->isAddMode == true && all_name.contains(tag_name)){
		QMessageBox::warning(this, "提示", "种类名称\"" + tag_name + "\"已存在。");
		return false;
	}
	// > 编辑校验（编辑成新名称，新名称重复情况）
	if (this->isAddMode == false &&
		old_name != tag_name &&
		all_name.contains(tag_name)){
		QMessageBox::warning(this, "提示", "种类名称\"" + tag_name + "\"已存在。");
		return false;
	}
	return true;
}
/*-------------------------------------------------
		窗口 - 提交数据
*/
void W_FCT_Classify::acceptData(){

	// > 校验
	bool pass = this->checkData();
	if ( pass == false){ return; }

	// > 提交
	this->putUiToData();
	this->accept();
};