#include "stdafx.h"
#include "W_ALEDataEdit.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		����֡�༭ ����.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�༭����֡��һЩ���á�
					ע�⣬���Ƕ���ѡ������˽�����ô��ڡ�һ�㲻���á�

		ʹ�÷�����
				>�򿪴���
					W_ALEDataEdit d;
					d.setDataInModifyMode(this->getConfigParam());
					if (d.exec() == QDialog::Accepted){
						this->setConfigParam( d.getData() );
					}
-----==========================================================-----
*/

W_ALEDataEdit::W_ALEDataEdit(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->local_index = -1;
	this->local_index_list = QList<int>();
	this->m_slotBlock = false;

	//-----------------------------------
	//----�¼���
	connect(ui.pushButton, &QPushButton::clicked, this, &W_ALEDataEdit::chooseFile);
	connect(ui.comboBox, &QComboBox::currentTextChanged, this, &W_ALEDataEdit::timeSelected);
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(timeEdited(double)));
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ALEDataEdit::acceptData);
	
	//-----------------------------------
	//----��ʼ��ui
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("ȷ��"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("ȡ��"); }
}

W_ALEDataEdit::~W_ALEDataEdit(){
}

/*-------------------------------------------------
		�س��¼�����
*/
void W_ALEDataEdit::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}

/*-------------------------------------------------
		�ؼ� - ѡ���ļ������ţ�
*/
void W_ALEDataEdit::chooseFile(){
	if (this->local_index == -1){ return; }

	QString result = "";
	QFileDialog fd;
	fd.setWindowTitle("����GIF");
	fd.setAcceptMode(QFileDialog::AcceptOpen);
	fd.setNameFilters(QStringList() << "ͼƬ(*.png)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//�����ļ�
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ; }
		result = fd.selectedFiles().at(0);
	}else {
		return ;
	}

	// > �ظ��ļ�
	QFileInfo fileinfo(result);
	QString file_name = fileinfo.completeBaseName();
	if (this->local_data.hasFileName(file_name) ||						//���������ļ� 
		this->local_data.getFileRoot() == fileinfo.absolutePath()){	//��ͬһ���ļ�����
		this->local_data.op_replace(this->local_index,file_name);
		ui.lineEdit->setText(file_name);
		return;
	}

	// > ���ظ��ļ�
	C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->local_data.getFileRoot() + "/" + fileinfo.fileName());
	this->local_data.op_replace(this->local_index, file_name);
	ui.lineEdit->setText(file_name);

}
/*-------------------------------------------------
		�ؼ� - ѡ������
*/
void W_ALEDataEdit::timeSelected(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "0.01��"){ ui.doubleSpinBox->setValue(0.01); }
	if (text == "0.02��"){ ui.doubleSpinBox->setValue(0.02); }
	if (text == "0.03��"){ ui.doubleSpinBox->setValue(0.03); }
	if (text == "0.04��"){ ui.doubleSpinBox->setValue(0.04); }
	if (text == "0.10��"){ ui.doubleSpinBox->setValue(0.1); }
	if (text == "0.50��"){ ui.doubleSpinBox->setValue(0.5); }
	if (text == "1.00��"){ ui.doubleSpinBox->setValue(1); }
	if (text == "5.00��"){ ui.doubleSpinBox->setValue(5); }
	
	this->m_slotBlock = false;
}
/*-------------------------------------------------
		�ؼ� - �༭����
*/
void W_ALEDataEdit::timeEdited(double value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 0.01){ ui.comboBox->setCurrentText("0.01��"); }
	else if (value == 0.02){ ui.comboBox->setCurrentText("0.02��"); }
	else if (value == 0.03){ ui.comboBox->setCurrentText("0.03��"); }
	else if (value == 0.04){ ui.comboBox->setCurrentText("0.04��"); }
	else if (value == 0.1){ ui.comboBox->setCurrentText("0.10��"); }
	else if (value == 0.5){ ui.comboBox->setCurrentText("0.50��"); }
	else if (value == 0.1){ ui.comboBox->setCurrentText("1.00��"); }
	else if (value == 0.5){ ui.comboBox->setCurrentText("5.00��"); }
	else{ ui.comboBox->setCurrentText("�Զ���"); }

	this->m_slotBlock = false;
}

/*-------------------------------------------------
		���� - �������ݣ��޸ģ�
*/
void W_ALEDataEdit::setDataInModifyMode_One(C_ALEData data, int index) {
	this->local_data = data;
	this->local_index = index;
	this->putDataToUi();
}
void W_ALEDataEdit::setDataInModifyMode_Multi(C_ALEData data, QList<int> index_list) {
	this->local_data = data;
	this->local_index_list = index_list;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - ȡ������
*/
C_ALEData W_ALEDataEdit::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_ALEDataEdit::putDataToUi() {

	// > ��֡�༭ʱ
	if (this->local_index != -1){

		ui.pushButton->setEnabled(true);
		QString file_name = this->local_data.getFile(local_index).completeBaseName();
		ui.lineEdit->setText(file_name);

		double value = this->local_data.getAllInterval().at(local_index)*0.01;
		ui.doubleSpinBox->setValue(value);

	// > ��֡�༭ʱ
	}else{

		ui.pushButton->setEnabled(false);
		ui.lineEdit->setText("�����֡��Դ��");

		double value = this->local_data.getAllInterval().at(local_index_list.at(0))*0.01;
		ui.doubleSpinBox->setValue(value);

	}
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_ALEDataEdit::putUiToData() {
	//��ͼƬ��Դֱ�Ӹ�ֵ��

	if (this->local_index != -1){
		this->local_data.op_replace_interval(local_index, ui.doubleSpinBox->value() * 100);
	}else{
		this->local_data.op_replace_interval(local_index_list, ui.doubleSpinBox->value() * 100);
	}
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_ALEDataEdit::acceptData(){
	this->putUiToData();

	this->accept();
};