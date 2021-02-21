#include "stdafx.h"
#include "p_PictureSelector.h"

#include "private/p_PiSBlock.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		��ѡ/��ѡ ͼƬ��.cpp
		�汾��		v1.01
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		��ͼƬȫ����ʾ�����ܵ�ѡ/��ѡ��

		�ӹ��ܣ�	-> �ı�ѡ��
						-> ��ѡ
						-> ��ѡ
						-> �Զ����
					-> ͼƬ������
						-> �����и�
						-> ����Ҷ����ʾ�ı�
					
		ʹ�÷�����
				>��ʼ��
					this->m_p_PictureSelector = new P_PictureSelector(ui.treeWidget);
					this->m_p_PictureSelector->setSource(bitmap_list);		
					
-----==========================================================-----
*/
P_PictureSelector::P_PictureSelector(QListWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----������ʼ��
	this->m_listWidget = parent;									//ͼƬ�ж���
	this->m_listWidgetStyle = this->m_listWidget->styleSheet();		//ͼƬ��Ĭ����ʽ
	this->m_itemTank = QList<QListWidgetItem*>();					//�ؼ��б�
	this->m_widgetTank = QList<QWidget*>();							//�ؼ��б�
	this->m_last_index = -1;										//��һ��ѡ�е�������

	// > ����
	this->m_config = C_PiSConfig();

	//-----------------------------------
	//----ui��ʼ��
	this->m_listWidget->setContextMenuPolicy(Qt::CustomContextMenu);									//�����Ҽ�
	this->m_listWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);			//��ѡ
	this->m_listWidget->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);			//ѡ��
	this->m_listWidget->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);				//�ر�˫���༭

	this->m_listWidget->setFlow(QListView::LeftToRight);
	this->m_listWidget->setMovement(QListView::Static);
	this->m_listWidget->setViewMode(QListView::ListMode);
	this->m_listWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOn);
	this->m_listWidget->setDragEnabled(false);
	this->m_listWidget->setDragDropMode(QListView::NoDragDrop);

	//-----------------------------------
	//----�¼���
	this->m_selectionSignalBlock_Root = false;				//������
	connect(this->m_listWidget, &QListWidget::customContextMenuRequested, this, &P_PictureSelector::itemRightClicked);
	connect(this->m_listWidget, &QListWidget::itemSelectionChanged, this, &P_PictureSelector::itemSelectionChanged);

}
P_PictureSelector::~P_PictureSelector(){
}


/*-------------------------------------------------
		�ؼ� - �ؽ�ͼƬ�У����ͼƬ�࣬�����鷴�����ã�
*/
void P_PictureSelector::rebuildListUi() {
	QList<int> last_selected_item = this->getSelectedIndex_Multi();
	int item_height = this->m_config.getItemHeight();

	// > ����
	this->m_itemTank.clear();
	this->m_widgetTank.clear();
	this->m_listWidget->clear();

	// > ��ѡ/��ѡ�л�
	if (this->m_config.isMultiSelect == true && this->m_listWidget->selectionMode() == QAbstractItemView::SelectionMode::SingleSelection){
		this->m_listWidget->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
	}
	if (this->m_config.isMultiSelect == false && this->m_listWidget->selectionMode() != QAbstractItemView::SelectionMode::SingleSelection){
		this->m_listWidget->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
	}

	// > �и�ˢ��
	this->m_listWidget->setFixedHeight(this->m_config.height);
	this->m_listWidget->setMinimumHeight(this->m_config.height);
	this->m_listWidget->setMaximumHeight(this->m_config.height);
	this->m_listWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	//this->m_listWidget->setStyleSheet(this->m_listWidgetStyle + "\n QListView::item { height: " + QString::number(this->m_config.getItemHeight()) + "px;}");

	// > ˢ������
	this->m_listWidget->setIconSize(QSize(item_height, 1));	//��ͼ���С��
	for (int i = 0; i < this->m_org_bitmapList.count(); i++){

		// > ������
		QListWidgetItem* item = this->createPictureItem();

		// > ÿ�����
		this->m_listWidget->addItem(item);
		this->m_itemTank.append(item);

		// > �����ؼ���������addItem���棩
		QWidget* widget = this->createPictureWidget(i, this->m_org_bitmapList.at(i));

		// > ��ӵ� �б�ؼ�
		this->m_listWidget->setItemWidget(item, widget);
		this->m_widgetTank.append(widget);
	}

	// > ѡ�б仯��
	if (this->m_config.isMultiSelect){
		this->selectIndex_Multi(last_selected_item);
	}else{
		if (last_selected_item.count() > 0){
			this->selectIndex(last_selected_item.at(0));
		}
	}
}

/*-------------------------------------------------
		�ؼ� - ����ͼƬ��
*/
QListWidgetItem* P_PictureSelector::createPictureItem(){
	QListWidgetItem* item = new QListWidgetItem(this->m_listWidget);
	int item_height = this->m_config.getItemHeight();

	// > �ſ�ÿ��
	//QPixmap temp_bitmap(item_height, 1);				//��iconMode�У���ͼ��ſ�����ͼ�겻�ܳſ�����
	//temp_bitmap.fill(Qt::white);						//��������listMode�ˣ����Բ���Ҫ�ˡ���
	//item->setIcon(QIcon(temp_bitmap));				//
	item->setText(this->m_config.getLineExpand());		//���û��з��ſ�����
	item->setSizeHint(QSize(this->m_config.getItemHeight(), this->m_config.getItemHeight()));	//��������ſ���ѡ�е� ��ɫ���� �����ң�

	return item;
}
/*-------------------------------------------------
		�ؼ� - ÿ����ؼ�
*/
QWidget* P_PictureSelector::createPictureWidget(int i, QPixmap pixmap){
	int item_height = this->m_config.getItemHeight();

	// > ����ͼƬ��
	P_PiSBlock* widget = new P_PiSBlock(item_height, item_height, this->m_listWidget);
	if (this->m_config.zeroFill == true){
		widget->setCountText(TTool::_zeroFill_(i + 1, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1())));
	}else{
		widget->setCountText(QString::number(i + 1));
	}

	// > ����ͼƬ
	widget->setPixmap(pixmap);
	
	return widget;
}
/*-------------------------------------------------
		�ؼ� - ����ȫ��
*/
void P_PictureSelector::clearAll(){

	// > �ؼ�����
	this->m_itemTank.clear();
	this->m_widgetTank.clear();
	this->m_listWidget->clear();
	this->m_last_index = -1;

	// > ͼƬ������
	this->m_config = C_PiSConfig();

}

/*-------------------------------------------------
		ͼƬ������ - ���ò���
*/
void P_PictureSelector::setConfigParam(C_PiSConfig config){
	this->m_config = config;
	this->rebuildListUi();
}
/*-------------------------------------------------
		ͼƬ������ - ȡ������
*/
C_PiSConfig P_PictureSelector::getConfigParam(){
	return this->m_config;
}


/*-------------------------------------------------
		�¼� - �Ҽ��¼�
*/
void P_PictureSelector::itemRightClicked(QPoint point) {
	if (this->m_selectionSignalBlock_Root == true){ return; }
	this->m_selectionSignalBlock_Root = true;

	QListWidgetItem *item = this->m_listWidget->itemAt(point);	//����ǰ�Ҽ�ѡ�е��
	if (item == nullptr){

		// > �㵽�հ״�
		this->event_itemRightClicked(QList<QListWidgetItem*>());
	}else{

		// > �㵽��
		this->event_itemRightClicked(this->m_listWidget->selectedItems());
	}

	this->m_selectionSignalBlock_Root = false;
}
/*-------------------------------------------------
		�¼� - �Ҽ��¼�������������������
*/
void P_PictureSelector::event_itemRightClicked(QList<QListWidgetItem*> item_list){
	//�����ޣ�
}

/*-------------------------------------------------
		�¼� -  Ȧѡ�仯�¼�
*/
void P_PictureSelector::itemSelectionChanged(){
	if (this->m_selectionSignalBlock_Root == true){ return; }
	this->m_selectionSignalBlock_Root = true;
	
	QList<QListWidgetItem*> selected_item = this->m_listWidget->selectedItems();
	if (selected_item.count() == 0){ this->m_selectionSignalBlock_Root = false; return; }
	this->event_itemSelectionChanged(selected_item);

	this->m_selectionSignalBlock_Root = false;
}
/*-------------------------------------------------
		�¼� - Ȧѡ�仯�¼��������������
*/
void P_PictureSelector::event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list){

	// > ���ź�
	if (selected_item_list.count() == 1){

		int index = this->getSelectedIndex();
		if (this->m_last_index != index){
			this->m_last_index = index;
			emit currentIndexChanged(index);
			emit selectedIndexChanged_Multi(this->getSelectedIndex_Multi());
		}

	}else if (selected_item_list.count() > 1){

		emit selectedIndexChanged_Multi(this->getSelectedIndex_Multi());
	}
}


/*-------------------------------------------------
		��Դ���� - ��������
*/
void P_PictureSelector::setSource(QList<QFileInfo> file_list) {
	this->m_org_bitmapList = QList<QPixmap>();
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo info = file_list.at(i);
		QImage image = QImage(info.absoluteFilePath());
		QPixmap pixmap = QPixmap::fromImage(image);
		this->m_org_bitmapList.append(pixmap);
	}
	this->rebuildListUi();
}
void P_PictureSelector::setSource(QList<QPixmap> bitmap_list) {
	this->m_org_bitmapList = bitmap_list;
	this->rebuildListUi();
}


/*-------------------------------------------------
		ѡ�� - ����ѡ��
*/
void P_PictureSelector::selectIndex(int index){
	if (index == -1){ return; }
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QListWidgetItem* item = this->m_itemTank.at(i);
		if (i == index){
			item->setSelected(true);
		}else{
			item->setSelected(false);
		}
	}
}
/*-------------------------------------------------
		ѡ�� - ��ȡѡ������
*/
int P_PictureSelector::getSelectedIndex(){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		if (this->m_itemTank.at(i)->isSelected()){
			return i;
		}
	}
	return -1;
}

/*-------------------------------------------------
		ѡ�� - ����ѡ�У���ѡʱ��
*/
void P_PictureSelector::selectIndex_Multi(QList<int> index_list){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		QListWidgetItem* item = this->m_itemTank.at(i);
		if (index_list.contains( i )){
			item->setSelected(true);
		}else{
			item->setSelected(false);
		}
	}
}
/*-------------------------------------------------
		ѡ�� - ��ȡѡ�����ݣ���ѡʱ��
*/
QList<int> P_PictureSelector::getSelectedIndex_Multi(){
	QList<int> result_list = QList<int>();
	for (int i = 0; i < this->m_itemTank.count(); i++){
		if (this->m_itemTank.at(i)->isSelected()){
			result_list.push_back(i);
		}
	}
	return result_list;
}