#include "stdafx.h"
#include "P_ProjectCreate.h"
#include "../s_ProjectManager.h"

/*
-----==========================================================-----
		�ࣺ		�½���Ŀ��.cpp
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		���������̺󣬽��½���Ŀ�ļ��С�
					�����½���Ŀ�����Ϊ��Ŀ��
					
		ʹ�÷�����
				>��ʼ��
					m_P_ProjectCreate = new P_ProjectCreate(this);
					QVBoxLayout* latout = new QVBoxLayout(ui.widget);
					latout->addWidget(m_P_ProjectCreate);
					latout->setMargin(0);
				>����
					m_P_ProjectCreate->setDataInModifyMode(this->local_ProjectData);
				>��ȡ����У�飩
					this->local_ProjectData = m_P_ProjectCreate->getData();

-----==========================================================-----
*/

P_ProjectCreate::P_ProjectCreate(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----������ʼ��

	// > �����ʷĿ¼
	QList<QDir> dir_list = S_ProjectManager::getInstance()->getHistoryDirTank();
	for (int i = 0; i < dir_list.count(); i++){
		this->m_pathList.append(dir_list.at(i).absolutePath());
	}

	// > ��ӵ�ǰĿ¼
	QDir dir(this->local_ProjectData.getProjectRootPath());
	dir.mkdir(this->local_ProjectData.getProjectRootPath());
	this->insertPath(this->local_ProjectData.getProjectRootPath());
	this->selectPath(this->local_ProjectData.getProjectRootPath());

	//-----------------------------------
	//----�¼���
	connect(ui.pushButton, &QPushButton::clicked, this, &P_ProjectCreate::chooseFiles);
	connect(ui.lineEdit_name, &QLineEdit::textChanged, this, &P_ProjectCreate::nameChanged);
	connect(ui.comboBox_path, &QComboBox::currentTextChanged, this, &P_ProjectCreate::pathChanged);
	connect(ui.checkBox, &QCheckBox::stateChanged, this, &P_ProjectCreate::radioChanged);

	//-----------------------------------
	//----ui��ʼ��
	ui.lineEdit_name->setValidator(new QRegExpValidator(QRegExp("[^/\\\\]*")));
}
P_ProjectCreate::~P_ProjectCreate()
{
}

/*-----------------------------------
		���� - �����޸�
*/
void P_ProjectCreate::nameChanged(QString s) {
	this->refreshOutput();
}
/*-----------------------------------
		���� - ·���޸�
*/
void P_ProjectCreate::pathChanged(QString s) {
	this->refreshOutput();
}
/*-----------------------------------
		���� - ѡ���ļ���
*/
void P_ProjectCreate::chooseFiles() {
	QFileDialog fd;
	QString path;
	path = fd.getExistingDirectory(this, ("��ѡ���ļ���"), this->local_ProjectData.getParentPath());
	if (path == "") {
		return;
	}
	this->insertPath(path);
	this->selectPath(path);
	this->refreshOutput();
}
/*-----------------------------------
		���� - �����ļ����޸�
*/
void P_ProjectCreate::radioChanged(bool toggled) {
	this->refreshOutput();
}
/*-----------------------------------
		���� - ˢ������·��
*/
void P_ProjectCreate::refreshOutput() {
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
		λ�� - ���λ��
*/
void P_ProjectCreate::insertPath(QString path){

	// > ȥ��ĩβб��
	if (path.at(path.size() - 1) == '/'){
		path = path.mid(0, path.size() - 1);
	}

	// > ��ˢ�б�
	if (this->m_pathList.contains(path)){ return; }
	this->m_pathList.insert(0, path);
	ui.comboBox_path->clear();
	ui.comboBox_path->addItems(m_pathList);
}
/*-------------------------------------------------
		λ�� - ѡ��λ��
*/
void P_ProjectCreate::selectPath(QString path){

	// > ȥ��ĩβб��
	if (path.at(path.size() - 1) == '/'){
		path = path.mid(0, path.size() - 1);
	}
	ui.comboBox_path->setCurrentText(path);
}
/*-------------------------------------------------
		λ�� - ��ȡѡ���λ��
*/
QString P_ProjectCreate::getCurrentPath(){
	return ui.comboBox_path->currentText();
}


/*-------------------------------------------------
		���� - ��������
*/
void P_ProjectCreate::setDataInModifyMode(C_ProjectData p) {
	this->local_ProjectData = p;
	this->putDataToUi();
}

/*-------------------------------------------------
		���� - ȡ������
*/
C_ProjectData P_ProjectCreate::getData(){
	this->putUiToData();

	// > У��
	QString name = ui.lineEdit_name->text();
	if (name == "") {
		QMessageBox::warning(this, ("����"), ("���Ʋ���Ϊ�ա�"));
		return C_ProjectData();
	}
	if (this->getCurrentPath() == "") {
		QMessageBox::warning(this, ("����"), ("λ�ò���Ϊ�ա�"));
		return C_ProjectData();
	}

	return this->local_ProjectData;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void P_ProjectCreate::putDataToUi() {

	ui.lineEdit_name->setText(local_ProjectData.getName());
	this->insertPath(local_ProjectData.getProjectRootPath());

	ui.checkBox->setChecked(true);
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_ProjectCreate::putUiToData() {
	local_ProjectData.setName( ui.lineEdit_name->text() );
	if (ui.checkBox->isChecked()) {
		local_ProjectData.setPath( this->getCurrentPath() + "/" + ui.lineEdit_name->text() );
	}else {
		local_ProjectData.setPath( this->getCurrentPath() );
	}
};