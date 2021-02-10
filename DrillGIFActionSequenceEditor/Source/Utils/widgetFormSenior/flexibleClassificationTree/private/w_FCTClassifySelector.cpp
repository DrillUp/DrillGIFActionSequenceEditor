#include "stdafx.h"
#include "w_FCTClassifySelector.h"

#include "w_FCTClassify.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		类型 选择窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		类型的设置内容在该窗口设置。
-----==========================================================-----
*/

W_FCTClassifySelector::W_FCTClassifySelector(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数


	//-----------------------------------
	//----事件绑定
	connect(ui.pushButton, &QPushButton::clicked, this, &W_FCTClassifySelector::openWindowAddClassify);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_FCTClassifySelector::acceptData);

	//-----------------------------------
	//----初始化ui
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);				//列自适应
	TTool::_chinese_(ui.buttonBox);

}
W_FCTClassifySelector::~W_FCTClassifySelector(){
}


/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_FCTClassifySelector::openWindowAddClassify(){

	W_FCTClassify d(this);
	d.setDataInAddMode(this->m_classifyList);
	if (d.exec() == QDialog::Accepted){
		C_FCTClassify c_c = d.getData();
		
		// > 刷新当前列表
		int index = ui.tableWidget->rowCount();
		ui.tableWidget->setRowCount(index + 1);
		ui.tableWidget->setItem(index - 1, 0, new QTableWidgetItem(c_c.name));
		ui.tableWidget->setItem(index , 0, new QTableWidgetItem("未分类"));
		
		// > 发送信号
		emit classifyAdded(c_c);
	}
}


/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_FCTClassifySelector::setDataInModifyMode(QString selected_name, QList<C_FCTClassify>* dataFrom) {
	this->m_classifyList = dataFrom;
	this->setWindowTitle("移动到");

	// > 刷新列表
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(this->m_classifyList->count());
	for (int i = 0; i < this->m_classifyList->count(); i++){
		C_FCTClassify c_c = this->m_classifyList->at(i);
		QString name = c_c.name;
		if (name == ""){
			name = "未分类";
		}
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(name));
	}

	// > 选择之前选中的
	for (int i = 0; i < this->m_classifyList->count(); i++){
		C_FCTClassify c_c = this->m_classifyList->at(i);
		if (c_c.name == selected_name){
			ui.tableWidget->selectRow(i);
		}
	}

}

/*-------------------------------------------------
		窗口 - 取出数据
*/
C_FCTClassify W_FCTClassifySelector::getSelectedData(){

	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, ("提示"), ("请选择一行。"));
		return C_FCTClassify();
	}
	int pos = range.at(0).topRow();

	return this->m_classifyList->at(pos);
};

/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_FCTClassifySelector::acceptData(){

	this->accept();
};