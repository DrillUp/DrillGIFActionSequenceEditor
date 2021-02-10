#include "stdafx.h"
#include "p_FlexiblePageTree.h"

#include "private/w_FCTConfig.h"

#include "Source/Utils/common/TTool.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/c_ObjectSortData.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/p_ObjectSortController.h"
#include "Source/Utils/manager/chineseManager/S_ChineseManager.h"

/*
-----==========================================================-----
		�ࣺ		��������.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ܹ���ʾһ�����ݣ����ҽ���Щ���ݷ����ת�Ƶ���ͬ����֦�У����ڲ�ѯ��
					ע�⣬������װ�κ���Ҫ�Ըÿؼ�����н�������Ҫ��������ֱ�ӽ�����
					������id��֧�����Ʒ�֧�������Զ����֧��

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
						x-> �Զ����֧�����಻���Զ����֧���ܣ�
							x-> ��֦�Ҽ�������ӷ���
							x-> Ҷ���Ҽ�������ӷ���/�ƶ������ࣨ��ȥ�����з��ࣩ
							x-> ͬһ����������ڶ�������г��֣���������ڶ���ѡ�������� ��ͻ��
					-> ������
						-> �����и�
						-> ����Ҷ����ʾ�ı�

		˵����		���ǵ���Ҷ���ܻ�ǳ��࣬����ֻ����rebuildTreeDataʱ���Ż���������
					���ֻ�Ǽ��޸���ui����ӷ��࣬�ǲ���ˢ�µġ�
					
		ʹ�÷�����
				>��ʼ��
					this->m_p_FlexiblePageTree = new P_FlexiblePageTree(ui.treeWidget);
					this->m_p_FlexiblePageTree->setData(obj);		//���洢��������Ҫ��loadǰ��ɸ�ֵ��
					this->m_p_FlexiblePageTree->loadSource(data_list, "id", "name", "type");
					
-----==========================================================-----
*/
P_FlexiblePageTree::P_FlexiblePageTree(QTreeWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----������ʼ��
	this->m_tree = parent;								//������
	this->m_treeStyle = this->m_tree->styleSheet();		//��Ĭ����ʽ
	this->m_slotBlock = false;							//������
	this->m_leafItem = QList<I_FCTLeaf*>();				//Ҷ���б�
	this->m_branchItem = QList<I_FCTBranch*>();			//��֦�б�
	this->m_leafOuterControlEnabled = true;

	// > ����
	this->local_treeData = QJsonObject();
	this->m_sortMode = "ID��֧����id��������";
	this->m_sortModeList = QStringList() << "ID��֧����id��������" << "���Ʒ�֧�������Ƶ�������";
	this->m_config = C_FCTConfig();

	// > ���¼�
	this->m_selectionSignalBlock_Root = false;
	this->m_mainMenu = nullptr;
	this->m_modeMenu = nullptr;
	this->m_menuIconSrcPath = ":/code_piece_qt/Resources/images";

	// > ��Դ����
	this->m_source_ObjectSortController = new P_ObjectSortController();
	this->m_source_list = QList<C_ObjectSortData>();

	//-----------------------------------
	//----ui��ʼ��
	this->m_tree->setContextMenuPolicy(Qt::CustomContextMenu);								//�����Ҽ�
	this->m_tree->setSelectionMode(QAbstractItemView::SelectionMode::ContiguousSelection);	//��ѡҶ��

	connect(this->m_tree, &QTreeWidget::itemClicked, this, &P_FlexiblePageTree::sltItemClicked);
	connect(this->m_tree, &QTreeWidget::itemDoubleClicked, this, &P_FlexiblePageTree::sltItemDoubleClicked);
	connect(this->m_tree, &QTreeWidget::customContextMenuRequested, this, &P_FlexiblePageTree::sltItemRightClicked);
	connect(this->m_tree, &QTreeWidget::itemSelectionChanged, this, &P_FlexiblePageTree::sltItemSelectionChanged);

}
P_FlexiblePageTree::~P_FlexiblePageTree(){
}


/*-------------------------------------------------
		�ؼ� - ˢ����
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
		I_FCTBranch* branch = this->m_branchItem.at(i);
		branch->refreshItemSelf();
	}
	// > ˢ��Ҷ������
	for (int i = 0; i < this->m_leafItem.count(); i++){
		I_FCTLeaf* leaf = this->m_leafItem.at(i);
		leaf->refreshItemSelf();
	}

	// > �����֧����
	this->refreshTreeUi_special();

	// > �и�ˢ��
	this->m_tree->setStyleSheet(this->m_treeStyle + "\n QTreeView::item { height: " + QString::number(this->m_config.rowHeight) + "px;}");
	
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
		�ؼ� - ˢ����_��֧����
*/
void P_FlexiblePageTree::refreshTreeUi_special() {

	// > Ҷ��λ��ת��
	if (this->m_sortMode == "ID��֧����id��������"){ this->refreshTreeUi_id_inc(); }
	if (this->m_sortMode == "���Ʒ�֧�������Ƶ�������"){ this->refreshTreeUi_name_inc(); }

}
/*-------------------------------------------------
		�ؼ� - ˢ����_ID��֧
*/
void P_FlexiblePageTree::refreshTreeUi_id_inc() {
	
	// > ����֦�仯���ڲ���ˢ������£�id�ǲ����ܻ��ģ�

}
/*-------------------------------------------------
		�ؼ� - ˢ����_���Ʒ�֧
*/
void P_FlexiblePageTree::refreshTreeUi_name_inc() {
	
	// > �����޸ĺ��޸ĵ�Ҷ�ӿ��ܻ��ܵ�����֦��
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
		�ؼ� - ����ȫ��
*/
void P_FlexiblePageTree::clearAll(){

	// > ȥ��ȫ������
	for (int i = this->m_tree->topLevelItemCount() - 1; i >= 0; i--){
		this->m_tree->takeTopLevelItem(i);
	}

	// > �ؼ�����
	this->m_leafItem.clear();
	this->m_branchItem.clear();

	// > ������
	this->m_config = C_FCTConfig();

	// > ��Դ����
	this->m_source_list.clear();
}


/*-------------------------------------------------
		Ҷ�� - ���ⲿ�޸ġ�Ҷ������
*/
void P_FlexiblePageTree::outerModifyLeafName(int id, QString name){
	I_FCTLeaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return; }
	
	// > �޸�Ҷ��
	leaf->setName(name);
	// > �޸�Ҷ�� - ���Ʒ�֧ʱ��Ҷ�Ӹ������ת��
	if (this->m_sortMode == "���Ʒ�֧�������Ƶ�������"){
		if (name == ""){
			leaf->setLeaf_name_Symbol("���ַ�");
		}else{
			leaf->setLeaf_name_Symbol(S_ChineseManager::getInstance()->getChineseFirstSpell(name));	//����ĸ
		}
	}

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
	I_FCTLeaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return; }

	// > �޸�Ҷ��
	leaf->setType(type);

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
		Ҷ�� - ��ȡ - ��ȡ���󣨸���ID��
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
I_FCTLeaf* P_FlexiblePageTree::getLeafByName(QString name){
	for (int i = 0; i < this->m_leafItem.count(); i++){
		if (this->m_leafItem.at(i)->getName() == name){
			return this->m_leafItem.at(i);
		}
	}
	return nullptr;
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
		Ҷ�ӽӿ� - ���أ�Ĭ�Ͽ�����
*/
void P_FlexiblePageTree::setLeafOuterControlEnabled(bool enabled){
	this->m_leafOuterControlEnabled = enabled;
}
/*-------------------------------------------------
		Ҷ�ӽӿ� - ���ư���
*/
void P_FlexiblePageTree::menuCopyLeafInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int id = cur_action->data().value<int>();
	emit menuCopyLeafTriggered(id);
}
/*-------------------------------------------------
		Ҷ�ӽӿ� - ճ������
*/
void P_FlexiblePageTree::menuPasteLeafInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int id = cur_action->data().value<int>();
	emit menuPasteLeafTriggered(id);
}
/*-------------------------------------------------
		Ҷ�ӽӿ� - ��հ���
*/
void P_FlexiblePageTree::menuClearLeafInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int id = cur_action->data().value<int>();
	emit menuClearLeafTriggered(id);
}


/*-------------------------------------------------
		��֦ - ��ȡ��֦�����Ʒ�֧ר�ã�
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
		���� - ��������
*/
QJsonObject P_FlexiblePageTree::getData(){
	this->putUiToData();
	return local_treeData;
}
/*-------------------------------------------------
		���� - ȡ��
*/
void P_FlexiblePageTree::setData(QJsonObject obj){
	this->local_treeData = obj;
	this->putDataToUi();
}
/*-------------------------------------------------
		���� - �������� -> ui����
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
		���� - ui���� -> ��������
*/
void P_FlexiblePageTree::putUiToData(){
	this->local_treeData = QJsonObject();

	this->local_treeData.insert("FCTConfig", this->m_config.getJsonObject());
	this->local_treeData.insert("sortMode", this->m_sortMode);
}


/*-------------------------------------------------
		ģʽ - �л�ģʽ
*/
void  P_FlexiblePageTree::changeSortMode(QString sortMode){
	if (this->m_sortMode == sortMode){ return; }
	this->m_sortMode = sortMode;
	this->m_config.sortType = sortMode;
	this->rebuildTreeData();
}
void  P_FlexiblePageTree::changeSortModeInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QString sortMode = cur_action->data().value<QString>();

	if (this->m_sortMode == sortMode){ return; }
	this->m_sortMode = sortMode;
	this->m_config.sortType = sortMode;
	this->rebuildTreeData();
}
/*-------------------------------------------------
		ģʽ - ��ȡģʽ
*/
QString P_FlexiblePageTree::getCurSortMode(){
	return this->m_sortMode;
}


/*-------------------------------------------------
		������ - ���ò���
*/
void P_FlexiblePageTree::setConfigParam(QJsonObject config){
	this->m_config.setJsonObject(config);
	this->refreshTreeUi();
}
/*-------------------------------------------------
		������ - ȡ������
*/
QJsonObject P_FlexiblePageTree::getConfigParam(){
	return this->m_config.getJsonObject();
}
/*-------------------------------------------------
		������ - �༭����
*/
void P_FlexiblePageTree::openConfigParamWindow(){
	W_FCTConfig d(this->m_tree);
	d.setDataInModifyMode(this->m_config);
	d.setModeType(this->m_sortMode, this->m_sortModeList);
	if (d.exec() == QDialog::Accepted){
		this->m_config = d.getData() ;
		
		// > id��֧ʱ����Ȼ��ˢ
		if (this->m_config.sortType == "ID��֧����id��������" ){ 
			this->m_sortMode = this->m_config.sortType;
			this->rebuildTreeData(); 
		}
		
		// > һ���֧
		if (this->m_config.sortType != m_sortMode){
			this->changeSortMode(this->m_config.sortType);
		}else{
			this->refreshTreeUi();
		}
	}
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

	//...
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
		I_FCTLeaf* leaf = dynamic_cast<I_FCTLeaf*>(selected_items.at(0));

		action = new QAction("����", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Copy.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuCopyLeafInAction);
		this->m_mainMenu->addAction(action);
		
		action = new QAction("ճ��", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Paste.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuPasteLeafInAction);
		this->m_mainMenu->addAction(action);

		action = new QAction("���", this);
		action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Clear.png"));
		action->setData(leaf->getId());
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_FlexiblePageTree::menuClearLeafInAction);
		this->m_mainMenu->addAction(action);

		this->m_mainMenu->addSeparator();
	}

	/*------------------------�������------------------------*/
	action = new QAction("չ��ȫ��", this);
	action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Unfold.png"));
	connect(action, &QAction::triggered, this->m_tree, &QTreeWidget::expandAll);
	this->m_mainMenu->addAction(action);

	action = new QAction("����ȫ��", this);
	action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Fold.png"));
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
	action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Refresh.png"));
	connect(action, &QAction::triggered, this, &P_FlexiblePageTree::rebuildTreeData);
	this->m_mainMenu->addAction(action);

	action = new QAction("������...", this);
	action->setIcon(QIcon(this->m_menuIconSrcPath+ "/menu/Common_Setting.png"));
	connect(action, &QAction::triggered, this, &P_FlexiblePageTree::openConfigParamWindow);
	this->m_mainMenu->addAction(action);

}
/*-------------------------------------------------
		�Ҽ��˵� - �����˵���ģʽ��
*/
void P_FlexiblePageTree::drawMenuMode(){
	QAction *action;
	this->m_modeMenu->setTitle("��֧ģʽ");

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

	// > ��ʼ��������
	this->putDataToUi();

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

	// > ��ʼ��������
	this->putDataToUi();

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
	I_FCTLeaf* leaf = nullptr;
	for (int i = 0; i < item_list.count(); i++){
		QTreeWidgetItem* item = item_list.at(i);
		if (this->isLeaf(item)){
			leaf = dynamic_cast<I_FCTLeaf*>(item);;
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

	if (this->m_sortMode == ""){
		QMessageBox::warning(nullptr, "����", "���ؼ��г���������ķ�֧ģʽ���ã��ѹ�ΪĬ�����á�", QMessageBox::Yes);
		this->m_sortMode = "ID��֧����id��������";
	}
	if (this->m_sortMode == "ID��֧����id��������"){
		this->rebuildTreeData_id_inc();
		this->refreshTreeUi();
	}
	if (this->m_sortMode == "���Ʒ�֧�������Ƶ�������"){
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
	int per_num = this->m_config.pagePerNum;
	int page_count = qCeil((double)all_count / per_num);

	// > �����֦
	for (int i = 0; i < page_count; i++){
		int bottom = i * per_num + 1;
		int top = (i + 1) * per_num;
		if (top > all_count){
			top = all_count;
		}
		QString page_name = "";
		page_name = "��" + S_ChineseManager::getInstance()->toChineseNumLower(i + 1) + "ҳ ";

		// > id���
		if (this->m_config.zeroFill){
			page_name += TTool::_zeroFill_(bottom, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1()))
				+ "��" + TTool::_zeroFill_(top, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1()));
		}else{
			page_name += QString::number(bottom) + "��" + QString::number(top);
		}

		I_FCTBranch* branch_item = new I_FCTBranch();
		branch_item->setId(i);							//���
		branch_item->setBranch_id_Index(i);				//ID��֧ - ��ǰҳ������λ��
		branch_item->setBranch_id_Bottom(bottom);		//ID��֧ - ��
		branch_item->setBranch_id_Top(top);				//ID��֧ - ��
		branch_item->setBranch_id_Name(page_name);		//ID��֧ - ����
		this->m_branchItem.append(branch_item);
	}

	// > ��ˢ����Ҷ��
	this->m_leafItem.clear();
	for (int i = 0; i < this->m_branchItem.count(); i++){
		I_FCTBranch* branch_item = this->m_branchItem.at(i);
		int page_index = branch_item->getBranch_id_Index();	//��ǰҳ������λ��
		QList<int> index_list = this->m_source_ObjectSortController->get_IdInc_PageIndexList(page_index, this->m_config.pagePerNum);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > ��Ҷ������������
			I_FCTLeaf* leaf = new I_FCTLeaf(&this->m_config);
			leaf->setId(data->id);					//��ʶ
			leaf->setName(data->name);				//����
			leaf->setType(data->type);				//����
			this->m_leafItem.append(leaf);

			I_FCTBranch* p_item = this->m_branchItem.at(i);
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
	QStringList page_name_list = QStringList() << "- A -" << "- B -" << "- C -" << "- D -" << "- E -" << "- F -" << "- G -" << "- H -" << "- I -" << "- J -" << "- K -" << "- L -" << "- M -" << "- N -" << "- O -" << "- P -" << "- Q -" << "- R -" << "- S -" << "- T -" << "- U -" << "- V -" << "- W -" << "- X -" << "- Y -" << "- Z -" << "��Ƨ��";
	QStringList page_symbol_list = QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z" << "@";
	page_name_list << "������";
	page_symbol_list << "������";
	// > ���ҳ
	for (int i = 0; i < page_name_list.count(); i++){
		QString page_name = page_name_list.at(i);
		QString page_symbol = page_symbol_list.at(i);

		I_FCTBranch* branch_item = new I_FCTBranch();
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
			I_FCTLeaf* leaf = new I_FCTLeaf(&this->m_config);
			leaf->setId(data->id);					//��ʶ
			leaf->setName(data->name);				//����
			leaf->setType(data->type);				//����
			leaf->setLeaf_name_Symbol(symbol);		//���Ʒ�֧ - ����ĸ��ʶ
			this->m_leafItem.append(leaf);

			I_FCTBranch* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}
}