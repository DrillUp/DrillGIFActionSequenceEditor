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
		�ࣺ		��������������Զ����֧��.cpp
		�汾��		v1.02
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ܹ���ʾһ�����ݣ����ҽ���Щ�������ͻ�ת�Ƶ���ͬ����֦�У����ڲ�ѯ��
					ע�⣬������װ�κ���Ҫ�Ըÿؼ�����н�������Ҫ��������ֱ�ӽ�����

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
						-> �Զ����֧�����಻���Զ����֧���ܣ�
							-> ��֦�Ҽ�������ӷ���
							-> Ҷ���Ҽ�������ӷ���/�ƶ������ࣨ��ȥ�����з��ࣩ
							-> ͬһ����������ڶ�������г��֣���������ڶ���ѡ�������� ��ͻ��
					-> ������
						-> �����и�
						-> ����Ҷ����ʾ�ı�

		˵����		���������ֱ�Ӱ�����һ��container���ݴ洢�ṹ��
					������������Container��WindowManager�Ĵ���ģʽ��
					
		ʹ�÷�����
				>��ʼ��
					this->m_p_FlexibleClassificationTree = new P_FlexibleClassificationTree(ui.treeWidget_2);
					this->m_p_FlexibleClassificationTree->setData(obj);		//���洢��������Ҫ��loadǰ��ɸ�ֵ��
					this->m_p_FlexibleClassificationTree->loadSource(data_list, "id", "name", "type");

-----==========================================================-----
*/
P_FlexibleClassificationTree::P_FlexibleClassificationTree(QTreeWidget *parent)
	: P_FlexiblePageTree(parent)
{

	//-----------------------------------
	//----ui��ʼ��

	// > ����
	this->m_sortModeList << "�Զ����֧����id��������" << "�Զ����֧�������Ƶ�������";
	this->m_classifyData = QList<C_FCTClassify>();

}
P_FlexibleClassificationTree::~P_FlexibleClassificationTree(){
}


/*-------------------------------------------------
		�ؼ� - ˢ����
*/
void P_FlexibleClassificationTree::refreshTreeUi_special() {
	P_FlexiblePageTree::refreshTreeUi_special();

	// > Ҷ��λ��ת��
	if (this->m_sortMode == "�Զ����֧����id��������"){ this->refreshTreeUi_classify_idInc(); }
	if (this->m_sortMode == "�Զ����֧�������Ƶ�������"){ this->refreshTreeUi_classify_nameInc(); }
}
/*-------------------------------------------------
		�ؼ� - ˢ����_�Զ����֧_id����
*/
void P_FlexibleClassificationTree::refreshTreeUi_classify_idInc() {

	// > �����޸ĺ��޸ĵ�Ҷ�ӿ��ܻ��ܵ�����֦��
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
		�ؼ� - ˢ����_�Զ����֧_���Ƶ���
*/
void P_FlexibleClassificationTree::refreshTreeUi_classify_nameInc() {

	// > �����޸ĺ��޸ĵ�Ҷ�ӿ��ܻ��ܵ�����֦��
	this->refreshTreeUi_classify_idInc();	//����idһģһ����

}
/*-------------------------------------------------
		�ؼ� - ����ȫ��
*/
void P_FlexibleClassificationTree::clearAll(){
	P_FlexiblePageTree::clearAll();

	// > ��������
	this->m_classifyData.clear();
}


/*-------------------------------------------------
		Ҷ�� - ���ⲿ�޸ġ�Ҷ������
*/
void P_FlexibleClassificationTree::outerModifyLeafName(int id, QString name){
	P_FlexiblePageTree::outerModifyLeafName(id, name);
	//���ޱ仯��
}
/*-------------------------------------------------
		Ҷ�� - ���ⲿ�޸ġ�Ҷ������
*/
void P_FlexibleClassificationTree::outerModifyLeafType(int id, QString name){
	P_FlexiblePageTree::outerModifyLeafType(id, name);
	// ���ޱ仯��
}
/*-------------------------------------------------
		Ҷ�� - ���ⲿ�޸ġ�ѡ�е�Ҷ������
*/
void P_FlexibleClassificationTree::outerModifySelectedLeafName( QString name){
	P_FlexiblePageTree::outerModifySelectedLeafName( name);
	//���ޱ仯��
}
/*-------------------------------------------------
		Ҷ�� - ���ⲿ�޸ġ�ѡ�е�Ҷ������
*/
void P_FlexibleClassificationTree::outerModifySelectedLeafType( QString name){
	P_FlexiblePageTree::outerModifySelectedLeafType( name);
	// ���ޱ仯��
}
/*-------------------------------------------------
		Ҷ�� - ��ȡ - �������ͻ�ȡ
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
		Ҷ�� - ��ȡ - ����
*/
QString P_FlexibleClassificationTree::getLeafType(int id){
	I_FCTLeaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return ""; }
	return leaf->getType();
}
/*-------------------------------------------------
		��֦ - ��ȡ��֦���Զ����֧ר�ã�
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
		���� - �������� -> ui����
*/
void P_FlexibleClassificationTree::putDataToUi(){
	P_FlexiblePageTree::putDataToUi();

	// > ���� 
	this->m_classifyData.clear();

	// > ���� - �洢������
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

	// > ���� - ����µ�����
	QStringList other_classify_list = this->m_source_ObjectSortController->get_Type_DistinguishedList();
	for (int i = 0; i < other_classify_list.count(); i++){
		QString classify_name = other_classify_list.at(i);
		if (classify_name != "" && this->hasClassify(classify_name) == false){

			C_FCTClassify c_c = C_FCTClassify();	//����������������ⲿ���޸ģ����������޸ģ�
			c_c.name = classify_name;
			c_c.description = "";
			this->m_classifyData.append(c_c);
		}
	}

	// > ���� - δ���ࣨ������ӣ�
	this->m_classifyData.append(C_FCTClassify());

}
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void P_FlexibleClassificationTree::putUiToData(){
	P_FlexiblePageTree::putUiToData();

	// > �������ݣ��Զ����֧��
	QJsonArray arr = QJsonArray();
	for (int i = 0; i < this->m_classifyData.count(); i++){
		C_FCTClassify c_c = this->m_classifyData.at(i);
		if (c_c.isNull() == false){		//���ų�"δ����"��
			arr.append(c_c.getJsonObject());
		}
	}
	this->local_treeData.insert("FCTClassifyList", arr);

}


/*-------------------------------------------------
		���� - ��ȡ - ȫ������
*/
QStringList P_FlexibleClassificationTree::getAllClassify(){
	QStringList result_list = QStringList();
	for (int i = 0; i < this->m_classifyData.count(); i++){
		result_list.append(this->m_classifyData.at(i).name);
	}
	return result_list;
}
/*-------------------------------------------------
		���� - ��ȡ - �ж����ʹ���
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
		���� - ��ȡ - �ж����ʹ���
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
		���� - ��ǰΪ�Զ����֧ģʽ
*/
bool P_FlexibleClassificationTree::isClassifyMode(){
	return this->m_sortMode.contains("�Զ����֧");
}
/*-------------------------------------------------
		���� - ���
*/
void P_FlexibleClassificationTree::addClassify(C_FCTClassify classify){
	if (this->hasClassify(classify.name) == true){ return; }

	// > ��Ӷ���
	int index = this->m_classifyData.count() - 1;
	this->m_classifyData.insert(index, classify);

	// > ��֦�仯
	I_FCTBranch* branch = new I_FCTBranch();
	branch->setId(index);										//���
	branch->setBranch_type_Name(classify.name);					//�Զ����֧ - ��������
	branch->setBranch_type_Description(classify.description);	//�Զ����֧ - ��������
	this->m_branchItem.insert(index, branch);

	this->refreshTreeUi();
}
/*-------------------------------------------------
		���� - �޸�
*/
void P_FlexibleClassificationTree::modifyClassify(QString classify_name, C_FCTClassify classify){
	if (this->hasClassify(classify_name) == false){ return; }

	// > �޸Ķ���
	for (int i = 0; i < this->m_classifyData.count(); i++){
		C_FCTClassify c_c = this->m_classifyData.at(i);
		if (c_c.name == classify_name){
			this->m_classifyData.replace(i, classify);
			break;
		}
	}

	// > ��֦�仯
	I_FCTBranch* branch = this->getBranchByTypeName(classify_name);
	branch->setBranch_type_Name(classify.name);					//�Զ����֧ - ��������
	branch->setBranch_type_Description(classify.description);	//�Զ����֧ - ��������

	// > Ҷ�ӱ仯
	QList<I_FCTLeaf*> leaf_list = this->getLeafByType(classify_name);
	for (int i = 0; i < leaf_list.count(); i++){
		I_FCTLeaf* leaf = leaf_list.at(i);
		this->innerModifyLeafType(leaf->getId(), classify.name);
	}

	this->refreshTreeUi();
}
/*-------------------------------------------------
		���� - ȥ��
*/
void P_FlexibleClassificationTree::removeClassify(QString classify_name){
	if (this->hasClassify(classify_name) == false){ return; }
	if ( classify_name == ""){ return; }

	// > ȥ������
	for (int i = 0; i < this->m_classifyData.count(); i++){
		C_FCTClassify c_c = this->m_classifyData.at(i);
		if (c_c.name == classify_name){
			this->m_classifyData.removeAt(i);
			break;
		}
	}

	// > ��֦�仯
	this->m_branchItem.removeOne(this->getBranchByTypeName(classify_name));

	// > Ҷ�ӱ仯
	QList<I_FCTLeaf*> leaf_list = this->getLeafByType(classify_name);
	for (int i = 0; i < leaf_list.count(); i++){
		I_FCTLeaf* leaf = leaf_list.at(i);
		this->innerModifyLeafType(leaf->getId(), "");
	}

	this->refreshTreeUi();
}
/*-------------------------------------------------
		���� - ��ӣ�action��
*/
void P_FlexibleClassificationTree::addClassifyInAction(){
	W_FCTClassify d(this->m_tree);
	d.setDataInAddMode( &this->m_classifyData );
	if (d.exec() == QDialog::Accepted){
		this->addClassify(d.getData());
	}
}
/*-------------------------------------------------
		���� - �޸ģ�action��
*/
void P_FlexibleClassificationTree::modifyClassifyInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
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
		���� - ȥ����action��
*/
void P_FlexibleClassificationTree::removeClassifyInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QString classify_name = cur_action->data().value<QString>();

	this->removeClassify(classify_name);
}
/*-------------------------------------------------
		���� - ȥ����action��
*/
void P_FlexibleClassificationTree::removeClassifyListInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QStringList classify_list = cur_action->data().value<QStringList>();

	for (int i = 0; i < classify_list.count(); i++){
		this->removeClassify(classify_list.at(i));
	}
}
/*-------------------------------------------------
		���� - ����
*/
void P_FlexibleClassificationTree::moveUpInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QString classify_name = cur_action->data().value<QString>();

	// > ���ݱ仯
	int index = this->indexOfClassify(classify_name);
	if (index == -1){ return; }
	if (index == 0){ return; }
	if (index == this->m_classifyData.count() - 1){ return; }
	this->m_classifyData.swap(index, index - 1);

	// > ��֦�仯
	this->m_branchItem.swap(index, index - 1);

	this->refreshTreeUi();
}
/*-------------------------------------------------
		���� - ����
*/
void P_FlexibleClassificationTree::moveDownInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QString classify_name = cur_action->data().value<QString>();

	// > ���ݱ仯
	int index = this->indexOfClassify(classify_name);
	if (index == -1){ return; }
	if (index >= this->m_classifyData.count() - 2){ return; }
	this->m_classifyData.swap(index, index + 1);

	// > ��֦�仯
	this->m_branchItem.swap(index, index + 1);

	this->refreshTreeUi();

}


/*-------------------------------------------------
		�޸ĵ����� - ���ڲ��޸ġ�Ҷ�ӵ�����
*/
void P_FlexibleClassificationTree::innerModifyLeafType(int id, QString type){
	I_FCTLeaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return; }

	// > Ҷ�ӱ仯
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

	// > ��¼�ı�
	C_ObjectSortData changeData = C_ObjectSortData();
	changeData.id = leaf->getId();
	changeData.type = leaf->getType();
	this->appendChangedSource(changeData);

}
void P_FlexibleClassificationTree::innerModifyLeafTypeInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QStringList leaf_idList = cur_action->data().value<QStringList>();

	// > ��ȡѡ�е�Ҷ��
	if (leaf_idList.count() == 0){ return; }
	QString id_str = leaf_idList.at(0);
	QString classify = this->getLeafById(id_str.toInt())->getType();

	// > ѡ������
	W_FCTClassifySelector d(this->m_tree);
	d.setDataInModifyMode(classify, &this->m_classifyData);
	connect(&d, &W_FCTClassifySelector::classifyAdded, this, &P_FlexibleClassificationTree::addClassify);
	if (d.exec() == QDialog::Accepted){
		C_FCTClassify c_c = d.getSelectedData();
		
		// > �����޸�
		for (int i = 0; i < leaf_idList.count(); i++){
			this->innerModifyLeafType(leaf_idList.at(i).toInt(), c_c.name);
		}
		this->refreshTreeUi();
	}

}
/*-------------------------------------------------
		�޸ĵ����� - ��¼�޸ģ�ֻ��type������
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
		�޸ĵ����� - �޸ĵ����ݣ�ֻ��type������
*/
QList<C_ObjectSortData> P_FlexibleClassificationTree::getChangedSource(){
	return this->m_changedSource;
}


/*-------------------------------------------------
		�Ҽ��˵� - һ���˵�
*/
void P_FlexibleClassificationTree::drawMenuMain(){
	QAction *action;
	QList<QTreeWidgetItem*> selected_items = this->m_tree->selectedItems();

	/*------------------------�Ҽ����������֦------------------------*/
	if (this->isBranchList(selected_items) == true && selected_items.count() == 1 && this->isClassifyMode() == true){
		I_FCTBranch* branch = dynamic_cast<I_FCTBranch*>(selected_items.at(0));
		QString classify_name = branch->getBranch_type_Name();


		action = new QAction("����", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Up.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::moveUpInAction);
		this->m_mainMenu->addAction(action);
		if (this->indexOfClassify(classify_name) <= 0 || this->indexOfClassify(classify_name) == this->m_classifyData.count() - 1){ action->setEnabled(false); }

		action = new QAction("����", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Down.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::moveDownInAction);
		this->m_mainMenu->addAction(action);
		if (this->indexOfClassify(classify_name) >= this->m_classifyData.count() - 2 ){ action->setEnabled(false); }

		action = new QAction("�������", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Add.png"));
		//������Ҫclassify_name��
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::addClassifyInAction);
		this->m_mainMenu->addAction(action);

		action = new QAction("�༭����", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Edit.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::modifyClassifyInAction);
		this->m_mainMenu->addAction(action);
		if (classify_name == ""){ action->setEnabled(false); }		//��δ�������֦���ܱ༭��

		action = new QAction("ɾ������", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Delete.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::removeClassifyInAction);
		this->m_mainMenu->addAction(action);
		if (classify_name == ""){ action->setEnabled(false); }		//��δ�������֦���ܱ༭��

		this->m_mainMenu->addSeparator();
	}

	/*------------------------�Ҽ���������֦------------------------*/
	if (this->isBranchList(selected_items) == true && selected_items.count() > 1 && this->isClassifyMode() == true){
		QStringList classify_nameList = QStringList();
		for (int i = 0; i < selected_items.count(); i++){
			I_FCTBranch* branch = dynamic_cast<I_FCTBranch*>(selected_items.at(i));
			QString classify_name = branch->getBranch_type_Name();
			if (classify_name == ""){ continue; }
			classify_nameList.append(classify_name);
		}
		
		action = new QAction("ɾ��ѡ�е�����", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Delete.png"));
		action->setData(classify_nameList);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::removeClassifyListInAction);
		this->m_mainMenu->addAction(action);

		this->m_mainMenu->addSeparator();
	}

	/*------------------------�Ҽ������������Ҷ��------------------------*/
	if (this->isLeafList(selected_items) == true && this->isClassifyMode() == true){
		QTreeWidgetItem *item = selected_items.at(0);
		QStringList leaf_idList = QStringList();
		for (int i = 0; i < selected_items.count(); i++){
			I_FCTLeaf* leaf = dynamic_cast<I_FCTLeaf*>(selected_items.at(i));
			QString id_str = QString::number( leaf->getId() );
			leaf_idList.append(id_str);
		}

		action = new QAction("�ƶ���...", this);
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
		�Ҽ��˵� - �����˵���ģʽ��
*/
void P_FlexibleClassificationTree::drawMenuMode(){
	P_FlexiblePageTree::drawMenuMode();
	
}


/*-------------------------------------------------
		��Դ���� - �ؽ����ݣ�˽�У�
*/
void P_FlexibleClassificationTree::rebuildTreeData(){
	P_FlexiblePageTree::rebuildTreeData();

	if (this->m_sortMode == "�Զ����֧����id��������"){
		this->rebuildTreeData_classify_idInc();
		this->refreshTreeUi();
	}
	if (this->m_sortMode == "�Զ����֧�������Ƶ�������"){
		this->rebuildTreeData_classify_nameInc();
		this->refreshTreeUi();
	}
}
/*-------------------------------------------------
		��Դ���� - �ؽ�����_�Զ����֧_id������˽�У�
*/
void P_FlexibleClassificationTree::rebuildTreeData_classify_idInc(){

	// > ��ˢ������֦
	for (int i = 0; i < this->m_branchItem.count(); i++){
		this->m_branchItem.at(i)->takeChildren();
	}
	this->m_branchItem.clear();

	// > �����֦
	for (int i = 0; i < this->m_classifyData.count(); i++){
		C_FCTClassify c_c = this->m_classifyData.at(i);

		I_FCTBranch* branch = new I_FCTBranch();
		branch->setId(i);										//���
		if (c_c.name == ""){
			branch->setBranch_type_Name("");					//�Զ����֧ - ��������
		}else{
			branch->setBranch_type_Name(c_c.name);				//�Զ����֧ - ��������
		}
		branch->setBranch_type_Description(c_c.description);	//�Զ����֧ - ��������
		this->m_branchItem.append(branch);
	}

	// > ��ˢ����Ҷ��
	this->m_leafItem.clear();
	QStringList classify_list = this->getAllClassify();
	for (int i = 0; i < classify_list.count(); i++){
		QString classify_name = classify_list.at(i);
		QList<int> index_list = this->m_source_ObjectSortController->get_TypeAndId_IndexListByType(classify_name);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > ��Ҷ������������
			I_FCTLeaf* leaf = new I_FCTLeaf(&this->m_config);
			leaf->setId(data->id);					//��ʶ
			leaf->setName(data->name);				//����
			leaf->setType(data->type);				//����
			this->m_leafItem.append(leaf);

			QTreeWidgetItem* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}

}
/*-------------------------------------------------
		��Դ���� - �ؽ�����_�Զ����֧_���Ƶ�����˽�У�
*/
void P_FlexibleClassificationTree::rebuildTreeData_classify_nameInc(){
	//��������ĺ���һģһ������ͬ����Controller����get_TypeAndName_IndexListByType��

	// > ��ˢ������֦
	for (int i = 0; i < this->m_branchItem.count(); i++){this->m_branchItem.at(i)->takeChildren();}
	this->m_branchItem.clear();
	// > �����֦
	for (int i = 0; i < this->m_classifyData.count(); i++){
		C_FCTClassify c_c = this->m_classifyData.at(i);
	
		I_FCTBranch* branch = new I_FCTBranch();
		branch->setId(i);										//���
		if (c_c.name == ""){
			branch->setBranch_type_Name("");					//�Զ����֧ - ��������
		}
		else{
			branch->setBranch_type_Name(c_c.name);				//�Զ����֧ - ��������
		}
		branch->setBranch_type_Description(c_c.description);	//�Զ����֧ - ��������
		this->m_branchItem.append(branch);
	}
	// > ��ˢ����Ҷ��
	this->m_leafItem.clear();
	QStringList classify_list = this->getAllClassify();
	for (int i = 0; i < classify_list.count(); i++){
		QString classify_name = classify_list.at(i);
		QList<int> index_list = this->m_source_ObjectSortController->get_TypeAndName_IndexListByType(classify_name);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > ��Ҷ������������
			I_FCTLeaf* leaf = new I_FCTLeaf(&this->m_config);
			leaf->setId(data->id);					//��ʶ
			leaf->setName(data->name);				//����
			leaf->setType(data->type);				//����
			this->m_leafItem.append(leaf);

			QTreeWidgetItem* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}


}