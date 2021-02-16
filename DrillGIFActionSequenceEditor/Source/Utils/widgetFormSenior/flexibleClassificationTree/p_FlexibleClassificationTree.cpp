#include "stdafx.h"
#include "p_FlexibleClassificationTree.h"

#include "private/w_FCTConfig.h"
#include "private/w_FCTClassify.h"
#include "private/w_FCTClassifySelector.h"

#include "Source/Utils/common/TTool.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/c_ObjectSortData.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/p_ObjectSortController.h"
#include "Source/Utils/manager/chineseManager/S_ChineseManager.h"

/*
-----==========================================================-----
		类：		灵活类型树（含自定义分支）.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		能够显示一堆数据，并且将这些数据类型或转移到不同的树枝中，便于查询。
					注意，该树被装饰后，需要对该控件块进行交互，不要对树进行直接交互。

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
						-> 自定义分支（该类不含自定义分支功能）
							-> 树枝右键可以添加分类
							-> 叶子右键可以添加分类/移动到分类（可去除所有分类）
							-> 同一个对象可以在多个分类中出现（功能与后期对象选择器排序 冲突）
					-> 树配置
						-> 设置行高
						-> 设置叶子显示文本

		说明：		由于这棵树直接包裹了一个container数据存储结构，
					所以里面会夹杂Container、WindowManager的处理模式。
					
		使用方法：
				>初始化
					this->m_p_FlexibleClassificationTree = new P_FlexibleClassificationTree(ui.treeWidget_2);
					this->m_p_FlexibleClassificationTree->setData(obj);		//（存储的数据需要在load前完成赋值）
					this->m_p_FlexibleClassificationTree->loadSource(data_list, "id", "name", "type");

-----==========================================================-----
*/
P_FlexibleClassificationTree::P_FlexibleClassificationTree(QTreeWidget *parent)
	: P_FlexiblePageTree(parent)
{

	//-----------------------------------
	//----ui初始化

	// > 数据
	this->m_sortModeList << "自定义分支（按id递增排序）" << "自定义分支（按名称递增排序）";
	this->m_classifyData = QList<C_FCTClassify>();

}
P_FlexibleClassificationTree::~P_FlexibleClassificationTree(){
}


/*-------------------------------------------------
		控件 - 刷新树
*/
void P_FlexibleClassificationTree::refreshTreeUi_special() {
	P_FlexiblePageTree::refreshTreeUi_special();

	// > 叶子位置转移
	if (this->m_sortMode == "自定义分支（按id递增排序）"){ this->refreshTreeUi_classify_idInc(); }
	if (this->m_sortMode == "自定义分支（按名称递增排序）"){ this->refreshTreeUi_classify_nameInc(); }
}
/*-------------------------------------------------
		控件 - 刷新树_自定义分支_id递增
*/
void P_FlexibleClassificationTree::refreshTreeUi_classify_idInc() {

	// > 类型修改后，修改的叶子可能会跑到新树枝下
	for (int i = 0; i < this->m_leafItem.count(); i++){
		I_FCTLeaf* leaf = this->m_leafItem.at(i);
		I_FCTBranch* parent_branch = dynamic_cast<I_FCTBranch*>(leaf->parent());

		QString classify_name = leaf->getType();
		QString p_classify_name = parent_branch->getBranch_type_Name();
		if (classify_name != p_classify_name){
			leaf->parent()->removeChild(leaf);

			I_FCTBranch* n_branch = this->getBranchByTypeName(classify_name);
			if (n_branch == nullptr){ continue; }
			n_branch->addChild(leaf);
		}
	}

}
/*-------------------------------------------------
		控件 - 刷新树_自定义分支_名称递增
*/
void P_FlexibleClassificationTree::refreshTreeUi_classify_nameInc() {

	// > 类型修改后，修改的叶子可能会跑到新树枝下
	this->refreshTreeUi_classify_idInc();	//（与id一模一样）

}
/*-------------------------------------------------
		控件 - 清理全部
*/
void P_FlexibleClassificationTree::clearAll(){
	P_FlexiblePageTree::clearAll();

	// > 数据清理
	this->m_classifyData.clear();
}


/*-------------------------------------------------
		叶子 - 【外部修改】叶子名称
*/
void P_FlexibleClassificationTree::outerModifyLeafName(int id, QString name){
	P_FlexiblePageTree::outerModifyLeafName(id, name);

	//（无变化）
}
/*-------------------------------------------------
		叶子 - 【外部修改】叶子类型
*/
void P_FlexibleClassificationTree::outerModifyLeafType(int id, QString name){
	P_FlexiblePageTree::outerModifyLeafType(id, name);

	// （无变化）
}
/*-------------------------------------------------
		叶子 - 获取 - 根据类型获取
*/
QList<I_FCTLeaf*> P_FlexibleClassificationTree::getLeafByType(QString type){
	QList<I_FCTLeaf*> result_list = QList<I_FCTLeaf*>();
	for (int i = 0; i < this->m_leafItem.count(); i++){
		if (this->m_leafItem.at(i)->getType() == type){
			result_list.append( this->m_leafItem.at(i) );
		}
	}
	return result_list;
}
/*-------------------------------------------------
		树枝 - 获取树枝（自定义分支专用）
*/
I_FCTBranch* P_FlexibleClassificationTree::getBranchByTypeName(QString classify_name){
	for (int i = 0; i < this->m_branchItem.count(); i++){
		if (this->m_branchItem.at(i)->getBranch_type_Name() == classify_name){
			return this->m_branchItem.at(i);
		}
	}
	return nullptr;
}



/*-------------------------------------------------
		数据 - 本地数据 -> ui数据
*/
void P_FlexibleClassificationTree::putDataToUi(){
	P_FlexiblePageTree::putDataToUi();

	// > 类型 
	this->m_classifyData.clear();

	// > 类型 - 存储的数据
	if (this->local_treeData.value("FCTClassifyList").isUndefined() == false){
		QJsonArray arr = this->local_treeData.value("FCTClassifyList").toArray();
		QList<C_FCTClassify> classify_list = QList<C_FCTClassify>();
		for (int i = 0; i < arr.count(); i++){
			C_FCTClassify c_c = C_FCTClassify();
			c_c.setJsonObject(arr.at(i).toObject());
			classify_list.append(c_c);
		}
		this->m_classifyData.append(classify_list);
	}

	// > 类型 - 检查新的类型
	QStringList other_classify_list = this->m_source_ObjectSortController->get_Type_DistinguishedList();
	for (int i = 0; i < other_classify_list.count(); i++){
		QString classify_name = other_classify_list.at(i);
		if (classify_name != "" && this->hasClassify(classify_name) == false){

			C_FCTClassify c_c = C_FCTClassify();	//（这里的类型来自外部的修改，而非树的修改）
			c_c.name = classify_name;
			c_c.description = "";
			this->m_classifyData.append(c_c);
		}
	}

	// > 类型 - 未分类（特殊添加）
	this->m_classifyData.append(C_FCTClassify());

}
/*-------------------------------------------------
		数据 - ui数据 -> 本地数据
*/
void P_FlexibleClassificationTree::putUiToData(){
	P_FlexiblePageTree::putUiToData();

	// > 类型数据（自定义分支）
	QJsonArray arr = QJsonArray();
	for (int i = 0; i < this->m_classifyData.count(); i++){
		C_FCTClassify c_c = this->m_classifyData.at(i);
		if (c_c.isNull() == false){		//（排除"未分类"）
			arr.append(c_c.getJsonObject());
		}
	}
	this->local_treeData.insert("FCTClassifyList", arr);

}


/*-------------------------------------------------
		类型 - 获取 - 全部类型
*/
QStringList P_FlexibleClassificationTree::getAllClassify(){
	QStringList result_list = QStringList();
	for (int i = 0; i < this->m_classifyData.count(); i++){
		result_list.append(this->m_classifyData.at(i).name);
	}
	return result_list;
}
/*-------------------------------------------------
		类型 - 获取 - 判断类型存在
*/
int P_FlexibleClassificationTree::indexOfClassify(QString classify_name){
	for (int i = 0; i < this->m_classifyData.count(); i++){
		if (this->m_classifyData.at(i).name == classify_name){
			return i;
		}
	}
	return -1;
}
/*-------------------------------------------------
		类型 - 获取 - 判断类型存在
*/
bool P_FlexibleClassificationTree::hasClassify(QString classify_name){
	for (int i = 0; i < this->m_classifyData.count(); i++){
		if (this->m_classifyData.at(i).name == classify_name){
			return true;
		}
	}
	return false;
}
/*-------------------------------------------------
		类型 - 当前为自定义分支模式
*/
bool P_FlexibleClassificationTree::isClassifyMode(){
	return this->m_sortMode.contains("自定义分支");
}
/*-------------------------------------------------
		类型 - 添加
*/
void P_FlexibleClassificationTree::addClassify(C_FCTClassify classify){
	if (this->hasClassify(classify.name) == true){ return; }

	// > 添加对象
	int index = this->m_classifyData.count() - 1;
	this->m_classifyData.insert(index, classify);

	// > 树枝变化
	I_FCTBranch* branch = new I_FCTBranch();
	branch->setId(index);										//序号
	branch->setBranch_type_Name(classify.name);					//自定义分支 - 类型名称
	branch->setBranch_type_Description(classify.description);	//自定义分支 - 类型描述
	this->m_branchItem.insert(index, branch);

	this->refreshTreeUi();
}
/*-------------------------------------------------
		类型 - 修改
*/
void P_FlexibleClassificationTree::modifyClassify(QString classify_name, C_FCTClassify classify){
	if (this->hasClassify(classify_name) == false){ return; }

	// > 修改对象
	for (int i = 0; i < this->m_classifyData.count(); i++){
		C_FCTClassify c_c = this->m_classifyData.at(i);
		if (c_c.name == classify_name){
			this->m_classifyData.replace(i, classify);
			break;
		}
	}

	// > 树枝变化
	I_FCTBranch* branch = this->getBranchByTypeName(classify_name);
	branch->setBranch_type_Name(classify.name);					//自定义分支 - 类型名称
	branch->setBranch_type_Description(classify.description);	//自定义分支 - 类型描述

	// > 叶子变化
	QList<I_FCTLeaf*> leaf_list = this->getLeafByType(classify_name);
	for (int i = 0; i < leaf_list.count(); i++){
		I_FCTLeaf* leaf = leaf_list.at(i);
		this->innerModifyLeafType(leaf->getId(), classify.name);
	}

	this->refreshTreeUi();
}
/*-------------------------------------------------
		类型 - 去除
*/
void P_FlexibleClassificationTree::removeClassify(QString classify_name){
	if (this->hasClassify(classify_name) == false){ return; }
	if ( classify_name == ""){ return; }

	// > 去除对象
	for (int i = 0; i < this->m_classifyData.count(); i++){
		C_FCTClassify c_c = this->m_classifyData.at(i);
		if (c_c.name == classify_name){
			this->m_classifyData.removeAt(i);
			break;
		}
	}

	// > 树枝变化
	this->m_branchItem.removeOne(this->getBranchByTypeName(classify_name));

	// > 叶子变化
	QList<I_FCTLeaf*> leaf_list = this->getLeafByType(classify_name);
	for (int i = 0; i < leaf_list.count(); i++){
		I_FCTLeaf* leaf = leaf_list.at(i);
		this->innerModifyLeafType(leaf->getId(), "");
	}

	this->refreshTreeUi();
}
/*-------------------------------------------------
		类型 - 添加（action）
*/
void P_FlexibleClassificationTree::addClassifyInAction(){
	W_FCTClassify d(this->m_tree);
	d.setDataInAddMode( &this->m_classifyData );
	if (d.exec() == QDialog::Accepted){
		this->addClassify(d.getData());
	}
}
/*-------------------------------------------------
		类型 - 修改（action）
*/
void P_FlexibleClassificationTree::modifyClassifyInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString classify_name = cur_action->data().value<QString>();

	C_FCTClassify c_c = C_FCTClassify();
	for (int i = 0; i < this->m_classifyData.count(); i++){
		C_FCTClassify temp_c = this->m_classifyData.at(i);
		if (temp_c.name == classify_name){
			c_c = temp_c;
		}
	}

	W_FCTClassify d(this->m_tree);
	d.setDataInModifyMode(c_c, &this->m_classifyData);
	if (d.exec() == QDialog::Accepted){
		this->modifyClassify(c_c.name, d.getData());
	}
}
/*-------------------------------------------------
		类型 - 去除（action）
*/
void P_FlexibleClassificationTree::removeClassifyInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString classify_name = cur_action->data().value<QString>();

	this->removeClassify(classify_name);
}
/*-------------------------------------------------
		类型 - 去除（action）
*/
void P_FlexibleClassificationTree::removeClassifyListInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QStringList classify_list = cur_action->data().value<QStringList>();

	for (int i = 0; i < classify_list.count(); i++){
		this->removeClassify(classify_list.at(i));
	}
}
/*-------------------------------------------------
		类型 - 上移
*/
void P_FlexibleClassificationTree::moveUpInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString classify_name = cur_action->data().value<QString>();

	// > 数据变化
	int index = this->indexOfClassify(classify_name);
	if (index == -1){ return; }
	if (index == 0){ return; }
	if (index == this->m_classifyData.count() - 1){ return; }
	this->m_classifyData.swap(index, index - 1);

	// > 树枝变化
	this->m_branchItem.swap(index, index - 1);

	this->refreshTreeUi();
}
/*-------------------------------------------------
		类型 - 下移
*/
void P_FlexibleClassificationTree::moveDownInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString classify_name = cur_action->data().value<QString>();

	// > 数据变化
	int index = this->indexOfClassify(classify_name);
	if (index == -1){ return; }
	if (index >= this->m_classifyData.count() - 2){ return; }
	this->m_classifyData.swap(index, index + 1);

	// > 树枝变化
	this->m_branchItem.swap(index, index + 1);

	this->refreshTreeUi();

}


/*-------------------------------------------------
		修改的数据 - 【内部修改】叶子的类型
*/
void P_FlexibleClassificationTree::innerModifyLeafType(int id, QString type){
	I_FCTLeaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return; }

	// > 叶子变化
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

	// > 记录改变
	C_ObjectSortData changeData = C_ObjectSortData();
	changeData.id = leaf->getId();
	changeData.type = leaf->getType();
	this->appendChangedSource(changeData);

}
void P_FlexibleClassificationTree::innerModifyLeafTypeInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QStringList leaf_idList = cur_action->data().value<QStringList>();

	// > 获取选中的叶子
	if (leaf_idList.count() == 0){ return; }
	QString id_str = leaf_idList.at(0);
	QString classify = this->getLeafById(id_str.toInt())->getType();

	// > 选择类型
	W_FCTClassifySelector d(this->m_tree);
	d.setDataInModifyMode(classify, &this->m_classifyData);
	connect(&d, &W_FCTClassifySelector::classifyAdded, this, &P_FlexibleClassificationTree::addClassify);
	if (d.exec() == QDialog::Accepted){
		C_FCTClassify c_c = d.getSelectedData();
		
		// > 批量修改
		for (int i = 0; i < leaf_idList.count(); i++){
			this->innerModifyLeafType(leaf_idList.at(i).toInt(), c_c.name);
		}
		this->refreshTreeUi();
	}

}
/*-------------------------------------------------
		修改的数据 - 记录修改（只改type参数）
*/
void P_FlexibleClassificationTree::appendChangedSource(C_ObjectSortData data){
	int index = this->m_changedSource.indexOf(data);
	if (index == -1){
		this->m_changedSource.append(data);
	}else{
		this->m_changedSource.replace(index, data);
	}
}
/*-------------------------------------------------
		修改的数据 - 修改的数据（只改type参数）
*/
QList<C_ObjectSortData> P_FlexibleClassificationTree::getChangedSource(){
	return this->m_changedSource;
}


/*-------------------------------------------------
		右键菜单 - 一级菜单
*/
void P_FlexibleClassificationTree::drawMenuMain(){
	QAction *action;
	QList<QTreeWidgetItem*> selected_items = this->m_tree->selectedItems();

	/*------------------------右键点击单个树枝------------------------*/
	if (this->isBranchList(selected_items) == true && selected_items.count() == 1 && this->isClassifyMode() == true){
		I_FCTBranch* branch = dynamic_cast<I_FCTBranch*>(selected_items.at(0));
		QString classify_name = branch->getBranch_type_Name();


		action = new QAction("上移", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Up.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::moveUpInAction);
		this->m_mainMenu->addAction(action);
		if (this->indexOfClassify(classify_name) <= 0 || this->indexOfClassify(classify_name) == this->m_classifyData.count() - 1){ action->setEnabled(false); }

		action = new QAction("下移", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Down.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::moveDownInAction);
		this->m_mainMenu->addAction(action);
		if (this->indexOfClassify(classify_name) >= this->m_classifyData.count() - 2 ){ action->setEnabled(false); }

		action = new QAction("添加类型", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Add.png"));
		//（不需要classify_name）
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::addClassifyInAction);
		this->m_mainMenu->addAction(action);

		action = new QAction("编辑类型", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Edit.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::modifyClassifyInAction);
		this->m_mainMenu->addAction(action);
		if (classify_name == ""){ action->setEnabled(false); }		//（未分类的树枝不能编辑）

		action = new QAction("删除类型", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Delete.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::removeClassifyInAction);
		this->m_mainMenu->addAction(action);
		if (classify_name == ""){ action->setEnabled(false); }		//（未分类的树枝不能编辑）

		this->m_mainMenu->addSeparator();
	}

	/*------------------------右键点击多个树枝------------------------*/
	if (this->isBranchList(selected_items) == true && selected_items.count() > 1 && this->isClassifyMode() == true){
		QStringList classify_nameList = QStringList();
		for (int i = 0; i < selected_items.count(); i++){
			I_FCTBranch* branch = dynamic_cast<I_FCTBranch*>(selected_items.at(i));
			QString classify_name = branch->getBranch_type_Name();
			if (classify_name == ""){ continue; }
			classify_nameList.append(classify_name);
		}
		
		action = new QAction("删除选中的类型", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Delete.png"));
		action->setData(classify_nameList);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::removeClassifyListInAction);
		this->m_mainMenu->addAction(action);

		this->m_mainMenu->addSeparator();
	}

	/*------------------------右键点击单个或多个叶子------------------------*/
	if (this->isLeafList(selected_items) == true && this->isClassifyMode() == true){
		QTreeWidgetItem *item = selected_items.at(0);
		QStringList leaf_idList = QStringList();
		for (int i = 0; i < selected_items.count(); i++){
			I_FCTLeaf* leaf = dynamic_cast<I_FCTLeaf*>(selected_items.at(i));
			QString id_str = QString::number( leaf->getId() );
			leaf_idList.append(id_str);
		}

		action = new QAction("移动到...", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Right.png"));
		action->setData(leaf_idList);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::innerModifyLeafTypeInAction);
		this->m_mainMenu->addAction(action);

		this->m_mainMenu->addSeparator();
	}

	P_FlexiblePageTree::drawMenuMain();
}
void P_FlexibleClassificationTree::drawMenuMainLast(){
	P_FlexiblePageTree::drawMenuMainLast();

}
/*-------------------------------------------------
		右键菜单 - 二级菜单（模式）
*/
void P_FlexibleClassificationTree::drawMenuMode(){
	P_FlexiblePageTree::drawMenuMode();
	
}


/*-------------------------------------------------
		资源数据 - 重建数据（私有）
*/
void P_FlexibleClassificationTree::rebuildTreeData(){
	P_FlexiblePageTree::rebuildTreeData();

	if (this->m_sortMode == "自定义分支（按id递增排序）"){
		this->rebuildTreeData_classify_idInc();
		this->refreshTreeUi();
	}
	if (this->m_sortMode == "自定义分支（按名称递增排序）"){
		this->rebuildTreeData_classify_nameInc();
		this->refreshTreeUi();
	}
}
/*-------------------------------------------------
		资源数据 - 重建数据_自定义分支_id递增（私有）
*/
void P_FlexibleClassificationTree::rebuildTreeData_classify_idInc(){

	// > 重刷所有树枝
	for (int i = 0; i < this->m_branchItem.count(); i++){
		this->m_branchItem.at(i)->takeChildren();
	}
	this->m_branchItem.clear();

	// > 添加树枝
	for (int i = 0; i < this->m_classifyData.count(); i++){
		C_FCTClassify c_c = this->m_classifyData.at(i);

		I_FCTBranch* branch = new I_FCTBranch();
		branch->setId(i);										//序号
		if (c_c.name == ""){
			branch->setBranch_type_Name("");					//自定义分支 - 类型名称
		}else{
			branch->setBranch_type_Name(c_c.name);				//自定义分支 - 类型名称
		}
		branch->setBranch_type_Description(c_c.description);	//自定义分支 - 类型描述
		this->m_branchItem.append(branch);
	}

	// > 重刷所有叶子
	this->m_leafItem.clear();
	QStringList classify_list = this->getAllClassify();
	for (int i = 0; i < classify_list.count(); i++){
		QString classify_name = classify_list.at(i);
		QList<int> index_list = this->m_source_ObjectSortController->get_TypeAndId_IndexListByType(classify_name);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > 在叶子中填入数据
			I_FCTLeaf* leaf = new I_FCTLeaf(&this->m_config);
			leaf->setId(data->id);					//标识
			leaf->setName(data->name);				//名称
			leaf->setType(data->type);				//分类
			this->m_leafItem.append(leaf);

			QTreeWidgetItem* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}

}
/*-------------------------------------------------
		资源数据 - 重建数据_自定义分支_名称递增（私有）
*/
void P_FlexibleClassificationTree::rebuildTreeData_classify_nameInc(){
	//（与上面的函数一模一样，不同的是Controller调了get_TypeAndName_IndexListByType）

	// > 重刷所有树枝
	for (int i = 0; i < this->m_branchItem.count(); i++){this->m_branchItem.at(i)->takeChildren();}
	this->m_branchItem.clear();
	// > 添加树枝
	for (int i = 0; i < this->m_classifyData.count(); i++){
		C_FCTClassify c_c = this->m_classifyData.at(i);
	
		I_FCTBranch* branch = new I_FCTBranch();
		branch->setId(i);										//序号
		if (c_c.name == ""){
			branch->setBranch_type_Name("");					//自定义分支 - 类型名称
		}
		else{
			branch->setBranch_type_Name(c_c.name);				//自定义分支 - 类型名称
		}
		branch->setBranch_type_Description(c_c.description);	//自定义分支 - 类型描述
		this->m_branchItem.append(branch);
	}
	// > 重刷所有叶子
	this->m_leafItem.clear();
	QStringList classify_list = this->getAllClassify();
	for (int i = 0; i < classify_list.count(); i++){
		QString classify_name = classify_list.at(i);
		QList<int> index_list = this->m_source_ObjectSortController->get_TypeAndName_IndexListByType(classify_name);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > 在叶子中填入数据
			I_FCTLeaf* leaf = new I_FCTLeaf(&this->m_config);
			leaf->setId(data->id);					//标识
			leaf->setName(data->name);				//名称
			leaf->setType(data->type);				//分类
			this->m_leafItem.append(leaf);

			QTreeWidgetItem* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}


}