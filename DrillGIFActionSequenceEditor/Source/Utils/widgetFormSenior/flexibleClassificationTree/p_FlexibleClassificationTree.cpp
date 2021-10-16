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
		�ࣺ		�����������������֧��.cpp
		�汾��		v1.04
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ܹ���ʾһ�����ݣ����ҽ���Щ���ݷ����ת�Ƶ���ͬ����֦�У����ڲ�ѯ��
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
						-> �����֧�����಻�������֧���ܣ�
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

	// > ������
	this->m_config = this->createConfigData();

}
P_FlexibleClassificationTree::~P_FlexibleClassificationTree(){
}


/*-------------------------------------------------
		���� - ���� ������ ���ݣ���д���ࣩ
*/
C_FPT_Config* P_FlexibleClassificationTree::createConfigData(){
	return new C_FCT_Config();
}
/*-------------------------------------------------
		���� - ���� ���� ���ݣ��ɸ�д��
*/
C_FCT_Classify* P_FlexibleClassificationTree::createClassifyData(){
	return new C_FCT_Classify();
}
/*-------------------------------------------------
		���� - ���� ���� �༭���ڣ��ɸ�д��
*/
W_FCT_Classify* P_FlexibleClassificationTree::createClassifyWindow(){
	return new W_FCT_Classify(this);
}


/*-------------------------------------------------
		������ - ����ȫ��
*/
void P_FlexibleClassificationTree::clearAll(){
	P_FlexiblePageTree::clearAll();

	// > �����ã�����������Դ��ȡ�޹أ�������

}
/*-------------------------------------------------
		������ - ˢ���� - ��֧
*/
void P_FlexibleClassificationTree::refreshTreeUi_special() {
	P_FlexiblePageTree::refreshTreeUi_special();

	// > Ҷ��λ��ת��
	if (this->getConfigEx()->is_classify_idInc_Mode()){ this->refreshTreeUi_classify_idInc(); }
	if (this->getConfigEx()->is_classify_nameInc_Mode()){ this->refreshTreeUi_classify_nameInc(); }
}
/*-------------------------------------------------
		������ - ˢ���� - ��֧ - �����֧_id����
*/
void P_FlexibleClassificationTree::refreshTreeUi_classify_idInc() {

	// > �����޸ĺ��޸ĵ�Ҷ�ӿ��ܻ��ܵ�����֦��
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
		������ - ˢ���� - ��֧ - �����֧_���Ƶ���
*/
void P_FlexibleClassificationTree::refreshTreeUi_classify_nameInc() {

	// > �����޸ĺ��޸ĵ�Ҷ�ӿ��ܻ��ܵ�����֦��
	this->refreshTreeUi_classify_idInc();	//����idһģһ����

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
		Ҷ�� - ��ȡ - ���������ȡ
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
		Ҷ�� - ��ȡ - ����
*/
QString P_FlexibleClassificationTree::getLeafType(int id){
	I_FPT_Leaf* leaf = this->getLeafById(id);
	if (leaf == nullptr){ return ""; }
	return leaf->getType();
}
/*-------------------------------------------------
		��֦ - ��ȡ��֦�������֧ר�ã�
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
		���� - ��ȡ - �Ƿ�Ϊ�����֧
*/
bool P_FlexibleClassificationTree::isClassifyMode(){
	if (this->getConfigEx()->is_classify_idInc_Mode()){ return true; }
	if (this->getConfigEx()->is_classify_nameInc_Mode()){ return true; }
	return false;
}
/*-------------------------------------------------
		���� - ��ȡ - ȫ��������
*/
QStringList P_FlexibleClassificationTree::getAllClassifyName(){
	return this->getConfigEx()->getAll_classify_NameList();
}

/*-------------------------------------------------
		���� - ���
*/
void P_FlexibleClassificationTree::addClassify(C_FCT_Classify* classify){

	// > ��ӵ�����
	if (this->getConfigEx()->has_classify(classify->getName()) == true){ return; }
	this->getConfigEx()->add_classify(classify);

	// > ��֦�仯
	int index = this->getConfigEx()->get_classify_Index(classify->getName());
	I_FPT_Branch* branch = this->createFCTBranch();
	branch->setId(index);											//���
	branch->setBranch_type_Name(classify->getName());				//�����֧ - ��������
	branch->setBranch_type_Description(classify->getDescription());	//�����֧ - ��������
	this->m_branchItem.insert(index, branch);

	this->refreshTreeUi();
}
/*-------------------------------------------------
		���� - ��ӣ�action��
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
		���� - ��� - ��֤������
*/
void P_FlexibleClassificationTree::addClassifyDistinguishedList(QStringList new_classify_nameList){
	C_FCT_Config* config = this->getConfigEx();

	// > ���� - ����µ�����
	for (int i = 0; i < new_classify_nameList.count(); i++){
		QString classify_name = new_classify_nameList.at(i);
		if (classify_name == ""){ continue; }
		if (config->has_classify(classify_name)){ continue; }

		C_FCT_Classify* c_c = this->createClassifyData();	//����������������ⲿ���޸ģ����������޸ģ�
		c_c->setName(classify_name);
		c_c->setDescription("");
		config->add_classify(c_c);
	}
}
/*-------------------------------------------------
		���� - �޸�
*/
void P_FlexibleClassificationTree::modifyClassify(QString classify_name, C_FCT_Classify* classify){
	if (this->getConfigEx()->has_classify(classify_name) == true){ return; }

	// > �޸�����
	//��ָ�룬ֱ���޸ģ�

	// > ��֦�仯
	I_FPT_Branch* branch = this->getBranchByTypeName(classify_name);
	branch->setBranch_type_Name(classify->getName());				//�����֧ - ��������
	branch->setBranch_type_Description(classify->getDescription());	//�����֧ - ��������

	// > Ҷ�ӱ仯
	QList<I_FPT_Leaf*> leaf_list = this->getLeafByType(classify_name);
	for (int i = 0; i < leaf_list.count(); i++){
		I_FPT_Leaf* leaf = leaf_list.at(i);
		this->innerModifyLeafType(leaf->getId(), classify->getName());
	}

	this->refreshTreeUi();
}
/*-------------------------------------------------
		���� - �޸ģ�action��
*/
void P_FlexibleClassificationTree::modifyClassifyInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QString classify_name = cur_action->data().value<QString>();

	C_FCT_Classify* c_c = this->getConfigEx()->get_classify(classify_name);

	// > �޸Ĵ���
	W_FCT_Classify* d = this->createClassifyWindow();
	d->setDataInModifyMode(c_c);
	if (d->exec() == QDialog::Accepted){
		this->modifyClassify(classify_name, d->getData());
	}
	d->deleteLater();
}
/*-------------------------------------------------
		���� - ȥ��
*/
void P_FlexibleClassificationTree::removeClassify(QString classify_name){
	if (classify_name == ""){ return; }
	if (this->getConfigEx()->has_classify(classify_name) == false){ return; }

	// > ȥ������
	this->getConfigEx()->remove_classify(classify_name);

	// > ��֦�仯
	this->m_branchItem.removeOne(this->getBranchByTypeName(classify_name));

	// > Ҷ�ӱ仯
	QList<I_FPT_Leaf*> leaf_list = this->getLeafByType(classify_name);
	for (int i = 0; i < leaf_list.count(); i++){
		I_FPT_Leaf* leaf = leaf_list.at(i);
		this->innerModifyLeafType(leaf->getId(), "");
	}

	this->refreshTreeUi();
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

	for (int i = classify_list.count()-1; i >=0; i--){
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
	this->getConfigEx()->set_classify_moveUp(classify_name);

	// > ��֦�仯
	int index = this->getConfigEx()->get_classify_Index(classify_name);
	this->m_branchItem.swap(index, index + 1);

	this->refreshTreeUi();
}
/*-------------------------------------------------
		���� - ����
*/
void P_FlexibleClassificationTree::moveDownInAction(){
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	QString classify_name = cur_action->data().value<QString>();

	// > ���ݱ仯
	this->getConfigEx()->set_classify_moveDown(classify_name);

	// > ��֦�仯
	int index = this->getConfigEx()->get_classify_Index(classify_name);
	this->m_branchItem.swap(index, index - 1);

	this->refreshTreeUi();

}


/*-------------------------------------------------
		�޸ĵ����� - ���ڲ��޸ġ�Ҷ�ӵ�����
*/
void P_FlexibleClassificationTree::innerModifyLeafType(int id, QString type){
	I_FPT_Leaf* leaf = this->getLeafById(id);
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
	W_FCT_ClassifySelector d(this);
	d.setData(this->getConfigEx(), classify);
	if (d.exec() == QDialog::Accepted){
		QString name = d.getSelectedData();
		
		// > �����޸�
		for (int i = 0; i < leaf_idList.count(); i++){
			this->innerModifyLeafType(leaf_idList.at(i).toInt(), name);
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
	if (this->isBranchList(selected_items) == true && selected_items.count() == 1 && this->isClassifyMode() == true ){
		I_FPT_Branch* branch = dynamic_cast<I_FPT_Branch*>(selected_items.at(0));
		QString classify_name = branch->getBranch_type_Name();

		int index = this->getConfigEx()->get_classify_Index(classify_name);
		int count = this->getConfigEx()->get_classify_Count();

		action = new QAction("����", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Up.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::moveUpInAction);
		this->m_mainMenu->addAction(action);
		if (index <= 0 || index == count - 1){ action->setEnabled(false); }

		action = new QAction("����", this);
		action->setIcon(QIcon(QRC_IconSrcPath+ "/menu/Common_Down.png"));
		action->setData(classify_name);
		connect(action, &QAction::triggered, this, &P_FlexibleClassificationTree::moveDownInAction);
		this->m_mainMenu->addAction(action);
		if (index >= count - 2){ action->setEnabled(false); }

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
			I_FPT_Branch* branch = dynamic_cast<I_FPT_Branch*>(selected_items.at(i));
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
			I_FPT_Leaf* leaf = dynamic_cast<I_FPT_Leaf*>(selected_items.at(i));
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
		������ - ���ò���
*/
void P_FlexibleClassificationTree::setConfig(C_FPT_Config* config){
	P_FlexiblePageTree::setConfig(config);
}
/*-------------------------------------------------
		������ - ȡ������
*/
C_FPT_Config* P_FlexibleClassificationTree::getConfig(){
	return P_FlexiblePageTree::getConfig();
}
/*-------------------------------------------------
		������ - ���ò���Ex
*/
void P_FlexibleClassificationTree::setConfigEx(C_FCT_Config* config){
	P_FlexiblePageTree::setConfig(config);
}
/*-------------------------------------------------
		������ - ȡ������Ex
*/
C_FCT_Config* P_FlexibleClassificationTree::getConfigEx(){
	return dynamic_cast<C_FCT_Config*>(P_FlexiblePageTree::getConfig());
}

/*-------------------------------------------------
		��Դ���� - �ؽ����ݣ�˽�У�
*/
void P_FlexibleClassificationTree::rebuildTreeData(){
	P_FlexiblePageTree::rebuildTreeData();

	// > �������������
	QStringList new_classify_list = this->m_source_ObjectSortController->get_Type_DistinguishedList();
	this->addClassifyDistinguishedList(new_classify_list);

	// > �ؽ�Ҷ�Ӻ���֦
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
		��Դ���� - �ؽ�����_�����֧_id������˽�У�
*/
void P_FlexibleClassificationTree::rebuildTreeData_classify_idInc(){

	// > ��ˢ������֦
	for (int i = 0; i < this->m_branchItem.count(); i++){
		this->m_branchItem.at(i)->takeChildren();
	}
	this->m_branchItem.clear();

	// > �����֦
	QList<C_FCT_Classify*> classify_list = this->getConfigEx()->get_classify_DataList();
	for (int i = 0; i < classify_list.count(); i++){
		C_FCT_Classify* c_c = classify_list.at(i);

		I_FPT_Branch* branch = this->createFCTBranch();
		branch->setId(i);										//���
		if (c_c->getName() == ""){
			branch->setBranch_type_Name("");					//�����֧ - ��������
		}else{
			branch->setBranch_type_Name(c_c->getName());		//�����֧ - ��������
		}
		branch->setBranch_type_Description(c_c->getDescription());	//�����֧ - ��������
		this->m_branchItem.append(branch);
	}

	// > ��ˢ����Ҷ��
	this->m_leafItem.clear();
	QStringList name_list = this->getAllClassifyName();
	for (int i = 0; i < name_list.count(); i++){
		QString classify_name = name_list.at(i);
		QList<int> index_list = this->m_source_ObjectSortController->get_TypeAndId_IndexListByType(classify_name);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > ��Ҷ������������
			I_FPT_Leaf* leaf = this->createFCTLeaf();
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
		��Դ���� - �ؽ�����_�����֧_���Ƶ�����˽�У�
*/
void P_FlexibleClassificationTree::rebuildTreeData_classify_nameInc(){
	//��������ĺ���һģһ������ͬ����Controller����get_TypeAndName_IndexListByType��

	// > ��ˢ������֦
	for (int i = 0; i < this->m_branchItem.count(); i++){this->m_branchItem.at(i)->takeChildren();}
	this->m_branchItem.clear();
	// > �����֦
	QList<C_FCT_Classify*> classify_list = this->getConfigEx()->get_classify_DataList();
	for (int i = 0; i < classify_list.count(); i++){
		C_FCT_Classify* c_c = classify_list.at(i);
	
		I_FPT_Branch* branch = this->createFCTBranch();
		branch->setId(i);										//���
		if (c_c->getName() == ""){
			branch->setBranch_type_Name("");					//�����֧ - ��������
		}else{
			branch->setBranch_type_Name(c_c->getName());		//�����֧ - ��������
		}
		branch->setBranch_type_Description(c_c->getDescription());	//�����֧ - ��������
		this->m_branchItem.append(branch);
	}
	// > ��ˢ����Ҷ��
	this->m_leafItem.clear();
	QStringList name_list = this->getAllClassifyName();
	for (int i = 0; i < name_list.count(); i++){
		QString classify_name = name_list.at(i);
		QList<int> index_list = this->m_source_ObjectSortController->get_TypeAndName_IndexListByType(classify_name);
		QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
		for (int j = 0; j < data_list.count(); j++){
			C_ObjectSortData* data = data_list.at(j);

			// > ��Ҷ������������
			I_FPT_Leaf* leaf = this->createFCTLeaf();
			leaf->setId(data->id);					//��ʶ
			leaf->setName(data->name);				//����
			leaf->setType(data->type);				//����
			this->m_leafItem.append(leaf);

			QTreeWidgetItem* p_item = this->m_branchItem.at(i);
			p_item->addChild(leaf);
		}
	}


}