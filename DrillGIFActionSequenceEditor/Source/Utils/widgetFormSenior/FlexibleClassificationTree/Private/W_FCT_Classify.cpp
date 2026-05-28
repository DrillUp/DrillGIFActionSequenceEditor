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

W_FCT_Classify::W_FCT_Classify(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----参数初始化
	this->m_isAddMode = true;

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
		控件 - 控件初始化
*/
void W_FCT_Classify::initWidget(C_FCT_Config* config, bool isAddMode){
	this->m_configPtr = config;
	this->m_isAddMode = isAddMode;
	
	// > 窗口名称
	QString treeName = this->m_configPtr->getTreeName();
	if (isAddMode == true){
		if (treeName == ""){
			this->setWindowTitle("添加种类");
		}else{
			QString window_title = treeName;
			window_title.append(" - 添加种类");
			this->setWindowTitle(window_title);
		}
	}else{
		if (treeName == ""){
			this->setWindowTitle("编辑种类");
		}else{
			QString window_title = treeName;
			window_title.append(" - 编辑种类");
			this->setWindowTitle(window_title);
		}
	}
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void W_FCT_Classify::setData(C_FCT_Classify* p) {
	this->m_dataPtr = p;
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
	QStringList all_name = this->m_configPtr->getAll_classify_NameList();

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
	if (this->m_isAddMode == true && all_name.contains(tag_name)){
		QMessageBox::warning(this, "提示", "种类名称\"" + tag_name + "\"已存在。");
		return false;
	}
	// > 编辑校验（编辑成新名称，新名称重复情况）
	if (this->m_isAddMode == false &&
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