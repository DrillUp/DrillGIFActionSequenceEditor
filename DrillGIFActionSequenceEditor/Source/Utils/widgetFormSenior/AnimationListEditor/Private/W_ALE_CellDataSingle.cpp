#include "stdafx.h"
#include "W_ALE_CellDataSingle.h"

#include "S_ALE_FileManager.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		编辑数据-单选 窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑数据-单选 的配置窗口。

		使用方法：
				>打开窗口
					W_ALE_CellDataSingle d;
					d.setDataInModifyMode(this->getConfigParam());
					if (d.exec() == QDialog::Accepted){
						this->setConfigParam( d.getData() );
					}
-----==========================================================-----
*/

W_ALE_CellDataSingle::W_ALE_CellDataSingle(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_slotBlock = false;
	this->m_data;
	this->m_unit;
	this->m_parentDir = "";
	
	//-----------------------------------
	//----初始化ui

	// > 数字设置
	ui.comboBox_frameUnit->clear();
	ui.comboBox_frameUnit->setView(new QListView());
	ui.comboBox_frameUnit->addItems(QStringList()
		<< "1帧" << "2帧" << "3帧" << "4帧" << "6帧" << "8帧" << "12帧"
		<< "5帧" << "10帧" << "15帧" << "20帧" << "30帧" << "40帧" << "60帧"
		<< "120帧" << "600帧" << "自定义");
	ui.comboBox_secondUnit->clear();
	ui.comboBox_secondUnit->setView(new QListView());
	ui.comboBox_secondUnit->addItems(QStringList()
		<< "0.01秒" << "0.02秒" << "0.03秒" << "0.04秒" << "0.06秒" << "0.08秒" << "0.12秒"
		<< "0.05秒" << "0.10秒" << "0.15秒" << "0.20秒" << "0.30秒" << "0.40秒" << "0.60秒"
		<< "1.20秒" << "6.00秒" << "自定义");

	// > 按钮文本转换
	TTool::_chinese_(ui.buttonBox);

	//-----------------------------------
	//----事件绑定
	connect(ui.comboBox_frameUnit, &QComboBox::currentTextChanged, this, &W_ALE_CellDataSingle::timeSelected_frameUnit);
	connect(ui.spinBox_frameUnit, SIGNAL(valueChanged(int)), this, SLOT(timeEdited_frameUnit(int)));
	connect(ui.comboBox_secondUnit, &QComboBox::currentTextChanged, this, &W_ALE_CellDataSingle::timeSelected_secondUnit);
	connect(ui.doubleSpinBox_secondUnit, SIGNAL(valueChanged(double)), this, SLOT(timeEdited_secondUnit(double)));

	connect(ui.pushButton_chooseFile, &QPushButton::clicked, this, &W_ALE_CellDataSingle::chooseFile);
	connect(ui.pushButton_openParentDir, &QPushButton::clicked, this, &W_ALE_CellDataSingle::openParentDir);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ALE_CellDataSingle::acceptData);

}
W_ALE_CellDataSingle::~W_ALE_CellDataSingle(){
}


/*-------------------------------------------------
		控件 - 回车过滤
*/
void W_ALE_CellDataSingle::keyPressEvent(QKeyEvent *event) {
	if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
		this->focusNextChild();
	}else{
		QDialog::keyPressEvent(event);
	}
}

/*-------------------------------------------------
		控件 - 选择文件（修改当前帧资源）
*/
void W_ALE_CellDataSingle::chooseFile(){

	// > 导入图片 - 选择文件
	QString selected_path = "";
	QFileDialog fd;
	fd.setWindowTitle("选择单张图片");
	fd.setAcceptMode(QFileDialog::AcceptOpen);
	fd.setNameFilters(QStringList() << "图片(*.png)");
	fd.setViewMode(QFileDialog::Detail);
	fd.setFileMode(QFileDialog::ExistingFile);		//单个文件
	if (fd.exec() == QDialog::Accepted) {
		if (fd.selectedFiles().empty()){ return ; }
		selected_path = fd.selectedFiles().at(0);
	}else {
		return ;
	}

	// > 导入图片 - 复制文件到文件夹
	QString new_file_path = S_ALE_FileManager::getInstance()->addPicFile_ToDir(selected_path, this->m_parentDir);
	if (new_file_path == ""){ return; }

	// > 更新内容
	QFileInfo fileinfo(new_file_path);
	this->m_data->m_bitmapPath = new_file_path;
	ui.lineEdit->setText(fileinfo.completeBaseName());
}
/*-------------------------------------------------
		控件 - 打开动画帧文件夹
*/
void W_ALE_CellDataSingle::openParentDir(){
	QDesktopServices::openUrl(QUrl("file:/" + this->m_parentDir + "/"));
}


/*-------------------------------------------------
		控件 - 选择数字（帧单位）
*/
void W_ALE_CellDataSingle::timeSelected_frameUnit(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "1帧"){ ui.spinBox_frameUnit->setValue(1); }
	if (text == "2帧"){ ui.spinBox_frameUnit->setValue(2); }
	if (text == "3帧"){ ui.spinBox_frameUnit->setValue(3); }
	if (text == "4帧"){ ui.spinBox_frameUnit->setValue(4); }
	if (text == "6帧"){ ui.spinBox_frameUnit->setValue(6); }
	if (text == "8帧"){ ui.spinBox_frameUnit->setValue(8); }
	if (text == "12帧"){ ui.spinBox_frameUnit->setValue(12); }
	if (text == "5帧"){ ui.spinBox_frameUnit->setValue(5); }
	if (text == "10帧"){ ui.spinBox_frameUnit->setValue(10); }
	if (text == "15帧"){ ui.spinBox_frameUnit->setValue(15); }
	if (text == "20帧"){ ui.spinBox_frameUnit->setValue(20); }
	if (text == "30帧"){ ui.spinBox_frameUnit->setValue(30); }
	if (text == "40帧"){ ui.spinBox_frameUnit->setValue(40); }
	if (text == "60帧"){ ui.spinBox_frameUnit->setValue(60); }
	if (text == "120帧"){ ui.spinBox_frameUnit->setValue(120); }
	if (text == "600帧"){ ui.spinBox_frameUnit->setValue(600); }

	this->m_slotBlock = false;
}
/*-------------------------------------------------
		控件 - 编辑数字（帧单位）
*/
void W_ALE_CellDataSingle::timeEdited_frameUnit(int value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 1){ ui.comboBox_frameUnit->setCurrentText("1帧"); }
	else if (value == 2){ ui.comboBox_frameUnit->setCurrentText("2帧"); }
	else if (value == 3){ ui.comboBox_frameUnit->setCurrentText("3帧"); }
	else if (value == 4){ ui.comboBox_frameUnit->setCurrentText("4帧"); }
	else if (value == 6){ ui.comboBox_frameUnit->setCurrentText("6帧"); }
	else if (value == 8){ ui.comboBox_frameUnit->setCurrentText("8帧"); }
	else if (value == 12){ ui.comboBox_frameUnit->setCurrentText("12帧"); }
	else if (value == 5){ ui.comboBox_frameUnit->setCurrentText("5帧"); }
	else if (value == 10){ ui.comboBox_frameUnit->setCurrentText("10帧"); }
	else if (value == 15){ ui.comboBox_frameUnit->setCurrentText("15帧"); }
	else if (value == 20){ ui.comboBox_frameUnit->setCurrentText("20帧"); }
	else if (value == 30){ ui.comboBox_frameUnit->setCurrentText("30帧"); }
	else if (value == 40){ ui.comboBox_frameUnit->setCurrentText("40帧"); }
	else if (value == 60){ ui.comboBox_frameUnit->setCurrentText("60帧"); }
	else if (value == 120){ ui.comboBox_frameUnit->setCurrentText("120帧"); }
	else if (value == 600){ ui.comboBox_frameUnit->setCurrentText("600帧"); }
	else{ ui.comboBox_frameUnit->setCurrentText("自定义"); }

	this->m_slotBlock = false;
}
/*-------------------------------------------------
		控件 - 选择数字（秒单位）
*/
void W_ALE_CellDataSingle::timeSelected_secondUnit(QString text){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (text == "0.01秒"){ ui.doubleSpinBox_secondUnit->setValue(0.01); }
	if (text == "0.02秒"){ ui.doubleSpinBox_secondUnit->setValue(0.02); }
	if (text == "0.03秒"){ ui.doubleSpinBox_secondUnit->setValue(0.03); }
	if (text == "0.04秒"){ ui.doubleSpinBox_secondUnit->setValue(0.04); }
	if (text == "0.06秒"){ ui.doubleSpinBox_secondUnit->setValue(0.06); }
	if (text == "0.08秒"){ ui.doubleSpinBox_secondUnit->setValue(0.08); }
	if (text == "0.12秒"){ ui.doubleSpinBox_secondUnit->setValue(0.12); }
	if (text == "0.05秒"){ ui.doubleSpinBox_secondUnit->setValue(0.05); }
	if (text == "0.10秒"){ ui.doubleSpinBox_secondUnit->setValue(0.1); }
	if (text == "0.15秒"){ ui.doubleSpinBox_secondUnit->setValue(0.15); }
	if (text == "0.20秒"){ ui.doubleSpinBox_secondUnit->setValue(0.2); }
	if (text == "0.30秒"){ ui.doubleSpinBox_secondUnit->setValue(0.3); }
	if (text == "0.40秒"){ ui.doubleSpinBox_secondUnit->setValue(0.4); }
	if (text == "0.60秒"){ ui.doubleSpinBox_secondUnit->setValue(0.6); }
	if (text == "1.20秒"){ ui.doubleSpinBox_secondUnit->setValue(1.2); }
	if (text == "6.00秒"){ ui.doubleSpinBox_secondUnit->setValue(6); }
	
	this->m_slotBlock = false;
}
/*-------------------------------------------------
		控件 - 编辑数字（秒单位）
*/
void W_ALE_CellDataSingle::timeEdited_secondUnit(double value){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;

	if (value == 0.01){ ui.comboBox_secondUnit->setCurrentText("0.01秒"); }
	else if (value == 0.02){ ui.comboBox_secondUnit->setCurrentText("0.02秒"); }
	else if (value == 0.03){ ui.comboBox_secondUnit->setCurrentText("0.03秒"); }
	else if (value == 0.04){ ui.comboBox_secondUnit->setCurrentText("0.04秒"); }
	else if (value == 0.06){ ui.comboBox_secondUnit->setCurrentText("0.06秒"); }
	else if (value == 0.08){ ui.comboBox_secondUnit->setCurrentText("0.08秒"); }
	else if (value == 0.12){ ui.comboBox_secondUnit->setCurrentText("0.12秒"); }
	else if (value == 0.05){ ui.comboBox_secondUnit->setCurrentText("0.05秒"); }
	else if (value == 0.1){ ui.comboBox_secondUnit->setCurrentText("0.10秒"); }
	else if (value == 0.15){ ui.comboBox_secondUnit->setCurrentText("0.15秒"); }
	else if (value == 0.2){ ui.comboBox_secondUnit->setCurrentText("0.20秒"); }
	else if (value == 0.3){ ui.comboBox_secondUnit->setCurrentText("0.30秒"); }
	else if (value == 0.4){ ui.comboBox_secondUnit->setCurrentText("0.40秒"); }
	else if (value == 0.6){ ui.comboBox_secondUnit->setCurrentText("0.60秒"); }
	else if (value == 1.2){ ui.comboBox_secondUnit->setCurrentText("1.20秒"); }
	else if (value == 6){ ui.comboBox_secondUnit->setCurrentText("6.00秒"); }
	else{ ui.comboBox_secondUnit->setCurrentText("自定义"); }

	this->m_slotBlock = false;
}


/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_ALE_CellDataSingle::setDataInModifyMode(C_AnimationBlockDataPtr data, C_ALE_DataSet::DATA_UNIT unit, QString parent_dir) {
	this->m_data = data;
	this->m_unit = unit;
	this->m_parentDir = parent_dir;
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
C_AnimationBlockDataPtr W_ALE_CellDataSingle::getData(){
	return this->m_data;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_ALE_CellDataSingle::putDataToUi() {

	// > 单位
	if (this->m_unit == C_ALE_DataSet::FrameUnit){
		ui.stackedWidget->setCurrentIndex(0);
	}
	if (this->m_unit == C_ALE_DataSet::SecondUnit){
		ui.stackedWidget->setCurrentIndex(1);
	}

	// > 图片资源
	QString file_name = QFileInfo(this->m_data->m_bitmapPath).completeBaseName();
	ui.lineEdit->setText(file_name);

	// > 帧间隔值
	int value = this->m_data->m_intervalValue;
	ui.spinBox_frameUnit->setValue(value);
	ui.doubleSpinBox_secondUnit->setValue(value*0.01);

	// > 刷新UI
	this->timeEdited_frameUnit(value);
	this->timeEdited_secondUnit(value*0.01);
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_ALE_CellDataSingle::putUiToData() {
	//（图片资源直接赋值）

	// > 帧单位时
	if (this->m_unit == C_ALE_DataSet::FrameUnit){
		this->m_data->m_intervalValue = ui.spinBox_frameUnit->value();
	}
	// > 秒单位时
	if (this->m_unit == C_ALE_DataSet::SecondUnit){
		this->m_data->m_intervalValue = ui.doubleSpinBox_secondUnit->value() * 100;
	}
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_ALE_CellDataSingle::acceptData(){
	this->putUiToData();

	this->accept();
};