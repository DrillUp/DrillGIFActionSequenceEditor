#include "stdafx.h"
#include "W_ObjectSelector.h"

#include "private/w_ObjectSelectorConfig.h"

#include "Source/Utils/common/TTool.h"
#include "Source/Utils/manager/chineseManager/s_ChineseManager.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/c_ObjectSortData.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/p_ObjectSortController.h"

/*
-----==========================================================-----
		�ࣺ		����ѡ�� ����.cpp
		�汾��		v1.01
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�� QJsonObject�б� ��ѡ��һ������Ĵ��ڡ���Ҫ��id��name��id����Ϊ���֣�obj����Ϊ�գ�
					Ҳ���Դ� id_list��name_list ��ѡ��һ��id�������ַ������볤����ȣ�
					
		�ṹ��		> ��ҳ����
						> ����ID��ѡ��
						> ����������ѡ��
						> ���ݷ��ࣨType����ѡ��
						> �ޣ�0����һ�����أ�		x
						> ���⣨-1��				x
					> �л�ʱ��¼ҳλ��
					> ѡ������������
						> �и�
						> �����
						> ID����
						> ��������

		ʹ�÷�����
				>��ѡ����
					QList<QJsonObject> data_list;		//����һ����obj�б���Ҫ��ֵ
					W_ObjectSelector d();
					d.setData(&data_list);
					if (d.exec() == QDialog::Accepted){
						QMessageBox::information(this, "��ʾ", "��ѡ���� "+ QString::number( d.getSelectedDataId() ) + " "+ d.getSelectedDataName()+" ��", QMessageBox::Yes);
					}
-----==========================================================-----
*/

W_ObjectSelector::W_ObjectSelector(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----��ʼ������
	this->cur_selectIndex_page = 0;
	this->cur_selectIndex_context = 0;
	this->m_config = C_ObjectSelectorConfig();

	this->m_source_ObjectSortController = new P_ObjectSortController();
	this->m_source_list = nullptr;

	//-----------------------------------
	//----�¼���
	connect(ui.pushButton_config, &QPushButton::clicked, this, &W_ObjectSelector::openConfigParamWindow);
	connect(ui.comboBox_sort, &QComboBox::currentTextChanged, this, &W_ObjectSelector::refreshTable);
	connect(ui.tableWidget_page, &QTableWidget::currentItemChanged, this, &W_ObjectSelector::currentPageChanged);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ObjectSelector::acceptData);
	
	//-----------------------------------
	//----��ʼ��ui
	ui.tableWidget_context->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);				//������Ӧ
	ui.tableWidget_page->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);				//������Ӧ
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("ȷ��"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("ȡ��"); }
	// > �޸��������и�
	QStyledItemDelegate* comboBoxDelegate = new QStyledItemDelegate(ui.comboBox_sort);
	ui.comboBox_sort->setItemDelegate(comboBoxDelegate);
	ui.comboBox_sort->setStyleSheet("QComboBox QAbstractItemView::item{ min-height:30px; }");
	// > ��ӷ���
	if (this->m_source_ObjectSortController->hasAnyData_Type() == true && ui.comboBox_sort->count() == 4){
		ui.comboBox_sort->addItem("����������");
	}
	
}

W_ObjectSelector::~W_ObjectSelector(){
}


/*-------------------------------------------------
		ѡ�������� - ���ܿ���
*/
void W_ObjectSelector::setConfigBtnVisible(bool visible){
	ui.pushButton_config->setVisible(visible);
}
/*-------------------------------------------------
		ѡ�������� - ���ò���
*/
void W_ObjectSelector::setConfigParam(QJsonObject config){
	this->m_config.setJsonObject(config);
	this->refreshTable();
}
/*-------------------------------------------------
		ѡ�������� - ȡ������
*/
QJsonObject W_ObjectSelector::getConfigParam(){
	return this->m_config.getJsonObject();
}
/*-------------------------------------------------
		ѡ�������� - �༭����
*/
void W_ObjectSelector::openConfigParamWindow(){
	W_ObjectSelectorConfig d(this);
	d.setDataInModifyMode(this->m_config);
	if (d.exec() == QDialog::Accepted){
		this->m_config = d.getData() ;
		this->refreshTable();
	}
}



/*-------------------------------------------------
		�ؼ� - ˢ�±��
*/
void W_ObjectSelector::refreshTable(){
	ui.tableWidget_page->clear();
	ui.tableWidget_context->clear();

	this->refreshTable_page();		//ˢ�� ҳ�� ��
	this->refreshTable_context();	//ˢ�� ҳ��Ϣ ��
}
/*-------------------------------------------------
		�ؼ� - ˢ�� ҳ�� ��
*/
void W_ObjectSelector::refreshTable_page(){
	if (ui.comboBox_sort->currentText() == "��id����"){

		// > ����׼��
		int all_count = this->m_source_ObjectSortController->getDataCount();
		int per_num = this->m_config.pagePerNum;
		int page_count = qCeil((double)all_count / per_num);

		// > ���ҳ
		ui.tableWidget_page->setRowCount(page_count);
		for (int i = 0; i < page_count; i++){
			int bottom = i * per_num + 1;
			int top = (i + 1) * per_num;
			if (top > all_count){
				top = all_count;
			}
			QString page_name = "";
			page_name = "��" + S_ChineseManager::getInstance()->toChineseNumLower(i+1) + "ҳ ";

			// > id���
			if (this->m_config.zeroFill){
				page_name += TTool::_zeroFill_(bottom, this->m_config.zeroFillCount, QLatin1Char( this->m_config.zeroFillChar.toLatin1()))
					+ "��" + TTool::_zeroFill_(top, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1()));
			}else{
				page_name += QString::number(bottom) + "��" + QString::number(top);
			}

			QTableWidgetItem* page_item = new QTableWidgetItem(page_name);
			page_item->setData(Qt::UserRole + 1, i);						//��ǰҳ������λ��
			ui.tableWidget_page->setItem(i, 0, page_item);
			ui.tableWidget_page->setRowHeight(i, this->m_config.rowHeight);
		}
	}
	if (ui.comboBox_sort->currentText() == "��id�ݼ�"){

		// > ����׼��
		int all_count = this->m_source_ObjectSortController->getDataCount();
		int per_num = this->m_config.pagePerNum;
		int page_count = qCeil((double)all_count / per_num);

		// > ���ҳ
		ui.tableWidget_page->setRowCount(page_count);
		for (int i = 0; i < page_count; i++){
			int index = page_count - i - 1;
			int bottom = index * per_num + 1;
			int top = (index + 1) * per_num;
			if (top > all_count){
				top = all_count;
			}
			QString page_name = "";
			page_name = "��" + S_ChineseManager::getInstance()->toChineseNumLower(i+1) + "ҳ ";

			// > id���
			if (this->m_config.zeroFill){
				page_name += TTool::_zeroFill_(top, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1()))
					+ "��" + TTool::_zeroFill_(bottom, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1()));
			}else{
				page_name += QString::number(top) + "��" + QString::number(bottom);
			}

			QTableWidgetItem* page_item = new QTableWidgetItem(page_name);
			page_item->setData(Qt::UserRole + 1, i);						//��ǰҳ������λ��
			ui.tableWidget_page->setItem(i, 0, page_item);
			ui.tableWidget_page->setRowHeight(i, this->m_config.rowHeight);
		}
	}
	if (ui.comboBox_sort->currentText() == "�����Ƶ���" || ui.comboBox_sort->currentText() == "�����Ƶݼ�"){

		// > ����׼��
		QStringList page_name_list = QStringList() << "- A -" << "- B -" << "- C -" << "- D -" << "- E -" << "- F -" << "- G -" << "- H -" << "- I -" << "- J -" << "- K -" << "- L -" << "- M -" << "- N -" << "- O -" << "- P -" << "- Q -" << "- R -" << "- S -" << "- T -" << "- U -" << "- V -" << "- W -" << "- X -" << "- Y -" << "- Z -" << "��Ƨ��";
		QStringList page_symbol_list = QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z" << "@";
		if (this->m_config.showHavingDataInitials == true){
			//��������ĸɸѡ��
			QStringList screening_name = QStringList();
			QStringList screening_symbol = QStringList();
			for (int j = 0; j < page_symbol_list.count(); j++){		
				QString name = page_name_list.at(j);
				QString symbol = page_symbol_list.at(j);
				if (this->m_source_ObjectSortController->hasAnyData_Name_ByCharacter(symbol.at(0)) == true){
					screening_name.push_back(name);
					screening_symbol.push_back(symbol);
				}
			}
			page_name_list = screening_name;
			page_symbol_list = screening_symbol;
		}
		page_name_list << "������";
		page_symbol_list << "������";
		if (ui.comboBox_sort->currentText() == "�����Ƶݼ�"){
			QStringList page_rlist = QStringList();
			for (int i = 0; i < page_name_list.count(); i++){ page_rlist.push_front(page_name_list.at(i)); }
			page_name_list = page_rlist;		//����ת���飩
			QStringList page_symbol_rlist = QStringList();
			for (int i = 0; i < page_symbol_list.count(); i++){ page_symbol_rlist.push_front(page_symbol_list.at(i)); }
			page_symbol_list = page_symbol_rlist;		//����ת���飩
		}
		// > ���ҳ
		ui.tableWidget_page->setRowCount(page_name_list.count());
		for (int i = 0; i < page_name_list.count(); i++){
			QString page_name = page_name_list.at(i);
			QString page_symbol = page_symbol_list.at(i);

			QTableWidgetItem* page_item = new QTableWidgetItem(page_name);
			page_item->setData(Qt::UserRole + 1, i);					//��ǰҳ������λ��
			page_item->setData(Qt::UserRole + 3, page_symbol);			//���Ʋ����ַ��ı�־
			ui.tableWidget_page->setItem(i, 0, page_item);
			ui.tableWidget_page->setRowHeight(i, this->m_config.rowHeight);
		}
	}
	if (ui.comboBox_sort->currentText() == "����������"){
		QStringList page_type_list = this->m_source_ObjectSortController->get_Type_DistinguishedList();

		// > ���ҳ
		ui.tableWidget_page->setRowCount(page_type_list.count());
		for (int i = 0; i < page_type_list.count(); i++){
			QString page_type = page_type_list.at(i);
			QString page_name = page_type;
			if (page_type == ""){ page_name = "δ����"; }

			QTableWidgetItem* page_item = new QTableWidgetItem(page_name);
			page_item->setData(Qt::UserRole + 1, i);					//��ǰҳ������λ��
			page_item->setData(Qt::UserRole + 3, page_type);			//��ǰ������
			ui.tableWidget_page->setItem(i, 0, page_item);
			ui.tableWidget_page->setRowHeight(i, this->m_config.rowHeight);
		}
	}

	// > ѡ��֮ǰ��ҳλ��
	if (ui.tableWidget_page->rowCount() > 0){
		if (this->cur_selectIndex_page > ui.tableWidget_page->rowCount() - 1){
			this->cur_selectIndex_page = ui.tableWidget_page->rowCount() - 1;
		}
		ui.tableWidget_page->selectRow(this->cur_selectIndex_page);
	}
}
/*-------------------------------------------------
		�ؼ� - ˢ�� ҳ��Ϣ ��
*/
void W_ObjectSelector::refreshTable_context(){
	QList<int> index_list;

	if (ui.comboBox_sort->currentText() == "��id����" || ui.comboBox_sort->currentText() == "��id�ݼ�"){
		// > ÿҳ��Ϣ
		if (ui.comboBox_sort->currentText() == "��id����" ){
			index_list = this->m_source_ObjectSortController->get_IdInc_PageIndexList(this->cur_selectIndex_page, this->m_config.pagePerNum);
		}
		if (ui.comboBox_sort->currentText() == "��id�ݼ�"){
			index_list = this->m_source_ObjectSortController->get_IdDec_PageIndexList(this->cur_selectIndex_page, this->m_config.pagePerNum);
		}
	}

	if (ui.comboBox_sort->currentText() == "�����Ƶ���" || ui.comboBox_sort->currentText() == "�����Ƶݼ�"){
		// > ÿҳ��Ϣ
		QString symbol = ui.tableWidget_page->item(this->cur_selectIndex_page, 0)->data(Qt::UserRole + 3).toString();
		if (symbol == "������"){
			index_list = this->m_source_ObjectSortController->get_Name_IndexListByEmptyName();
		}else{
			QChar ch = symbol.at(0);
			index_list = this->m_source_ObjectSortController->get_Name_IndexListByCharacter(ch);
		}
		if (ui.comboBox_sort->currentText() == "�����Ƶݼ�"){
			QList<int> index_rlist = QList<int>();
			for (int i = 0; i < index_list.count(); i++){ index_rlist.push_front(index_list.at(i)); }
			index_list = index_rlist;		//����ת���飩
		}
	}

	if (ui.comboBox_sort->currentText() == "����������"){
		// > ÿҳ��Ϣ
		QString type = ui.tableWidget_page->item(this->cur_selectIndex_page, 0)->data(Qt::UserRole + 3).toString();
		index_list = this->m_source_ObjectSortController->get_TypeAndId_IndexListByType(type);
	}

	QList<C_ObjectSortData*> data_list = this->m_source_ObjectSortController->getSortDataList_ByIndex(index_list);
	ui.tableWidget_context->setRowCount(data_list.count());
	for (int i = 0; i < data_list.count(); i++){
		C_ObjectSortData* data = data_list.at(i);
		QString context_name = "";
		if (this->m_config.zeroFill){
			context_name = TTool::_zeroFill_(data->id, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1())) + " " + data->name;
		}else{
			context_name = QString::number(data->id) + " " + data->name;
		}
		QTableWidgetItem* context_item = new QTableWidgetItem(context_name);
		context_item->setData(Qt::UserRole + 1, i);						//��ǰҳ������λ��
		context_item->setData(Qt::UserRole + 2, data->arrIndex);		//ʵ������������λ��
		ui.tableWidget_context->setItem(i, 0, context_item);
		ui.tableWidget_context->setRowHeight(i, this->m_config.rowHeight);
	}

	// > ѡ��֮ǰ��ҳ��Ϣλ��
	if (ui.tableWidget_context->rowCount() > 0){
		if (this->cur_selectIndex_context > ui.tableWidget_context->rowCount() - 1){
			this->cur_selectIndex_context = ui.tableWidget_context->rowCount() - 1;
		}
		ui.tableWidget_context->selectRow(this->cur_selectIndex_context);
	}
}
/*-------------------------------------------------
		�ؼ� - ҳ�仯
*/
void W_ObjectSelector::currentPageChanged(QTableWidgetItem* cur, QTableWidgetItem* pre){
	if (cur == nullptr){ return; }
	if (pre == nullptr){ return; }
	
	// > ��¼ҳλ��
	this->cur_selectIndex_page = cur->data(Qt::UserRole + 1).toInt();
	if (ui.tableWidget_context->currentItem() != nullptr){
		this->cur_selectIndex_context = ui.tableWidget_context->currentItem()->data(Qt::UserRole + 1).toInt();
	}
	
	// > ˢ��ҳ��Ϣ
	this->refreshTable_context();
}


/*-------------------------------------------------
		��Դ���� - ����
*/
void W_ObjectSelector::loadSource(QList<QJsonObject>* obj_list) {
	this->m_source_ObjectSortController->setData_FromObject(obj_list);
	this->putDataToUi();
}
void W_ObjectSelector::loadSource(QList<QJsonObject>* obj_list, QString id_symbol, QString name_symbol, QString type_symbol) {
	this->m_source_ObjectSortController->setData_FromObject(obj_list, id_symbol, name_symbol, type_symbol);
	this->putDataToUi();
}

/*-------------------------------------------------
		��Դ���� - ��ȡѡ�е�����
*/
int W_ObjectSelector::getSelectedSourceId(){
	if (this->getSelectedSource() == nullptr){ return -1; }
	return this->getSelectedSource()->id;
};
QString W_ObjectSelector::getSelectedSourceName(){
	if (this->getSelectedSource() == nullptr){ return ""; }
	return this->getSelectedSource()->name;
};
QString W_ObjectSelector::getSelectedSourceType(){
	if (this->getSelectedSource() == nullptr){ return ""; }
	return this->getSelectedSource()->type;
};
/*-------------------------------------------------
		��Դ���� - ��ȡѡ�еĶ���˽�У�
*/
C_ObjectSortData* W_ObjectSelector::getSelectedSource(){
	if (ui.tableWidget_context->rowCount() == 0){ return nullptr; }
	QTableWidgetItem* item = ui.tableWidget_context->currentItem();
	int arr_index = item->data(Qt::UserRole + 2).toInt();		//����item�л�ȡ���������ݣ�
	return this->m_source_ObjectSortController->getSortData_ByIndex(arr_index);
}

/*-------------------------------------------------
		���� - �������� -> ui����
*/
void W_ObjectSelector::putDataToUi() {
	this->refreshTable();

	// > ��ӷ���
	if (this->m_source_ObjectSortController->hasAnyData_Type() == true && ui.comboBox_sort->count() == 4){
		ui.comboBox_sort->addItem("����������");
	}
};
/*-------------------------------------------------
		���� - ui���� -> ��������
*/
void W_ObjectSelector::putUiToData() {
	//�����ޣ�
};
/*-------------------------------------------------
		���� - �ύ���ݣ�У�飩
*/
void W_ObjectSelector::acceptData(){
	this->putUiToData();

	// > ��У��
	if (ui.tableWidget_context->rowCount() == 0){
		QMessageBox::warning(this, "��ʾ", "ѡ��Ķ�����Ϊ�ա�");
		return;
	}

	this->accept();
};