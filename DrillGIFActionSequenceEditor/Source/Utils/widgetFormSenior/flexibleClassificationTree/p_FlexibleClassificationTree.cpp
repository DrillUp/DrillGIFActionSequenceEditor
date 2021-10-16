#include "stdafx.h"
#include "p_FlexibleClassificationTree.h"

#include "private/c_FCT_Config.h"
#include "private/c_FCT_Classify.h"
#include "private/w_FCT_Classify.h"
#include "private/w_FCT_ClassifySelector.h"

#include "Source/Utils/common/TTool.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/c_ObjectSortData.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/p_ObjectSortController.h"
#include "Source/Utils/manager/chineseManager/S_ChineseManager.h"

/*
-----==========================================================-----
		类：		灵活分类树（含种类分支）.cpp
		版本：		v1.04
		作者：		drill_up
		所属模块：	工具模块
		功能：		能够显示一堆数据，并且将这些数据分类或转移到不同的树枝中，便于查询。
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
						-> 种类分支（该类不含种类分支功能）
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

	// > 树设置
	this->m_config = this->createConfigData();

}
P_FlexibleClassificationTree::~P_FlexibleClassificationTree(){
}


/*-------------------------------------------------
		工厂 - 创建 树设置 数据（覆写父类）
*/
C_FPT_Config* P_FlexibleClassificationTree::createConfigData(){
	return new C_FCT_Config();
}
/*-------------------------------------------------
		工厂 - 创建 种类 数据（可覆写）
*/
C_FCT_Classify* P_FlexibleClassificationTree::createClassifyData(){
	return new C_FCT_Classify();
}
/*-------------------------------------------------
		工厂 - 创建 种类 编辑窗口（可覆写）
*/
W_FCT_Classify* P_FlexibleClassificationTree::createClassifyWindow(){
	return new W_FCT_Classify(this);
}


/*-------------------------------------------------
		树对象 - 清理全部
*/
void P_FlexibleClassificationTree::clearAll(){
	P_FlexiblePageTree::clearAll();

	// > 树设置（树设置与资源读取无关，不清理）

}
/*-------------------------------------------------
		树对象 - 刷新树 - 分支
*/
void P_FlexibleClassificationTree::refreshTreeUi_special() {
	P_FlexiblePageTree::refreshTreeUi_special();

	// > 叶子位置转移
	if (this->getConfigEx()->is_classify_idInc_Mode()){ this->refreshTreeUi_classify_idInc(); }
	if (this->getConfigEx()->is_classify_nameInc_Mode()){ this->refreshTreeUi_classify_nameInc(); }
}
/*-------------------------------------------------
		树对象 - 刷新树 - 分支 - 种类分支_id递增
*/
void P_FlexibleClassificationTree::refreshTreeUi_classify_idInc() {

	// > 种类修改后，修改的叶子可能会跑到新树枝下
	for (int i = 0; i < this->m_leafItem.count(); i++){
		I_FPT_Leaf* leaf = this->m_leafItem.at(i);
		I_FPT_Branch* parent_branch = dynamic_cast<I_FPT_Branch*>(leaf->parent());

		QString classify_name = leaf->getType();
		QString p_classify_name = parent_branch->getBranch_type_Name();
		if (classify_name != p_classify_name){
			leaf->parent()->removeChild(leaf);

			I_FPT_Branch* n_branch = this->getBranchByTypeName(classify_name);
			if (n_branch == nullptr){ continue; }
			n_branch->addChild(leaf);
		}
	}

}
/*-------------------------------------------------
		树对象 - 刷新树 - 分支 - 种类分支_名称递增
*/
void P_FlexibleClassificationTree::refreshTreeUi_classify_nameInc() {

	// > 种类修改后，修改的叶子可能会跑到新树枝下
	this->refreshTreeUi_classify_idInc();	//（与id一模一样）

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
		叶子 - 【外部修改】选中的叶子名称
*/
void P_FlexibleClassificationTree::outerModifySelectedLeafName( QString name){
	P_FlexiblePageTree::outerModifySelectedLeafName( name);
	//（无变化）
}
/*-------------------------------------------------
		叶子 - 【外部修改】选中的叶子类型
*/
void P_FlexibleClassificationTree::outerModifySelectedLeafType( QString name){
	P_FlexiblePageTree::outerModifySelectedLeafType( name);
	// （无变化）
}
/*-------------------------------------------------
		叶子 - 获取 - 根据种类获取
*/
QList<I_FPT_Leaf*> P_FlexibleClassificationTree::getLeafByType(QString type){
	QList<I_FPT_Leaf*> result_list = QList<I_FPT_Leaf*>();
	for (int i = 0; i < this->m_leafItem.count(); i++){
		if (this->m_leafItem.at(i)->getType() == type){
			result_list.append( this->m_leafItem.at(i) );
		}
	}
	return result_list;
}
/*-------------------------------------------------
		叶子 - 获取 - 种类
*/
QString P_FlexibleClassificationTree::getLeafType(int id){
	I_FPT_Leaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return ""; }
	return leaf->getType();
}
/*-------------------------------------------------
		树枝 - 获取树枝（种类分支专用）
*/
I_FPT_Branch* P_FlexibleClassificationTree::getBranchByTypeName(QString classify_name){
	for (int i = 0; i < this->m_branchItem.count(); i++){
		if (this->m_branchItem.at(i)->getBranch_type_Name() == classify_name){
			return this->m_branchItem.at(i);
		}
	}
	return nullptr;
}


/*-------------------------------------------------
		种类 - 获取 - 是否为种类分支
*/
bool P_FlexibleClassificationTree::isClassifyMode(){
	if (this->getConfigEx()->is_classify_idInc_Mode()){ return true; }
	if (this->getConfigEx()->is_classify_nameInc_Mode()){ return true; }
	return false;
}
/*-------------------------------------------------
		种类 - 获取 - 全部种类名
*/
QStringList P_FlexibleClassificationTree::getAllClassifyName(){
	return this->getConfigEx()->getAll_classify_NameList();
}

/*-------------------------------------------------
		种类 - 添加
*/
void P_FlexibleClassificationTree::addClassify(C_FCT_Classify* classify){

	// > 添加到数据
	if (this->getConfigEx()->has_classify(classify->getName()) == true){ return; }
	this->getConfigEx()->add_classify(classify);

	// > 树枝变化
	int index = this->getConfigEx()->get_classify_Index(classify->getName());
	I_FPT_Branch* branch = this->createFCTBranch();
	branch->setId(index);											//序号
	branch->setBranch_type_Name(classify->getName());				//种类分支 - 种类名称
	branch->setBranch_type_Description(classify->getDescription());	//种类分支 - 种类描述
	this->m_branchItem.insert(index, branch);

	this->refreshTreeUi();
}
/*-------------------------------------------------
		种类 - 添加（action）
*/
void P_FlexibleClassificationTree::addClassifyInAction(){
	W_FCT_Classify* d = this->createClassifyWindow();
	d->setDataInAddMode();
	if (d->exec() == QDialog::Accepted){
		this->addClassify(d->getData());
	}
	d->deleteLater();
}
/*-------------------------------------------------
		种类 - 添加 - 验证新种类
*/
void P_FlexibleClassificationTree::addClassifyDistinguishedList(QStringList new_classify_nameList){
	C_FCT_Config* config = this->getConfigEx();

	// > 种类 - 检查新的种类
	for (int i = 0; i < new_classify_nameList.count(); i++){
		QString classify_name = new_classify_nameList.at(i);
		if (classify_name == ""){ continue; }
		if (config->has_classify(classify_name)){ continue; }

		C_FCT_Classify* c_c = this->createClassifyData();	//（这里的种类来自外部的修改，而非树的修改）
		c_c->setName(classify_name);
		c_c->setDescription("");
		config->add_classify(c_c);
	}
}
/*-------------------------------------------------
		种类 - 修改
*/
void P_FlexibleClassificationTree::modifyClassify(QString classify_name, C_FCT_Classify* classify){
	if (this->getConfigEx()->has_classify(classify_name) == true){ return; }

	// > 修改数据
	//（指针，直接修改）

	// > 树枝变化
	I_FPT_Branch* branch = this->getBranchByTypeName(classify_name);
	branch->setBranch_type_Name(classify->getName());				//种类分支 - 种类名称
	branch->setBranch_type_Description(classify->getDescription());	//种类分支 - 种类描述

	// > 叶子变化
	QList<I_FPT_Leaf*> leaf_list = this->getLeafByType(classify_name);
	for (int i = 0; i < leaf_list.count(); i++){
		I_FPT_Leaf* leaf = leaf_list.at(i);
		this->innerModifyLeafType(leaf->getId(), classify->getName());
	}

	this->refreshTreeUi();
}
/*-------------------------------------------------
		种类 - 修改（action）
*/
void P_FlexibleClassificationTree::modifyClassifyInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString classify_name = cur_action->data().value<QString>();

	C_FCT_Classify* c_c = this->getConfigEx()->get_classify(classify_name);

	// > 修改窗口
	W_FCT_Classify* d = this->createClassifyWindow();
	d->setDataInModifyMode(c_c);
	if (d->exec() == QDialog::Accepted){
		this->modifyClassify(classify_name, d->getData());
	}
	d->deleteLater();
}
/*-------------------------------------------------
		种类 - 去除
*/
void P_FlexibleClassificationTree::removeClassify(QString classify_name){
	if (classify_name == ""){ return; }
	if (this->getConfigEx()->has_classify(classify_name) == false){ return; }

	// > 去除数据
	this->getConfigEx()->remove_classify(classify_name);

	// > 树枝变化
	this->m_branchItem.removeOne(this->getBranchByTypeName(classify_name));

	// > 叶子变化
	QList<I_FPT_Leaf*> leaf_list = this->getLeafByType(classify_name);
	for (int i = 0; i < leaf_list.count(); i++){
		I_FPT_Leaf* leaf = leaf_list.at(i);
		this->innerModifyLeafType(leaf->getId(), "");
	}

	this->refreshTreeUi();
}
/*-------------------------------------------------
		种类 - 去除（action）
*/
void P_FlexibleClassificationTree::removeClassifyInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString classify_name = cur_action->data().value<QString>();

	this->removeClassify(classify_name);
}
/*-------------------------------------------------
		种类 - 去除（action）
*/
void P_FlexibleClassificationTree::removeClassifyListInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QStringList classify_list = cur_action->data().value<QStringList>();

	for (int i = classify_list.count()-1; i >=0; i--){
		this->removeClassify(classify_list.at(i));
	}
}
/*-------------------------------------------------
		种类 - 上移
*/
void P_FlexibleClassificationTree::moveUpInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString classify_name = cur_action->data().value<QString>();

	// > 数据变化
	this->getConfigEx()->set_classify_moveUp(classify_name);

	// > 树枝变化
	int index = this->getConfigEx()->get_classify_Index(classify_name);
	this->m_branchItem.swap(index, index + 1);

	this->refreshTreeUi();
}
/*-------------------------------------------------
		种类 - 下移
*/
void P_FlexibleClassificationTree::moveDownInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//从action里面取出数据
	QString classify_name = cur_action->data().value<QString>();

	// > 数据变化
	this->getConfigEx()->set_classify_moveDown(classify_name);

	// > 树枝变化
	int index = this->getConfigEx()->get_classify_Index(classify_name);
	this->m_branchItem.swap(index, index - 1);

	this->refreshTreeUi();

}


/*-------------------------------------------------
		修改的数据 - 【内部修改】叶子的类型
*/
void P_FlexibleClassificationTree::innerModifyLeafType(int id, QString type){
	I_FPT_Leaf* leaf = this->getLeafById(id);
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

	// > 选择种类
	W_FCT_ClassifySelector d(this);
	d.setData(this->getConfigEx(), classify);
	if (d.exec() == QDialog::Accepted){
		QString name = d.getSelectedData();
		
		// > 批量修改
		for (int i = 0; i < leaf_idList.count(); i++){
			this->innerModifyLeafType(leaf_idList.at(i).toInt(), name);
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
	if (this->isBranchList(selected_items) == true && selected_items.count() == 1 && this->isClassifyMode() == true ){
		I_FPT_Branch* branch = dynamic_cast<I_FPT_Branch*>(selected_items.at(0));
		QString classify_name = branch->getBranch_type_Name();

		int index = this->getConfigEx()->get_classify_Index(classify_name);
		int count = this->getConfigEx()->get_classify_Count();

		action = new QAction("上移", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Up.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::moveUpInAction);
		this->m_mainMenu->addAction(action);
		if (index <= 0 || index == count - 1){ action->setEnabled(false); }

		action = new QAction("下移", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Down.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::moveDownInAction);
		this->m_mainMenu->addAction(action);
		if (index >= count - 2){ action->setEnabled(false); }

		action = new QAction("添加种类", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Add.png"));
		//（不需要classify_name）
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::addClassifyInAction);
		this->m_mainMenu->addAction(action);

		action = new QAction("编辑种类", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Edit.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::modifyClassifyInAction);
		this->m_mainMenu->addAction(action);
		if (classify_name == ""){ action->setEnabled(false); }		//（未分类的树枝不能编辑）

		action = new QAction("删除种类", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Delete.png"));
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
			I_FPT_Branch* branch = dynamic_cast<I_FPT_Branch*>(selected_items.at(i));
			QString classify_name = branch->getBranch_type_Name();
			if (classify_name == ""){ continue; }
			classify_nameList.append(classify_name);
		}
		
		action = new QAction("删除选中的种类", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Delete.png"));
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
			I_FPT_Leaf* leaf = dynamic_cast<I_FPT_Leaf*>(selected_items.at(i));
			QString id_str = QString::number( leaf->getId() );
			leaf_idList.append(id_str);
		}

		action = new QAction("移动到...", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Right.png"));
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
		树设置 - 设置参数
*/
void P_FlexibleClassificationTree::setConfig(C_FPT_Config* config){
	P_FlexiblePageTree::setConfig(config);
}
/*-------------------------------------------------
		树设置 - 取出参数
*/
C_FPT_Config* P_FlexibleClassificationTree::getConfig(){
	return P_FlexiblePageTree::getConfig();
}
/*-------------------------------------------------
		树设置 - 设置参数Ex
*/
void P_FlexibleClassificationTree::setConfigEx(C_FCT_Config* config){
	P_FlexiblePageTree::setConfig(config);
}
/*-------------------------------------------------
		树设置 - 取出参数Ex
*/
C_FCT_Config* P_FlexibleClassificationTree::getConfigEx(){
	return dynamic_cast<C_FCT_Config*>(P_FlexiblePageTree::getConfig());
}

/*-------------------------------------------------
		资源数据 - 重建数据（私有）
*/
void P_FlexibleClassificationTree::rebuildTreeData(){
	P_FlexiblePageTree::rebuildTreeData();

	// > 添加新种类数据
	QStringList new_classify_list = this->m_source_ObjectSortController->get_Type_DistinguishedList();
	this->addClassifyDistinguishedList(new_classify_list);

	// > 重建叶子和树枝
	if (this->getConfigEx()->is_classify_idInc_Mode() ){
		this->rebuildTreeData_classify_idInc();
		this->refreshTreeUi();
	}
	if (this->getConfigEx()->is_classify_nameInc_Mode() ){
		this->rebuildTreeData_classify_nameInc();
		this->refreshTreeUi();
	}
}
/*-------------------------------------------------
		资源数据 - 重建数据_种类分支_id递增（私有）
*/
void P_FlexibleClassificationTree::rebuildTreeData_classify_idInc(){

	// > 重刷所有树枝
	for (int i = 0; i < this->m_branchItem.count(); i++){
		this->m_branchItem.at(i)->takeChildren();
	}
	this->m_branchItem.clear();

	// > 添加树枝
	QList<C_FCT_Classify*> classify_list = this->getConfigEx()->get_classify_DataList();
	for (int i = 0; i < classify_list.count(); i++){
		C_FCT_Classify* c_c = classify_list.at(i);

		I_FPT_Branch* branch = this->createFCTBranch();
		branch->setId(i);										//序号
		if (c_c->getName() == ""){
			branch->setBranch_type_Name("");					//种类分支 - 种类名称
		}else{
			branch->setBranch_type_Name(c_c->getName());		//种类分支 - 种类名称
		}
		branch->setBranch_type_Description(c_c->getDescription());	//种类分支 - 种类描述
		this->m_branchItem.append(branch);
	}

	// > 重刷所有叶子
	this->m_leafItem.clear();
	QStringList name_list = this->getAllClassifyName();
	for (int i = 0; i < name_list.count(); i++){
		QString classify_name = name_list.at(i);
		QList<int> index_list = this->m_source_ObjectSortController->get_TypeAndId_IndexListByType(classify_name);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > 在叶子中填入数据
			I_FPT_Leaf* leaf = this->createFCTLeaf();
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
		资源数据 - 重建数据_种类分支_名称递增（私有）
*/
void P_FlexibleClassificationTree::rebuildTreeData_classify_nameInc(){
	//（与上面的函数一模一样，不同的是Controller调了get_TypeAndName_IndexListByType）

	// > 重刷所有树枝
	for (int i = 0; i < this->m_branchItem.count(); i++){this->m_branchItem.at(i)->takeChildren();}
	this->m_branchItem.clear();
	// > 添加树枝
	QList<C_FCT_Classify*> classify_list = this->getConfigEx()->get_classify_DataList();
	for (int i = 0; i < classify_list.count(); i++){
		C_FCT_Classify* c_c = classify_list.at(i);
	
		I_FPT_Branch* branch = this->createFCTBranch();
		branch->setId(i);										//序号
		if (c_c->getName() == ""){
			branch->setBranch_type_Name("");					//种类分支 - 种类名称
		}else{
			branch->setBranch_type_Name(c_c->getName());		//种类分支 - 种类名称
		}
		branch->setBranch_type_Description(c_c->getDescription());	//种类分支 - 种类描述
		this->m_branchItem.append(branch);
	}
	// > 重刷所有叶子
	this->m_leafItem.clear();
	QStringList name_list = this->getAllClassifyName();
	for (int i = 0; i < name_list.count(); i++){
		QString classify_name = name_list.at(i);
		QList<int> index_list = this->m_source_ObjectSortController->get_TypeAndName_IndexListByType(classify_name);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > 在叶子中填入数据
			I_FPT_Leaf* leaf = this->createFCTLeaf();
			leaf->setId(data->id);					//标识
			leaf->setName(data->name);				//名称
			leaf->setType(data->type);				//分类
			this->m_leafItem.append(leaf);

			QTreeWidgetItem* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}


}