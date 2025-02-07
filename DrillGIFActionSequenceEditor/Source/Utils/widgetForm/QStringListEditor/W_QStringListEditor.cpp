#include "stdafx.h"
#include "W_QStringListEditor.h"

#include "W_QStringEditor.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		字符串编辑列表 窗口.cpp
		版本：		v1.07
		作者：		drill_up
		所属模块：	工具模块
		功能：		数据编辑的字符串列表控制。
		
		目标：		1.直接编辑字符串列表。
					2.导入、导出。

		使用方法：
				> 创建填写列表
					W_QStringListEditor d(this);
					d.setData(this->local_class, this->local_data);
					if (d.exec() == QDialog::Accepted) {
						this->local_data = d.getData();
					}
				> 高级（字符串将对指定object中的一个具体参数进行编辑）
					
-----==========================================================-----
*/

W_QStringListEditor::W_QStringListEditor(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->local_dataList;
	this->m_paramShowingName = "字符串";
	this->m_paramShowingSuffix = "";
	this->m_paramDescription = "";
	this->m_paramListDescription = "";
	this->m_notNull = false;
	this->m_noRepeat = false;
	this->m_size_width = 400;
	this->m_size_height = 450;

	// > 窗口内容刷新
	this->refreshWindow();


	//-----------------------------------
	//----事件绑定
	connect(ui.pushButton_add, &QPushButton::clicked, this, &W_QStringListEditor::addOneRow);
	connect(ui.pushButton_modify, &QPushButton::clicked, this, &W_QStringListEditor::modifyOneRow);
	connect(ui.pushButton_delete, &QPushButton::clicked, this, &W_QStringListEditor::deleteOneRow);
	connect(ui.tableWidget, &QTableWidget::itemDoubleClicked, this, &W_QStringListEditor::modifyOneRow);		//双击编辑行
	connect(ui.pushButton_moveUp, &QPushButton::clicked, this, &W_QStringListEditor::moveUpOneRow);
	connect(ui.pushButton_moveDown, &QPushButton::clicked, this, &W_QStringListEditor::moveDownOneRow);
	connect(ui.pushButton_clear, &QPushButton::clicked, this, &W_QStringListEditor::clearAllRow);
	connect(ui.pushButton_copy, &QPushButton::clicked, this, &W_QStringListEditor::shortcut_copyData);
	connect(ui.pushButton_paste, &QPushButton::clicked, this, &W_QStringListEditor::shortcut_pasteData);
	connect(ui.buttonBox, &QDialogButtonBox::accepted, this, &W_QStringListEditor::acceptData);

	//-----------------------------------
	//----初始化ui
	ui.tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);		//自适应大小
	ui.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);				//单行模式
	TTool::_chinese_(ui.buttonBox);
}

W_QStringListEditor::~W_QStringListEditor(){
}




/*-------------------------------------------------
		控件 - 设置参数显示名
*/
void W_QStringListEditor::setParamShowingName(QString name){
	this->m_paramShowingName = name;
};
/*-------------------------------------------------
		控件 - 设置参数后缀名
*/
void W_QStringListEditor::setParamShowingSuffix(QString suffix) {
	this->m_paramShowingSuffix = suffix;
}
/*-------------------------------------------------
		控件 - 设置参数编辑的描述
*/
void W_QStringListEditor::setParamDescription(QString description){
	this->m_paramDescription = description;
}
/*-------------------------------------------------
		控件 - 设置列表编辑的描述
*/
void W_QStringListEditor::setParamListDescription(QString description){
	this->m_paramListDescription = description;
}
/*-------------------------------------------------
		控件 - 设置非空
*/
void W_QStringListEditor::setConditionNotNull(bool b){
	this->m_notNull = b;
};
/*-------------------------------------------------
		控件 - 设置不可重复
*/
void W_QStringListEditor::setConditionNoRepeat(bool b){
	this->m_noRepeat = b;
};
/*-------------------------------------------------
		控件 - 设置窗口大小
*/
void W_QStringListEditor::setConditionWindowSize(int width, int height){
	this->m_size_width = width;
	this->m_size_height = height;
	this->resize(this->m_size_width, this->m_size_height);
}


/*-----------------------------------
		控件 - 添加
*/
void W_QStringListEditor::addOneRow() {
	W_QStringEditor d(this, this);
	d.setParamShowingName(m_paramShowingName);
	d.setParamShowingSuffix(m_paramShowingSuffix);
	d.setParamDescription(m_paramDescription);
	d.setConditionNotNull(m_notNull);
	d.setConditionNoRepeat(m_noRepeat);
	d.setDataInAddMode();
	if (d.exec() == QDialog::Accepted ){
		QString str = d.getData();
		local_dataList.append(str);
		this->refreshTable();
	}

}
/*-----------------------------------
		控件 - 编辑
*/
int W_QStringListEditor::modifyOneRow() {
	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, tr("提示"), tr("请先选择需要编辑的一行。"));
		return -1;
	}
	int pos = range.at(0).topRow();

	QString old_str = local_dataList.at(pos);
	W_QStringEditor d(this, this);
	d.setParamShowingName(m_paramShowingName);
	d.setParamShowingSuffix(m_paramShowingSuffix);
	d.setParamDescription(m_paramDescription);
	d.setConditionNotNull(m_notNull);
	d.setConditionNoRepeat(m_noRepeat);
	d.setDataInModifyMode(old_str);
	if (d.exec() == QDialog::Accepted) {
		QString new_str = d.getData();
		local_dataList.replace(pos, new_str);
		this->refreshTable();
	}
	return pos;
}
/*-----------------------------------
		控件 - 删除
*/
int W_QStringListEditor::deleteOneRow() {
	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, tr("提示"), tr("请先选择需要删除的一行。"));
		return -1;
	}
	int pos = range.at(0).topRow();

	local_dataList.removeAt(pos);
	this->refreshTable();

	return pos;
}
/*-----------------------------------
		控件 - 上移
*/
int W_QStringListEditor::moveUpOneRow(){
	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, tr("提示"), tr("请先选择一行。"));
		return -1;
	}
	int pos = range.at(0).topRow();

	// > 执行上移
	if (pos <= 0){ return -1; }
	local_dataList.swap(pos, pos - 1);

	// > 刷新内容
	this->refreshTable();

	// > 刷新选中
	ui.tableWidget->clearSelection();
	ui.tableWidget->selectRow(pos - 1);
	return pos;
}
/*-----------------------------------
		控件 - 下移
*/
int W_QStringListEditor::moveDownOneRow(){
	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) {
		QMessageBox::warning(this, tr("提示"), tr("请先选择一行。"));
		return -1;
	}
	int pos = range.at(0).topRow();

	// > 执行下移
	if (pos >= local_dataList.count()-1){ return -1; }
	local_dataList.swap(pos, pos + 1);

	// > 刷新内容
	this->refreshTable();

	// > 刷新选中
	ui.tableWidget->clearSelection();
	ui.tableWidget->selectRow(pos + 1);
	return pos;
}
/*-------------------------------------------------
		控件 - 清空
*/
bool W_QStringListEditor::clearAllRow(){
	switch (QMessageBox::information(this, tr("提示"), tr("你确定要清空全部?"), tr("确定"), tr("取消"), 0, 1)){
		case 0:
			this->local_dataList.clear();
			this->refreshTable();
			return true;
		case 1:
		default:
			return false;
	}
}
/*-----------------------------------
		控件 - 刷新表格
*/
void W_QStringListEditor::refreshTable() {
	ui.tableWidget->setColumnCount(1);
	ui.tableWidget->setRowCount(this->local_dataList.count());

	// > 表头
	ui.tableWidget->setHorizontalHeaderLabels(QStringList() << m_paramShowingName);

	// > 表数据
	for (int i = 0; i < this->local_dataList.count(); i++) {
		QString str = this->local_dataList.at(i);
		str.append(" ");
		str.append(this->m_paramShowingSuffix);
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(str));
	}
}
/*-----------------------------------
		控件 - 刷新窗口内容
*/
void W_QStringListEditor::refreshWindow(){

	// > 窗口名称
	this->setWindowTitle(tr("编辑") + this->m_paramShowingName + tr("列表"));
	ui.groupBox->setTitle(this->m_paramShowingName);

	// > 描述
	ui.label_description->setText(this->m_paramListDescription);
	if (this->m_paramListDescription.isEmpty()){
		ui.label_description->setVisible(false);
	}else{
		ui.label_description->setVisible(true);
	}

	// > 窗口大小
	this->resize(this->m_size_width, this->m_size_height);
}



/*-------------------------------------------------
		快捷键 - 事件
*/
void W_QStringListEditor::keyPressEvent(QKeyEvent *event){

	if (event->modifiers() & Qt::ControlModifier){
		if (event->key() == Qt::Key_C){
			this->shortcut_copyData();
		}
		if (event->key() == Qt::Key_V){
			this->shortcut_pasteData();
		}
	}
	if (event->key() == Qt::Key_Delete){
		this->shortcut_deleteData();
	}
}
/*-------------------------------------------------
		快捷键 - 复制
*/
void W_QStringListEditor::shortcut_copyData(){

	// > 获取字符串
	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) { return; }
	int pos = range.at(0).topRow();
	QString data = this->local_dataList.at(pos);

	// > 启用粘贴功能
	if (data != ""){
		ui.pushButton_paste->setEnabled(true);
	}

	// > 剪贴板 - 清除
	QClipboard* clipboard = QApplication::clipboard();
	clipboard->clear();

	// > 剪贴板 - 赋值
	QMimeData* mimeData = new QMimeData();
	mimeData->setText(data);
	clipboard->setMimeData(mimeData);
}
/*-------------------------------------------------
		快捷键 - 粘贴
*/
void W_QStringListEditor::shortcut_pasteData(){

	// > 剪贴板 - 获取
	QClipboard* clipboard = QApplication::clipboard();
	const QMimeData* mimeData = clipboard->mimeData();
	QString data = mimeData->text();
	if (data.isEmpty()){ return; }

	// > 赋值字符串
	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) { return; }
	int pos = range.at(0).topRow();

	// > 不可重复 - 相同字符串自动+1
	if (this->m_noRepeat == true){
		while (true){
			data = TTool::_QString_suffix_addOne_(data);
			if (this->local_dataList.contains(data) == false){
				this->local_dataList.insert(pos + 1, data);
				break;
			}
		}

	// > 可重复 - 直接复制
	}else{
		this->local_dataList.insert(pos + 1, data);
	}

	this->refreshTable();
}
/*-------------------------------------------------
		快捷键 - 删除
*/
void W_QStringListEditor::shortcut_deleteData(){
	QList<QTableWidgetSelectionRange> range = ui.tableWidget->selectedRanges();
	if (range.size() == 0) { return; }
	int pos = range.at(0).topRow();
	this->local_dataList.removeAt(pos);
	this->refreshTable();
}


/*-------------------------------------------------
		窗口 - 设置数据（修改）
*/
void W_QStringListEditor::setDataInModifyMode(QStringList data) {

	// > 窗口内容刷新
	this->refreshWindow();

	// > 窗口数据
	this->local_dataList = data;
	this->putDataToUi();
}

/*-------------------------------------------------
		窗口 - 取出数据
*/
QStringList W_QStringListEditor::getData(){
	return this->local_dataList;
};
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void W_QStringListEditor::putDataToUi() {
	//直接控制datalist
	this->refreshTable();
};
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void W_QStringListEditor::putUiToData() {
	//直接控制datalist
};
/*-------------------------------------------------
		窗口 - 提交数据（校验）
*/
void W_QStringListEditor::acceptData(){
	this->putUiToData();

	emit acceptDataSignal(this->local_dataList);
	this->accept();
};