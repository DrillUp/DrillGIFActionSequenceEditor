#include "stdafx.h"
#include "P_TwoTableStringFiller.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		双表格字符串填写器.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		可以将左表格的内容填到右表格。
					【只操作字符串，左表格字符串可以重复，右表格可以选重复或不重复】
					
		使用方法：
				> 初始化
					this->m_exchanger = new P_TwoTableStringFiller(ui.tableWidget_left, ui.tableWidget_right);
					this->m_exchanger->setData(data_list, data_list);
					this->m_exchanger->setLeftTitle("水果篮A");
					this->m_exchanger->setRightTitle("水果篮B");
					this->m_exchanger->setTableRightData(QStringList() << "桑葚" << "西瓜" << "不存在的水果");
-----==========================================================-----
*/
P_TwoTableStringFiller::P_TwoTableStringFiller(QTableWidget *parent_left, QTableWidget *parent_right)
	: QObject(parent_left)
{

	//-----------------------------------
	//----参数初始化
	this->m_slotBlock = false;

	//-----------------------------------
	//----ui初始化
	this->m_leftTable = parent_left;		//左表格
	this->m_leftTable->setColumnCount(1);
	this->m_leftTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);	//（列自适应）
	this->m_leftTable->horizontalHeader()->setVisible(false);
	this->m_leftTable->verticalHeader()->setVisible(false);
	this->m_leftTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->m_leftTable->setSelectionMode(QAbstractItemView::ExtendedSelection);			//（多选）

	this->m_rightTable = parent_right;		//右表格
	this->m_rightTable->setColumnCount(1);
	this->m_rightTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);	//（列自适应）
	this->m_rightTable->horizontalHeader()->setVisible(false);
	this->m_rightTable->verticalHeader()->setVisible(false);
	this->m_rightTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
	this->m_rightTable->setSelectionMode(QAbstractItemView::ExtendedSelection);			//（多选）

	//-----------------------------------
	//----事件绑定
	connect(this->m_leftTable, &QTableWidget::itemSelectionChanged, this, &P_TwoTableStringFiller::clearSelectionRight);
	connect(this->m_rightTable, &QTableWidget::itemSelectionChanged, this, &P_TwoTableStringFiller::clearSelectionLeft);

}
P_TwoTableStringFiller::~P_TwoTableStringFiller(){
}


/*-------------------------------------------------
		控件 - 刷新表格
*/
void P_TwoTableStringFiller::refreshTable() {
	this->refreshTable_Left();
	this->refreshTable_Right();
}
/*-------------------------------------------------
		控件 - 刷新表格 左
*/
void P_TwoTableStringFiller::refreshTable_Left(){
	this->m_slotBlock = true;

	// > 清理
	this->m_leftTable->clearContents();
	this->m_leftTable->setRowCount(this->m_leftTank.count());

	// > 左边字符串
	for (int i = 0; i < this->m_leftTank.count(); i++){
		QString str = this->m_leftTank.at(i);
		QTableWidgetItem* item = new QTableWidgetItem();
		item->setText(str);
		item->setToolTip(str);
		this->m_leftTable->setItem(i, 0, item);
	}

	this->m_slotBlock = false;
}
/*-------------------------------------------------
		控件 - 刷新表格 右
*/
void P_TwoTableStringFiller::refreshTable_Right(){
	this->m_slotBlock = true;

	// > 清理
	this->m_rightTable->clearContents();
	this->m_rightTable->setRowCount(this->m_rightTank.count());

	// > 右边字符串
	for (int i = 0; i < this->m_rightTank.count(); i++){
		QString str = this->m_rightTank.at(i);
		QTableWidgetItem* item = new QTableWidgetItem();
		item->setText(str);
		item->setToolTip(str);
		if (this->m_leftTank.contains(str) == false){
			item->setTextColor(QColor(255, 55, 55));
		}
		this->m_rightTable->setItem(i, 0, item);
	}

	this->m_slotBlock = false;
}
/*-------------------------------------------------
		控件 - 设置左表格标题
*/
void P_TwoTableStringFiller::setLeftTitle(QString title){
	this->m_leftTable->horizontalHeader()->setVisible(true);
	this->m_leftTable->setHorizontalHeaderLabels(QStringList() << title);
}
/*-------------------------------------------------
		控件 - 设置右表格标题
*/
void P_TwoTableStringFiller::setRightTitle(QString title){
	this->m_rightTable->horizontalHeader()->setVisible(true);
	this->m_rightTable->setHorizontalHeaderLabels(QStringList() << title);
}
/*-------------------------------------------------
		控件 - 显示左表格 首行序号
*/
void P_TwoTableStringFiller::setLeftRowHeaderVisible(bool visible){
	this->m_leftTable->verticalHeader()->setVisible(visible);
}
/*-------------------------------------------------
		控件 - 显示右表格 首行序号
*/
void P_TwoTableStringFiller::setRightRowHeaderVisible(bool visible){
	this->m_rightTable->verticalHeader()->setVisible(visible);
}


/*-------------------------------------------------
		选中 - 移动到左表格
*/
void P_TwoTableStringFiller::moveSelectedToLeft(){
	//（由于可重复，这里实际上就是删除，并没有移动）

	QList<QTableWidgetSelectionRange> range = this->m_rightTable->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(nullptr, ("提示"), ("请先选择需要左移的一行。"));
		return;
	}
	int pos = range.at(0).topRow();//（可选多行）
	int count = range.at(0).bottomRow() - pos + 1;

	for (int i = 0; i < count; i++){
		this->m_rightTank.removeAt(pos);
	}
	this->refreshTable_Right();
}
/*-------------------------------------------------
		选中 - 选中项移动到右表格
*/
void P_TwoTableStringFiller::moveSelectedToRight(){
	//（由于可重复，这里实际上就是添加，并没有移动）

	QList<QTableWidgetSelectionRange> range = this->m_leftTable->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this->m_rightTable, ("提示"), ("请先选择需要右移的一行。"));
		return;
	}
	int pos = range.at(0).topRow();//（可选多行）
	int count = range.at(0).bottomRow() - pos + 1;

	for (int i = 0; i < count; i++){
		this->m_rightTank.append(this->m_leftTank.at(pos + i));
	}
	this->refreshTable_Right();
}
/*-------------------------------------------------
		选中 - 左表格 清除选中
*/
void P_TwoTableStringFiller::clearSelectionLeft(){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;
	this->m_leftTable->clearSelection();
	this->m_slotBlock = false;
}
/*-------------------------------------------------
		选中 - 右表格 清除选中
*/
void P_TwoTableStringFiller::clearSelectionRight(){
	if (this->m_slotBlock == true){ return; }
	this->m_slotBlock = true;
	this->m_rightTable->clearSelection();
	this->m_slotBlock = false;
}


/*-------------------------------------------------
		数据 - 设置左边数据
*/
void P_TwoTableStringFiller::setLeftString(QStringList str_list){
	this->m_leftTank = str_list;
	this->refreshTable();
}
/*-------------------------------------------------
		数据 - 设置右边数据
*/
void P_TwoTableStringFiller::setRightString(QStringList str_list){
	this->m_rightTank = str_list;
	this->refreshTable_Right();	//（左边不需要刷新）
}
/*-------------------------------------------------
		数据 - 获取左边数据
*/
QStringList P_TwoTableStringFiller::getLeftString(){
	return this->m_leftTank;
}
/*-------------------------------------------------
		数据 - 获取右边数据
*/
QStringList P_TwoTableStringFiller::getRightString(){
	return this->m_rightTank;
}
/*-------------------------------------------------
		数据 - 清理全部
*/
void P_TwoTableStringFiller::clearAll(){
	this->m_leftTank.clear();
	this->m_rightTank.clear();
	this->refreshTable();
}
/*-------------------------------------------------
		数据 - 右边数据上移
*/
void P_TwoTableStringFiller::moveUpRightString(){
	QList<QTableWidgetSelectionRange> range = this->m_rightTable->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this->m_rightTable, ("提示"), ("请先选择一行。"));
		return;
	}
	int pos = range.at(0).topRow();//（只选一行）
	if (pos != range.at(0).bottomRow()) {
		QMessageBox::warning(this->m_rightTable, ("提示"), ("只能选择一行上移。"));
		return;
	}

	// > 执行上移
	if (pos <= 0){ return; }
	this->m_rightTank.swap(pos, pos - 1);

	// > 刷新内容
	this->refreshTable_Right();

	// > 刷新选中
	this->m_rightTable->clearSelection();
	this->m_rightTable->selectRow(pos - 1);
}
/*-------------------------------------------------
		数据 - 右边数据下移
*/
void P_TwoTableStringFiller::moveDownRightString(){
	QList<QTableWidgetSelectionRange> range = this->m_rightTable->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this->m_rightTable, ("提示"), ("请先选择一行。"));
		return;
	}
	int pos = range.at(0).topRow();//（只选一行）
	if (pos != range.at(0).bottomRow()) {
		QMessageBox::warning(this->m_rightTable, ("提示"), ("只能选择一行下移。"));
		return;
	}

	// > 执行下移
	if (pos >= this->m_rightTank.count() - 1){ return; }
	this->m_rightTank.swap(pos, pos + 1);

	// > 刷新内容
	this->refreshTable_Right();

	// > 刷新选中
	this->m_rightTable->clearSelection();
	this->m_rightTable->selectRow(pos + 1);
}