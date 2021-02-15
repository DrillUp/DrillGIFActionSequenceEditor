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
	//----参数初始化
	QDir dir(this->local_ProjectData.getProjectRootPath());
	dir.mkdir(this->local_ProjectData.getProjectRootPath());
	this->addPath(this->local_ProjectData.getProjectRootPath());

	//-----------------------------------
	//----事件绑定
	connect(ui.pushButton, &QPushButton::clicked, this, &W_ProjectCreate::chooseFiles);
	connect(ui.lineEdit_name, &QLineEdit::textChanged, this, &W_ProjectCreate::nameChanged);
	connect(ui.comboBox_path, &QComboBox::currentTextChanged, this, &W_ProjectCreate::pathChanged);
	connect(ui.checkBox, &QCheckBox::stateChanged, this, &W_ProjectCreate::radioChanged);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));

	//-----------------------------------
	//----ui初始化
	ui.lineEdit_name->setValidator(new QRegExpValidator(QRegExp("[^/\\\\]*")));
	ui.buttonBox->button(QDialogButtonBox::Ok)->setText( ("确定"));
	ui.buttonBox->button(QDialogButtonBox::Cancel)->setText( ("取消"));
}
W_ProjectCreate::~W_ProjectCreate()
{
}

/*-----------------------------------
		操作 - 名称修改
*/
void W_ProjectCreate::nameChanged(QString s) {
	this->refreshOutput();
}
/*-----------------------------------
		操作 - 路径修改
*/
void W_ProjectCreate::pathChanged(QString s) {
	this->refreshOutput();
}
/*-----------------------------------
		操作 - 选择文件夹
*/
void W_ProjectCreate::chooseFiles() {
	QFileDialog fd;
	QString path;
	path = fd.getExistingDirectory(this, ("请选择文件夹"), this->local_ProjectData.getParentPath());
	if (path == "") {
		return;
	}
	this->addPath(path);
	this->selectPath(path);
	this->refreshOutput();
}
/*-----------------------------------
		操作 - 建立文件夹修改
*/
void W_ProjectCreate::radioChanged(bool toggled) {
	this->refreshOutput();
}
/*-----------------------------------
		操作 - 刷新完整路径
*/
void W_ProjectCreate::refreshOutput() {
	QString name = ui.lineEdit_name->text();
	if (name == "") {
		ui.lineEdit_outPut->setText("未填写名称");
		return;
	}
	if (ui.checkBox->isChecked()) {
		ui.lineEdit_outPut->setText(this->getCurrentPath() + "/" + name + "/" + name + "." + PROJECT_SUFFIX);
	}else {
		ui.lineEdit_outPut->setText(this->getCurrentPath() + "/" + name + "." + PROJECT_SUFFIX);
	}
}

/*-------------------------------------------------
		位置 - 添加位置（这里将ui封装了一下，防止路径混乱调用）
*/
void W_ProjectCreate::addPath(QString path){

	// > 去掉末尾斜杠
	if (path.at(path.size() - 1) == '/'){
		path = path.mid(0, path.size() - 1);
	}

	// > 重刷列表
	if (this->m_pathList.contains(path)){ return; }
	this->m_pathList.append(path);
	ui.comboBox_path->clear();
	ui.comboBox_path->addItems(m_pathList);
}
/*-------------------------------------------------
		位置 - 添加位置（这里将ui封装了一下，防止路径混乱调用）
*/
void W_ProjectCreate::selectPath(QString path){

	// > 去掉末尾斜杠
	if (path.at(path.size() - 1) == '/'){
		path = path.mid(0, path.size() - 1);
	}
	ui.comboBox_path->setCurrentText(path);
}
/*-------------------------------------------------
		位置 - 获取选择的位置（这里将ui封装了一下，防止路径混乱调用）
*/
QString W_ProjectCreate::getCurrentPath(){
	return ui.comboBox_path->currentText();
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void W_ProjectCreate::setDataInModifyMode(C_ProjectData p) {
	this->local_ProjectData = p;
	this->putDataToUi();
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
C_ProjectData W_ProjectCreate::getData(){
	return this->local_ProjectData;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_ProjectCreate::putDataToUi() {

	ui.lineEdit_name->setText(local_ProjectData.getName());
	this->addPath(local_ProjectData.getProjectRootPath());

	ui.checkBox->setChecked(true);
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_ProjectCreate::putUiToData() {
	local_ProjectData.setName( ui.lineEdit_name->text() );
	if (ui.checkBox->isChecked()) {
		local_ProjectData.setPath( this->getCurrentPath() + "/" + ui.lineEdit_name->text() );
	}else {
		local_ProjectData.setPath( this->getCurrentPath() );
	}
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_ProjectCreate::acceptData(){
	this->putUiToData();

	//校验
	QString name = ui.lineEdit_name->text();
	if (name == "") {
		QMessageBox::warning(this,  ("错误"), ("名称不能为空。"));
		return;
	}
	if (this->getCurrentPath() == "") {
		QMessageBox::warning(this, ("错误"), ("位置不能为空。"));
		return;
	}

	this->accept();
};