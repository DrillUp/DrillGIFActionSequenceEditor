#include "stdafx.h"
#include "w_ProjectCreate.h"
#include "../s_ProjectManager.h"

/*
-----==========================================================-----
		类：		新建项目窗口.cpp
		所属模块：	项目管理模块
		功能：		经过该流程后，将新建项目文件夹。
					比如新建项目、另存为项目。
-----==========================================================-----
*/

W_ProjectCreate::W_ProjectCreate(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	//-----------------------------------
	//----控件初始化
	this->m_P_ProjectCreate = new P_ProjectCreate();
	QVBoxLayout* l = new QVBoxLayout(ui.widget);
	ui.widget->setLayout(l);
	l->setMargin(0);
	l->addWidget(this->m_P_ProjectCreate);

	//-----------------------------------
	//----事件绑定
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));

	//-----------------------------------
	//----ui初始化
	ui.buttonBox->button(QDialogButtonBox::Ok)->setText( ("确定"));
	ui.buttonBox->button(QDialogButtonBox::Cancel)->setText( ("取消"));
}
W_ProjectCreate::~W_ProjectCreate(){
}


/*-------------------------------------------------
		位置 - 添加位置
*/
void W_ProjectCreate::insertPath(QString path){
	this->m_P_ProjectCreate->insertPath(path);
}
/*-------------------------------------------------
		位置 - 选择位置
*/
void W_ProjectCreate::selectPath(QString path){
	this->m_P_ProjectCreate->selectPath(path);
}
/*-------------------------------------------------
		位置 - 获取选择的位置
*/
QString W_ProjectCreate::getCurrentPath(){
	return this->m_P_ProjectCreate->getCurrentPath();
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void W_ProjectCreate::setDataInModifyMode(C_ProjectData p) {
	this->m_P_ProjectCreate->setDataInModifyMode(p);
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
C_ProjectData W_ProjectCreate::getData(){
	return this->m_P_ProjectCreate->getData();
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_ProjectCreate::acceptData(){

	C_ProjectData data = this->getData();
	if (data.isNull()) { return; }

	this->accept();
};