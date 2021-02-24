#include "stdafx.h"
#include "p_PictureSelector.h"

#include "private/p_PiSBlock.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		单选/多选 图片列.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		将图片全部显示，并能单选/多选。

		子功能：	-> 文本选择
						-> 单选
						-> 多选
						-> 自动编号
					-> 图片列配置
						-> 设置行高
						-> 设置叶子显示文本
					
		使用方法：
				>初始化
					this->m_p_PictureSelector = new P_PictureSelector(ui.treeWidget);
					this->m_p_PictureSelector->setSource(bitmap_list);		
					
-----==========================================================-----
*/
P_PictureSelector::P_PictureSelector(QListWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----参数初始化
	this->m_listWidget = parent;									//图片列对象
	this->m_listWidgetStyle = this->m_listWidget->styleSheet();		//图片列默认样式
	this->m_itemTank = QList<QListWidgetItem*>();					//控件列表
	this->m_widgetTank = QList<QWidget*>();							//控件列表
	this->m_last_index = -1;										//上一个选中的索引项

	// > 数据
	this->m_config = C_PiSConfig();

	//-----------------------------------
	//----ui初始化
	this->m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);									//激活右键
	this->m_listWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);			//单选
	this->m_listWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);			//选行
	this->m_listWidget->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);				//关闭双击编辑

	this->m_listWidget->setFlow(QListView::LeftToRight);
	this->m_listWidget->setMovement(QListView::Static);
	this->m_listWidget->setViewMode(QListView::ListMode);
	this->m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
	this->m_listWidget->setDragEnabled(false);
	this->m_listWidget->setDragDropMode(QListView::NoDragDrop);

	//-----------------------------------
	//----事件绑定
	this->m_selectionSignalBlock_Root = false;				//槽阻塞
	connect(this->m_listWidget, &QListWidget::customContextMenuRequested, this, &P_PictureSelector::itemRightClicked);
	connect(this->m_listWidget, &QListWidget::itemSelectionChanged, this, &P_PictureSelector::itemSelectionChanged);

}
P_PictureSelector::~P_PictureSelector(){
}


/*-------------------------------------------------
		控件 - 重建图片列（如果图片多，不建议反复调用）
*/
void P_PictureSelector::rebuildListUi() {
	QList<int> last_selected_item = this->getSelectedIndex_Multi();
	int item_height = this->m_config.getItemHeight();

	// > 清理
	this->m_itemTank.clear();
	this->m_widgetTank.clear();
	this->m_listWidget->clear();

	// > 单选/多选切换
	if (this->m_config.m_isMultiSelect == true && this->m_listWidget->selectionMode() == QAbstractItemView::SelectionMode::SingleSelection){
		this->m_listWidget->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	}
	if (this->m_config.m_isMultiSelect == false && this->m_listWidget->selectionMode() != QAbstractItemView::SelectionMode::SingleSelection){
		this->m_listWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	}

	// > 行高刷新
	this->m_listWidget->setFixedHeight(this->m_config.m_height);
	this->m_listWidget->setMinimumHeight(this->m_config.m_height);
	this->m_listWidget->setMaximumHeight(this->m_config.m_height);
	this->m_listWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

	// > 刷新内容
	this->m_listWidget->setIconSize(QSize(item_height, 1));	//（图标大小）
	for (int i = 0; i < this->m_org_bitmapList.count(); i++){

		// > 建立项
		QListWidgetItem* item = this->createPictureItem();

		// > 每项添加
		this->m_listWidget->addItem(item);
		this->m_itemTank.append(item);

		// > 建立控件（必须在addItem后面）
		QWidget* widget = this->createPictureWidget(i, this->m_org_bitmapList.at(i));

		// > 添加到 列表控件
		this->m_listWidget->setItemWidget(item, widget);
		this->m_widgetTank.append(widget);
	}

	// > 选中变化项
	if (this->m_config.m_isMultiSelect){
		this->selectIndex_Multi(last_selected_item);
	}else{
		if (last_selected_item.count() > 0){
			this->selectIndex(last_selected_item.at(0));
		}
	}
}

/*-------------------------------------------------
		控件 - 建立图片项
*/
QListWidgetItem* P_PictureSelector::createPictureItem(){
	QListWidgetItem* item = new QListWidgetItem(this->m_listWidget);
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
		控件 - 每项建立控件
*/
QWidget* P_PictureSelector::createPictureWidget(int i, QPixmap pixmap){
	int item_height = this->m_config.getItemHeight();

	// > 建立图片块
	P_PiSBlock* widget = new P_PiSBlock(item_height, item_height, this->m_listWidget);
	if (this->m_config.m_zeroFill == true){
		widget->setCountText(TTool::_zeroFill_(i + 1, this->m_config.m_zeroFillCount, QLatin1Char(this->m_config.m_zeroFillChar.toLatin1())));
	}else{
		widget->setCountText(QString::number(i + 1));
	}

	// > 绘制图片
	widget->setPixmap(pixmap);
	widget->setMaskEnabled(this->m_config.m_isMaskEnabled);
	
	return widget;
}
/*-------------------------------------------------
		控件 - 清理全部
*/
void P_PictureSelector::clearAll(){

	// > 控件清理
	this->m_itemTank.clear();
	this->m_widgetTank.clear();
	this->m_listWidget->clear();
	this->m_last_index = -1;

	// > 图片列设置
	this->m_config = C_PiSConfig();

}

/*-------------------------------------------------
		图片列设置 - 设置参数
*/
void P_PictureSelector::setConfigParam(C_PiSConfig config){
	this->m_config = config;
	this->rebuildListUi();
}
/*-------------------------------------------------
		图片列设置 - 取出参数
*/
C_PiSConfig P_PictureSelector::getConfigParam(){
	return this->m_config;
}


/*-------------------------------------------------
		事件 - 右键事件
*/
void P_PictureSelector::itemRightClicked(QPoint point) {
	if (this->m_selectionSignalBlock_Root == true){ return; }
	this->m_selectionSignalBlock_Root = true;

	QListWidgetItem *item = this->m_listWidget->itemAt(point);	//（当前右键选中的项）
	if (item == nullptr){

		// > 点到空白处
		this->event_itemRightClicked(QList<QListWidgetItem*>());
	}else{

		// > 点到项
		this->event_itemRightClicked(this->m_listWidget->selectedItems());
	}

	this->m_selectionSignalBlock_Root = false;
}
/*-------------------------------------------------
		事件 - 右键事件（零个、单个、多个）
*/
void P_PictureSelector::event_itemRightClicked(QList<QListWidgetItem*> item_list){
	//（暂无）
}

/*-------------------------------------------------
		事件 -  圈选变化事件
*/
void P_PictureSelector::itemSelectionChanged(){
	if (this->m_selectionSignalBlock_Root == true){ return; }
	this->m_selectionSignalBlock_Root = true;
	
	QList<QListWidgetItem*> selected_item = this->m_listWidget->selectedItems();
	if (selected_item.count() == 0){ this->m_selectionSignalBlock_Root = false; return; }
	this->event_itemSelectionChanged(selected_item);

	this->m_selectionSignalBlock_Root = false;
}
/*-------------------------------------------------
		事件 - 圈选变化事件（单个、多个）
*/
void P_PictureSelector::event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list){

	// > 发信号
	if (selected_item_list.count() == 1){

		int index = this->getSelectedIndex();
		if (this->m_last_index != index){
			this->m_last_index = index;
			emit currentIndexChanged(index);
			emit selectedIndexChanged_Multi(this->getSelectedIndex_Multi());
		}

	}else if (selected_item_list.count() > 1){

		emit selectedIndexChanged_Multi(this->getSelectedIndex_Multi());
	}
}


/*-------------------------------------------------
		资源数据 - 设置数据
*/
void P_PictureSelector::setSource(QList<QFileInfo> file_list) {
	this->m_org_bitmapList = QList<QPixmap>();
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo info = file_list.at(i);
		QImage image = QImage(info.absoluteFilePath());
		QPixmap pixmap = QPixmap::fromImage(image);
		this->m_org_bitmapList.append(pixmap);
	}
	this->rebuildListUi();
	
	emit animBitmapChanged();
}
void P_PictureSelector::setSource(QList<QPixmap> bitmap_list) {
	this->m_org_bitmapList = bitmap_list;
	this->rebuildListUi();

	emit animBitmapChanged();
}


/*-------------------------------------------------
		选中 - 设置选中
*/
void P_PictureSelector::selectIndex(int index){
	if (index < 0){ index = 0; }
	if (index >= this->m_itemTank.count()){ index = this->m_itemTank.count() - 1; }

	for (int i = 0; i < this->m_itemTank.count(); i++){
		QListWidgetItem* item = this->m_itemTank.at(i);
		if (i == index){
			item->setSelected(true);
			this->m_listWidget->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
}
/*-------------------------------------------------
		选中 - 获取选中数据
*/
int P_PictureSelector::getSelectedIndex(){
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
void P_PictureSelector::selectIndex_Multi(QList<int> index_list){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QListWidgetItem* item = this->m_itemTank.at(i);
		if (index_list.contains( i )){
			item->setSelected(true);
			this->m_listWidget->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
}
/*-------------------------------------------------
		选中 - 获取选中数据（多选时）
*/
QList<int> P_PictureSelector::getSelectedIndex_Multi(){
	QList<int> result_list = QList<int>();
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
void P_PictureSelector::selectLast(){
	this->selectIndex(this->getSelectedIndex() - 1);
}
/*-------------------------------------------------
		选中 - 选中下一项
*/
void P_PictureSelector::selectNext(){
	this->selectIndex(this->getSelectedIndex() + 1);
}
/*-------------------------------------------------
		选中 - 选中首项
*/
void P_PictureSelector::selectStart(){
	this->selectIndex(0);
}
/*-------------------------------------------------
		选中 - 选中尾项
*/
void P_PictureSelector::selectEnd(){
	this->selectIndex(this->m_itemTank.count()-1);
}