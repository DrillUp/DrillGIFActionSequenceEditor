#include "stdafx.h"
#include "W_ALEDataEdit.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动画帧编辑 窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑动画帧的一些配置。
					注意，这是对象选择器的私有配置窗口。一般不外用。

		使用方法：
				>打开窗口
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
	//----初始化参数
	this->local_index = -1;
	this->local_index_list = QList<int>();
	this->m_slotBlock = false;

	//-----------------------------------
	//----事件绑定
	connect(ui.pushButton, &QPushButton::clicked, this, &W_ALEDataEdit::chooseFile);
	connect(ui.comboBox, &QComboBox::currentTextChanged, this, &W_ALEDataEdit::timeSelected);
	connect(ui.doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(timeEdited(double)));
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ALEDataEdit::acceptData);
	
	//-----------------------------------
	//----初始化ui
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("确定"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("取消"); }
}

W_ALEDataEdit::~W_ALEDataEdit(){
}

/*-------------------------------------------------
		回车事件过滤
*/
void W_ALEDataEdit::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}

/*-------------------------------------------------
		控件 - 选择文件（单张）
*/
void W_ALEDataEdit::chooseFile(){
	if (this->local_index == -1){ return; }

	QString result = "";
	QFileDialog fd;
	fd.setWindowTitle("导入GIF");
	fd.setAcceptMode(QFileDialog::AcceptOpen);
	fd.setNameFilters(QStringList() << "图片(*.png)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//单个文件
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ; }
		result = fd.selectedFiles().at(0);
	}else {
		return ;
	}

	// > 重复文件
	QFileInfo fileinfo(result);
	QString file_name = fileinfo.completeBaseName();
	if (this->local_data.hasFileName(file_name) ||						//含有重名文件 
		this->local_data.getFileRoot() == fileinfo.absolutePath()){	//在同一个文件夹下
		this->local_data.op_replace(this->local_index,file_name);
		ui.lineEdit->setText(file_name);
		return;
	}

	// > 不重复文件
	C_ALEData::copyFile(fileinfo.absoluteFilePath(), this->local_data.getFileRoot() + "/" + fileinfo.fileName());
	this->local_data.op_replace(this->local_index, file_name);
	ui.lineEdit->setText(file_name);

}
/*-------------------------------------------------
		控件 - 选择数字
*/
void W_ALEDataEdit::timeSelected(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "0.01秒"){ ui.doubleSpinBox->setValue(0.01); }
	if (text == "0.02秒"){ ui.doubleSpinBox->setValue(0.02); }
	if (text == "0.03秒"){ ui.doubleSpinBox->setValue(0.03); }
	if (text == "0.04秒"){ ui.doubleSpinBox->setValue(0.04); }
	if (text == "0.10秒"){ ui.doubleSpinBox->setValue(0.1); }
	if (text == "0.50秒"){ ui.doubleSpinBox->setValue(0.5); }
	if (text == "1.00秒"){ ui.doubleSpinBox->setValue(1); }
	if (text == "5.00秒"){ ui.doubleSpinBox->setValue(5); }
	
	this->m_slotBlock = false;
}
/*-------------------------------------------------
		控件 - 编辑数字
*/
void W_ALEDataEdit::timeEdited(double value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 0.01){ ui.comboBox->setCurrentText("0.01秒"); }
	else if (value == 0.02){ ui.comboBox->setCurrentText("0.02秒"); }
	else if (value == 0.03){ ui.comboBox->setCurrentText("0.03秒"); }
	else if (value == 0.04){ ui.comboBox->setCurrentText("0.04秒"); }
	else if (value == 0.1){ ui.comboBox->setCurrentText("0.10秒"); }
	else if (value == 0.5){ ui.comboBox->setCurrentText("0.50秒"); }
	else if (value == 0.1){ ui.comboBox->setCurrentText("1.00秒"); }
	else if (value == 0.5){ ui.comboBox->setCurrentText("5.00秒"); }
	else{ ui.comboBox->setCurrentText("自定义"); }

	this->m_slotBlock = false;
}

/*-------------------------------------------------
		窗口 - 设置数据（修改）
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
		窗口 - 取出数据
*/
C_ALEData W_ALEDataEdit::getData(){
	return this->local_data;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_ALEDataEdit::putDataToUi() {

	// > 单帧编辑时
	if (this->local_index != -1){

		ui.pushButton->setEnabled(true);
		QString file_name = this->local_data.getFile(local_index).completeBaseName();
		ui.lineEdit->setText(file_name);

		double value = this->local_data.getAllInterval().at(local_index)*0.01;
		ui.doubleSpinBox->setValue(value);

	// > 多帧编辑时
	}else{

		ui.pushButton->setEnabled(false);
		ui.lineEdit->setText("（多个帧资源）");

		double value = this->local_data.getAllInterval().at(local_index_list.at(0))*0.01;
		ui.doubleSpinBox->setValue(value);

	}
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_ALEDataEdit::putUiToData() {
	//（图片资源直接赋值）

	if (this->local_index != -1){
		this->local_data.op_replace_interval(local_index, ui.doubleSpinBox->value() * 100);
	}else{
		this->local_data.op_replace_interval(local_index_list, ui.doubleSpinBox->value() * 100);
	}
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_ALEDataEdit::acceptData(){
	this->putUiToData();

	this->accept();
};