#include "stdafx.h"
#include "W_StateNodeRelationTable.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		状态节点关系表.cpp
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		编辑 状态节点关系表 窗口的一些配置。

		使用方法：
				> 打开窗口
					W_StateNodeRelationTable d;
					d.setData_StateData(this->getConfigParam());
					d.setData_StateNodeData(this->getConfigParam());
					d.exec();
-----==========================================================-----
*/

W_StateNodeRelationTable::W_StateNodeRelationTable(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_availableStateDataList.clear();
	this->m_availableStateNodeDataList.clear();

	//-----------------------------------
	//----事件绑定
	
	//-----------------------------------
	//----初始化ui
	TTool::_chinese_(ui.buttonBox);
}

W_StateNodeRelationTable::~W_StateNodeRelationTable(){
}


/*-------------------------------------------------
		控件 - 刷新表格
*/
void W_StateNodeRelationTable::refreshTable(){

	// > 列
	this->setTableColumnCount(this->getMaxLayerCount()+2);

	// > 行
	ui.tableWidget->setRowCount(this->m_availableStateNodeDataList.count());

	// > 动画序列
	QTableWidgetItem* first_item = new QTableWidgetItem("动画序列");
	ui.tableWidget->setItem(0, 0, first_item);
	ui.tableWidget->setSpan(0, 0, ui.tableWidget->rowCount(), 1);

	// > 第一层
	for (int i = 0; i < this->m_availableStateNodeDataList.count(); i++){
		QJsonObject data = this->m_availableStateNodeDataList.at(i);
		QWidget* w = this->createItemByNodeData(data);
		ui.tableWidget->setCellWidget(i, 1, w);
	}

}
/*-------------------------------------------------
		控件 - 设置表格列
*/
void W_StateNodeRelationTable::setTableColumnCount(int count){
	if (count < 3){ return; }
	
	// > 列数
	ui.tableWidget->setColumnCount(count);

	// > 列名
	QStringList colName_list;
	colName_list.append("动画\n序列");
	for (int i = 0; i < count - 2; i++){
		colName_list.append("状态节点"+QString::number(i+1)+"层");
	}
	colName_list.append("状态元");
	ui.tableWidget->setHorizontalHeaderLabels(colName_list);

	// > 列宽情况
	ui.tableWidget->horizontalHeader()->resizeSection(0, 40);
}
/*-------------------------------------------------
		控件 - 设置状态节点
*/
QWidget* W_StateNodeRelationTable::createItemByNodeData(QJsonObject node_data){
	QWidget* w = new QWidget(ui.tableWidget);
	QHBoxLayout* layout = new QHBoxLayout(w);
	layout->setMargin(2);
	w->setLayout(layout);

	QLabel* label_name = new QLabel();
	QString name = node_data["节点名称"].toString();
	label_name->setText(name);

	QLabel* label_suffix = new QLabel();
	QString play_type = node_data.value("播放方式").toString();
	if (play_type == "随机播放状态元" || play_type == "随机播放嵌套集合"){
		QString ran_num = node_data.value("随机播放的次数上限").toString("0");
		label_suffix->setText("R" + ran_num +"→");
	}
	if (play_type == "顺序播放状态元" || play_type == "顺序播放嵌套集合"){
		label_suffix->setText("L→");
	}
	label_suffix->setStyleSheet("color:#DD2222;");

	layout->addWidget(label_name);
	layout->addWidget(label_suffix);
	layout->setStretch(0, 1);
	layout->setStretch(1, 0);
	return w;
}

/*-------------------------------------------------
		数据 - 获取最大层数
*/
int W_StateNodeRelationTable::getMaxLayerCount(){
	return 1;
}


/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_StateNodeRelationTable::setData_StateData(QList<QJsonObject> data_list) {
	
	// > 排除空对象
	this->m_availableStateDataList.clear();
	for (int i = 0; i < data_list.count(); i++){
		QJsonObject data = data_list.at(i);
		if (data["状态元名称"].toString() == ""){ continue; }
		this->m_availableStateDataList.append(data);
	}
	this->putDataToUi();
}
void W_StateNodeRelationTable::setData_StateNodeData(QList<QJsonObject> data_list) {

	// > 排除空对象
	this->m_availableStateNodeDataList.clear();
	for (int i = 0; i < data_list.count(); i++){
		QJsonObject data = data_list.at(i);
		if (data["节点名称"].toString() == ""){ continue; }
		this->m_availableStateNodeDataList.append(data);
	}
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_StateNodeRelationTable::putDataToUi() {
	if (this->m_availableStateDataList.count() == 0){ return; }
	if (this->m_availableStateNodeDataList.count() == 0){ return; }

	this->refreshTable();
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_StateNodeRelationTable::putUiToData() {
	//（无操作）
};