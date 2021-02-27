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

		�ӹ��ܣ�	-> �ı���ѡ
						-> �Զ����
					-> �������
						-> �����и�
						-> ����Ҷ����ʾ�ı�
					
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
	this->m_table = parent;									//������
	this->m_tableStyle = this->m_table->styleSheet();		//���Ĭ����ʽ
	this->m_itemTank = QList<QTableWidgetItem*>();			//������б�

	// > ����
	this->m_config = C_RaTConfig();

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
	QList<QTableWidgetSelectionRange> last_range = this->m_table->selectedRanges();

	// > ����
	this->m_table->clearContents();
	this->m_table->setRowCount(local_text.count());

	// > ˢ������
	this->m_itemTank = QList<QTableWidgetItem*>();
	for (int i = 0; i < local_text.count(); i++){
		QTableWidgetItem* item = new QTableWidgetItem();
		item->setText(TTool::_zeroFill_( i+1, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1())) + " " + local_text.at(i));
		item->setData(Qt::UserRole + 1, local_text.at(i));
		this->m_table->setItem(i, 0, item);
		this->m_itemTank.append(item);
	}

	// > �и�ˢ��
	this->m_table->setStyleSheet(this->m_tableStyle + "\n QTableView::item { height: " + QString::number(this->m_config.rowHeight) + "px;}");

	// > ѡ�б仯��
	if (last_range.count() > 0){
		this->m_table->setRangeSelected(last_range.at(0), true);
	}else{
		QTableWidgetSelectionRange range = QTableWidgetSelectionRange(0,0,1,1);
		this->m_table->setRangeSelected(range, true);
	}
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
	this->m_table->clearContents();
	this->m_table->setRowCount(0);

	// > �������
	this->m_config = C_RaTConfig();

}

/*-------------------------------------------------
		������� - ���ò���
*/
void P_RadioTable::setConfigParam(QJsonObject config){
	this->m_config.setJsonObject(config);
	this->refreshTableUi();
}
/*-------------------------------------------------
		������� - ȡ������
*/
QJsonObject P_RadioTable::getConfigParam(){
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
	if (selected_item.count() > 0) {
		QString text = selected_item.at(0)->data(Qt::UserRole + 1).toString();
		int index = this->getSelectedIndex();
		emit currentTextChanged(text);
		emit currentIndexChanged(index);
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

	//...�����ޣ�

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
			return item->data(Qt::UserRole + 1).toString();
		}
	}
	return "";
}


/*-------------------------------------------------
		ѡ�� - ����ѡ��
*/
void  P_RadioTable::selectIndex(int index){
	if (index < 0){ index = 0; }
	if (index >= this->m_itemTank.count()){ index = this->m_itemTank.count() - 1; }

	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);
		if (i == index){
			item->setSelected(true);
			this->m_table->scrollToItem(item);
		}else{
			item->setSelected(false);
		}
	}
	emit currentIndexChanged(index);
}
void  P_RadioTable::selectText(QString text){
	bool index = -1;
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QTableWidgetItem* item = this->m_itemTank.at(i);

		if (item->data(Qt::UserRole + 1).toString() == text &&
			index == -1){
			item->setSelected(true);
			this->m_table->scrollToItem(item);
			index = i;
		}else{
			item->setSelected(false);
		}
	}
	if (index != -1){
		emit currentIndexChanged(index);
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