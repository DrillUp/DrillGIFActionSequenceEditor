#include "stdafx.h"
#include "P_PiS_Selector.h"

#include "Source/Utils/WidgetForm/PictureBitmapCache/S_PictureBitmapCache.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		图片选择块.cpp
		版本：		v1.03
		作者：		drill_up
		所属模块：	工具模块
		功能：		能显示图片列表，并能单选/多选。（PiS全称：Picture_Selector）
					【块中以 refresh刷新 为主，少用 rebuild重建 】

		子功能：	->工厂
					->父控件
					->控件
						->重建UI
					->鼠标事件
						->右键事件（零个、单个、多个）
						->圈选变化事件（单个、多个）
					->配置类
						->行高
						->马赛克背景
					->资源数据
						->设置贴图
						->获取贴图
						->贴图变化（信号）
					->选中
						->单选
						->多选
						->选中选项变化（信号）
						->选中选项变化（多选时，信号）
					
		使用方法：
				> 初始化
					this->m_p_PictureSelector = new P_PictureSelector(ui.listWidget);
					this->m_p_PictureSelector->setSourceBitmap(bitmap_list);		
-----==========================================================-----
*/

P_PiS_Selector::P_PiS_Selector(QListWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----参数初始化

	// > 父控件
	this->m_parent = parent;							//父控件
	this->m_parentStyle = this->m_parent->styleSheet();	//父控件样式
	
	// > 控件
	this->m_cellTank.clear();							//元胞列表

	// > 鼠标事件
	this->m_slotBlock_selection = false;				//鼠标事件 锁
	this->m_selection_lastIndex = -1;					//上一个选中项

	// > 配置项
	this->m_config;


	//-----------------------------------
	//----ui初始化

	// > 父控件
	this->m_parent->setContextMenuPolicy(Qt::CustomContextMenu);									//激活右键
	this->m_parent->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);			//单选
	this->m_parent->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);			//选行
	this->m_parent->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);				//关闭双击编辑

	this->m_parent->setFlow(QListView::LeftToRight);
	this->m_parent->setMovement(QListView::Static);
	this->m_parent->setViewMode(QListView::ListMode);
	this->m_parent->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	this->m_parent->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
	this->m_parent->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	this->m_parent->setDragEnabled(false);
	this->m_parent->setDragDropMode(QListView::NoDragDrop);


	//-----------------------------------
	//----事件绑定

	// > 鼠标事件
	connect(this->m_parent, &QListWidget::customContextMenuRequested, this, &P_PiS_Selector::itemRightClicked);
	connect(this->m_parent, &QListWidget::itemSelectionChanged, this, &P_PiS_Selector::itemSelectionChanged);

}
P_PiS_Selector::~P_PiS_Selector(){
}


/*-------------------------------------------------
		工厂 - 建立一个元胞数据
*/
C_PictureBlockDataPtr P_PiS_Selector::createPictureCellData(){
	return C_PictureBlockDataPtr::create();
}
/*-------------------------------------------------
		工厂 - 建立一个元胞
*/
I_PiS_Cell* P_PiS_Selector::createPictureCell(C_PictureBlockDataPtr data){

	// > 控件 - 建立图片块
	P_PictureBlock* widget = new P_PictureBlock(this->m_parent);
	widget->setDataPtr(data);

	// > 项 - 建立项
	QListWidgetItem* item = new QListWidgetItem(this->m_parent);


	// > 元胞
	I_PiS_Cell* cell = new I_PiS_Cell();
	cell->item = item;
	cell->widget = widget;

	return cell;
}
/*-------------------------------------------------
		工厂 - 刷新元胞
*/
void P_PiS_Selector::refreshPictureCell(int index){
	if (index < 0){ return; }
	if (index >= this->m_cellTank.length()){ return; }
	I_PiS_Cell* cell = this->m_cellTank.at(index);

	// > 项
	QListWidgetItem* item = cell->item;
	int item_width = this->m_config.getSize_ItemWidth();
	int item_height = this->m_config.getSize_ItemHeight();

	// > 项 - 撑开每项
	//QPixmap temp_bitmap(item_height, 1);				//（iconMode中，用图标撑开横向，图标不能撑开纵向）
	//temp_bitmap.fill(Qt::white);						//（现在是listMode了，所以不需要了。）
	//item->setIcon(QIcon(temp_bitmap));				//
	item->setText(this->m_config.getSize_LineExpand());		//（用换行符撑开纵向）
	item->setSizeHint(QSize(item_width, item_height));		//（如果不撑开，选中的 蓝色方块 会紊乱）


	// > 控件
	P_PictureBlock* widget = cell->getBlock();

	// > 控件 - 刷新大小
	widget->setSize(item_width, item_height);

	// > 控件 - 刷新标题文本
	if (this->m_config.m_zeroFill == true){
		widget->setTitleNumWithZeroFill(widget->getTitleNum(), this->m_config.m_zeroFillCount, QLatin1Char(this->m_config.m_zeroFillChar.toLatin1()));
	}else{
		widget->setTitleNum(widget->getTitleNum());
	}

	// > 控件 - 刷新图片
	widget->refreshPixmap();

	// > 控件 - 刷新马赛克背景
	widget->setMaskConfig(this->m_config.m_isMaskEnabled, this->m_config.m_isMaskBorderEnabled);
}



/*-------------------------------------------------
		控件 - 重建UI
*/
void P_PiS_Selector::rebuildUI(QStringList bitmapPath_list) {
	int item_width = this->m_config.getSize_ItemWidth();
	int item_height = this->m_config.getSize_ItemHeight();

	// > 清理
	this->m_cellTank.clear();
	this->m_parent->clear();

	// > 重建内容
	for (int i = 0; i < bitmapPath_list.count(); i++){
		QString bitmapPath = bitmapPath_list.at(i);

		// > 加入缓存容器
		S_PictureBitmapCache::getInstance()->addPath(bitmapPath);

		// > 建立元胞数据
		C_PictureBlockDataPtr cellData = this->createPictureCellData();
		cellData->init(
			item_width,
			item_height, 
			i, 
			bitmapPath
		);

		// > 建立元胞
		I_PiS_Cell* cell = this->createPictureCell(cellData);
		this->m_cellTank.append(cell);

		// > 添加 - 项
		this->m_parent->addItem(cell->item);
		// > 添加 - 控件
		this->m_parent->setItemWidget(cell->item, cell->widget);
	}

	// > 刷新UI
	this->refreshUI();
}
/*-------------------------------------------------
		控件 - 刷新UI
*/
void P_PiS_Selector::refreshUI(){
	
	// > 选中变化项 - 记录
	QList<int> last_selected_item = this->getSelectedIndex_Multiple();

	// > 单选/多选切换
	if (this->m_config.m_isMultiSelect == true && this->m_parent->selectionMode() == QAbstractItemView::SelectionMode::SingleSelection){
		this->m_parent->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	}
	if (this->m_config.m_isMultiSelect == false && this->m_parent->selectionMode() != QAbstractItemView::SelectionMode::SingleSelection){
		this->m_parent->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	}

	// > 行高刷新
	int item_height = this->m_config.getSize_ItemHeight();
	int parent_height = this->m_config.getSize_ParentHeight();
	this->m_parent->setFixedHeight(parent_height);
	this->m_parent->setMinimumHeight(parent_height);
	this->m_parent->setMaximumHeight(parent_height);
	this->m_parent->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);


	// > 刷新内容
	this->m_parent->setIconSize(QSize(item_height, 1));	//（图标大小）
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);

		// > 数据 - 设置序号
		cell->getBlockDataPtr()->m_titleNum = i;

		// > 数据 - 设置颜色
		cell->getBlockDataPtr()->m_titleColorType = "Normal";

		// > 刷新单块
		this->refreshPictureCell(i);
	}
	

	// > 选中变化项 - 多选
	if (this->m_config.m_isMultiSelect){
		this->selectIndex_MultipleWithOtherUnselect(last_selected_item);
		
	// > 选中变化项 - 单选
	}else{
		if (last_selected_item.count() > 0){
			this->selectIndex_Single(last_selected_item.at(0));
		}
	}
}
/*-------------------------------------------------
		控件 - 获取全部资源路径
*/
QStringList P_PiS_Selector::getAllBitmapPath(){
	QStringList result_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		result_list.append(cell->widget->getBitmapPath());
	}
	return result_list;
}
/*-------------------------------------------------
		控件 - 清理项
*/
void P_PiS_Selector::clearCells(){

	// > 清理 - 父控件
	this->m_parent->clear();

	// > 清理 - 控件
	for (int i = this->m_cellTank.count() - 1; i >= 0; i--){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		delete cell;
	}
	this->m_cellTank.clear();
	this->m_selection_lastIndex = -1;

	// > 清理 - 鼠标事件
	//	（无）

	// > 清理 - 配置类
	//	（无）

	// > 清理 - 资源数据
	//	（无）

}
/*-------------------------------------------------
		控件 - 清理全部
*/
void P_PiS_Selector::clearAll(){

	// > 清理 - 父控件
	this->m_parent->clear();

	// > 清理 - 控件
	for (int i = this->m_cellTank.count()-1; i >= 0; i--){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		delete cell;
	}
	this->m_cellTank.clear();
	this->m_selection_lastIndex = -1;

	// > 清理 - 鼠标事件
	//	（无）

	// > 清理 - 配置类
	this->m_config = C_PiS_Config();

}
/*-------------------------------------------------
		控件 - 获取元胞索引
*/
int P_PiS_Selector::getCellIndexByItem(QListWidgetItem* item){
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		if (cell->item == item){
			return i;
		}
	}
	return -1;
}
int P_PiS_Selector::getCellIndexByWidget(P_PictureBlock* widget){
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		if (cell->widget == widget){
			return i;
		}
	}
	return -1;
}


/*-------------------------------------------------
		鼠标事件 -  圈选变化事件槽
*/
void P_PiS_Selector::itemSelectionChanged(){
	if (this->m_slotBlock_selection == true){ return; }
	this->m_slotBlock_selection = true;
	
	QList<QListWidgetItem*> selected_item = this->m_parent->selectedItems();
	if (selected_item.count() == 0){ this->m_slotBlock_selection = false; return; }
	this->event_itemSelectionChanged(selected_item);

	this->m_slotBlock_selection = false;
}
/*-------------------------------------------------
		鼠标事件 - 圈选变化事件（单个、多个）
*/
void P_PiS_Selector::event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list){

	// > 选中选项变化（发信号）
	if (selected_item_list.count() == 1){
		int index = this->getSelectedIndex_Single();
		if (this->m_selection_lastIndex != index){
			this->m_selection_lastIndex = index;
			emit signal_currentIndexChanged_Single(index);
			emit signal_selectedIndexChanged_Multiple(this->getSelectedIndex_Multiple());
		}

	}else if (selected_item_list.count() > 1){
		emit signal_selectedIndexChanged_Multiple(this->getSelectedIndex_Multiple());
	}
}


/*-------------------------------------------------
		鼠标事件 - 右键事件槽
*/
void P_PiS_Selector::itemRightClicked(QPoint point) {
	if (this->m_slotBlock_selection == true){ return; }
	this->m_slotBlock_selection = true;

	QListWidgetItem *item = this->m_parent->itemAt(point);	//（当前右键选中的项）
	if (item == nullptr){

		// > 点到空白处
		this->event_itemRightClicked(QList<QListWidgetItem*>());
	}else{

		// > 点到项
		this->event_itemRightClicked(this->m_parent->selectedItems());
	}

	this->m_slotBlock_selection = false;
}
/*-------------------------------------------------
		鼠标事件 - 右键事件（零个、单个、多个）
*/
void P_PiS_Selector::event_itemRightClicked(QList<QListWidgetItem*> item_list){
	
	// > menu - 零个（点击空白处）
	if (item_list.count() == 0){
		QMenu* menu = this->createMenu_selectNone();
		if (menu != nullptr){
			menu->exec(QCursor::pos());
			menu->deleteLater();
			return;
		}
		
	// > menu - 单个
	}else if (item_list.count() == 1){
		int pos = this->getCellIndexByItem( item_list.at(0) );
		if (pos == -1){ return; }

		QMenu* menu = this->createMenu_selectSingle(pos);
		if (menu != nullptr){
			menu->exec(QCursor::pos());
			menu->deleteLater();
			return;
		}
		
	// > menu - 多个
	}else{
		QList<int> pos_list;
		for (int i = 0; i < item_list.count(); i++){
			QListWidgetItem* item = item_list.at(i);
			int pos = this->getCellIndexByItem(item);
			if (pos == -1){ continue; }
			pos_list.append( pos );
		}
		qSort(pos_list);		//（排序，防止删除时顺序乱了）

		QMenu* menu = this->createMenu_selectMultiple(pos_list);
		if (menu != nullptr){
			menu->exec(QCursor::pos());
			menu->deleteLater();
			return;
		}
	}
}
/*-------------------------------------------------
		右键菜单 - 零个（点击空白处）
*/
QMenu* P_PiS_Selector::createMenu_selectNone(){
	return nullptr;
}
/*-------------------------------------------------
		右键菜单 - 单个
*/
QMenu* P_PiS_Selector::createMenu_selectSingle(int pos){
	return nullptr;
}
/*-------------------------------------------------
		右键菜单 - 多个
*/
QMenu* P_PiS_Selector::createMenu_selectMultiple(QList<int> pos_list){
	return nullptr;
}
/*-------------------------------------------------
		右键菜单 - menu - 修改选中
*/
QMenu* P_PiS_Selector::createMenu_changeSelection(){
	QAction* action;
	QMenu* menu_select = new QMenu("选择");
	menu_select->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));

	action = new QAction("全选", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
	action->setShortcut(QKeySequence::SelectAll);
	connect(action, &QAction::triggered, this, &P_PiS_Selector::action_selectAll);
	menu_select->addAction(action);

	action = new QAction("选择左边全部", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
	action->setData(QString::number(this->m_selection_lastIndex));
	connect(action, &QAction::triggered, this, &P_PiS_Selector::action_selectLeftAll);
	menu_select->addAction(action);

	action = new QAction("选择右边全部", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
	action->setData(QString::number(this->m_selection_lastIndex));
	connect(action, &QAction::triggered, this, &P_PiS_Selector::action_selectRightAll);
	menu_select->addAction(action);

	action = new QAction("选择奇数项", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
	connect(action, &QAction::triggered, this, &P_PiS_Selector::action_selectOdd);
	menu_select->addAction(action);
	
	action = new QAction("选择偶数项", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_SelectAll.png"));
	connect(action, &QAction::triggered, this, &P_PiS_Selector::action_selectEven);
	menu_select->addAction(action);

	return menu_select;
}


/*-------------------------------------------------
		配置类 - 设置配置
*/
void P_PiS_Selector::setConfigParam(C_PiS_Config config){
	this->m_config = config;

	// > 刷新UI
	this->refreshUI();
}
/*-------------------------------------------------
		配置类 - 取出配置
*/
C_PiS_Config P_PiS_Selector::getConfigParam(){
	return this->m_config;
}


/*-------------------------------------------------
		选中 - 设置选中（单选）
*/
void P_PiS_Selector::selectIndex_Single(int index){
	if (index < 0){ index = 0; }
	if (index >= this->m_cellTank.count()){ index = this->m_cellTank.count() - 1; }

	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		QListWidgetItem* item = cell->item;
		if (item == nullptr){ continue; }
		if (i == index){
			item->setSelected(true);
			//this->m_parent->scrollToItem(item);	//（不要控制滚动条）
		}else{
			item->setSelected(false);
		}
	}
}
/*-------------------------------------------------
		选中 - 获取选中数据
*/
int P_PiS_Selector::getSelectedIndex_Single(){
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		QListWidgetItem* item = cell->item;
		if (item == nullptr){ continue; }
		if (item->isSelected()){
			return i;
		}
	}
	return -1;
}

/*-------------------------------------------------
		选中 - 设置选中（多选）
*/
void P_PiS_Selector::selectIndex_Multiple(QList<int> index_list){
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		QListWidgetItem* item = cell->item;
		if (index_list.contains( i )){
			item->setSelected(true);
			//this->m_parent->scrollToItem(item);	//（不要控制滚动条，外部手动调用scrollToSelected）
		}
	}
}
void P_PiS_Selector::selectIndex_MultipleWithOtherUnselect(QList<int> index_list){
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		QListWidgetItem* item = cell->item;
		if (index_list.contains( i )){
			item->setSelected(true);
		}else{
			item->setSelected(false);
		}
	}
}
/*-------------------------------------------------
		选中 - 获取选中数据（多选时）
*/
QList<int> P_PiS_Selector::getSelectedIndex_Multiple(){
	QList<int> result_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		QListWidgetItem* item = cell->item;
		if (item == nullptr){ continue; }
		if (item->isSelected()){
			result_list.push_back(i);
		}
	}
	return result_list;
}
/*-------------------------------------------------
		选中 - 选中上一项
*/
void P_PiS_Selector::selectLast(){
	this->selectIndex_Single(this->getSelectedIndex_Single() - 1);
}
/*-------------------------------------------------
		选中 - 选中下一项
*/
void P_PiS_Selector::selectNext(){
	this->selectIndex_Single(this->getSelectedIndex_Single() + 1);
}
/*-------------------------------------------------
		选中 - 选中首项
*/
void P_PiS_Selector::selectStart(){
	this->selectIndex_Single(0);
}
/*-------------------------------------------------
		选中 - 选中尾项
*/
void P_PiS_Selector::selectEnd(){
	this->selectIndex_Single(this->m_cellTank.count() - 1);
}
/*-------------------------------------------------
		选中 - 聚焦选中项
*/
void P_PiS_Selector::scrollToSelected(){
	for (int i = this->m_cellTank.count()-1; i >= 0; i--){
		I_PiS_Cell* cell = this->m_cellTank.at(i);
		QListWidgetItem* item = cell->item;
		if (item->isSelected()){
			this->m_parent->scrollToItem(item);
			break;
		}
	}
}


/*-------------------------------------------------
		选中（多选） - action - 全选
*/
void P_PiS_Selector::action_selectAll(){
	this->m_parent->selectAll();
}
/*-------------------------------------------------
		选中（多选） - action - 选择左边全部
*/
void P_PiS_Selector::action_selectLeftAll(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int index = cur_action->data().value<int>();
	QList<int> index_list;
	for (int i = 0; i <= index; i++){
		index_list.append(i);
	}
	this->selectIndex_Multiple(index_list);
}
/*-------------------------------------------------
		选中（多选） - action - 选择右边全部
*/
void P_PiS_Selector::action_selectRightAll(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int index = cur_action->data().value<int>();
	QList<int> index_list;
	for (int i = index; i < this->m_cellTank.count(); i++){
		index_list.append(i);
	}
	this->selectIndex_Multiple(index_list);
}
/*-------------------------------------------------
		选中（多选） - action - 选奇数
*/
void P_PiS_Selector::action_selectOdd(){
	QList<int> index_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		if (i%2 == 0){
			index_list.append(i);
		}
	}
	this->selectIndex_MultipleWithOtherUnselect(index_list);
}
/*-------------------------------------------------
		选中（多选） - action - 选偶数
*/
void P_PiS_Selector::action_selectEven(){
	QList<int> index_list;
	for (int i = 0; i < this->m_cellTank.count(); i++){
		if (i % 2 == 1){
			index_list.append(i);
		}
	}
	this->selectIndex_MultipleWithOtherUnselect(index_list);
}