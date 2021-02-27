#include "stdafx.h"
#include "p_RadioTable.h"

#include "private/w_RaTConfig.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		单选表格.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		将数据全部显示，并能单选。（不含排序功能）

		子功能：	-> 文本单选
						-> 自动编号
					-> 表格配置
						-> 设置行高
						-> 设置叶子显示文本
					
		使用方法：
				>初始化
					this->m_p_RadioTable = new P_RadioTable(ui.treeWidget);
					this->m_p_RadioTable->setData(obj);		
					
-----==========================================================-----
*/
P_RadioTable::P_RadioTable(QTableWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----参数初始化
	this->m_table = parent;									//表格对象
	this->m_tableStyle = this->m_table->styleSheet();		//表格默认样式
	this->m_itemTank = QList<QTableWidgetItem*>();			//表格项列表

	// > 数据
	this->m_config = C_RaTConfig();

	//-----------------------------------
	//----ui初始化
	this->m_table->setContextMenuPolicy(Qt::CustomContextMenu);									//激活右键
	this->m_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);			//单选
	this->m_table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);		//选行

	this->m_table->setColumnCount(1);
	this->m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);				//列自适应
	this->m_table->horizontalHeader()->setVisible(false);
	this->m_table->verticalHeader()->setVisible(false);

	//-----------------------------------
	//----事件绑定
	this->m_selectionSignalBlock_Root = false;				//槽阻塞
	connect(this->m_table, &QTableWidget::customContextMenuRequested, this, &P_RadioTable::sltItemRightClicked);
	connect(this->m_table, &QTableWidget::itemSelectionChanged, this, &P_RadioTable::sltItemSelectionChanged);

}
P_RadioTable::~P_RadioTable(){
}


/*-------------------------------------------------
		控件 - 刷新表格
*/
void P_RadioTable::refreshTableUi() {
	QList<QTableWidgetSelectionRange> last_range = this->m_table->selectedRanges();

	// > 清理
	this->m_table->clearContents();
	this->m_table->setRowCount(local_text.count());

	// > 刷新内容
	this->m_itemTank = QList<QTableWidgetItem*>();
	for (int i = 0; i < local_text.count(); i++){
		QTableWidgetItem* item = new QTableWidgetItem();
		item->setText(TTool::_zeroFill_( i+1, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1())) + " " + local_text.at(i));
		item->setData(Qt::UserRole + 1, local_text.at(i));
		this->m_table->setItem(i, 0, item);
		this->m_itemTank.append(item);
	}

	// > 行高刷新
	this->m_table->setStyleSheet(this->m_tableStyle + "\n QTableView::item { height: " + QString::number(this->m_config.rowHeight) + "px;}");

	// > 选中变化项
	if (last_range.count() > 0){
		this->m_table->setRangeSelected(last_range.at(0), true);
	}else{
		QTableWidgetSelectionRange range = QTableWidgetSelectionRange(0,0,1,1);
		this->m_table->setRangeSelected(range, true);
	}
}
/*-------------------------------------------------
		控件 - 修改指定位置文本（不发信号）
*/
void P_RadioTable::modifyText(int index, QString text){
	if (index == -1 ){ return; }
	if (index < 0){ index = 0; }
	if (index >= this->m_itemTank.count()){ index = this->m_itemTank.count() - 1; }

	QTableWidgetItem* item = this->m_itemTank.at(index);
	item->setText(TTool::_zeroFill_(index + 1, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1())) + " " + text);
	item->setData(Qt::UserRole + 1, text);
}
/*-------------------------------------------------
		控件 - 修改选中项文本（不发信号）
*/
void P_RadioTable::modifyText_Selected(QString text){
	this->modifyText(this->getSelectedIndex(),text);
}
/*-------------------------------------------------
		控件 - 清理全部
*/
void P_RadioTable::clearAll(){

	// > 控件清理
	this->m_table->clearContents();
	this->m_table->setRowCount(0);

	// > 表格设置
	this->m_config = C_RaTConfig();

}

/*-------------------------------------------------
		表格设置 - 设置参数
*/
void P_RadioTable::setConfigParam(QJsonObject config){
	this->m_config.setJsonObject(config);
	this->refreshTableUi();
}
/*-------------------------------------------------
		表格设置 - 取出参数
*/
QJsonObject P_RadioTable::getConfigParam(){
	return this->m_config.getJsonObject();
}
/*-------------------------------------------------
		表格设置 - 编辑窗口
*/
void P_RadioTable::openConfigParamWindow(){
	W_RaTConfig d(this->m_table);
	d.setDataInModifyMode(this->m_config);
	if (d.exec() == QDialog::Accepted){
		this->m_config = d.getData() ;
		this->refreshTableUi();
	}
}


/*-------------------------------------------------
		表格事件 -  圈选变化事件
*/
void P_RadioTable::sltItemSelectionChanged(){
	if (this->m_selectionSignalBlock_Root == true){ return; }
	this->m_selectionSignalBlock_Root = true;
	
	QList<QTableWidgetItem*> selected_item = this->m_table->selectedItems();
	if (selected_item.count() > 0) {
		QString text = selected_item.at(0)->data(Qt::UserRole + 1).toString();
		int index = this->getSelectedIndex();
		emit currentTextChanged(text);
		emit currentIndexChanged(index);
	}

	this->m_selectionSignalBlock_Root = false;
}
/*-------------------------------------------------
		表格事件 - 右键事件
*/
void P_RadioTable::sltItemRightClicked(QPoint point) {
	if (this->m_selectionSignalBlock_Root == true){ return; }
	this->m_selectionSignalBlock_Root = true;
	QTableWidgetItem *item = this->m_table->itemAt(point);	//（当前右键选中的项）
	if (item == nullptr){ return; }
	if (this->m_table->selectedItems().count() == 0){ return; }

	//...（暂无）

	this->m_selectionSignalBlock_Root = false;
}


/*-------------------------------------------------
		资源数据 - 设置数据
*/
void P_RadioTable::setSource(QStringList text_list) {
	this->local_text = text_list;
	this->refreshTableUi();
}
/*-------------------------------------------------
		资源数据 - 取出数据
*/
int P_RadioTable::getSelectedIndex(){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		if (this->m_itemTank.at(i)->isSelected()){
			return i;
		}
	}
	return -1;
}
QString P_RadioTable::getSelectedText(){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (item->isSelected()){
			return item->data(Qt::UserRole + 1).toString();
		}
	}
	return "";
}


/*-------------------------------------------------
		选中 - 设置选中
*/
void  P_RadioTable::selectIndex(int index){
	if (index < 0){ index = 0; }
	if (index >= this->m_itemTank.count()){ index = this->m_itemTank.count() - 1; }

	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (i == index){
			item->setSelected(true);
			this->m_table->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
	emit currentIndexChanged(index);
}
void  P_RadioTable::selectText(QString text){
	bool index = -1;
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);

		if (item->data(Qt::UserRole + 1).toString() == text &&
			index == -1){
			item->setSelected(true);
			this->m_table->scrollToItem(item);
			index = i;
		}else{
			item->setSelected(false);
		}
	}
	if (index != -1){
		emit currentIndexChanged(index);
	}
}
/*-------------------------------------------------
		选中 - 选中上一项
*/
void  P_RadioTable::selectLast(){
	this->selectIndex(this->getSelectedIndex() - 1);
}
/*-------------------------------------------------
		选中 - 选中下一项
*/
void  P_RadioTable::selectNext(){
	this->selectIndex(this->getSelectedIndex() + 1);
}
/*-------------------------------------------------
		选中 - 选中首项
*/
void  P_RadioTable::selectStart(){
	this->selectIndex(0);
}
/*-------------------------------------------------
		选中 - 选中尾项
*/
void  P_RadioTable::selectEnd(){
	this->selectIndex(this->m_itemTank.count() - 1);
}