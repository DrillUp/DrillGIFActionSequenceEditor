#include "stdafx.h"
#include "p_FlexiblePageTree.h"

#include "private/w_FCTConfig.h"

#include "Source/Utils/common/TTool.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/c_ObjectSortData.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/p_ObjectSortController.h"
#include "Source/Utils/manager/chineseManager/S_ChineseManager.h"

/*
-----==========================================================-----
		类：		灵活分类树.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		能够显示一堆数据，并且将这些数据分类或转移到不同的树枝中，便于查询。
					注意，该树被装饰后，需要对该控件块进行交互，不要对树进行直接交互。
					【包含id分支、名称分支，不含自定义分支】

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
						x-> 自定义分支（该类不含自定义分支功能）
							x-> 树枝右键可以添加分类
							x-> 叶子右键可以添加分类/移动到分类（可去除所有分类）
							x-> 同一个对象可以在多个分类中出现（功能与后期对象选择器排序 冲突）
					-> 树配置
						-> 设置行高
						-> 设置叶子显示文本

		说明：		考虑到树叶可能会非常多，所以只有在rebuildTreeData时，才会重新排序。
					如果只是简单修改树ui，添加分类，是不会刷新的。
					
		使用方法：
				> 初始化
					this->m_p_FlexiblePageTree = new P_FlexiblePageTree(ui.treeWidget);
					this->m_p_FlexiblePageTree->setData(obj);		//（存储的数据需要在load前完成赋值）
					this->m_p_FlexiblePageTree->loadSource(data_list, "id", "name", "type");
				> 设置默认值
					QJsonObject obj_config = QJsonObject();		//由于交互只能通过QJsonObject，所以，默认值也需要对应到object中。
					QJsonObject obj_tree = QJsonObject();
					obj_tree.insert("rowHeight", 32);
					obj_config.insert("sortMode", "自定义分支（按id递增排序）");
					obj_config.insert("FCTConfig", obj_tree);
					this->m_p_tree->setData(obj_config);
					
-----==========================================================-----
*/
P_FlexiblePageTree::P_FlexiblePageTree(QTreeWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----参数初始化
	this->m_tree = parent;								//树对象
	this->m_treeStyle = this->m_tree->styleSheet();		//树默认样式
	this->m_slotBlock = false;							//槽阻塞
	this->m_leafItem = QList<I_FCTLeaf*>();				//叶子列表
	this->m_branchItem = QList<I_FCTBranch*>();			//树枝列表
	this->m_leafOuterControlEnabled = true;				//叶子编辑开关

	// > 数据
	this->local_treeData = QJsonObject();
	this->m_sortMode = "ID分支（按id递增排序）";
	this->m_sortModeList = QStringList() << "ID分支（按id递增排序）" << "名称分支（按名称递增排序）";
	this->m_config = C_FCTConfig();

	// > 树事件
	this->m_selectionSignalBlock_Root = false;
	this->m_mainMenu = nullptr;
	this->m_modeMenu = nullptr;
	this->m_last_item = nullptr;
	this->m_last_leaf = nullptr;
	this->m_menuIconSrcPath = ":/DrillGIFActionSequenceEditor/Resources/icons";

	// > 资源数据
	this->m_source_ObjectSortController = new P_ObjectSortController();
	this->m_source_list = QList<C_ObjectSortData>();

	//-----------------------------------
	//----ui初始化
	this->m_tree->setContextMenuPolicy(Qt::CustomContextMenu);								//激活右键
	this->m_tree->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);	//多选叶子

	connect(this->m_tree, &QTreeWidget::itemClicked, this, &P_FlexiblePageTree::sltItemClicked);
	connect(this->m_tree, &QTreeWidget::itemDoubleClicked, this, &P_FlexiblePageTree::sltItemDoubleClicked);
	connect(this->m_tree, &QTreeWidget::customContextMenuRequested, this, &P_FlexiblePageTree::sltItemRightClicked);
	connect(this->m_tree, &QTreeWidget::itemSelectionChanged, this, &P_FlexiblePageTree::sltItemSelectionChanged);

}
P_FlexiblePageTree::~P_FlexiblePageTree(){
}


/*-------------------------------------------------
		控件 - 刷新树
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
		I_FCTBranch* branch = this->m_branchItem.at(i);
		branch->refreshItemSelf();
	}
	// > 刷新叶子数据
	for (int i = 0; i < this->m_leafItem.count(); i++){
		I_FCTLeaf* leaf = this->m_leafItem.at(i);
		leaf->refreshItemSelf();
	}

	// > 特殊分支操作
	this->refreshTreeUi_special();

	// > 行高刷新
	this->m_tree->setStyleSheet(this->m_treeStyle + "\n QTreeView::item { height: " + QString::number(this->m_config.rowHeight) + "px;}");
	
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
		控件 - 刷新树_分支操作
*/
void P_FlexiblePageTree::refreshTreeUi_special() {

	// > 叶子位置转移
	if (this->m_sortMode == "ID分支（按id递增排序）"){ this->refreshTreeUi_id_inc(); }
	if (this->m_sortMode == "名称分支（按名称递增排序）"){ this->refreshTreeUi_name_inc(); }

}
/*-------------------------------------------------
		控件 - 刷新树_ID分支
*/
void P_FlexiblePageTree::refreshTreeUi_id_inc() {
	
	// > 无树枝变化（在不重刷的情况下，id是不可能会变的）

}
/*-------------------------------------------------
		控件 - 刷新树_名称分支
*/
void P_FlexiblePageTree::refreshTreeUi_name_inc() {
	
	// > 名称修改后，修改的叶子可能会跑到新树枝下
	for (int i = 0; i < this->m_leafItem.count(); i++){
		I_FCTLeaf* leaf = this->m_leafItem.at(i);
		I_FCTBranch* parent_branch = dynamic_cast<I_FCTBranch*>(leaf->parent());

		QString branch_symbol = parent_branch->getBranch_name_Symbol();
		QString leaf_symbol = leaf->getLeaf_name_Symbol();
		if (branch_symbol != leaf_symbol){
			leaf->parent()->removeChild(leaf);

			I_FCTBranch* n_branch = this->getBranchByNameSymbol(leaf_symbol);
			if (n_branch == nullptr){ continue; }
			n_branch->addChild(leaf);
		}
	}

}
/*-------------------------------------------------
		控件 - 清理全部
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
}


/*-------------------------------------------------
		叶子 - 【外部修改】叶子名称
*/
void P_FlexiblePageTree::outerModifyLeafName(int id, QString name){
	I_FCTLeaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return; }
	
	// > 修改叶子
	leaf->setName(name);
	// > 修改叶子 - 名称分支时，叶子父类可能转移
	if (this->m_sortMode == "名称分支（按名称递增排序）"){
		if (name == ""){
			leaf->setLeaf_name_Symbol("空字符");
		}else{
			leaf->setLeaf_name_Symbol(S_ChineseManager::getInstance()->getChineseFirstSpell(name));	//首字母
		}
	}

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
	I_FCTLeaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return; }

	// > 修改叶子
	leaf->setType(type);

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
		叶子 - 获取 - 获取对象（根据ID）
*/
I_FCTLeaf* P_FlexiblePageTree::getLeafById(int id){
	for (int i = 0; i < this->m_leafItem.count(); i++){
		int l_id = this->m_leafItem.at(i)->getId();
		if (l_id == id){
			return this->m_leafItem.at(i);
		}
	}
	return nullptr;
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
I_FCTLeaf* P_FlexiblePageTree::getLeafByName(QString name){
	for (int i = 0; i < this->m_leafItem.count(); i++){
		if (this->m_leafItem.at(i)->getName() == name){
			return this->m_leafItem.at(i);
		}
	}
	return nullptr;
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
		叶子接口 - 开关（默认开启）
*/
void P_FlexiblePageTree::setLeafOuterControlEnabled(bool enabled){
	this->m_leafOuterControlEnabled = enabled;
}
/*-------------------------------------------------
		叶子接口 - 复制按下
*/
void P_FlexiblePageTree::menuCopyLeafInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int id = cur_action->data().value<int>();
	emit menuCopyLeafTriggered(id);
}
/*-------------------------------------------------
		叶子接口 - 粘贴按下
*/
void P_FlexiblePageTree::menuPasteLeafInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int id = cur_action->data().value<int>();
	emit menuPasteLeafTriggered(id);
}
/*-------------------------------------------------
		叶子接口 - 清空按下
*/
void P_FlexiblePageTree::menuClearLeafInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	int id = cur_action->data().value<int>();
	emit menuClearLeafTriggered(id);
}


/*-------------------------------------------------
		树枝 - 获取树枝（名称分支专用）
*/
I_FCTBranch* P_FlexiblePageTree::getBranchByNameSymbol(QString symbol){
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
		数据 - 设置数据
*/
QJsonObject P_FlexiblePageTree::getData(){
	this->putUiToData();
	return local_treeData;
}
/*-------------------------------------------------
		数据 - 取出
*/
void P_FlexiblePageTree::setData(QJsonObject obj){
	this->local_treeData = obj;
	this->putDataToUi();
}
/*-------------------------------------------------
		数据 - 本地数据 -> ui数据
*/
void P_FlexiblePageTree::putDataToUi(){

	if (this->local_treeData.value("sortMode").isUndefined() == false){
		this->m_sortMode = this->local_treeData.value("sortMode").toString();
	}
	if (this->local_treeData.value("FCTConfig").isUndefined() == false){
		this->m_config.setJsonObject(this->local_treeData.value("FCTConfig").toObject());
	}

}
/*-------------------------------------------------
		数据 - ui数据 -> 本地数据
*/
void P_FlexiblePageTree::putUiToData(){
	this->local_treeData = QJsonObject();

	this->local_treeData.insert("FCTConfig", this->m_config.getJsonObject());
	this->local_treeData.insert("sortMode", this->m_sortMode);
}


/*-------------------------------------------------
		模式 - 切换模式
*/
void  P_FlexiblePageTree::changeSortMode(QString sortMode){
	if (this->m_sortMode == sortMode){ return; }
	this->m_sortMode = sortMode;
	this->m_config.sortType = sortMode;
	this->rebuildTreeData();
}
void  P_FlexiblePageTree::changeSortModeInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString sortMode = cur_action->data().value<QString>();

	if (this->m_sortMode == sortMode){ return; }
	this->m_sortMode = sortMode;
	this->m_config.sortType = sortMode;
	this->rebuildTreeData();
}
/*-------------------------------------------------
		模式 - 获取模式
*/
QString P_FlexiblePageTree::getCurSortMode(){
	return this->m_sortMode;
}


/*-------------------------------------------------
		树设置 - 设置参数
*/
void P_FlexiblePageTree::setConfigParam(QJsonObject config){
	this->m_config.setJsonObject(config);
	this->refreshTreeUi();
}
/*-------------------------------------------------
		树设置 - 取出参数
*/
QJsonObject P_FlexiblePageTree::getConfigParam(){
	return this->m_config.getJsonObject();
}
/*-------------------------------------------------
		树设置 - 编辑窗口
*/
void P_FlexiblePageTree::openConfigParamWindow(){
	W_FCTConfig d(this->m_tree);
	d.setDataInModifyMode(this->m_config);
	d.setModeType(this->m_sortMode, this->m_sortModeList);
	if (d.exec() == QDialog::Accepted){
		this->m_config = d.getData() ;
		
		// > id分支时，必然重刷
		if (this->m_config.sortType == "ID分支（按id递增排序）" ){ 
			this->m_sortMode = this->m_config.sortType;
			this->rebuildTreeData(); 
		}
		
		// > 一般分支
		if (this->m_config.sortType != m_sortMode){
			this->changeSortMode(this->m_config.sortType);
		}else{
			this->refreshTreeUi();
		}
	}
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

	if (this->m_last_item != item){
		this->m_last_item = item;
		emit currentItemChanged(item);
	}
	if ( this->isLeaf(item) && 
		 this->m_last_leaf != item){
		this->m_last_leaf = item;
		I_FCTLeaf* leaf = dynamic_cast<I_FCTLeaf*>(item);	
		emit currentLeafChanged(item, leaf->getId(),leaf->getName());
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
		I_FCTLeaf* leaf = dynamic_cast<I_FCTLeaf*>(selected_items.at(0));

		action = new QAction("复制", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Copy.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuCopyLeafInAction);
		this->m_mainMenu->addAction(action);
		
		action = new QAction("粘贴", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Paste.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuPasteLeafInAction);
		this->m_mainMenu->addAction(action);

		action = new QAction("清空", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Clear.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuClearLeafInAction);
		this->m_mainMenu->addAction(action);

		this->m_mainMenu->addSeparator();
	}

	/*------------------------任意情况------------------------*/
	action = new QAction("展开全部", this);
	action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Unfold.png"));
	connect(action, &QAction::triggered, this->m_tree, &QTreeWidget::expandAll);
	this->m_mainMenu->addAction(action);

	action = new QAction("收起全部", this);
	action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Fold.png"));
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
	action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Refresh.png"));
	connect(action, &QAction::triggered, this, &P_FlexiblePageTree::rebuildTreeData);
	this->m_mainMenu->addAction(action);

	action = new QAction("树设置...", this);
	action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Setting.png"));
	connect(action, &QAction::triggered, this, &P_FlexiblePageTree::openConfigParamWindow);
	this->m_mainMenu->addAction(action);

}
/*-------------------------------------------------
		右键菜单 - 二级菜单（模式）
*/
void P_FlexiblePageTree::drawMenuMode(){
	QAction *action;
	this->m_modeMenu->setTitle("分支模式");

	for (int i = 0; i < this->m_sortModeList.count(); i++){
		QString mode_name = this->m_sortModeList.at(i);
		action = new QAction(mode_name, this);
		action->setData(mode_name);
		if (this->getCurSortMode() == mode_name){
			action->setCheckable(true);
			action->setChecked(true);
		}
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::changeSortModeInAction);
		this->m_modeMenu->addAction(action);
	}

}


/*-------------------------------------------------
		资源数据 - 载入
*/
void P_FlexiblePageTree::loadSource(QList<QJsonObject> obj_list) {

	// > 全清
	this->clearAll();

	// > 解析数据
	for (int i = 0; i < obj_list.count(); i++){
		C_ObjectSortData data = C_ObjectSortData();
		data.setJsonObject(obj_list.at(i));
		if (data.isNull()){ continue; }
		this->m_source_list.append(data);
	}
	// > 解析后必须初始化一次（子类树要用）
	this->m_source_ObjectSortController->setData_FromSortData(this->m_source_list);

	// > 初始化树数据
	this->putDataToUi();

	// > 重建全部数据
	this->rebuildTreeData();
}
void P_FlexiblePageTree::loadSource(QList<QJsonObject> obj_list, QString id_symbol, QString name_symbol, QString type_symbol) {

	// > 全清
	this->clearAll();

	// > 解析数据
	for (int i = 0; i < obj_list.count(); i++){
		C_ObjectSortData data = C_ObjectSortData();
		data.setJsonObject(obj_list.at(i), id_symbol, name_symbol, type_symbol);	//注意symbol不存
		if (data.isNull()){ continue; }
		this->m_source_list.append(data);
	}
	// > 解析后必须初始化一次（子类树要用）
	this->m_source_ObjectSortController->setData_FromSortData(this->m_source_list);

	// > 初始化树数据
	this->putDataToUi();

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
	I_FCTLeaf* leaf = nullptr;
	for (int i = 0; i < item_list.count(); i++){
		QTreeWidgetItem* item = item_list.at(i);
		if (this->isLeaf(item)){
			leaf = dynamic_cast<I_FCTLeaf*>(item);;
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

	if (this->m_sortMode == ""){
		QMessageBox::warning(nullptr, "错误", "树控件中出现了有误的分支模式设置，已归为默认设置。", QMessageBox::Yes);
		this->m_sortMode = "ID分支（按id递增排序）";
	}
	if (this->m_sortMode == "ID分支（按id递增排序）"){
		this->rebuildTreeData_id_inc();
		this->refreshTreeUi();
	}
	if (this->m_sortMode == "名称分支（按名称递增排序）"){
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
	int per_num = this->m_config.pagePerNum;
	int page_count = qCeil((double)all_count / per_num);

	// > 添加树枝
	for (int i = 0; i < page_count; i++){
		int bottom = i * per_num + 1;
		int top = (i + 1) * per_num;
		if (top > all_count){
			top = all_count;
		}
		QString page_name = "";
		page_name = "第" + S_ChineseManager::getInstance()->toChineseNumLower(i + 1) + "页 ";

		// > id编号
		if (this->m_config.zeroFill){
			page_name += TTool::_zeroFill_(bottom, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1()))
				+ "至" + TTool::_zeroFill_(top, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1()));
		}else{
			page_name += QString::number(bottom) + "至" + QString::number(top);
		}

		I_FCTBranch* branch_item = new I_FCTBranch();
		branch_item->setId(i);							//序号
		branch_item->setBranch_id_Index(i);				//ID分支 - 当前页索引的位置
		branch_item->setBranch_id_Bottom(bottom);		//ID分支 - 底
		branch_item->setBranch_id_Top(top);				//ID分支 - 顶
		branch_item->setBranch_id_Name(page_name);		//ID分支 - 名称
		this->m_branchItem.append(branch_item);
	}

	// > 重刷所有叶子
	this->m_leafItem.clear();
	for (int i = 0; i < this->m_branchItem.count(); i++){
		I_FCTBranch* branch_item = this->m_branchItem.at(i);
		int page_index = branch_item->getBranch_id_Index();	//当前页索引的位置
		QList<int> index_list = this->m_source_ObjectSortController->get_IdInc_PageIndexList(page_index, this->m_config.pagePerNum);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > 在叶子中填入数据
			I_FCTLeaf* leaf = new I_FCTLeaf(&this->m_config);
			leaf->setId(data->id);					//标识
			leaf->setName(data->name);				//名称
			leaf->setType(data->type);				//分类
			this->m_leafItem.append(leaf);

			I_FCTBranch* p_item = this->m_branchItem.at(i);
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
	QStringList page_name_list = QStringList() << "- A -" << "- B -" << "- C -" << "- D -" << "- E -" << "- F -" << "- G -" << "- H -" << "- I -" << "- J -" << "- K -" << "- L -" << "- M -" << "- N -" << "- O -" << "- P -" << "- Q -" << "- R -" << "- S -" << "- T -" << "- U -" << "- V -" << "- W -" << "- X -" << "- Y -" << "- Z -" << "生僻字";
	QStringList page_symbol_list = QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z" << "@";
	page_name_list << "空名称";
	page_symbol_list << "空名称";
	// > 添加页
	for (int i = 0; i < page_name_list.count(); i++){
		QString page_name = page_name_list.at(i);
		QString page_symbol = page_symbol_list.at(i);

		I_FCTBranch* branch_item = new I_FCTBranch();
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
		QList<int> index_list = QList<int>();
		if (symbol == "空名称"){
			index_list = this->m_source_ObjectSortController->get_Name_IndexListByEmptyName();
		}else{
			index_list = this->m_source_ObjectSortController->get_Name_IndexListByCharacter(symbol.at(0));
		}
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > 在叶子中填入数据
			I_FCTLeaf* leaf = new I_FCTLeaf(&this->m_config);
			leaf->setId(data->id);					//标识
			leaf->setName(data->name);				//名称
			leaf->setType(data->type);				//分类
			leaf->setLeaf_name_Symbol(symbol);		//名称分支 - 首字母标识
			this->m_leafItem.append(leaf);

			I_FCTBranch* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}
}