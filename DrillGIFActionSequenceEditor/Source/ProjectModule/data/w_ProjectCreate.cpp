#include "stdafx.h"
#include "w_ProjectCreate.h"
#include "../s_ProjectManager.h"

/*
-----==========================================================-----
		�ࣺ		�½���Ŀ����.cpp
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		���������̺󣬽��½���Ŀ�ļ��С�
					�����½���Ŀ�����Ϊ��Ŀ��
-----==========================================================-----
*/

W_ProjectCreate::W_ProjectCreate(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----������ʼ��
	QDir dir(this->local_ProjectData.getProjectRootPath());
	dir.mkdir(this->local_ProjectData.getProjectRootPath());
	this->addPath(this->local_ProjectData.getProjectRootPath());

	//-----------------------------------
	//----�¼���
	connect(ui.pushButton, &QPushButton::clicked, this, &W_ProjectCreate::chooseFiles);
	connect(ui.lineEdit_name, &QLineEdit::textChanged, this, &W_ProjectCreate::nameChanged);
	connect(ui.comboBox_path, &QComboBox::currentTextChanged, this, &W_ProjectCreate::pathChanged);
	connect(ui.checkBox, &QCheckBox::stateChanged, this, &W_ProjectCreate::radioChanged);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));

	//-----------------------------------
	//----ui��ʼ��
	ui.lineEdit_name->setValidator(new QRegExpValidator(QRegExp("[^/\\\\]*")));
	ui.buttonBox->button(QDialogButtonBox::Ok)->setText( ("ȷ��"));
	ui.buttonBox->button(QDialogButtonBox::Cancel)->setText( ("ȡ��"));
}
W_ProjectCreate::~W_ProjectCreate()
{
}

/*-----------------------------------
		���� - �����޸�
*/
void W_ProjectCreate::nameChanged(QString s) {
	this->refreshOutput();
}
/*-----------------------------------
		���� - ·���޸�
*/
void W_ProjectCreate::pathChanged(QString s) {
	this->refreshOutput();
}
/*-----------------------------------
		���� - ѡ���ļ���
*/
void W_ProjectCreate::chooseFiles() {
	QFileDialog fd;
	QString path;
	path = fd.getExistingDirectory(this, ("��ѡ���ļ���"), this->local_ProjectData.getParentPath());
	if (path == "") {
		return;
	}
	this->addPath(path);
	this->selectPath(path);
	this->refreshOutput();
}
/*-----------------------------------
		���� - �����ļ����޸�
*/
void W_ProjectCreate::radioChanged(bool toggled) {
	this->refreshOutput();
}
/*-----------------------------------
		���� - ˢ������·��
*/
void W_ProjectCreate::refreshOutput() {
	QString name = ui.lineEdit_name->text();
	if (name == "") {
		ui.lineEdit_outPut->setText("δ��д����");
		return;
	}
	if (ui.checkBox->isChecked()) {
		ui.lineEdit_outPut->setText(this->getCurrentPath() + "/" + name + "/" + name + "." + PROJECT_SUFFIX);
	}else {
		ui.lineEdit_outPut->setText(this->getCurrentPath() + "/" + name + "." + PROJECT_SUFFIX);
	}
}

/*-------------------------------------------------
		λ�� - ���λ�ã����ｫui��װ��һ�£���ֹ·�����ҵ��ã�
*/
void W_ProjectCreate::addPath(QString path){

	// > ȥ��ĩβб��
	if (path.at(path.size() - 1) == '/'){
		path = path.mid(0, path.size() - 1);
	}

	// > ��ˢ�б�
	if (this->m_pathList.contains(path)){ return; }
	this->m_pathList.append(path);
	ui.comboBox_path->clear();
	ui.comboBox_path->addItems(m_pathList);
}
/*-------------------------------------------------
		λ�� - ���λ�ã����ｫui��װ��һ�£���ֹ·�����ҵ��ã�
*/
void W_ProjectCreate::selectPath(QString path){

	// > ȥ��ĩβб��
	if (path.at(path.size() - 1) == '/'){
		path = path.mid(0, path.size() - 1);
	}
	ui.comboBox_path->setCurrentText(path);
}
/*-------------------------------------------------
		λ�� - ��ȡѡ���λ�ã����ｫui��װ��һ�£���ֹ·�����ҵ��ã�
*/
QString W_ProjectCreate::getCurrentPath(){
	return ui.comboBox_path->currentText();
}


/*-------------------------------------------------
		���� - ��������
*/
void W_ProjectCreate::setDataInModifyMode(C_ProjectData p) {
	this->local_ProjectData = p;
	this->putDataToUi();
}

/*-------------------------------------------------
		���� - ȡ������
*/
C_ProjectData W_ProjectCreate::getData(){
	return this->local_ProjectData;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_ProjectCreate::putDataToUi() {

	ui.lineEdit_name->setText(local_ProjectData.getName());
	this->addPath(local_ProjectData.getProjectRootPath());

	ui.checkBox->setChecked(true);
};
/*-------------------------------------------------
		���� - ui���� -> ��������
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
		���� - �ύ���ݣ�У�飩
*/
void W_ProjectCreate::acceptData(){
	this->putUiToData();

	//У��
	QString name = ui.lineEdit_name->text();
	if (name == "") {
		QMessageBox::warning(this,  ("����"), ("���Ʋ���Ϊ�ա�"));
		return;
	}
	if (this->getCurrentPath() == "") {
		QMessageBox::warning(this, ("����"), ("λ�ò���Ϊ�ա�"));
		return;
	}

	this->accept();
};