#include "stdafx.h"
#include "W_ObjectSelector.h"

#include "private/w_ObjectSelectorConfig.h"

#include "Source/Utils/common/TTool.h"
#include "Source/Utils/manager/chineseManager/s_ChineseManager.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/c_ObjectSortData.h"
#include "Source/Utils/widgetFormSenior/ObjectSortController/p_ObjectSortController.h"

/*
-----==========================================================-----
		类：		对象选择 窗口.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		从 QJsonObject列表 中选择一个对象的窗口。（要有id和name，id必须为数字，obj不能为空）
					也可以从 id_list和name_list 中选择一个id。（两字符串必须长度相等）
					
		结构：		> 分页处理
						> 根据ID来选择
						> 根据名称来选择
						> 根据分类（Type）来选择
						> 无（0，第一条隐藏）		x
						> 特殊（-1）				x
					> 切换时记录页位置
					> 选择器配置数据
						> 行高
						> 零填充
						> ID配置
						> 名称配置

		使用方法：
				>打开选择器
					QList<QJsonObject> data_list;		//这是一个大obj列表，需要赋值
					W_ObjectSelector d();
					d.setData(&data_list);
					if (d.exec() == QDialog::Accepted){
						QMessageBox::information(this, "提示", "你选择了 "+ QString::number( d.getSelectedDataId() ) + " "+ d.getSelectedDataName()+" 。", QMessageBox::Yes);
					}
-----==========================================================-----
*/

W_ObjectSelector::W_ObjectSelector(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->cur_selectIndex_page = 0;
	this->cur_selectIndex_context = 0;
	this->m_config = C_ObjectSelectorConfig();

	this->m_source_ObjectSortController = new P_ObjectSortController();
	this->m_source_list = nullptr;

	//-----------------------------------
	//----事件绑定
	connect(ui.pushButton_config, &QPushButton::clicked, this, &W_ObjectSelector::openConfigParamWindow);
	connect(ui.comboBox_sort, &QComboBox::currentTextChanged, this, &W_ObjectSelector::refreshTable);
	connect(ui.tableWidget_page, &QTableWidget::currentItemChanged, this, &W_ObjectSelector::currentPageChanged);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_ObjectSelector::acceptData);
	
	//-----------------------------------
	//----初始化ui
	ui.tableWidget_context->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);				//列自适应
	ui.tableWidget_page->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);				//列自适应
	if (ui.buttonBox->button(QDialogButtonBox::Ok) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Ok)->setText("确定"); }
	if (ui.buttonBox->button(QDialogButtonBox::Cancel) != nullptr){ ui.buttonBox->button(QDialogButtonBox::Cancel)->setText("取消"); }
	// > 修改下拉框行高
	QStyledItemDelegate* comboBoxDelegate = new QStyledItemDelegate(ui.comboBox_sort);
	ui.comboBox_sort->setItemDelegate(comboBoxDelegate);
	ui.comboBox_sort->setStyleSheet("QComboBox QAbstractItemView::item{ min-height:30px; }");
	// > 添加分类
	if (this->m_source_ObjectSortController->hasAnyData_Type() == true && ui.comboBox_sort->count() == 4){
		ui.comboBox_sort->addItem("按分类索引");
	}
	
}

W_ObjectSelector::~W_ObjectSelector(){
}


/*-------------------------------------------------
		选择器设置 - 功能开关
*/
void W_ObjectSelector::setConfigBtnVisible(bool visible){
	ui.pushButton_config->setVisible(visible);
}
/*-------------------------------------------------
		选择器设置 - 设置参数
*/
void W_ObjectSelector::setConfigParam(QJsonObject config){
	this->m_config.setJsonObject(config);
	this->refreshTable();
}
/*-------------------------------------------------
		选择器设置 - 取出参数
*/
QJsonObject W_ObjectSelector::getConfigParam(){
	return this->m_config.getJsonObject();
}
/*-------------------------------------------------
		选择器设置 - 编辑窗口
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
		控件 - 刷新表格
*/
void W_ObjectSelector::refreshTable(){
	ui.tableWidget_page->clear();
	ui.tableWidget_context->clear();

	this->refreshTable_page();		//刷新 页面 表
	this->refreshTable_context();	//刷新 页信息 表
}
/*-------------------------------------------------
		控件 - 刷新 页面 表
*/
void W_ObjectSelector::refreshTable_page(){
	if (ui.comboBox_sort->currentText() == "按id递增"){

		// > 数据准备
		int all_count = this->m_source_ObjectSortController->getDataCount();
		int per_num = this->m_config.pagePerNum;
		int page_count = qCeil((double)all_count / per_num);

		// > 添加页
		ui.tableWidget_page->setRowCount(page_count);
		for (int i = 0; i < page_count; i++){
			int bottom = i * per_num + 1;
			int top = (i + 1) * per_num;
			if (top > all_count){
				top = all_count;
			}
			QString page_name = "";
			page_name = "第" + S_ChineseManager::getInstance()->toChineseNumLower(i+1) + "页 ";

			// > id编号
			if (this->m_config.zeroFill){
				page_name += TTool::_zeroFill_(bottom, this->m_config.zeroFillCount, QLatin1Char( this->m_config.zeroFillChar.toLatin1()))
					+ "至" + TTool::_zeroFill_(top, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1()));
			}else{
				page_name += QString::number(bottom) + "至" + QString::number(top);
			}

			QTableWidgetItem* page_item = new QTableWidgetItem(page_name);
			page_item->setData(Qt::UserRole + 1, i);						//当前页索引的位置
			ui.tableWidget_page->setItem(i, 0, page_item);
			ui.tableWidget_page->setRowHeight(i, this->m_config.rowHeight);
		}
	}
	if (ui.comboBox_sort->currentText() == "按id递减"){

		// > 数据准备
		int all_count = this->m_source_ObjectSortController->getDataCount();
		int per_num = this->m_config.pagePerNum;
		int page_count = qCeil((double)all_count / per_num);

		// > 添加页
		ui.tableWidget_page->setRowCount(page_count);
		for (int i = 0; i < page_count; i++){
			int index = page_count - i - 1;
			int bottom = index * per_num + 1;
			int top = (index + 1) * per_num;
			if (top > all_count){
				top = all_count;
			}
			QString page_name = "";
			page_name = "第" + S_ChineseManager::getInstance()->toChineseNumLower(i+1) + "页 ";

			// > id编号
			if (this->m_config.zeroFill){
				page_name += TTool::_zeroFill_(top, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1()))
					+ "至" + TTool::_zeroFill_(bottom, this->m_config.zeroFillCount, QLatin1Char(this->m_config.zeroFillChar.toLatin1()));
			}else{
				page_name += QString::number(top) + "至" + QString::number(bottom);
			}

			QTableWidgetItem* page_item = new QTableWidgetItem(page_name);
			page_item->setData(Qt::UserRole + 1, i);						//当前页索引的位置
			ui.tableWidget_page->setItem(i, 0, page_item);
			ui.tableWidget_page->setRowHeight(i, this->m_config.rowHeight);
		}
	}
	if (ui.comboBox_sort->currentText() == "按名称递增" || ui.comboBox_sort->currentText() == "按名称递减"){

		// > 数据准备
		QStringList page_name_list = QStringList() << "- A -" << "- B -" << "- C -" << "- D -" << "- E -" << "- F -" << "- G -" << "- H -" << "- I -" << "- J -" << "- K -" << "- L -" << "- M -" << "- N -" << "- O -" << "- P -" << "- Q -" << "- R -" << "- S -" << "- T -" << "- U -" << "- V -" << "- W -" << "- X -" << "- Y -" << "- Z -" << "生僻字";
		QStringList page_symbol_list = QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z" << "@";
		if (this->m_config.showHavingDataInitials == true){
			//（按首字母筛选）
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
		page_name_list << "空名称";
		page_symbol_list << "空名称";
		if (ui.comboBox_sort->currentText() == "按名称递减"){
			QStringList page_rlist = QStringList();
			for (int i = 0; i < page_name_list.count(); i++){ page_rlist.push_front(page_name_list.at(i)); }
			page_name_list = page_rlist;		//（反转数组）
			QStringList page_symbol_rlist = QStringList();
			for (int i = 0; i < page_symbol_list.count(); i++){ page_symbol_rlist.push_front(page_symbol_list.at(i)); }
			page_symbol_list = page_symbol_rlist;		//（反转数组）
		}
		// > 添加页
		ui.tableWidget_page->setRowCount(page_name_list.count());
		for (int i = 0; i < page_name_list.count(); i++){
			QString page_name = page_name_list.at(i);
			QString page_symbol = page_symbol_list.at(i);

			QTableWidgetItem* page_item = new QTableWidgetItem(page_name);
			page_item->setData(Qt::UserRole + 1, i);					//当前页索引的位置
			page_item->setData(Qt::UserRole + 3, page_symbol);			//名称查找字符的标志
			ui.tableWidget_page->setItem(i, 0, page_item);
			ui.tableWidget_page->setRowHeight(i, this->m_config.rowHeight);
		}
	}
	if (ui.comboBox_sort->currentText() == "按分类索引"){
		QStringList page_type_list = this->m_source_ObjectSortController->get_Type_DistinguishedList();

		// > 添加页
		ui.tableWidget_page->setRowCount(page_type_list.count());
		for (int i = 0; i < page_type_list.count(); i++){
			QString page_type = page_type_list.at(i);
			QString page_name = page_type;
			if (page_type == ""){ page_name = "未分类"; }

			QTableWidgetItem* page_item = new QTableWidgetItem(page_name);
			page_item->setData(Qt::UserRole + 1, i);					//当前页索引的位置
			page_item->setData(Qt::UserRole + 3, page_type);			//当前的类型
			ui.tableWidget_page->setItem(i, 0, page_item);
			ui.tableWidget_page->setRowHeight(i, this->m_config.rowHeight);
		}
	}

	// > 选择之前的页位置
	if (ui.tableWidget_page->rowCount() > 0){
		if (this->cur_selectIndex_page > ui.tableWidget_page->rowCount() - 1){
			this->cur_selectIndex_page = ui.tableWidget_page->rowCount() - 1;
		}
		ui.tableWidget_page->selectRow(this->cur_selectIndex_page);
	}
}
/*-------------------------------------------------
		控件 - 刷新 页信息 表
*/
void W_ObjectSelector::refreshTable_context(){
	QList<int> index_list;

	if (ui.comboBox_sort->currentText() == "按id递增" || ui.comboBox_sort->currentText() == "按id递减"){
		// > 每页信息
		if (ui.comboBox_sort->currentText() == "按id递增" ){
			index_list = this->m_source_ObjectSortController->get_IdInc_PageIndexList(this->cur_selectIndex_page, this->m_config.pagePerNum);
		}
		if (ui.comboBox_sort->currentText() == "按id递减"){
			index_list = this->m_source_ObjectSortController->get_IdDec_PageIndexList(this->cur_selectIndex_page, this->m_config.pagePerNum);
		}
	}

	if (ui.comboBox_sort->currentText() == "按名称递增" || ui.comboBox_sort->currentText() == "按名称递减"){
		// > 每页信息
		QString symbol = ui.tableWidget_page->item(this->cur_selectIndex_page, 0)->data(Qt::UserRole + 3).toString();
		if (symbol == "空名称"){
			index_list = this->m_source_ObjectSortController->get_Name_IndexListByEmptyName();
		}else{
			QChar ch = symbol.at(0);
			index_list = this->m_source_ObjectSortController->get_Name_IndexListByCharacter(ch);
		}
		if (ui.comboBox_sort->currentText() == "按名称递减"){
			QList<int> index_rlist = QList<int>();
			for (int i = 0; i < index_list.count(); i++){ index_rlist.push_front(index_list.at(i)); }
			index_list = index_rlist;		//（反转数组）
		}
	}

	if (ui.comboBox_sort->currentText() == "按分类索引"){
		// > 每页信息
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
		context_item->setData(Qt::UserRole + 1, i);						//当前页索引的位置
		context_item->setData(Qt::UserRole + 2, data->arrIndex);		//实际数组索引的位置
		ui.tableWidget_context->setItem(i, 0, context_item);
		ui.tableWidget_context->setRowHeight(i, this->m_config.rowHeight);
	}

	// > 选择之前的页信息位置
	if (ui.tableWidget_context->rowCount() > 0){
		if (this->cur_selectIndex_context > ui.tableWidget_context->rowCount() - 1){
			this->cur_selectIndex_context = ui.tableWidget_context->rowCount() - 1;
		}
		ui.tableWidget_context->selectRow(this->cur_selectIndex_context);
	}
}
/*-------------------------------------------------
		控件 - 页变化
*/
void W_ObjectSelector::currentPageChanged(QTableWidgetItem* cur, QTableWidgetItem* pre){
	if (cur == nullptr){ return; }
	if (pre == nullptr){ return; }
	
	// > 记录页位置
	this->cur_selectIndex_page = cur->data(Qt::UserRole + 1).toInt();
	if (ui.tableWidget_context->currentItem() != nullptr){
		this->cur_selectIndex_context = ui.tableWidget_context->currentItem()->data(Qt::UserRole + 1).toInt();
	}
	
	// > 刷新页信息
	this->refreshTable_context();
}


/*-------------------------------------------------
		资源数据 - 载入
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
		资源数据 - 获取选中的数据
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
		资源数据 - 获取选中的对象（私有）
*/
C_ObjectSortData* W_ObjectSelector::getSelectedSource(){
	if (ui.tableWidget_context->rowCount() == 0){ return nullptr; }
	QTableWidgetItem* item = ui.tableWidget_context->currentItem();
	int arr_index = item->data(Qt::UserRole + 2).toInt();		//（从item中获取到索引数据）
	return this->m_source_ObjectSortController->getSortData_ByIndex(arr_index);
}

/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_ObjectSelector::putDataToUi() {
	this->refreshTable();

	// > 添加分类
	if (this->m_source_ObjectSortController->hasAnyData_Type() == true && ui.comboBox_sort->count() == 4){
		ui.comboBox_sort->addItem("按分类索引");
	}
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_ObjectSelector::putUiToData() {
	//（暂无）
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_ObjectSelector::acceptData(){
	this->putUiToData();

	// > 空校验
	if (ui.tableWidget_context->rowCount() == 0){
		QMessageBox::warning(this, "提示", "选择的对象不能为空。");
		return;
	}

	this->accept();
};