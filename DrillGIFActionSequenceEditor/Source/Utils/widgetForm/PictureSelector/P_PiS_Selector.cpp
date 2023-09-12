#include "stdafx.h"
#include "P_PiS_Selector.h"

#include "Source/Utils/WidgetForm/PictureBlock/P_PictureBlock.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		图片选择块.cpp
		版本：		v1.03
		作者：		drill_up
		所属模块：	工具模块
		功能：		能显示图片列表，并能单选/多选。（PiS全称：Picture_Selector）

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
	this->m_itemTank.clear();							//项列表
	this->m_widgetTank.clear();							//控件列表
	this->m_last_index = -1;							//上一个选中的索引项

	// > 鼠标事件
	this->m_slotBlock_selection = false;

	// > 配置项
	this->m_config;

	// > 资源数据
	this->m_org_bitmapList.clear();


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
	this->m_parent->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
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
		工厂 - 建立一个图片控件
*/
QWidget* P_PiS_Selector::createPictureWidget(int i, QPixmap pixmap){
	int item_height = this->m_config.getItemHeight();

	// > 建立图片块
	P_PictureBlock* widget = new P_PictureBlock(item_height, item_height, this->m_parent);
	if (this->m_config.m_zeroFill == true){
		widget->setTitle(TTool::_zeroFill_(i + 1, this->m_config.m_zeroFillCount, QLatin1Char(this->m_config.m_zeroFillChar.toLatin1())));
	}else{
		widget->setTitle(QString::number(i + 1));
	}

	// > 绘制图片
	widget->setPixmap(pixmap);
	widget->setMaskEnabled(this->m_config.m_isMaskEnabled);
	
	return widget;
}


/*-------------------------------------------------
		控件 - 重建UI（如果图片多，不建议反复调用）
*/
void P_PiS_Selector::rebuildUI() {
	QList<int> last_selected_item = this->getSelectedIndex_Multiple();
	int item_height = this->m_config.getItemHeight();

	// > 清理
	this->m_itemTank.clear();
	this->m_widgetTank.clear();
	this->m_parent->clear();

	// > 单选/多选切换
	if (this->m_config.m_isMultiSelect == true && this->m_parent->selectionMode() == QAbstractItemView::SelectionMode::SingleSelection){
		this->m_parent->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	}
	if (this->m_config.m_isMultiSelect == false && this->m_parent->selectionMode() != QAbstractItemView::SelectionMode::SingleSelection){
		this->m_parent->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	}

	// > 行高刷新
	this->m_parent->setFixedHeight(this->m_config.m_height);
	this->m_parent->setMinimumHeight(this->m_config.m_height);
	this->m_parent->setMaximumHeight(this->m_config.m_height);
	this->m_parent->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);


	// > 刷新内容
	this->m_parent->setIconSize(QSize(item_height, 1));	//（图标大小）
	for (int i = 0; i < this->m_org_bitmapList.count(); i++){

		// > 建立项
		QListWidgetItem* item = this->createPictureItem();

		// > 每项添加
		this->m_parent->addItem(item);
		this->m_itemTank.append(item);

		// > 建立控件（必须在addItem后面）
		QWidget* widget = this->createPictureWidget(i, this->m_org_bitmapList.at(i));

		// > 添加到 列表控件
		this->m_parent->setItemWidget(item, widget);
		this->m_widgetTank.append(widget);
	}


	// > 选中变化项 - 多选
	if (this->m_config.m_isMultiSelect){
		this->selectIndex_Multiple(last_selected_item);
		
	// > 选中变化项 - 单选
	}else{
		if (last_selected_item.count() > 0){
			this->selectIndex_Single(last_selected_item.at(0));
		}
	}
}
/*-------------------------------------------------
		控件 - 建立图片项
*/
QListWidgetItem* P_PiS_Selector::createPictureItem(){
	QListWidgetItem* item = new QListWidgetItem(this->m_parent);
	int item_height = this->m_config.getItemHeight();

	// > 撑开每项
	//QPixmap temp_bitmap(item_height, 1);				//（iconMode中，用图标撑开横向，图标不能撑开纵向）
	//temp_bitmap.fill(Qt::white);						//（现在是listMode了，所以不需要了。）
	//item->setIcon(QIcon(temp_bitmap));				//
	item->setText(this->m_config.getLineExpand());		//（用换行符撑开纵向）
	item->setSizeHint(QSize(this->m_config.getItemHeight(), this->m_config.getItemHeight()));	//（如果不撑开，选中的 蓝色方块 会紊乱）

	return item;
}
/*-------------------------------------------------
		控件 - 清理项
*/
void P_PiS_Selector::clearItems(){

	// > 清理 - 父控件
	this->m_parent->clear();

	// > 清理 - 控件
	this->m_itemTank.clear();
	this->m_widgetTank.clear();
	this->m_last_index = -1;

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
	this->m_itemTank.clear();
	this->m_widgetTank.clear();
	this->m_last_index = -1;

	// > 清理 - 鼠标事件
	//	（无）

	// > 清理 - 配置类
	this->m_config = C_PiS_Config();

	// > 清理 - 资源数据
	this->m_org_bitmapList.clear();

}



/*-------------------------------------------------
		鼠标事件 - 右键事件
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
	//（暂无）
}

/*-------------------------------------------------
		鼠标事件 -  圈选变化事件
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

	// > 发信号
	if (selected_item_list.count() == 1){

		int index = this->getSelectedIndex_Single();
		if (this->m_last_index != index){
			this->m_last_index = index;
			emit signal_currentIndexChanged_Single(index);
			emit signal_selectedIndexChanged_Multiple(this->getSelectedIndex_Multiple());
		}

	}else if (selected_item_list.count() > 1){

		emit signal_selectedIndexChanged_Multiple(this->getSelectedIndex_Multiple());
	}
}


/*-------------------------------------------------
		配置类 - 设置配置
*/
void P_PiS_Selector::setConfigParam(C_PiS_Config config){
	this->m_config = config;
	this->rebuildUI();
}
/*-------------------------------------------------
		配置类 - 取出配置
*/
C_PiS_Config P_PiS_Selector::getConfigParam(){
	return this->m_config;
}


/*-------------------------------------------------
		资源数据 - 设置贴图
*/
void P_PiS_Selector::setSourceBitmap(QList<QPixmap> bitmap_list) {
	this->m_org_bitmapList = bitmap_list;
	this->rebuildUI();

	emit signal_SourceBitmapChanged();
}
/*-------------------------------------------------
		资源数据 - 获取贴图
*/
QList<QPixmap> P_PiS_Selector::getSourceBitmap(){
	return this->m_org_bitmapList;
}


/*-------------------------------------------------
		选中 - 设置选中
*/
void P_PiS_Selector::selectIndex_Single(int index){
	if (index < 0){ index = 0; }
	if (index >= this->m_itemTank.count()){ index = this->m_itemTank.count() - 1; }

	for (int i = 0; i < this->m_itemTank.count(); i++){
		QListWidgetItem* item = this->m_itemTank.at(i);
		if (i == index){
			item->setSelected(true);
			this->m_parent->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
}
/*-------------------------------------------------
		选中 - 获取选中数据
*/
int P_PiS_Selector::getSelectedIndex_Single(){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		if (this->m_itemTank.at(i)->isSelected()){
			return i;
		}
	}
	return -1;
}

/*-------------------------------------------------
		选中 - 设置选中（多选时）
*/
void P_PiS_Selector::selectIndex_Multiple(QList<int> index_list){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QListWidgetItem* item = this->m_itemTank.at(i);
		if (index_list.contains( i )){
			item->setSelected(true);
			this->m_parent->scrollToItem(item);
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
	for (int i = 0; i < this->m_itemTank.count(); i++){
		if (this->m_itemTank.at(i)->isSelected()){
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
	this->selectIndex_Single(this->m_itemTank.count()-1);
}