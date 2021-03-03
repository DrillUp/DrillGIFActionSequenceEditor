#include "stdafx.h"
#include "p_RadioTable.h"

#include "private/w_RaTConfig.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		��ѡ���.cpp
		�汾��		v1.02
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		������ȫ����ʾ�����ܵ�ѡ�������������ܣ�

		�ӹ��ܣ�	-> �ı�ѡ��
						-> ��ѡ
						-> ��ѡ
						-> �Զ����
					-> �������
						-> �����и�
						-> ����Ҷ����ʾ�ı�
					-> �Ҽ��˵�
						-> ֻ��ѡʱ��Ч
					
		ʹ�÷�����
				>��ʼ��
					this->m_p_RadioTable = new P_RadioTable(ui.treeWidget);
					this->m_p_RadioTable->setData(obj);		
					
-----==========================================================-----
*/
P_RadioTable::P_RadioTable(QTableWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----������ʼ��
	this->m_iconSrcPath = ":/DrillGIFActionSequenceEditor/Resources/icons";
	this->m_table = parent;									//������
	this->m_tableStyle = this->m_table->styleSheet();		//���Ĭ����ʽ
	this->m_itemTank = QList<QTableWidgetItem*>();			//������б�
	this->m_last_index = -1;								//��һ��ѡ�е�������

	// > ����
	this->m_config = C_RaTConfig();

	// > �Ҽ��˵�
	this->m_itemOuterControlEnabled = false;				//�༭ - ���أ�Ĭ�Ϲرգ�
	this->m_itemOuterControl_CopyActive = true;				//�༭ - ���Ƽ���
	this->m_itemOuterControl_PasteActive = false;			//�༭ - ճ������
	this->m_itemOuterControl_ClearActive = true;			//�༭ - ��ռ���

	//-----------------------------------
	//----ui��ʼ��
	this->m_table->setContextMenuPolicy(Qt::CustomContextMenu);									//�����Ҽ�
	this->m_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);			//��ѡ
	this->m_table->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);		//ѡ��

	this->m_table->setColumnCount(1);
	this->m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);				//������Ӧ
	this->m_table->horizontalHeader()->setVisible(false);
	this->m_table->verticalHeader()->setVisible(false);

	//-----------------------------------
	//----�¼���
	this->m_selectionSignalBlock_Root = false;				//������
	connect(this->m_table, &QTableWidget::customContextMenuRequested, this, &P_RadioTable::sltItemRightClicked);
	connect(this->m_table, &QTableWidget::itemSelectionChanged, this, &P_RadioTable::sltItemSelectionChanged);

}
P_RadioTable::~P_RadioTable(){
}


/*-------------------------------------------------
		�ؼ� - ˢ�±��
*/
void P_RadioTable::refreshTableUi() {
	this->m_selectionSignalBlock_Root = true;
	QList<int> last_selected_index = this->getSelectedIndex_Multi();

	// > ����
	this->m_table->clearContents();
	this->m_table->setRowCount(local_text.count());

	// > ˢ������
	this->m_itemTank = QList<QTableWidgetItem*>();
	for (int i = 0; i < local_text.count(); i++){
		QTableWidgetItem* item = new QTableWidgetItem();
		if (this->m_config.zeroFill == true){
			item->setText(TTool::_zeroFill_(i + 1, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1())) + " " + local_text.at(i));
		}else{
			item->setText(QString::number(i+1));
		}
		item->setData(Qt::UserRole + 1, local_text.at(i));	//����
		this->m_table->setItem(i, 0, item);
		this->m_itemTank.append(item);
	}

	// > ��ѡ/��ѡ�л�
	if (this->m_config.isMultiSelect == true && this->m_table->selectionMode() == QAbstractItemView::SelectionMode::SingleSelection){
		this->m_table->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	}
	if (this->m_config.isMultiSelect == false && this->m_table->selectionMode() != QAbstractItemView::SelectionMode::SingleSelection){
		this->m_table->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);	
	}

	// > �и�ˢ��
	this->m_table->setStyleSheet(this->m_tableStyle + "\n QTableView::item { height: " + QString::number(this->m_config.rowHeight) + "px;}");

	// > ѡ�б仯��
	if (this->m_config.isMultiSelect){
		this->selectIndex_Multi(last_selected_index);
	}else{
		if (last_selected_index.count() > 0){
			this->selectIndex(last_selected_index.at(0));
		}
	}
	this->m_selectionSignalBlock_Root = false;
}
/*-------------------------------------------------
		�ؼ� - �޸�ָ��λ���ı��������źţ�
*/
void P_RadioTable::modifyText(int index, QString text){
	if (index == -1 ){ return; }
	if (index < 0){ index = 0; }
	if (index >= this->m_itemTank.count()){ index = this->m_itemTank.count() - 1; }

	QTableWidgetItem* item = this->m_itemTank.at(index);
	item->setText(TTool::_zeroFill_(index + 1, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1())) + " " + text);
	item->setData(Qt::UserRole + 1, text);
}
/*-------------------------------------------------
		�ؼ� - �޸�ѡ�����ı��������źţ�
*/
void P_RadioTable::modifyText_Selected(QString text){
	this->modifyText(this->getSelectedIndex(),text);
}
/*-------------------------------------------------
		�ؼ� - ����ȫ��
*/
void P_RadioTable::clearAll(){

	// > �ؼ�����
	this->m_itemTank.clear();
	this->m_table->clearContents();
	this->m_table->setRowCount(0);

	// > �������
	this->m_config = C_RaTConfig();
	this->m_itemOuterControl_PasteActive = false;
}
/*-------------------------------------------------
		�ؼ� - ���������
*/
int P_RadioTable::count(){
	return this->m_itemTank.count();
}
/*-------------------------------------------------
		�ؼ� - ��ȡ�ı�
*/
QString P_RadioTable::getRealText(QTableWidgetItem* item){
	if (item == nullptr){ return ""; }
	return item->data(Qt::UserRole + 1).toString();
}

/*-------------------------------------------------
		������� - ���ò���
*/
void P_RadioTable::setConfigParam(C_RaTConfig config){
	this->m_config = config;
	this->refreshTableUi();
}
C_RaTConfig P_RadioTable::setConfigParam(){
	return this->m_config;
}
/*-------------------------------------------------
		������� - ���ò���
*/
void P_RadioTable::setConfigParam_obj(QJsonObject config){
	this->m_config.setJsonObject(config);
	this->refreshTableUi();
}
QJsonObject P_RadioTable::getConfigParam_obj(){
	return this->m_config.getJsonObject();
}
/*-------------------------------------------------
		������� - �༭����
*/
void P_RadioTable::openConfigParamWindow(){
	W_RaTConfig d(this->m_table);
	d.setDataInModifyMode(this->m_config);
	if (d.exec() == QDialog::Accepted){
		this->m_config = d.getData() ;
		this->refreshTableUi();
	}
}


/*-------------------------------------------------
		����¼� -  Ȧѡ�仯�¼�
*/
void P_RadioTable::sltItemSelectionChanged(){
	if (this->m_selectionSignalBlock_Root == true){ return; }
	this->m_selectionSignalBlock_Root = true;
	
	QList<QTableWidgetItem*> selected_item = this->m_table->selectedItems();
	if( selected_item.count() == 1 ){

		int index = this->getSelectedIndex();
		if (this->m_last_index != index){
			this->m_last_index = index;
			emit currentIndexChanged(index);
			emit currentTextChanged(this->getSelectedText());
			emit currentIndexChanged_Multi(this->getSelectedIndex_Multi());
		}

	}else if (selected_item.count() > 1){

		emit currentIndexChanged_Multi(this->getSelectedIndex_Multi());
	}

	this->m_selectionSignalBlock_Root = false;
}
/*-------------------------------------------------
		����¼� - �Ҽ��¼�
*/
void P_RadioTable::sltItemRightClicked(QPoint point) {
	if (this->m_selectionSignalBlock_Root == true){ return; }
	this->m_selectionSignalBlock_Root = true;
	QTableWidgetItem *item = this->m_table->itemAt(point);	//����ǰ�Ҽ�ѡ�е��
	if (item == nullptr){ return; }
	if (this->m_table->selectedItems().count() == 0){ return; }

	/*------------------------�Ҽ��������Ҷ��------------------------*/
	if (this->m_table->selectedItems().count() == 1 && this->m_itemOuterControlEnabled == true ){
		QAction* action;
		QMenu* menu = new QMenu(this->m_table);

		action = new QAction("����", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Copy.png"));
		action->setData( this->m_itemTank.indexOf(item) );
		action->setShortcut(QKeySequence::Copy);
		connect(action, &QAction::triggered, this, &P_RadioTable::menuCopyItemInAction);
		menu->addAction(action);
		action->setEnabled(this->m_itemOuterControl_CopyActive);

		action = new QAction("ճ��", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Paste.png"));
		action->setData( this->m_itemTank.indexOf(item) );
		action->setShortcut(QKeySequence::Paste);
		connect(action, &QAction::triggered, this, &P_RadioTable::menuPasteItemInAction);
		menu->addAction(action);
		action->setEnabled(this->m_itemOuterControl_PasteActive);

		action = new QAction("���", this);
		action->setIcon(QIcon(this->m_iconSrcPath + "/menu/Common_Clear.png"));
		action->setData( this->m_itemTank.indexOf(item) );
		action->setShortcut(QKeySequence::Delete);
		connect(action, &QAction::triggered, this, &P_RadioTable::menuClearItemInAction);
		menu->addAction(action);
		action->setEnabled(this->m_itemOuterControl_ClearActive);

		menu->exec(QCursor::pos());
		menu->deleteLater();
	}

	this->m_selectionSignalBlock_Root = false;
}


/*-------------------------------------------------
		��Դ���� - ��������
*/
void P_RadioTable::setSource(QStringList text_list) {
	this->local_text = text_list;
	this->refreshTableUi();
}
/*-------------------------------------------------
		��Դ���� - ȡ������
*/
int P_RadioTable::getSelectedIndex(){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		if (this->m_itemTank.at(i)->isSelected()){
			return i;
		}
	}
	return -1;
}
QString P_RadioTable::getSelectedText(){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (item->isSelected()){
			return this->getRealText( item );
		}
	}
	return "";
}
/*-------------------------------------------------
		��Դ���� - ȡ�����ݣ���ѡ��
*/
QList<int> P_RadioTable::getSelectedIndex_Multi(){
	QList<int> result_list = QList<int>();
	for (int i = 0; i < this->m_itemTank.count(); i++){
		if (this->m_itemTank.at(i)->isSelected()){
			result_list.append(i);
		}
	}
	return result_list;
}
QList<QString> P_RadioTable::getSelectedText_Multi(){
	QList<QString> result_list = QList<QString>();
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (item->isSelected()){
			result_list.append(this->getRealText( item ));
		}
	}
	return result_list;
}


/*-------------------------------------------------
		ѡ�� - ����ѡ��
*/
void  P_RadioTable::selectIndex(int index){
	if (index < 0){ index = 0; }
	if (index >= this->m_itemTank.count()){ index = this->m_itemTank.count() - 1; }

	QString c_text = "";
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (i == index){
			item->setSelected(true);
			c_text = this->getRealText(item);
			this->m_table->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
	if (this->m_selectionSignalBlock_Root == false){
		emit currentIndexChanged(index);
		emit currentTextChanged(c_text);
		emit currentIndexChanged_Multi(QList<int>() << index);
	}
}
void  P_RadioTable::selectText(QString text){
	QString c_text = "";
	int index = -1;
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);

		if (this->getRealText(item) == text &&
			index == -1){
			item->setSelected(true);
			c_text = this->getRealText(item);
			this->m_table->scrollToItem(item);
			index = i;
		}else{
			item->setSelected(false);
		}
	}
	if (this->m_selectionSignalBlock_Root == false && index != -1){
		emit currentIndexChanged(index);
		emit currentTextChanged(c_text);
		emit currentIndexChanged_Multi(QList<int>() << index);
	}
}
/*-------------------------------------------------
		ѡ�� - ����ѡ�У���ѡ��
*/
void  P_RadioTable::selectIndex_Multi(QList<int> index_list){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (index_list.contains(i)){
			item->setSelected(true);
			this->m_table->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
	if (this->m_selectionSignalBlock_Root == false){
		emit currentIndexChanged_Multi(this->getSelectedIndex_Multi());
	}
}
void  P_RadioTable::selectText_Multi(QList<QString> text_list){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (text_list.contains(this->getRealText(item))){
			item->setSelected(true);
			this->m_table->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
	if (this->m_selectionSignalBlock_Root == false){
		emit currentIndexChanged_Multi(this->getSelectedIndex_Multi());
	}
}
/*-------------------------------------------------
		ѡ�� - ѡ����һ��
*/
void  P_RadioTable::selectLast(){
	this->selectIndex(this->getSelectedIndex() - 1);
}
/*-------------------------------------------------
		ѡ�� - ѡ����һ��
*/
void  P_RadioTable::selectNext(){
	this->selectIndex(this->getSelectedIndex() + 1);
}
/*-------------------------------------------------
		ѡ�� - ѡ������
*/
void  P_RadioTable::selectStart(){
	this->selectIndex(0);
}
/*-------------------------------------------------
		ѡ�� - ѡ��β��
*/
void  P_RadioTable::selectEnd(){
	this->selectIndex(this->m_itemTank.count() - 1);
}


/*-------------------------------------------------
		�ӿ� - ���أ�Ĭ�Ϲرգ�
*/
void P_RadioTable::setItemOuterControlEnabled(bool enabled){
	this->m_itemOuterControlEnabled = enabled;
}
/*-------------------------------------------------
		�ӿ� - �������
*/
void P_RadioTable::setItemOuterControl_CopyActive(bool enabled){
	this->m_itemOuterControl_CopyActive = enabled;
}
void P_RadioTable::setItemOuterControl_PasteActive(bool enabled){
	this->m_itemOuterControl_PasteActive = enabled;
}
void P_RadioTable::setItemOuterControl_ClearActive(bool enabled){
	this->m_itemOuterControl_ClearActive = enabled;
}
/*-------------------------------------------------
		�ӿ� - ���ư���
*/
void P_RadioTable::menuCopyItemInAction(){
	if (this->m_itemOuterControl_CopyActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int index = cur_action->data().value<int>();
	emit menuCopyItemTriggered(index);
}
/*-------------------------------------------------
		�ӿ� - ճ������
*/
void P_RadioTable::menuPasteItemInAction(){
	if (this->m_itemOuterControl_PasteActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int index = cur_action->data().value<int>();
	emit menuPasteItemTriggered(index);
}
/*-------------------------------------------------
		�ӿ� - ��հ���
*/
void P_RadioTable::menuClearItemInAction(){
	if (this->m_itemOuterControl_ClearActive == false){ return; }
	QAction* cur_action = qobject_cast<QAction*>(sender());		//��action����ȡ������
	int index = cur_action->data().value<int>();
	emit menuClearItemTriggered(index);
}
