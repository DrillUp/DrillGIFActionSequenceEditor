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

		子功能：	-> 文本选择
						-> 单选
						-> 多选
						-> 自动编号
					-> 表格配置
						-> 设置行高
						-> 设置叶子显示文本
					-> 右键菜单
						-> 只单选时有效
					
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
	this->m_iconSrcPath = ":/DrillGIFActionSequenceEditor/Resources/icons";
	this->m_table = parent;									//表格对象
	this->m_tableStyle = this->m_table->styleSheet();		//表格默认样式
	this->m_itemTank = QList<QTableWidgetItem*>();			//表格项列表
	this->m_last_index = -1;								//上一个选中的索引项

	// > 数据
	this->m_config = C_RaTConfig();

	// > 右键菜单
	this->m_itemOuterControlEnabled = false;				//编辑 - 开关（默认关闭）
	this->m_itemOuterControl_CopyActive = true;				//编辑 - 复制激活
	this->m_itemOuterControl_PasteActive = false;			//编辑 - 粘贴激活
	this->m_itemOuterControl_ClearActive = true;			//编辑 - 清空激活

	//-----------------------------------
	//----ui初始化
	this->m_table->setContextMenuPolicy(Qt::CustomContextMenu);									//激活右键
	this->m_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);			//单选
	this->m_table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);		//选行
	this->m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);							//关闭编辑

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
	this->m_selectionSignalBlock_Root = true;
	QList<int> last_selected_index = this->getSelectedIndex_Multi();

	// > 清理
	this->m_table->clearContents();
	this->m_table->setRowCount(local_text.count());

	// > 刷新内容
	this->m_itemTank = QList<QTableWidgetItem*>();
	for (int i = 0; i < local_text.count(); i++){
		QTableWidgetItem* item = new QTableWidgetItem();
		item->setData(Qt::UserRole + 1, local_text.at(i));	//名称
		this->m_table->setItem(i, 0, item);
		this->m_itemTank.append(item);
		this->refreshItem(item);		//（刷新项）
	}

	// > 单选/多选切换
	if (this->m_config.isMultiSelect == true && this->m_table->selectionMode() == QAbstractItemView::SelectionMode::SingleSelection){
		this->m_table->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	}
	if (this->m_config.isMultiSelect == false && this->m_table->selectionMode() != QAbstractItemView::SelectionMode::SingleSelection){
		this->m_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);	
	}

	// > 行高刷新
	this->m_table->setStyleSheet(this->m_tableStyle + "\n QTableView::item { height: " + QString::number(this->m_config.rowHeight) + "px;}");

	// > 选中变化项
	if (this->m_config.isMultiSelect){
		this->selectIndex_Multi(last_selected_index);
	}else{
		if (last_selected_index.count() > 0){
			this->selectIndex(last_selected_index.at(0));
		}
	}
	this->m_selectionSignalBlock_Root = false;
}
/*-------------------------------------------------
		控件 - 清理全部
*/
void P_RadioTable::clearAll(){

	// > 控件清理
	this->m_itemTank.clear();
	this->m_table->clearContents();
	this->m_table->setRowCount(0);

	// > 表格设置
	this->m_config = C_RaTConfig();
	this->m_itemOuterControl_PasteActive = false;
}
/*-------------------------------------------------
		控件 - 表格项数量
*/
int P_RadioTable::count(){
	return this->m_itemTank.count();
}
/*-------------------------------------------------
		控件 - 修改指定位置文本（不发信号）
*/
void P_RadioTable::modifyText(int index, QString text){
	if (index == -1 ){ return; }
	if (index < 0){ index = 0; }
	if (index >= this->m_itemTank.count()){ index = this->m_itemTank.count() - 1; }

	// > 修改本地数据
	this->local_text.replace(index, text);

	// > 刷新项
	QTableWidgetItem* item = this->m_itemTank.at(index);
	item->setData(Qt::UserRole + 1, text);
	this->refreshItem(item);
}
/*-------------------------------------------------
		控件 - 修改选中项文本（不发信号）
*/
void P_RadioTable::modifyText_Selected(QString text){
	this->modifyText(this->getSelectedIndex(),text);
}
/*-------------------------------------------------
		控件 - 获取名称
*/
QString P_RadioTable::getTextByIndex(int index){
	if (index == -1){ return ""; }
	if (index < 0){ index = 0; }
	if (index >= this->m_itemTank.count()){ index = this->m_itemTank.count() - 1; }

	return this->getRealText(this->m_itemTank.at(index));
}
QStringList P_RadioTable::getTextByIndex(QList<int> index_list){
	QStringList result_list = QStringList();
	for (int i = 0; i < index_list.count(); i++){
		result_list.append(this->getTextByIndex(index_list.at(i)));
	}
	return result_list;
}
/*-------------------------------------------------
		控件 - 获取文本
*/
QString P_RadioTable::getRealText(QTableWidgetItem* item){
	if (item == nullptr){ return ""; }
	return item->data(Qt::UserRole + 1).toString();
}
/*-------------------------------------------------
		控件 - 刷新 项 自身文本
*/
void P_RadioTable::refreshItem(QTableWidgetItem* item){
	int index = this->m_itemTank.indexOf(item);
	if (index == -1){ return; }

	QString text = this->getRealText(item);
	if (this->m_config.showNumber == false){
		item->setText(text);
	}else{
		if (this->m_config.zeroFill == true){
			item->setText(TTool::_zeroFill_(index + 1, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1())) + " " + text);
		}else{
			item->setText(QString::number(index + 1) + " " + text);
		}
	}
	
}

/*-------------------------------------------------
		表格设置 - 设置参数
*/
void P_RadioTable::setConfigParam(C_RaTConfig config){
	this->m_config = config;
	this->refreshTableUi();
}
C_RaTConfig P_RadioTable::setConfigParam(){
	return this->m_config;
}
/*-------------------------------------------------
		表格设置 - 设置参数
*/
void P_RadioTable::setConfigParam_obj(QJsonObject config){
	this->m_config.setJsonObject(config);
	this->refreshTableUi();
}
QJsonObject P_RadioTable::getConfigParam_obj(){
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
	if( selected_item.count() == 1 ){

		int index = this->getSelectedIndex();
		if (this->m_last_index != index){
			this->m_last_index = index;
			emit currentIndexChanged(index);
			emit currentTextChanged(this->getSelectedText());
		}
		emit currentIndexChanged_Multi(this->getSelectedIndex_Multi());

	}else if (selected_item.count() > 1){

		emit currentIndexChanged_Multi(this->getSelectedIndex_Multi());
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

	/*------------------------右键点击单个叶子------------------------*/
	if (this->m_table->selectedItems().count() == 1 && this->m_itemOuterControlEnabled == true ){
		QAction* action;
		QMenu* menu = new QMenu(this->m_table);

		action = new QAction("复制", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Copy.png"));
		action->setData( this->m_itemTank.indexOf(item) );
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_RadioTable::menuCopyItemInAction);
		menu->addAction(action);
		action->setEnabled(this->m_itemOuterControl_CopyActive);

		action = new QAction("粘贴", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Paste.png"));
		action->setData( this->m_itemTank.indexOf(item) );
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_RadioTable::menuPasteItemInAction);
		menu->addAction(action);
		action->setEnabled(this->m_itemOuterControl_PasteActive);

		action = new QAction("清空", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Clear.png"));
		action->setData( this->m_itemTank.indexOf(item) );
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_RadioTable::menuClearItemInAction);
		menu->addAction(action);
		action->setEnabled(this->m_itemOuterControl_ClearActive);

		menu->exec(QCursor::pos());
		menu->deleteLater();
	}

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
			return this->getRealText( item );
		}
	}
	return "";
}
/*-------------------------------------------------
		资源数据 - 取出数据（多选）
*/
QList<int> P_RadioTable::getSelectedIndex_Multi(){
	QList<int> result_list = QList<int>();
	for (int i = 0; i < this->m_itemTank.count(); i++){
		if (this->m_itemTank.at(i)->isSelected()){
			result_list.append(i);
		}
	}
	return result_list;
}
QList<QString> P_RadioTable::getSelectedText_Multi(){
	QList<QString> result_list = QList<QString>();
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (item->isSelected()){
			result_list.append(this->getRealText( item ));
		}
	}
	return result_list;
}


/*-------------------------------------------------
		选中 - 设置选中
*/
void  P_RadioTable::selectIndex(int index){
	if (index < 0){ index = 0; }
	if (index >= this->m_itemTank.count()){ index = this->m_itemTank.count() - 1; }

	QString c_text = "";
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (i == index){
			item->setSelected(true);
			c_text = this->getRealText(item);
			this->m_table->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
	if (this->m_selectionSignalBlock_Root == false){
		emit currentIndexChanged(index);
		emit currentTextChanged(c_text);
		emit currentIndexChanged_Multi(QList<int>() << index);
	}
}
void  P_RadioTable::selectText(QString text){
	QString c_text = "";
	int index = -1;
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);

		if (this->getRealText(item) == text &&
			index == -1){
			item->setSelected(true);
			c_text = this->getRealText(item);
			this->m_table->scrollToItem(item);
			index = i;
		}else{
			item->setSelected(false);
		}
	}
	if (this->m_selectionSignalBlock_Root == false && index != -1){
		emit currentIndexChanged(index);
		emit currentTextChanged(c_text);
		emit currentIndexChanged_Multi(QList<int>() << index);
	}
}
/*-------------------------------------------------
		选中 - 设置选中（多选）
*/
void  P_RadioTable::selectIndex_Multi(QList<int> index_list){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (index_list.contains(i)){
			item->setSelected(true);
			this->m_table->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
	if (this->m_selectionSignalBlock_Root == false){
		emit currentIndexChanged_Multi(this->getSelectedIndex_Multi());
	}
}
void  P_RadioTable::selectText_Multi(QList<QString> text_list){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (text_list.contains(this->getRealText(item))){
			item->setSelected(true);
			this->m_table->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
	if (this->m_selectionSignalBlock_Root == false){
		emit currentIndexChanged_Multi(this->getSelectedIndex_Multi());
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


/*-------------------------------------------------
		接口 - 开关（默认关闭）
*/
void P_RadioTable::setItemOuterControlEnabled(bool enabled){
	this->m_itemOuterControlEnabled = enabled;
}
/*-------------------------------------------------
		接口 - 激活控制
*/
void P_RadioTable::setItemOuterControl_CopyActive(bool enabled){
	this->m_itemOuterControl_CopyActive = enabled;
}
void P_RadioTable::setItemOuterControl_PasteActive(bool enabled){
	this->m_itemOuterControl_PasteActive = enabled;
}
void P_RadioTable::setItemOuterControl_ClearActive(bool enabled){
	this->m_itemOuterControl_ClearActive = enabled;
}
/*-------------------------------------------------
		接口 - 复制按下
*/
void P_RadioTable::menuCopyItemInAction(){
	if (this->m_itemOuterControl_CopyActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int index = cur_action->data().value<int>();
	emit menuCopyItemTriggered(index);
}
/*-------------------------------------------------
		接口 - 粘贴按下
*/
void P_RadioTable::menuPasteItemInAction(){
	if (this->m_itemOuterControl_PasteActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int index = cur_action->data().value<int>();
	emit menuPasteItemTriggered(index);
}
/*-------------------------------------------------
		接口 - 清空按下
*/
void P_RadioTable::menuClearItemInAction(){
	if (this->m_itemOuterControl_ClearActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int index = cur_action->data().value<int>();
	emit menuClearItemTriggered(index);
}
