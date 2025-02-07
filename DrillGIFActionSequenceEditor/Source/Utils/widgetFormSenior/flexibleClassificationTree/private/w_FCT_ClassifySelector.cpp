#include "stdafx.h"
#include "W_FCT_ClassifySelector.h"

#include "C_FCT_Config.h"
#include "C_FCT_Classify.h"
#include "W_FCT_Classify.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		种类 选择窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		种类的设置内容在该窗口设置。
-----==========================================================-----
*/

W_FCT_ClassifySelector::W_FCT_ClassifySelector(P_FlexibleClassificationTree *p_obj)
	: QDialog(p_obj->getTree())
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_parentObj = p_obj;

	//-----------------------------------
	//----事件绑定
	connect(ui.pushButton, &QPushButton::clicked, this, &W_FCT_ClassifySelector::addClassifyInAction);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_FCT_ClassifySelector::acceptData);

	//-----------------------------------
	//----初始化ui
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);				//列自适应
	TTool::_chinese_(ui.buttonBox);

}
W_FCT_ClassifySelector::~W_FCT_ClassifySelector(){
}

/*-------------------------------------------------
		控件 - 添加类型
*/
void W_FCT_ClassifySelector::addClassifyInAction(){
	this->m_parentObj->addClassifyInAction();
	this->refreshTable();
}
/*-------------------------------------------------
		控件 - 刷新列表
*/
void W_FCT_ClassifySelector::refreshTable(){
	if (this->m_configPtr == nullptr){ return; }

	// > 记录之前的选择
	QList<int> last_selected_index;
	for (int i = 0; i < ui.tableWidget->rowCount(); i++){
		QTableWidgetItem* item = ui.tableWidget->item(i,0);
		if (item->isSelected()){
			last_selected_index.append(i);
		}
	}

	// > 刷新列表
	QList<C_FCT_Classify*> c_list = this->m_configPtr->get_classify_DataList();
	ui.tableWidget->clearContents();
	ui.tableWidget->setRowCount(c_list.count());
	for (int i = 0; i < c_list.count(); i++){
		C_FCT_Classify* c_c = c_list.at(i);
		QString name = c_c->getCustomObjectName();
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(name));
	}

	// > 重新选择
	for (int i = 0; i < ui.tableWidget->rowCount(); i++){
		QTableWidgetItem* item = ui.tableWidget->item(i, 0);
		if (last_selected_index.contains(i)){
			item->setSelected(true);
			ui.tableWidget->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
}

/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_FCT_ClassifySelector::setData(C_FCT_Config* config, QString last_selectedName) {
	this->m_configPtr = config;
	this->setWindowTitle("移动到");

	// > 刷新列表
	this->refreshTable();

	// > 选择指定项
	QList<C_FCT_Classify*> c_list = this->m_configPtr->get_classify_DataList();
	if (last_selectedName != ""){
		for (int i = 0; i < c_list.count(); i++){
			C_FCT_Classify* c_c = c_list.at(i);
			if (c_c->getName() == last_selectedName){
				ui.tableWidget->selectRow(i);
				break;
			}
		}
	}
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
QString W_FCT_ClassifySelector::getSelectedData(){

	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, ("提示"), ("请选择一行。"));
		return "";
	}
	int pos = range.at(0).topRow();

	return this->m_configPtr->get_classify_DataList().at(pos)->getName();
};

/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_FCT_ClassifySelector::acceptData(){
	//（无操作）

	this->accept();
};