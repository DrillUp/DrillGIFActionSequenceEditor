#include "stdafx.h"
#include "p_FlexiblePageTree.h"

#include "private/w_FPT_Config.h"

#include "Source/Utils/common/TTool.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/c_ObjectSortData.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/p_ObjectSortController.h"
#include "Source/Utils/manager/chineseManager/S_ChineseManager.h"

/*
-----==========================================================-----
		�ࣺ		��������.cpp
		�汾��		v1.04
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ܹ���ʾһ�����ݣ����ҽ���Щ���ݷ����ת�Ƶ���ͬ����֦�У����ڲ�ѯ��
					ע�⣬������װ�κ���Ҫ�Ըÿؼ�����н�������Ҫ��������ֱ�ӽ�����
					������id��֧�����Ʒ�֧�����������֧��

		�ӹ��ܣ�	-> ���ṹ
						-> ֻ��һ����֦����֦���Ƿ��࣬Ҷ�Ӷ��Ƕ���
						-> ����Ҷ�ӵ���ӡ�ɾ�����ܣ������Ա༭
						-> ���ɱ༭�������ֵ�����ޣ�
					-> ѡ��
						-> �ɶ�ѡ��ע��û��ѡ��ʱ����
						-> ÿ��ѡ��ʱ�����������ź�
					-> ��֧
						-> ID��֧
						-> ���Ʒ�֧
						x-> �����֧�����಻�������֧���ܣ�
							x-> ��֦�Ҽ�������ӷ���
							x-> Ҷ���Ҽ�������ӷ���/�ƶ������ࣨ��ȥ�����з��ࣩ
							x-> ͬһ����������ڶ�������г��֣���������ڶ���ѡ�������� ��ͻ��
					-> ������
						-> �����и�
						-> ����Ҷ����ʾ�ı�

		˵����		���ǵ���Ҷ���ܻ�ǳ��࣬����ֻ����rebuildTreeDataʱ���Ż���������
					���ֻ�Ǽ��޸���ui����ӷ��࣬�ǲ���ˢ�µġ�
					
		ʹ�÷�����
				> ��ʼ��
					this->m_p_FlexiblePageTree = new P_FlexiblePageTree(ui.treeWidget);
					this->m_p_FlexiblePageTree->setData(obj);		//���洢��������Ҫ��loadǰ��ɸ�ֵ��
					this->m_p_FlexiblePageTree->loadSource(data_list, "id", "name", "type");
				> ����Ĭ��ֵ
					QJsonObject obj_config = QJsonObject();		//���ڽ���ֻ��ͨ��QJsonObject�����ԣ�Ĭ��ֵҲ��Ҫ��Ӧ��object�С�
					QJsonObject obj_tree = QJsonObject();
					obj_tree.insert("rowHeight", 32);
					obj_config.insert("sortMode", "�����֧����id��������");
					obj_config.insert("FCTConfig", obj_tree);
					this->m_p_tree->setData(obj_config);
					
-----==========================================================-----
*/
P_FlexiblePageTree::P_FlexiblePageTree(QTreeWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----������ʼ��

	// > ������
	this->m_tree = parent;								//������
	this->m_treeStyle = this->m_tree->styleSheet();		//��Ĭ����ʽ

	// > Ҷ��
	this->m_leafItem = QList<I_FPT_Leaf*>();				//Ҷ���б�

	// > ��֦
	this->m_branchItem = QList<I_FPT_Branch*>();			//��֦�б�

	// > ѡ��
	this->m_last_selectedItem = nullptr;
	this->m_last_selectedLeaf = nullptr;

	// > ���¼�
	this->m_slotBlock = false;							//������

	// > �Ҽ��˵�
	this->m_mainMenu = nullptr;							//�Ҽ��˵�
	this->m_modeMenu = nullptr;							//��֧�˵�
	this->m_leafOuterControlEnabled = true;				//�˵� - ����
	this->m_leafOuterControl_CopyActive = true;			//�˵� - ���Ƽ���
	this->m_leafOuterControl_PasteActive = false;		//�˵� - ճ������
	this->m_leafOuterControl_ClearActive = true;		//�˵� - ��ռ���

	// > ������
	this->m_config = this->createConfigData();

	// > ��Դ����
	this->m_source_ObjectSortController = new P_ObjectSortController();
	this->m_source_list = QList<C_ObjectSortData>();


	//-----------------------------------
	//----ui��ʼ��
	this->m_tree->setContextMenuPolicy(Qt::CustomContextMenu);								//�����Ҽ�
	this->m_tree->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);	//��ѡҶ��


	//-----------------------------------
	//----�¼���
	connect(this->m_tree, &QTreeWidget::itemClicked, this, &P_FlexiblePageTree::sltItemClicked);
	connect(this->m_tree, &QTreeWidget::itemDoubleClicked, this, &P_FlexiblePageTree::sltItemDoubleClicked);
	connect(this->m_tree, &QTreeWidget::customContextMenuRequested, this, &P_FlexiblePageTree::sltItemRightClicked);
	connect(this->m_tree, &QTreeWidget::itemSelectionChanged, this, &P_FlexiblePageTree::sltItemSelectionChanged);

}
P_FlexiblePageTree::~P_FlexiblePageTree(){
}


/*-------------------------------------------------
		���� - ���� ������ ���ݣ��ɸ�д��
*/
C_FPT_Config* P_FlexiblePageTree::createConfigData(){
	return new C_FPT_Config();
}
/*-------------------------------------------------
		���� - ���� ������ �༭���ڣ��ɸ�д��
*/
W_FPT_Config* P_FlexiblePageTree::createConfigWindow(){
	return new W_FPT_Config(this->m_tree);
}
/*-------------------------------------------------
		���� - ���� Ҷ�ӿؼ����ɸ�д��
*/
I_FPT_Leaf* P_FlexiblePageTree::createFCTLeaf(){
	return new I_FPT_Leaf(this->m_config);
}
/*-------------------------------------------------
		���� - ���� ��֦�ؼ����ɸ�д��
*/
I_FPT_Branch* P_FlexiblePageTree::createFCTBranch(){
	return new I_FPT_Branch(this->m_config);
}


/*-------------------------------------------------
		������ - ��ȡ��
*/
QTreeWidget* P_FlexiblePageTree::getTree(){
	return this->m_tree;
}
/*-------------------------------------------------
		������ - ˢ����
*/
void P_FlexiblePageTree::refreshTreeUi() {
	QList<QTreeWidgetItem*> last_selected_item = this->m_tree->selectedItems();

	// > ȥ��ȫ����֦
	for (int i = this->m_tree->topLevelItemCount()-1; i >=0; i--){
		this->m_tree->takeTopLevelItem(i);
	}

	// > ���������֦
	for (int i = 0; i < this->m_branchItem.count(); i++){
		this->m_tree->addTopLevelItem(this->m_branchItem.at(i));
	}

	// > ˢ����֦����
	for (int i = 0; i < this->m_branchItem.count(); i++){
		I_FPT_Branch* branch = this->m_branchItem.at(i);
		branch->refreshItemSelf();
	}
	// > ˢ��Ҷ������
	for (int i = 0; i < this->m_leafItem.count(); i++){
		I_FPT_Leaf* leaf = this->m_leafItem.at(i);
		leaf->refreshItemSelf();
	}

	// > �����֧����
	this->refreshTreeUi_special();

	// > �и�ˢ��
	this->m_tree->setStyleSheet(this->m_treeStyle + "\n QTreeView::item { height: " + QString::number(this->m_config->rowHeight) + "px;}");
	
	// > չ��ȫ��
	this->m_tree->expandAll();

	// > ѡ�б仯��
	for (int i = 0; i < last_selected_item.count(); i++){
		if (i == 0){
			this->m_tree->scrollToItem(last_selected_item.at(0));
		}
		this->m_tree->setItemSelected(last_selected_item.at(i), true);
	}
}
/*-------------------------------------------------
		������ - ����ȫ��
*/
void P_FlexiblePageTree::clearAll(){

	// > ȥ��ȫ������
	for (int i = this->m_tree->topLevelItemCount() - 1; i >= 0; i--){
		this->m_tree->takeTopLevelItem(i);
	}

	// > �ؼ�����
	this->m_leafItem.clear();
	this->m_branchItem.clear();

	// > �����ã�����������Դ��ȡ�޹أ�������

	// > ��Դ����
	this->m_source_list.clear();
	this->m_leafOuterControl_PasteActive = false;
}

/*-------------------------------------------------
		������ - ˢ���� - ��֧
*/
void P_FlexiblePageTree::refreshTreeUi_special() {

	// > Ҷ��λ��ת��
	if (this->m_config->is_id_Mode()){ this->refreshTreeUi_id_inc(); }
	if (this->m_config->is_name_Mode()){ this->refreshTreeUi_name_inc(); }

}
/*-------------------------------------------------
		������ - ˢ���� - ��֧ - ID��֧
*/
void P_FlexiblePageTree::refreshTreeUi_id_inc() {
	
	// > ����֦�仯���ڲ���ˢ������£�id�ǲ����ܻ��ģ�

}
/*-------------------------------------------------
		������ - ˢ���� - ��֧ - ���Ʒ�֧
*/
void P_FlexiblePageTree::refreshTreeUi_name_inc() {
	
	// > �����޸ĺ��޸ĵ�Ҷ�ӿ��ܻ��ܵ�����֦��
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
		Ҷ�� - ���ⲿ�޸ġ�Ҷ������
*/
void P_FlexiblePageTree::outerModifyLeafName(int id, QString name){
	I_FPT_Leaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return; }
	
	// > �޸�Ҷ��
	leaf->setName(name);
	// > �޸�Ҷ�� - ���Ʒ�֧ʱ��Ҷ�Ӹ������ת��
	if (this->m_config->is_name_Mode()){
		if (name == ""){
			leaf->setLeaf_name_Symbol("���ַ�");
		}else{
			leaf->setLeaf_name_Symbol(S_ChineseManager::getInstance()->getChineseFirstSpell(name));	//����ĸ
		}
	}
	leaf->refreshItemSelf();

	// > �޸ı�������
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
		Ҷ�� - ���ⲿ�޸ġ�Ҷ������
*/
void P_FlexiblePageTree::outerModifyLeafType(int id, QString type){
	I_FPT_Leaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return; }

	// > �޸�Ҷ��
	leaf->setType(type);
	leaf->refreshItemSelf();

	// > �޸ı�������
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
		Ҷ�� - ���ⲿ�޸ġ�ѡ�е�Ҷ������
*/
void P_FlexiblePageTree::outerModifySelectedLeafName(QString name){
	if (this->m_last_selectedLeaf == nullptr){ return; }
	
	// > �޸�Ҷ��
	this->m_last_selectedLeaf->setName(name);
	// > �޸�Ҷ�� - ���Ʒ�֧ʱ��Ҷ�Ӹ������ת��
	if (this->m_config->is_name_Mode()){
		if (name == ""){
			this->m_last_selectedLeaf->setLeaf_name_Symbol("���ַ�");
		}else{
			this->m_last_selectedLeaf->setLeaf_name_Symbol(S_ChineseManager::getInstance()->getChineseFirstSpell(name));	//����ĸ
		}
	}
	this->m_last_selectedLeaf->refreshItemSelf();

	// > �޸ı�������
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
		Ҷ�� - ���ⲿ�޸ġ�ѡ�е�Ҷ������
*/
void P_FlexiblePageTree::outerModifySelectedLeafType(QString type){
	if (this->m_last_selectedLeaf == nullptr){ return; }

	// > �޸�Ҷ��
	this->m_last_selectedLeaf->setType(type);
	this->m_last_selectedLeaf->refreshItemSelf();

	// > �޸ı�������
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
		Ҷ�� - ��ȡ - ��ȡ���󣨸���ID��
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
		Ҷ�� - ��ȡ - ����
*/
QString P_FlexiblePageTree::getLeafName(int id){
	I_FPT_Leaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return ""; }
	return leaf->getName();
}
/*-------------------------------------------------
		Ҷ�� - ��ȡ - �ж�Ҷ��
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
		Ҷ�� - ��ȡ - �ж϶���
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
		��֦ - ��ȡ��֦�����Ʒ�֧ר�ã�
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
		��֦ - ��ȡ - �ж϶���
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
		���¼� - �����¼�
*/
void P_FlexiblePageTree::sltItemClicked(QTreeWidgetItem *item, int index) {
	if (item == nullptr){ return; }
	if (this->m_slotBlock == true){ return; }
	
	//...
}
/*-------------------------------------------------
		���¼� - ˫���¼�
*/
void P_FlexiblePageTree::sltItemDoubleClicked(QTreeWidgetItem *item, int index) {
	if (item == nullptr){ return; }
	if (this->m_slotBlock == true){ return; }

	//...
}
/*-------------------------------------------------
		���¼� -  Ȧѡ�仯�¼�
*/
void P_FlexiblePageTree::sltItemSelectionChanged(){
	if (this->m_slotBlock == true){ return; }
	QList<QTreeWidgetItem*> selected_items = this->m_tree->selectedItems();
	if (selected_items.count() == 0){ return; }
	QTreeWidgetItem *item = selected_items.at(0);

	// > ���ڵ�仯
	if (this->m_last_selectedItem != item){
		this->m_last_selectedItem = item;
		emit currentItemChanged(item);
	}
	// > Ҷ�ӱ仯
	if ( this->isLeaf(item) && 
		 this->m_last_selectedLeaf != item){
		this->m_last_selectedLeaf = dynamic_cast<I_FPT_Leaf*>(item);
		emit currentLeafChanged(item, this->m_last_selectedLeaf->getId(), this->m_last_selectedLeaf->getName());
	}
}
/*-------------------------------------------------
		���¼� - �Ҽ��¼�
*/
void P_FlexiblePageTree::sltItemRightClicked(QPoint point) {
	QTreeWidgetItem *item = this->m_tree->itemAt(point);	//����ǰ�Ҽ�ѡ�е��
	if (item == nullptr){ return; }
	if (this->m_tree->selectedItems().count() == 0){ return; }

	/*------------------------�Ҽ����һ������ ��֦/Ҷ��------------------------*/
	this->m_mainMenu = new QMenu(this->m_tree);

		this->m_modeMenu = new QMenu(this->m_mainMenu);
		this->drawMenuMode();

	this->drawMenuMain();
	this->drawMenuMainLast();

	this->m_mainMenu->exec(QCursor::pos());
	this->m_mainMenu->deleteLater();
}


/*-------------------------------------------------
		�Ҽ��˵� - һ���˵�
*/
void P_FlexiblePageTree::drawMenuMain(){
	QAction *action;
	QList<QTreeWidgetItem*> selected_items = this->m_tree->selectedItems();

	/*------------------------�Ҽ��������Ҷ��------------------------*/
	if (this->isLeafList(selected_items) == true && selected_items.count() == 1 && this->m_leafOuterControlEnabled == true){
		I_FPT_Leaf* leaf = dynamic_cast<I_FPT_Leaf*>(selected_items.at(0));

		action = new QAction("����", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Copy.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuCopyLeafInAction);
		this->m_mainMenu->addAction(action);
		action->setEnabled(this->m_leafOuterControl_CopyActive);
		
		action = new QAction("ճ��", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Paste.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuPasteLeafInAction);
		this->m_mainMenu->addAction(action);
		action->setEnabled(this->m_leafOuterControl_PasteActive);

		action = new QAction("���", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Clear.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuClearLeafInAction);
		this->m_mainMenu->addAction(action);
		action->setEnabled(this->m_leafOuterControl_ClearActive);

		this->m_mainMenu->addSeparator();
	}

	/*------------------------�������------------------------*/
	action = new QAction("չ��ȫ��", this);
	action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Unfold.png"));
	connect(action, &QAction::triggered, this->m_tree, &QTreeWidget::expandAll);
	this->m_mainMenu->addAction(action);

	action = new QAction("����ȫ��", this);
	action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Fold.png"));
	connect(action, &QAction::triggered, this->m_tree, &QTreeWidget::collapseAll);
	this->m_mainMenu->addAction(action);


}
/*-------------------------------------------------
		�Ҽ��˵� - һ���˵���β����
*/
void P_FlexiblePageTree::drawMenuMainLast(){
	QAction *action;

	this->m_mainMenu->addSeparator();
	this->m_mainMenu->addMenu(this->m_modeMenu);

	action = new QAction("ˢ������", this);
	action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Refresh.png"));
	connect(action, &QAction::triggered, this, &P_FlexiblePageTree::rebuildTreeData);
	this->m_mainMenu->addAction(action);

	action = new QAction("������...", this);
	action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Setting.png"));
	connect(action, &QAction::triggered, this, &P_FlexiblePageTree::openConfigParamWindow);
	this->m_mainMenu->addAction(action);

}
/*-------------------------------------------------
		�Ҽ��˵� - �����˵���ģʽ��
*/
void P_FlexiblePageTree::drawMenuMode(){
	QAction *action;
	this->m_modeMenu->setTitle("��֧ģʽ");

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
		�Ҽ��˵� - ���أ�Ĭ�Ͽ�����
*/
void P_FlexiblePageTree::setLeafOuterControlEnabled(bool enabled){
	this->m_leafOuterControlEnabled = enabled;
}
/*-------------------------------------------------
		�Ҽ��˵� - �������
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
		�Ҽ��˵� - ���ư���
*/
void P_FlexiblePageTree::menuCopyLeafInAction(){
	if (this->m_leafOuterControl_CopyActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int id = cur_action->data().value<int>();
	emit menuCopyLeafTriggered(id);
}
/*-------------------------------------------------
		�Ҽ��˵� - ճ������
*/
void P_FlexiblePageTree::menuPasteLeafInAction(){
	if (this->m_leafOuterControl_PasteActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int id = cur_action->data().value<int>();
	emit menuPasteLeafTriggered(id);
}
/*-------------------------------------------------
		�Ҽ��˵� - ��հ���
*/
void P_FlexiblePageTree::menuClearLeafInAction(){
	if (this->m_leafOuterControl_ClearActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int id = cur_action->data().value<int>();
	emit menuClearLeafTriggered(id);
}


/*-------------------------------------------------
		������ - ���ò���
*/
void P_FlexiblePageTree::setConfig(C_FPT_Config* config){
	this->m_config = config;
	this->refreshTreeUi();
}
/*-------------------------------------------------
		������ - ȡ������
*/
C_FPT_Config* P_FlexiblePageTree::getConfig(){
	return this->m_config;
}
/*-------------------------------------------------
		������ - �༭����
*/
void P_FlexiblePageTree::openConfigParamWindow(){
	QString last_sortMode = this->m_config->getCurrentMode();
	int last_pageNum = this->m_config->pagePerNum;

	W_FPT_Config* d = this->createConfigWindow();
	d->setData(this->m_config);
	if (d->exec() == QDialog::Accepted){
		this->m_config = d->getData() ;
		
		// > id��֧ʱ�������仯�ˣ����ؽ�
		if (this->m_config->is_id_Mode() && last_pageNum != this->m_config->pagePerNum){
			this->rebuildTreeData();

		// > ��֧ģʽ ���ˣ��ؽ�
		}else if ( last_sortMode != this->m_config->getCurrentMode() ){
			this->rebuildTreeData();

		// > ��ˢ���ݣ��иߺ��������˱���ˢ��ˢֻ���ui�����ƣ�����ı����з�ʽ��
		}else{
			this->refreshTreeUi();
		}

	}
	d->deleteLater();
}
/*-------------------------------------------------
		������ - �л�ģʽ
*/
void  P_FlexiblePageTree::changeSortMode(QString sortMode){
	if (this->getCurrentSortMode() == sortMode){ return; }
	this->m_config->setCurrentMode(sortMode);
	this->rebuildTreeData();
}
void  P_FlexiblePageTree::changeSortModeInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QString sortMode = cur_action->data().value<QString>();

	this->changeSortMode(sortMode);
}
/*-------------------------------------------------
		������ - ��ȡ��ǰģʽ
*/
QString P_FlexiblePageTree::getCurrentSortMode(){
	return this->m_config->getCurrentMode();
}


/*-------------------------------------------------
		��Դ���� - ����
*/
void P_FlexiblePageTree::loadSource(QList<QJsonObject> obj_list) {

	// > ȫ��
	this->clearAll();

	// > ��������
	for (int i = 0; i < obj_list.count(); i++){
		C_ObjectSortData data = C_ObjectSortData();
		data.setJsonObject(obj_list.at(i));
		if (data.isNull()){ continue; }
		this->m_source_list.append(data);
	}
	// > ����������ʼ��һ�Σ�������Ҫ�ã�
	this->m_source_ObjectSortController->setData_FromSortData(this->m_source_list);

	// > �ؽ�ȫ������
	this->rebuildTreeData();
}
void P_FlexiblePageTree::loadSource(QList<QJsonObject> obj_list, QString id_symbol, QString name_symbol, QString type_symbol) {

	// > ȫ��
	this->clearAll();

	// > ��������
	for (int i = 0; i < obj_list.count(); i++){
		C_ObjectSortData data = C_ObjectSortData();
		data.setJsonObject(obj_list.at(i), id_symbol, name_symbol, type_symbol);	//ע��symbol����
		if (data.isNull()){ continue; }
		this->m_source_list.append(data);
	}
	// > ����������ʼ��һ�Σ�������Ҫ�ã�
	this->m_source_ObjectSortController->setData_FromSortData(this->m_source_list);

	// > �ؽ�ȫ������
	this->rebuildTreeData();
}

/*-------------------------------------------------
		��Դ���� - ��ȡѡ�е�����
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
		��Դ���� - ��ȡѡ�еĶ���˽�У�
*/
C_ObjectSortData* P_FlexiblePageTree::getSelectedSource(){
	if (this->m_leafItem.count() == 0){ return nullptr; }
	QList<QTreeWidgetItem*> item_list = this->m_tree->selectedItems();
	if (item_list.count() == 0){ return nullptr; }

	// > �ҵ�ѡ�е�Ҷ��
	I_FPT_Leaf* leaf = nullptr;
	for (int i = 0; i < item_list.count(); i++){
		QTreeWidgetItem* item = item_list.at(i);
		if (this->isLeaf(item)){
			leaf = dynamic_cast<I_FPT_Leaf*>(item);;
			break;
		}
	}
	if (leaf == nullptr){ return nullptr; }

	int arr_index = leaf->getId();			//����item�л�ȡ��id��
	return this->m_source_ObjectSortController->getSortData_ByIndex(arr_index);
}
/*-------------------------------------------------
		��Դ���� - �ؽ����ݣ�˽�У�
*/
void P_FlexiblePageTree::rebuildTreeData(){
	this->m_source_ObjectSortController->setData_FromSortData(this->m_source_list);

	// > Ϊ��ʱ��ѡ���һ��
	if (this->getCurrentSortMode() == ""){
		QMessageBox::warning(nullptr, "����", "���ؼ��г���������ķ�֧ģʽ���ã��ѹ�ΪĬ�����á�", QMessageBox::Yes);
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
		��Դ���� - �ؽ�����_ID��֧��˽�У�
*/
void P_FlexiblePageTree::rebuildTreeData_id_inc(){

	// > ��ˢ������֦
	for (int i = 0; i < this->m_branchItem.count(); i++){
		this->m_branchItem.at(i)->takeChildren();
	}
	this->m_branchItem.clear();

	// > ����׼��
	int all_count = this->m_source_ObjectSortController->getDataCount();
	this->m_config->set_id_MaxCount(all_count);

	// > �����֦
	int page_count = this->m_config->get_id_PageCount();
	for (int i = 0; i < page_count; i++){
		int bottom = this->m_config->get_id_Bottom(i);
		int top = this->m_config->get_id_Top(i);
		if (bottom == -1){ break; }
		if (top == -1){ break; }

		I_FPT_Branch* branch_item = this->createFCTBranch();
		branch_item->setId(i);							//���
		branch_item->setBranch_id_Index(i);				//ID��֧ - ��ǰҳ������λ��
		branch_item->setBranch_id_Bottom(bottom);		//ID��֧ - ��
		branch_item->setBranch_id_Top(top);				//ID��֧ - ��
		this->m_branchItem.append(branch_item);
	}

	// > ��ˢ����Ҷ��
	this->m_leafItem.clear();
	for (int i = 0; i < this->m_branchItem.count(); i++){
		I_FPT_Branch* branch_item = this->m_branchItem.at(i);
		int page_index = branch_item->getBranch_id_Index();	//��ǰҳ������λ��
		QList<int> index_list = this->m_source_ObjectSortController->get_IdInc_PageIndexList(page_index, this->m_config->pagePerNum);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > ��Ҷ������������
			I_FPT_Leaf* leaf = this->createFCTLeaf();
			leaf->setId(data->id);					//��ʶ
			leaf->setName(data->name);				//����
			leaf->setType(data->type);				//����
			this->m_leafItem.append(leaf);

			I_FPT_Branch* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}

}
/*-------------------------------------------------
		��Դ���� - �ؽ�����_���Ʒ�֧��˽�У�
*/
void P_FlexiblePageTree::rebuildTreeData_name_inc(){

	// > ��ˢ������֦
	for (int i = 0; i < this->m_branchItem.count(); i++){
		this->m_branchItem.at(i)->takeChildren();
	}
	this->m_branchItem.clear();

	// > ����׼��
	QStringList page_name_list = this->m_config->get_name_PageNameList();
	QStringList page_symbol_list = this->m_config->get_name_PageSymbolList();

	// > ���ҳ
	for (int i = 0; i < page_name_list.count(); i++){
		QString page_name = page_name_list.at(i);
		QString page_symbol = page_symbol_list.at(i);

		I_FPT_Branch* branch_item = this->createFCTBranch();
		branch_item->setId(i);									//���
		branch_item->setBranch_name_Index(i);					//���Ʒ�֧ - ��ǰҳ������λ��
		branch_item->setBranch_name_Showing(page_name);			//���Ʒ�֧ - ��ʾ������
		branch_item->setBranch_name_Symbol(page_symbol);		//���Ʒ�֧ - ���Ʋ����ַ��ı�־
		this->m_branchItem.append(branch_item);
	}

	// > ��ˢ����Ҷ��
	this->m_leafItem.clear();
	for (int i = 0; i < page_symbol_list.count(); i++){
		QString symbol = page_symbol_list.at(i);
		QList<int> index_list = QList<int>();
		if (symbol == "������"){
			index_list = this->m_source_ObjectSortController->get_Name_IndexListByEmptyName();
		}else{
			index_list = this->m_source_ObjectSortController->get_Name_IndexListByCharacter(symbol.at(0));
		}
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > ��Ҷ������������
			I_FPT_Leaf* leaf = this->createFCTLeaf();
			leaf->setId(data->id);					//��ʶ
			leaf->setName(data->name);				//����
			leaf->setType(data->type);				//����
			leaf->setLeaf_name_Symbol(symbol);		//���Ʒ�֧ - ����ĸ��ʶ
			this->m_leafItem.append(leaf);

			I_FPT_Branch* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}
}