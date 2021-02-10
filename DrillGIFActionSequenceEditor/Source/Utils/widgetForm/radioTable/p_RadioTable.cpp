#include "stdafx.h"
#include "p_RadioTable.h"

#include "private/w_RaTConfig.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		��ѡ���.cpp
		�汾��		v1.00
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
	QList<QTableWidgetItem*> last_selected_item = this->m_table->selectedItems();

	// > ����
	this->m_table->clearContents();
	this->m_table->setRowCount(local_text.count());

	// > ˢ������
	for (int i = 0; i < local_text.count(); i++){
		QTableWidgetItem* item = new QTableWidgetItem();
		item->setText(TTool::_zeroFill_( i+1, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1())) + " " + local_text.at(i));
		item->setData(Qt::UserRole + 1, local_text.at(i));
		this->m_table->setItem(i, 0, item);
	}

	// > �и�ˢ��
	this->m_table->setStyleSheet(this->m_tableStyle + "\n QTreeView::item { height: " + QString::number(this->m_config.rowHeight) + "px;}");

	// > ѡ�б仯��
	if (last_selected_item.count() > 0){
		this->m_table->setItemSelected(last_selected_item.at(0), true);
		this->m_table->scrollToItem(last_selected_item.at(0));
	}else{
		QTableWidgetSelectionRange range = QTableWidgetSelectionRange(0,0,1,1);
		this->m_table->setRangeSelected(range, true);
	}
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
	QList<QTableWidgetSelectionRange> range = this->m_table->selectedRanges();
	if (range.size() == 0) {
		return -1;
	}
	return range.at(0).topRow();
}
QString P_RadioTable::getSelectedText(){
	QList<QTableWidgetItem*> selected_item = this->m_table->selectedItems();
	if (selected_item.count() == 0) {
		return "";
	}
	return selected_item.at(0)->data(Qt::UserRole + 1).toString();
}