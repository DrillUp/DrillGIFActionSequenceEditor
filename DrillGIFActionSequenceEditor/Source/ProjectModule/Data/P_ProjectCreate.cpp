#include "stdafx.h"
#include "P_ProjectCreate.h"
#include "../S_ProjectManager.h"

/*
-----==========================================================-----
		类：		新建项目块.cpp
		所属模块：	项目管理模块
		功能：		经过该流程后，将新建项目文件夹。
					比如新建项目、另存为项目。
					
		使用方法：
				>初始化
					m_P_ProjectCreate = new P_ProjectCreate(this);
					QVBoxLayout* latout = new QVBoxLayout(ui.widget);
					latout->addWidget(m_P_ProjectCreate);
					latout->setMargin(0);
				>设置
					m_P_ProjectCreate->setDataInModifyMode(this->local_ProjectData);
				>获取（会校验）
					this->local_ProjectData = m_P_ProjectCreate->getData();

-----==========================================================-----
*/

P_ProjectCreate::P_ProjectCreate(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----参数初始化

	// > 添加历史目录
	QList<QDir> dir_list = S_ProjectManager::getInstance()->getHistoryDirTank();
	for (int i = 0; i < dir_list.count(); i++){
		this->m_pathList.append(dir_list.at(i).absolutePath());
	}

	// > 添加当前目录
	QDir dir(this->local_ProjectData.getProjectRootPath());
	dir.mkdir(this->local_ProjectData.getProjectRootPath());
	this->insertPath(this->local_ProjectData.getProjectRootPath());
	this->selectPath(this->local_ProjectData.getProjectRootPath());

	//-----------------------------------
	//----事件绑定
	connect(ui.pushButton, &QPushButton::clicked, this, &P_ProjectCreate::chooseFiles);
	connect(ui.lineEdit_name, &QLineEdit::textChanged, this, &P_ProjectCreate::nameChanged);
	connect(ui.comboBox_path, &QComboBox::currentTextChanged, this, &P_ProjectCreate::pathChanged);
	connect(ui.checkBox, &QCheckBox::stateChanged, this, &P_ProjectCreate::radioChanged);

	//-----------------------------------
	//----ui初始化
	ui.lineEdit_name->setValidator(new QRegExpValidator(QRegExp("[^/\\\\]*")));
}
P_ProjectCreate::~P_ProjectCreate()
{
}

/*-----------------------------------
		操作 - 名称修改
*/
void P_ProjectCreate::nameChanged(QString s) {
	this->refreshOutput();
}
/*-----------------------------------
		操作 - 路径修改
*/
void P_ProjectCreate::pathChanged(QString s) {
	this->refreshOutput();
}
/*-----------------------------------
		操作 - 选择文件夹
*/
void P_ProjectCreate::chooseFiles() {
	QFileDialog fd;
	QString path;
	path = fd.getExistingDirectory(this, ("请选择文件夹"), this->local_ProjectData.getParentPath());
	if (path == "") {
		return;
	}
	this->insertPath(path);
	this->selectPath(path);
	this->refreshOutput();
}
/*-----------------------------------
		操作 - 建立文件夹修改
*/
void P_ProjectCreate::radioChanged(bool toggled) {
	this->refreshOutput();
}
/*-----------------------------------
		操作 - 刷新完整路径
*/
void P_ProjectCreate::refreshOutput() {
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
		位置 - 添加位置
*/
void P_ProjectCreate::insertPath(QString path){

	// > 去掉末尾斜杠
	if (path.at(path.size() - 1) == '/'){
		path = path.mid(0, path.size() - 1);
	}

	// > 重刷列表
	if (this->m_pathList.contains(path)){ return; }
	this->m_pathList.insert(0, path);
	ui.comboBox_path->clear();
	ui.comboBox_path->addItems(m_pathList);
}
/*-------------------------------------------------
		位置 - 选择位置
*/
void P_ProjectCreate::selectPath(QString path){

	// > 去掉末尾斜杠
	if (path.at(path.size() - 1) == '/'){
		path = path.mid(0, path.size() - 1);
	}
	ui.comboBox_path->setCurrentText(path);
}
/*-------------------------------------------------
		位置 - 获取选择的位置
*/
QString P_ProjectCreate::getCurrentPath(){
	return ui.comboBox_path->currentText();
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_ProjectCreate::setDataInModifyMode(C_ProjectData p) {
	this->local_ProjectData = p;
	this->putDataToUi();
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
C_ProjectData P_ProjectCreate::getData(){
	this->putUiToData();

	// > 校验
	QString name = ui.lineEdit_name->text();
	if (name == "") {
		QMessageBox::warning(this, ("错误"), ("名称不能为空。"));
		return C_ProjectData();
	}
	if (this->getCurrentPath() == "") {
		QMessageBox::warning(this, ("错误"), ("位置不能为空。"));
		return C_ProjectData();
	}

	return this->local_ProjectData;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_ProjectCreate::putDataToUi() {

	ui.lineEdit_name->setText(local_ProjectData.getName());
	this->insertPath(local_ProjectData.getProjectRootPath());

	ui.checkBox->setChecked(true);
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_ProjectCreate::putUiToData() {
	local_ProjectData.setName( ui.lineEdit_name->text() );
	if (ui.checkBox->isChecked()) {
		local_ProjectData.setPath( this->getCurrentPath() + "/" + ui.lineEdit_name->text() );
	}else {
		local_ProjectData.setPath( this->getCurrentPath() );
	}
};