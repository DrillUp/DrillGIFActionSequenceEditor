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
	//----��ʼ��ui
	ui.comboBox_frameUnit->clear();
	ui.comboBox_frameUnit->setView(new QListView());
	ui.comboBox_frameUnit->addItems(QStringList()
		<< "1֡" << "2֡" << "3֡" << "4֡" << "5֡"
		<< "10֡" << "20֡" << "30֡" << "40֡" << "50֡"
		<< "100֡" << "500֡" << "�Զ���");
	ui.comboBox_secondUnit->clear();
	ui.comboBox_secondUnit->setView(new QListView());
	ui.comboBox_secondUnit->addItems(QStringList()
		<< "0.01��" << "0.02��" << "0.03��" << "0.04��" << "0.05��"
		<< "0.10��" << "0.20��" << "0.30��" << "0.40��" << "0.50��"
		<< "1.00��" << "5.00��" << "�Զ���");
	TTool::_chinese_(ui.buttonBox);

	//-----------------------------------
	//----�¼���
	connect(ui.pushButton, &QPushButton::clicked, this, &W_ALEDataEdit::chooseFile);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ALEDataEdit::acceptData);

	connect(ui.comboBox_frameUnit, &QComboBox::currentTextChanged, this, &W_ALEDataEdit::timeSelected_frameUnit);
	connect(ui.spinBox_frameUnit, SIGNAL(valueChanged(int)), this, SLOT(timeEdited_frameUnit(int)));
	connect(ui.comboBox_secondUnit, &QComboBox::currentTextChanged, this, &W_ALEDataEdit::timeSelected_secondUnit);
	connect(ui.doubleSpinBox_secondUnit, SIGNAL(valueChanged(double)), this, SLOT(timeEdited_secondUnit(double)));
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
	fd.setWindowTitle("ѡ����ͼƬ");
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
void W_ALEDataEdit::timeSelected_frameUnit(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "1֡"){ ui.spinBox_frameUnit->setValue(1); }
	if (text == "2֡"){ ui.spinBox_frameUnit->setValue(2); }
	if (text == "3֡"){ ui.spinBox_frameUnit->setValue(3); }
	if (text == "4֡"){ ui.spinBox_frameUnit->setValue(4); }
	if (text == "5֡"){ ui.spinBox_frameUnit->setValue(5); }
	if (text == "10֡"){ ui.spinBox_frameUnit->setValue(10); }
	if (text == "20֡"){ ui.spinBox_frameUnit->setValue(20); }
	if (text == "30֡"){ ui.spinBox_frameUnit->setValue(30); }
	if (text == "40֡"){ ui.spinBox_frameUnit->setValue(40); }
	if (text == "50֡"){ ui.spinBox_frameUnit->setValue(50); }
	if (text == "100֡"){ ui.spinBox_frameUnit->setValue(100); }
	if (text == "500֡"){ ui.spinBox_frameUnit->setValue(500); }

	this->m_slotBlock = false;
}
/*-------------------------------------------------
		�ؼ� - �༭����
*/
void W_ALEDataEdit::timeEdited_frameUnit(int value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 1){ ui.comboBox_frameUnit->setCurrentText("1֡"); }
	else if (value == 2){ ui.comboBox_frameUnit->setCurrentText("2֡"); }
	else if (value == 3){ ui.comboBox_frameUnit->setCurrentText("3֡"); }
	else if (value == 4){ ui.comboBox_frameUnit->setCurrentText("4֡"); }
	else if (value == 5){ ui.comboBox_frameUnit->setCurrentText("5֡"); }
	else if (value == 10){ ui.comboBox_frameUnit->setCurrentText("10֡"); }
	else if (value == 20){ ui.comboBox_frameUnit->setCurrentText("20֡"); }
	else if (value == 30){ ui.comboBox_frameUnit->setCurrentText("30֡"); }
	else if (value == 40){ ui.comboBox_frameUnit->setCurrentText("40֡"); }
	else if (value == 50){ ui.comboBox_frameUnit->setCurrentText("50֡"); }
	else if (value == 100){ ui.comboBox_frameUnit->setCurrentText("100֡"); }
	else if (value == 500){ ui.comboBox_frameUnit->setCurrentText("500֡"); }
	else{ ui.comboBox_frameUnit->setCurrentText("�Զ���"); }

	this->m_slotBlock = false;
}
/*-------------------------------------------------
		�ؼ� - ѡ������
*/
void W_ALEDataEdit::timeSelected_secondUnit(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "0.01��"){ ui.doubleSpinBox_secondUnit->setValue(0.01); }
	if (text == "0.02��"){ ui.doubleSpinBox_secondUnit->setValue(0.02); }
	if (text == "0.03��"){ ui.doubleSpinBox_secondUnit->setValue(0.03); }
	if (text == "0.04��"){ ui.doubleSpinBox_secondUnit->setValue(0.04); }
	if (text == "0.05��"){ ui.doubleSpinBox_secondUnit->setValue(0.05); }
	if (text == "0.10��"){ ui.doubleSpinBox_secondUnit->setValue(0.1); }
	if (text == "0.20��"){ ui.doubleSpinBox_secondUnit->setValue(0.2); }
	if (text == "0.30��"){ ui.doubleSpinBox_secondUnit->setValue(0.3); }
	if (text == "0.40��"){ ui.doubleSpinBox_secondUnit->setValue(0.4); }
	if (text == "0.50��"){ ui.doubleSpinBox_secondUnit->setValue(0.5); }
	if (text == "1.00��"){ ui.doubleSpinBox_secondUnit->setValue(1); }
	if (text == "5.00��"){ ui.doubleSpinBox_secondUnit->setValue(5); }
	
	this->m_slotBlock = false;
}
/*-------------------------------------------------
		�ؼ� - �༭����
*/
void W_ALEDataEdit::timeEdited_secondUnit(double value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 0.01){ ui.comboBox_secondUnit->setCurrentText("0.01��"); }
	else if (value == 0.02){ ui.comboBox_secondUnit->setCurrentText("0.02��"); }
	else if (value == 0.03){ ui.comboBox_secondUnit->setCurrentText("0.03��"); }
	else if (value == 0.04){ ui.comboBox_secondUnit->setCurrentText("0.04��"); }
	else if (value == 0.05){ ui.comboBox_secondUnit->setCurrentText("0.05��"); }
	else if (value == 0.1){ ui.comboBox_secondUnit->setCurrentText("0.10��"); }
	else if (value == 0.2){ ui.comboBox_secondUnit->setCurrentText("0.20��"); }
	else if (value == 0.3){ ui.comboBox_secondUnit->setCurrentText("0.30��"); }
	else if (value == 0.4){ ui.comboBox_secondUnit->setCurrentText("0.40��"); }
	else if (value == 0.5){ ui.comboBox_secondUnit->setCurrentText("0.50��"); }
	else if (value == 1){ ui.comboBox_secondUnit->setCurrentText("1.00��"); }
	else if (value == 5){ ui.comboBox_secondUnit->setCurrentText("5.00��"); }
	else{ ui.comboBox_secondUnit->setCurrentText("�Զ���"); }

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

	// > ��λ
	if (this->local_data.getData_Unit() == C_ALEData::FrameUnit){
		ui.stackedWidget->setCurrentIndex(0);
	}
	if (this->local_data.getData_Unit() == C_ALEData::SecondUnit){
		ui.stackedWidget->setCurrentIndex(1);
	}

	// > ��֡�༭ʱ
	if (this->local_index != -1){

		ui.pushButton->setEnabled(true);
		QString file_name = this->local_data.getFile(local_index).completeBaseName();
		ui.lineEdit->setText(file_name);

		int value = this->local_data.getData_IntervalTank().at(local_index);
		ui.spinBox_frameUnit->setValue(value);
		ui.doubleSpinBox_secondUnit->setValue(value*0.01);

	// > ��֡�༭ʱ
	}else{

		ui.pushButton->setEnabled(false);
		ui.lineEdit->setText("�����֡��Դ��");

		int value = this->local_data.getData_IntervalTank().at(local_index_list.at(0));
		ui.spinBox_frameUnit->setValue(value);
		ui.doubleSpinBox_secondUnit->setValue(value*0.01);

	}
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_ALEDataEdit::putUiToData() {
	//��ͼƬ��Դֱ�Ӹ�ֵ��

	// > ֡��λʱ
	if (this->local_data.getData_Unit() == C_ALEData::FrameUnit){
		if (this->local_index != -1){
			this->local_data.op_replace_interval(local_index, ui.spinBox_frameUnit->value());
		}
		else{
			this->local_data.op_replace_interval(local_index_list, ui.spinBox_frameUnit->value());
		}
	}
	// > �뵥λʱ
	if (this->local_data.getData_Unit() == C_ALEData::SecondUnit){
		if (this->local_index != -1){
			this->local_data.op_replace_interval(local_index, ui.doubleSpinBox_secondUnit->value() * 100);
		}
		else{
			this->local_data.op_replace_interval(local_index_list, ui.doubleSpinBox_secondUnit->value() * 100);
		}
	}
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_ALEDataEdit::acceptData(){
	this->putUiToData();

	this->accept();
};