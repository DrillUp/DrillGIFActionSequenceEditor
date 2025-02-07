﻿#include "stdafx.h"
#include "P_FlexiblePageTree.h"

#include "Private/W_FPT_Config.h"

#include "Source/Utils/Common/TTool.h"
#include "Source/Utils/WidgetFormSenior/ObjectSortController/C_ObjectSortData.h"
#include "Source/Utils/WidgetFormSenior/ObjectSortController/P_ObjectSortController.h"
#include "Source/Utils/Manager/ChineseManager/S_ChineseManager.h"

/*
-----==========================================================-----
		类：		灵活分类树.cpp
		版本：		v1.05
		作者：		drill_up
		所属模块：	工具模块
		功能：		能够显示一堆数据，并且将这些数据分类或转移到不同的树枝中，便于查询。
					注意，该树被装饰后，需要对该控件块进行交互，不要对树进行直接交互。
					【包含id分支、名称分支，不含种类分支】

		子功能：	-> 树结构
						-> 只有一层树枝，树枝都是分类，叶子都是对象
						-> 不含叶子的添加、删除功能，但可以编辑
						-> 不可编辑对象最大值（上限）
					-> 选中
						-> 可多选，注意没有选项时问题
						-> 每次选中时，发射树的信号
					-> 分支
						-> ID分支
						-> 名称分支
						x-> 种类分支（该类不含种类分支功能）
					-> 树配置
						-> 显示
							> 设置行高
							> 设置叶子显示文本
						-> 分支模式
							> 修改模式
							> ID分支条数

		说明：		所有叶子在loadSource后不再增减。
						rebuildTreeData：可以使得叶子重新排序、刷新叶子显示
						refreshTreeData：刷新叶子显示
					树中反复改变的是树枝，树枝的数据，都在 树配置 中存储，与 资源数据 无关。
					
		使用方法：
				> 初始化
					this->m_p_tree = new P_FlexiblePageTree(ui.treeWidget_2);	//（创建对象）
				> 数据初始化
					C_FPT_Config* config = this->m_p_tree->createConfigData();				//（从工厂中获取 树配置 ，需要强转）
					config->setJsonObject(obj_treeData, this->m_p_tree);					//（树配置初始化）
					this->m_p_tree->setConfigEx(config);									//
					this->m_p_tree->loadSource(data_list, "id", "name", "type");			//（读取资源数据）
				> 获取树配置
					C_FPT_Config* config = this->m_p_tree->getConfig();
					
		使用注意：	【主要有两个交互对象：树配置 和 资源数据 】
					config传入的是指针，修改会立即生效，但不会刷新数据，所以仍然需要调用setConfig()函数。
					内部修改会直接改变 资源数据 的数据内容，如果外部需要同步刷新，可以获取变化的数据，然后刷新。
-----==========================================================-----
*/
P_FlexiblePageTree::P_FlexiblePageTree(QTreeWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----参数初始化

	// > 树对象
	this->m_tree = parent;								//树对象
	this->m_treeStyle = this->m_tree->styleSheet();		//树默认样式

	// > 叶子
	this->m_leafItem.clear();							//叶子列表

	// > 树枝
	this->m_branchItem.clear();							//树枝列表

	// > 选中
	this->m_last_selectedItem = nullptr;
	this->m_last_selectedLeaf = nullptr;

	// > 树事件
	this->m_slotBlock = false;							//槽阻塞

	// > 右键菜单
	this->m_mainMenu = nullptr;							//右键菜单
	this->m_modeMenu = nullptr;							//分支菜单
	this->m_leafOuterControlEnabled = true;				//菜单 - 开关
	this->m_leafOuterControl_CopyActive = true;			//菜单 - 复制激活
	this->m_leafOuterControl_PasteActive = false;		//菜单 - 粘贴激活
	this->m_leafOuterControl_ClearActive = true;		//菜单 - 清空激活

	// > 树设置
	this->m_config = this->createConfigData();

	// > 资源数据
	this->m_source_ObjectSortController = new P_ObjectSortController();
	this->m_source_list.clear();

	// > 窗口锁
	this->m_config_windowLockNum = 0;


	//-----------------------------------
	//----ui初始化
	this->m_tree->setContextMenuPolicy(Qt::CustomContextMenu);								//激活右键
	this->m_tree->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);	//多选叶子


	//-----------------------------------
	//----事件绑定
	connect(this->m_tree, &QTreeWidget::itemClicked, this, &P_FlexiblePageTree::sltItemClicked);
	connect(this->m_tree, &QTreeWidget::itemDoubleClicked, this, &P_FlexiblePageTree::sltItemDoubleClicked);
	connect(this->m_tree, &QTreeWidget::customContextMenuRequested, this, &P_FlexiblePageTree::sltItemRightClicked);
	connect(this->m_tree, &QTreeWidget::itemSelectionChanged, this, &P_FlexiblePageTree::sltItemSelectionChanged);

}
P_FlexiblePageTree::~P_FlexiblePageTree(){
}


/*-------------------------------------------------
		工厂 - 创建 树设置 数据（可覆写）
*/
C_FPT_Config* P_FlexiblePageTree::createConfigData(){
	return new C_FPT_Config();
}
/*-------------------------------------------------
		工厂 - 创建 树设置 编辑窗口（可覆写）
*/
W_FPT_Config* P_FlexiblePageTree::createConfigWindow(){
	return new W_FPT_Config(this->m_tree);
}
/*-------------------------------------------------
		工厂 - 创建 叶子控件（可覆写）
*/
I_FPT_Leaf* P_FlexiblePageTree::createFCTLeaf(){
	return new I_FPT_Leaf(this->m_config);
}
/*-------------------------------------------------
		工厂 - 创建 树枝控件（可覆写）
*/
I_FPT_Branch* P_FlexiblePageTree::createFCTBranch(){
	return new I_FPT_Branch(this->m_config);
}


/*-------------------------------------------------
		树对象 - 获取树
*/
QTreeWidget* P_FlexiblePageTree::getTree(){
	return this->m_tree;
}
/*-------------------------------------------------
		树对象 - 刷新树
*/
void P_FlexiblePageTree::refreshTreeUi() {
	QList<QTreeWidgetItem*> last_selected_item = this->m_tree->selectedItems();

	// > 去掉全部树枝
	for (int i = this->m_tree->topLevelItemCount()-1; i >=0; i--){
		this->m_tree->takeTopLevelItem(i);
	}

	// > 重新添加树枝
	for (int i = 0; i < this->m_branchItem.count(); i++){
		this->m_tree->addTopLevelItem(this->m_branchItem.at(i));
	}

	// > 刷新树枝数据
	for (int i = 0; i < this->m_branchItem.count(); i++){
		I_FPT_Branch* branch = this->m_branchItem.at(i);
		branch->refreshItemSelf();
	}
	// > 刷新叶子数据
	for (int i = 0; i < this->m_leafItem.count(); i++){
		I_FPT_Leaf* leaf = this->m_leafItem.at(i);
		leaf->refreshItemSelf();
	}

	// > 特殊分支操作
	this->refreshTreeUi_special();

	// > 行高刷新
	this->m_tree->setStyleSheet(this->m_treeStyle + "\n QTreeView::item { height: " + QString::number(this->m_config->rowHeight) + "px;}");
	
	// > 展开全部
	this->m_tree->expandAll();

	// > 选中变化项
	for (int i = 0; i < last_selected_item.count(); i++){
		if (i == 0){
			this->m_tree->scrollToItem(last_selected_item.at(0));
		}
		this->m_tree->setItemSelected(last_selected_item.at(i), true);
	}
}
/*-------------------------------------------------
		树对象 - 清理全部
*/
void P_FlexiblePageTree::clearAll(){

	// > 去掉全部分类
	for (int i = this->m_tree->topLevelItemCount() - 1; i >= 0; i--){
		this->m_tree->takeTopLevelItem(i);
	}

	// > 控件清理
	this->m_leafItem.clear();
	this->m_branchItem.clear();

	// > 树设置（树设置与资源读取无关，不清理）

	// > 资源清理
	this->m_source_list.clear();
	this->m_leafOuterControl_PasteActive = false;
}

/*-------------------------------------------------
		树对象 - 刷新树 - 分支
*/
void P_FlexiblePageTree::refreshTreeUi_special() {

	// > 叶子位置转移
	if (this->m_config->is_id_Mode()){ this->refreshTreeUi_id_inc(); }
	if (this->m_config->is_name_Mode()){ this->refreshTreeUi_name_inc(); }

}
/*-------------------------------------------------
		树对象 - 刷新树 - 分支 - ID分支
*/
void P_FlexiblePageTree::refreshTreeUi_id_inc() {
	
	// > 无树枝变化（在不重刷的情况下，id是不可能会变的）

}
/*-------------------------------------------------
		树对象 - 刷新树 - 分支 - 名称分支
*/
void P_FlexiblePageTree::refreshTreeUi_name_inc() {
	
	// > 名称修改后，修改的叶子可能会跑到新树枝下
	for (int i = 0; i < this->m_leafItem.count(); i++){
		I_FPT_Leaf* leaf = this->m_leafItem.at(i);
		I_FPT_Branch* parent_branch = dynamic_cast<I_FPT_Branch*>(leaf->parent());

		QString branch_symbol = parent_branch->getBranch_name_Symbol();
		QString leaf_symbol = leaf->getLeaf_name_Symbol();
		if (branch_symbol != leaf_symbol){
			leaf->parent()->removeChild(leaf);

			I_FPT_Branch* n_branch = this->getBranchByNameSymbol(leaf_symbol);
			if (n_branch == nullptr){ continue; }
			n_branch->addChild(leaf);
		}
	}

}


/*-------------------------------------------------
		叶子 - 【外部修改】叶子名称
*/
void P_FlexiblePageTree::outerModifyLeafName(int id, QString name){
	I_FPT_Leaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return; }
	
	// > 修改叶子
	leaf->setName(name);
	// > 修改叶子 - 名称分支时，叶子父类可能转移
	if (this->m_config->is_name_Mode()){
		if (name == ""){
			leaf->setLeaf_name_Symbol("空字符");
		}else{
			leaf->setLeaf_name_Symbol(S_ChineseManager::getInstance()->getChineseFirstSpell(name));	//首字母
		}
	}
	leaf->refreshItemSelf();

	// > 修改本地数据
	for (int i = 0; i < this->m_source_list.count(); i++){
		C_ObjectSortData c_data = this->m_source_list.at(i);
		if (c_data.id == id ){
			if (c_data.name != name){
				c_data.name = name;
				this->m_source_list.replace(i, c_data);
			}
			break;
		}
	}
}
/*-------------------------------------------------
		叶子 - 【外部修改】叶子类型
*/
void P_FlexiblePageTree::outerModifyLeafType(int id, QString type){
	I_FPT_Leaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return; }

	// > 修改叶子
	leaf->setType(type);
	leaf->refreshItemSelf();

	// > 修改本地数据
	for (int i = 0; i < this->m_source_list.count(); i++){
		C_ObjectSortData c_data = this->m_source_list.at(i);
		if (c_data.id == id){
			if (c_data.type != type){
				c_data.type = type;
				this->m_source_list.replace(i, c_data);
			}
			break;
		}
	}
}
/*-------------------------------------------------
		叶子 - 【外部修改】选中的叶子名称
*/
void P_FlexiblePageTree::outerModifySelectedLeafName(QString name){
	if (this->m_last_selectedLeaf == nullptr){ return; }
	
	// > 修改叶子
	this->m_last_selectedLeaf->setName(name);
	// > 修改叶子 - 名称分支时，叶子父类可能转移
	if (this->m_config->is_name_Mode()){
		if (name == ""){
			this->m_last_selectedLeaf->setLeaf_name_Symbol("空字符");
		}else{
			this->m_last_selectedLeaf->setLeaf_name_Symbol(S_ChineseManager::getInstance()->getChineseFirstSpell(name));	//首字母
		}
	}
	this->m_last_selectedLeaf->refreshItemSelf();

	// > 修改本地数据
	for (int i = 0; i < this->m_source_list.count(); i++){
		C_ObjectSortData c_data = this->m_source_list.at(i);
		if (c_data.id == this->m_last_selectedLeaf->getId()){
			if (c_data.name != name){
				c_data.name = name;
				this->m_source_list.replace(i, c_data);
			}
			break;
		}
	}
}
/*-------------------------------------------------
		叶子 - 【外部修改】选中的叶子类型
*/
void P_FlexiblePageTree::outerModifySelectedLeafType(QString type){
	if (this->m_last_selectedLeaf == nullptr){ return; }

	// > 修改叶子
	this->m_last_selectedLeaf->setType(type);
	this->m_last_selectedLeaf->refreshItemSelf();

	// > 修改本地数据
	for (int i = 0; i < this->m_source_list.count(); i++){
		C_ObjectSortData c_data = this->m_source_list.at(i);
		if (c_data.id == this->m_last_selectedLeaf->getId()){
			if (c_data.type != type){
				c_data.type = type;
				this->m_source_list.replace(i, c_data);
			}
			break;
		}
	}
}


/*-------------------------------------------------
		叶子 - 获取 - 获取对象（根据ID）
*/
I_FPT_Leaf* P_FlexiblePageTree::getLeafById(int id){
	for (int i = 0; i < this->m_leafItem.count(); i++){
		int l_id = this->m_leafItem.at(i)->getId();
		if (l_id == id){
			return this->m_leafItem.at(i);
		}
	}
	return nullptr;
}
I_FPT_Leaf* P_FlexiblePageTree::getLeafByName(QString name){
	for (int i = 0; i < this->m_leafItem.count(); i++){
		if (this->m_leafItem.at(i)->getName() == name){
			return this->m_leafItem.at(i);
		}
	}
	return nullptr;
}
/*-------------------------------------------------
		叶子 - 获取 - 名称
*/
QString P_FlexiblePageTree::getLeafName(int id){
	I_FPT_Leaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return ""; }
	return leaf->getName();
}
/*-------------------------------------------------
		叶子 - 获取 - 判断叶子
*/
bool P_FlexiblePageTree::hasLeafName(QString name){
	for (int i = 0; i < this->m_leafItem.count(); i++){
		if (this->m_leafItem.at(i)->getName() == name){
			return true;
		}
	}
	return false;
}
/*-------------------------------------------------
		叶子 - 获取 - 判断对象
*/
bool P_FlexiblePageTree::isLeaf(QTreeWidgetItem* item){
	for (int i = 0; i < this->m_leafItem.count(); i++){
		if (this->m_leafItem.at(i) == item){
			return true;
		}
	}
	return false;
}
bool P_FlexiblePageTree::isLeafList(QList<QTreeWidgetItem*> item_list){
	for (int i = 0; i < item_list.count(); i++){
		if (this->isLeaf(item_list.at(i)) == false){
			return false;
		}
	}
	return true;
}


/*-------------------------------------------------
		树枝 - 获取树枝（名称分支专用）
*/
I_FPT_Branch* P_FlexiblePageTree::getBranchByNameSymbol(QString symbol){
	for (int i = 0; i < this->m_branchItem.count(); i++){
		if (this->m_branchItem.at(i)->getBranch_name_Symbol() == symbol){
			return this->m_branchItem.at(i);
		}
	}
	return nullptr;
}
/*-------------------------------------------------
		树枝 - 获取 - 判断对象
*/
bool P_FlexiblePageTree::isBranch(QTreeWidgetItem* item){
	for (int i = 0; i < this->m_branchItem.count(); i++){
		if (this->m_branchItem.at(i) == item){
			return true;
		}
	}
	return false;
}
bool P_FlexiblePageTree::isBranchList(QList<QTreeWidgetItem*> item_list){
	for (int i = 0; i < item_list.count(); i++){
		if (this->isBranch(item_list.at(i)) == false){
			return false;
		}
	}
	return true;
}



/*-------------------------------------------------
		树事件 - 单击事件
*/
void P_FlexiblePageTree::sltItemClicked(QTreeWidgetItem *item, int index) {
	if (item == nullptr){ return; }
	if (this->m_slotBlock == true){ return; }
	
	//...
}
/*-------------------------------------------------
		树事件 - 双击事件
*/
void P_FlexiblePageTree::sltItemDoubleClicked(QTreeWidgetItem *item, int index) {
	if (item == nullptr){ return; }
	if (this->m_slotBlock == true){ return; }

	//...
}
/*-------------------------------------------------
		树事件 -  圈选变化事件
*/
void P_FlexiblePageTree::sltItemSelectionChanged(){
	if (this->m_slotBlock == true){ return; }
	QList<QTreeWidgetItem*> selected_items = this->m_tree->selectedItems();
	if (selected_items.count() == 0){ return; }
	QTreeWidgetItem *item = selected_items.at(0);

	// > 树节点变化
	if (this->m_last_selectedItem != item){
		this->m_last_selectedItem = item;
		emit signal_currentItemChanged(item);
	}
	// > 叶子变化
	if ( this->isLeaf(item) && 
		 this->m_last_selectedLeaf != item){
		this->m_last_selectedLeaf = dynamic_cast<I_FPT_Leaf*>(item);
		emit signal_currentLeafChanged(item, this->m_last_selectedLeaf->getId(), this->m_last_selectedLeaf->getName());
	}
}
/*-------------------------------------------------
		树事件 - 右键事件
*/
void P_FlexiblePageTree::sltItemRightClicked(QPoint point) {
	QTreeWidgetItem *item = this->m_tree->itemAt(point);	//（当前右键选中的项）
	if (item == nullptr){ return; }
	if (this->m_tree->selectedItems().count() == 0){ return; }

	/*------------------------右键点击一个或多个 树枝/叶子------------------------*/
	this->m_mainMenu = new QMenu(this->m_tree);

		this->m_modeMenu = new QMenu(this->m_mainMenu);
		this->drawMenuMode();

	this->drawMenuMain();
	this->drawMenuMainLast();

	this->m_mainMenu->exec(QCursor::pos());
	this->m_mainMenu->deleteLater();
}


/*-------------------------------------------------
		右键菜单 - 一级菜单
*/
void P_FlexiblePageTree::drawMenuMain(){
	QAction *action;
	QList<QTreeWidgetItem*> selected_items = this->m_tree->selectedItems();

	/*------------------------右键点击单个叶子------------------------*/
	if (this->isLeafList(selected_items) == true && selected_items.count() == 1 && this->m_leafOuterControlEnabled == true){
		I_FPT_Leaf* leaf = dynamic_cast<I_FPT_Leaf*>(selected_items.at(0));

		action = new QAction("复制", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Copy.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuCopyLeafInAction);
		this->m_mainMenu->addAction(action);
		action->setEnabled(this->m_leafOuterControl_CopyActive);
		
		action = new QAction("粘贴", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Paste.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuPasteLeafInAction);
		this->m_mainMenu->addAction(action);
		action->setEnabled(this->m_leafOuterControl_PasteActive);

		action = new QAction("清空", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Clear.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuClearLeafInAction);
		this->m_mainMenu->addAction(action);
		action->setEnabled(this->m_leafOuterControl_ClearActive);

		this->m_mainMenu->addSeparator();
	}

	/*------------------------任意情况------------------------*/
	action = new QAction("展开全部", this);
	action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Unfold.png"));
	connect(action, &QAction::triggered, this->m_tree, &QTreeWidget::expandAll);
	this->m_mainMenu->addAction(action);

	action = new QAction("收起全部", this);
	action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Fold.png"));
	connect(action, &QAction::triggered, this->m_tree, &QTreeWidget::collapseAll);
	this->m_mainMenu->addAction(action);


}
/*-------------------------------------------------
		右键菜单 - 一级菜单（尾部）
*/
void P_FlexiblePageTree::drawMenuMainLast(){
	QAction *action;

	this->m_mainMenu->addSeparator();
	this->m_mainMenu->addMenu(this->m_modeMenu);

	action = new QAction("刷新排序", this);
	action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Refresh.png"));
	connect(action, &QAction::triggered, this, &P_FlexiblePageTree::rebuildTreeData);
	this->m_mainMenu->addAction(action);

	action = new QAction("树设置...", this);
	action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Setting.png"));
	connect(action, &QAction::triggered, this, &P_FlexiblePageTree::openConfigParamWindow);
	this->m_mainMenu->addAction(action);

}
/*-------------------------------------------------
		右键菜单 - 二级菜单（模式）
*/
void P_FlexiblePageTree::drawMenuMode(){
	QAction *action;
	this->m_modeMenu->setTitle("分支模式");

	QStringList sort_mode_list = this->m_config->getModeList();
	for (int i = 0; i < sort_mode_list.count(); i++){
		QString mode_name = sort_mode_list.at(i);
		action = new QAction(mode_name, this);
		action->setData(mode_name);
		if (this->getCurrentSortMode() == mode_name){
			action->setCheckable(true);
			action->setChecked(true);
		}
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::changeSortModeInAction);
		this->m_modeMenu->addAction(action);
	}
}

/*-------------------------------------------------
		右键菜单 - 开关（默认开启）
*/
void P_FlexiblePageTree::setLeafOuterControlEnabled(bool enabled){
	this->m_leafOuterControlEnabled = enabled;
}
/*-------------------------------------------------
		右键菜单 - 激活控制
*/
void P_FlexiblePageTree::setLeafOuterControl_CopyActive(bool enabled){
	this->m_leafOuterControl_CopyActive = enabled;
}
void P_FlexiblePageTree::setLeafOuterControl_PasteActive(bool enabled){
	this->m_leafOuterControl_PasteActive = enabled;
}
void P_FlexiblePageTree::setLeafOuterControl_ClearActive(bool enabled){
	this->m_leafOuterControl_ClearActive = enabled;
}
/*-------------------------------------------------
		右键菜单 - 复制按下
*/
void P_FlexiblePageTree::menuCopyLeafInAction(){
	if (this->m_leafOuterControl_CopyActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int id = cur_action->data().value<int>();
	emit signal_menuCopyLeafTriggered(id);
}
/*-------------------------------------------------
		右键菜单 - 粘贴按下
*/
void P_FlexiblePageTree::menuPasteLeafInAction(){
	if (this->m_leafOuterControl_PasteActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int id = cur_action->data().value<int>();
	emit signal_menuPasteLeafTriggered(id);
}
/*-------------------------------------------------
		右键菜单 - 清空按下
*/
void P_FlexiblePageTree::menuClearLeafInAction(){
	if (this->m_leafOuterControl_ClearActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int id = cur_action->data().value<int>();
	emit signal_menuClearLeafTriggered(id);
}


/*-------------------------------------------------
		树设置 - 设置参数
*/
void P_FlexiblePageTree::setConfig(C_FPT_Config* config){
	this->m_config = config;
	this->refreshTreeUi();
}
/*-------------------------------------------------
		树设置 - 取出参数
*/
C_FPT_Config* P_FlexiblePageTree::getConfig(){
	return this->m_config;
}
/*-------------------------------------------------
		树设置 - 编辑窗口
*/
void P_FlexiblePageTree::openConfigParamWindow(){
	QString last_sortMode = this->m_config->getCurrentMode();
	int last_pageNum = this->m_config->pagePerNum;

	this->windowLock_incOne();
	W_FPT_Config* d = this->createConfigWindow();
	d->setData(this->m_config);
	if (d->exec() == QDialog::Accepted){
		this->m_config = d->getData() ;
		
		// > id分支时，数量变化了，则重建
		if (this->m_config->is_id_Mode() && last_pageNum != this->m_config->pagePerNum){
			this->rebuildTreeData();

		// > 分支模式 变了，重建
		}else if ( last_sortMode != this->m_config->getCurrentMode() ){
			this->rebuildTreeData();

		// > 重刷数据（行高和零填充变了必须刷，刷只会变ui和名称，不会改变排列方式）
		}else{
			this->refreshTreeUi();
		}

	}
	d->deleteLater();
	this->windowLock_decOne();
}
/*-------------------------------------------------
		树设置 - 切换模式
*/
void  P_FlexiblePageTree::changeSortMode(QString sortMode){
	if (this->getCurrentSortMode() == sortMode){ return; }
	this->m_config->setCurrentMode(sortMode);
	this->rebuildTreeData();
}
void  P_FlexiblePageTree::changeSortModeInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString sortMode = cur_action->data().value<QString>();

	this->changeSortMode(sortMode);
}
/*-------------------------------------------------
		树设置 - 获取当前模式
*/
QString P_FlexiblePageTree::getCurrentSortMode(){
	return this->m_config->getCurrentMode();
}


/*-------------------------------------------------
		资源数据 - 载入
*/
void P_FlexiblePageTree::loadSource(QList<QJsonObject> obj_list) {

	// > 全清
	this->clearAll();

	// > 解析数据
	for (int i = 0; i < obj_list.count(); i++){
		C_ObjectSortData data;
		data.setJsonObject(obj_list.at(i));
		if (data.isNull()){ continue; }
		this->m_source_list.append(data);
	}
	// > 解析后必须初始化一次（子类树要用）
	this->m_source_ObjectSortController->setData_FromSortData(this->m_source_list);

	// > 重建全部数据
	this->rebuildTreeData();
}
void P_FlexiblePageTree::loadSource(QList<QJsonObject> obj_list, QString id_symbol, QString name_symbol, QString type_symbol) {

	// > 全清
	this->clearAll();

	// > 解析数据
	for (int i = 0; i < obj_list.count(); i++){
		C_ObjectSortData data;
		data.setJsonObject(obj_list.at(i), id_symbol, name_symbol, type_symbol);	//注意symbol不存
		if (data.isNull()){ continue; }
		this->m_source_list.append(data);
	}
	// > 解析后必须初始化一次（子类树要用）
	this->m_source_ObjectSortController->setData_FromSortData(this->m_source_list);

	// > 重建全部数据
	this->rebuildTreeData();
}

/*-------------------------------------------------
		资源数据 - 获取选中的数据
*/
int P_FlexiblePageTree::getSelectedSourceId(){
	if (this->getSelectedSource() == nullptr){ return -1; }
	return this->getSelectedSource()->id;
};
QString P_FlexiblePageTree::getSelectedSourceName(){
	if (this->getSelectedSource() == nullptr){ return ""; }
	return this->getSelectedSource()->name;
};
QString P_FlexiblePageTree::getSelectedSourceType(){
	if (this->getSelectedSource() == nullptr){ return ""; }
	return this->getSelectedSource()->type;
};
/*-------------------------------------------------
		资源数据 - 获取选中的对象（私有）
*/
C_ObjectSortData* P_FlexiblePageTree::getSelectedSource(){
	if (this->m_leafItem.count() == 0){ return nullptr; }
	QList<QTreeWidgetItem*> item_list = this->m_tree->selectedItems();
	if (item_list.count() == 0){ return nullptr; }

	// > 找到选中的叶子
	I_FPT_Leaf* leaf = nullptr;
	for (int i = 0; i < item_list.count(); i++){
		QTreeWidgetItem* item = item_list.at(i);
		if (this->isLeaf(item)){
			leaf = dynamic_cast<I_FPT_Leaf*>(item);;
			break;
		}
	}
	if (leaf == nullptr){ return nullptr; }

	int arr_index = leaf->getId();			//（从item中获取到id）
	return this->m_source_ObjectSortController->getSortData_ByIndex(arr_index);
}
/*-------------------------------------------------
		资源数据 - 重建数据（私有）
*/
void P_FlexiblePageTree::rebuildTreeData(){
	this->m_source_ObjectSortController->setData_FromSortData(this->m_source_list);

	// > 为空时，选择第一个
	if (this->getCurrentSortMode() == ""){
		QMessageBox::warning(nullptr, "错误", "树控件中出现了有误的分支模式设置，已归为默认设置。", QMessageBox::Yes);
		this->m_config->setCurrentMode( this->m_config->getModeList().first() );
	}
	if (this->m_config->is_id_Mode()){
		this->rebuildTreeData_id_inc();
		this->refreshTreeUi();
	}
	if (this->m_config->is_name_Mode()){
		this->rebuildTreeData_name_inc();
		this->refreshTreeUi();
	}

}
/*-------------------------------------------------
		资源数据 - 重建数据_ID分支（私有）
*/
void P_FlexiblePageTree::rebuildTreeData_id_inc(){

	// > 重刷所有树枝
	for (int i = 0; i < this->m_branchItem.count(); i++){
		this->m_branchItem.at(i)->takeChildren();
	}
	this->m_branchItem.clear();

	// > 数据准备
	int all_count = this->m_source_ObjectSortController->getDataCount();
	this->m_config->set_id_MaxCount(all_count);

	// > 添加树枝
	int page_count = this->m_config->get_id_PageCount();
	for (int i = 0; i < page_count; i++){
		int bottom = this->m_config->get_id_Bottom(i);
		int top = this->m_config->get_id_Top(i);
		if (bottom == -1){ break; }
		if (top == -1){ break; }

		I_FPT_Branch* branch_item = this->createFCTBranch();
		branch_item->setId(i);							//序号
		branch_item->setBranch_id_Index(i);				//ID分支 - 当前页索引的位置
		branch_item->setBranch_id_Bottom(bottom);		//ID分支 - 底
		branch_item->setBranch_id_Top(top);				//ID分支 - 顶
		this->m_branchItem.append(branch_item);
	}

	// > 重刷所有叶子
	this->m_leafItem.clear();
	for (int i = 0; i < this->m_branchItem.count(); i++){
		I_FPT_Branch* branch_item = this->m_branchItem.at(i);
		int page_index = branch_item->getBranch_id_Index();	//当前页索引的位置
		QList<int> index_list = this->m_source_ObjectSortController->get_IdInc_PageIndexList(page_index, this->m_config->pagePerNum);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > 在叶子中填入数据
			I_FPT_Leaf* leaf = this->createFCTLeaf();
			leaf->setId(data->id);					//标识
			leaf->setName(data->name);				//名称
			leaf->setType(data->type);				//分类
			this->m_leafItem.append(leaf);

			I_FPT_Branch* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}

}
/*-------------------------------------------------
		资源数据 - 重建数据_名称分支（私有）
*/
void P_FlexiblePageTree::rebuildTreeData_name_inc(){

	// > 重刷所有树枝
	for (int i = 0; i < this->m_branchItem.count(); i++){
		this->m_branchItem.at(i)->takeChildren();
	}
	this->m_branchItem.clear();

	// > 数据准备
	QStringList page_name_list = this->m_config->get_name_PageNameList();
	QStringList page_symbol_list = this->m_config->get_name_PageSymbolList();

	// > 添加页
	for (int i = 0; i < page_name_list.count(); i++){
		QString page_name = page_name_list.at(i);
		QString page_symbol = page_symbol_list.at(i);

		I_FPT_Branch* branch_item = this->createFCTBranch();
		branch_item->setId(i);									//序号
		branch_item->setBranch_name_Index(i);					//名称分支 - 当前页索引的位置
		branch_item->setBranch_name_Showing(page_name);			//名称分支 - 显示的名称
		branch_item->setBranch_name_Symbol(page_symbol);		//名称分支 - 名称查找字符的标志
		this->m_branchItem.append(branch_item);
	}

	// > 重刷所有叶子
	this->m_leafItem.clear();
	for (int i = 0; i < page_symbol_list.count(); i++){
		QString symbol = page_symbol_list.at(i);
		QList<int> index_list;
		if (symbol == "空名称"){
			index_list = this->m_source_ObjectSortController->get_Name_IndexListByEmptyName();

		}else if (symbol == "其他符号"){
			index_list = this->m_source_ObjectSortController->get_Name_IndexListByNonAlphabetic();

		}else{
			index_list = this->m_source_ObjectSortController->get_Name_IndexListByCharacter(symbol.at(0));
		}
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > 在叶子中填入数据
			I_FPT_Leaf* leaf = this->createFCTLeaf();
			leaf->setId(data->id);					//标识
			leaf->setName(data->name);				//名称
			leaf->setType(data->type);				//分类
			leaf->setLeaf_name_Symbol(symbol);		//名称分支 - 首字母标识
			this->m_leafItem.append(leaf);

			I_FPT_Branch* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}
}

/*-------------------------------------------------
		窗口管理 - 窗口数量+1
*/
void P_FlexiblePageTree::windowLock_incOne(){
	this->m_config_windowLockNum += 1;
	if (this->m_config_windowLockNum == 1){
		emit signal_anyWindowLocked(true);
	}
}
/*-------------------------------------------------
		窗口管理 - 窗口数量-1
*/
void P_FlexiblePageTree::windowLock_decOne(){
	this->m_config_windowLockNum -= 1;
	if (this->m_config_windowLockNum <= 0){
		this->m_config_windowLockNum = 0;
		emit signal_anyWindowLocked(false);
	}
}