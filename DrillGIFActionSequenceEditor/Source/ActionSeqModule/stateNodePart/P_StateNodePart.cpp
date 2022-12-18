#include "stdafx.h"
#include "p_StateNodePart.h"

#include "RelationTable/W_StateNodeRelationTable.h"
#include "../actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/Utils/WidgetForm/QStringListEditor/W_QStringListEditor.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		状态节点块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供状态节点编辑功能。
					【该功能块是一个大集合组件，功能交织复杂，多注意注释部分】
		
		目标：		->控件
						->单选表格
						->快速表单
					->快捷键
						->复制
						->粘贴
						->清空
					->本地数据
						->列表
						->索引
						->窗口交互

		使用方法：
				>初始化

-----==========================================================-----
*/
P_StateNodePart::P_StateNodePart(P_StatePart* statePart, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_last_index = -1;
	this->m_slotBlock_source = false;
	this->m_curTagTank.clear();

	//-----------------------------------
	//----初始化ui
	QStyledItemDelegate* itemDelegate_playType = new QStyledItemDelegate(ui.comboBox_playType);
	ui.comboBox_playType->setItemDelegate(itemDelegate_playType);
	ui.comboBox_playType->setStyleSheet("QComboBox QAbstractItemView::item {min-height:24px;}");

	// > 编辑表格
	this->m_table = new P_RadioTable(ui.tableWidget);
	C_RaTConfig rat_config = C_RaTConfig();
	rat_config.zeroFillCount = 2;
	rat_config.rowHeight = 22;
	this->m_table->setConfigParam(rat_config);			//固定参数
	this->m_table->setItemOuterControlEnabled(true);	//开启右键菜单

	// > 状态元块
	this->m_P_StatePart = statePart;

	// > 双表格
	this->m_P_TwoTableStringFiller = new P_TwoTableStringFiller(ui.tableWidget_left, ui.tableWidget_right);

	//-----------------------------------
	//----事件绑定
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_StateNodePart::currentIndexChanged);
	connect(this->m_table, &P_RadioTable::menuPasteItemTriggered, this, &P_StateNodePart::shortcut_pasteData);
	connect(this->m_table, &P_RadioTable::menuCopyItemTriggered, this, &P_StateNodePart::shortcut_copyData);
	connect(this->m_table, &P_RadioTable::menuClearItemTriggered, this, &P_StateNodePart::shortcut_clearData);

	// > 标签列表编辑
	connect(ui.pushButton_tagTank, &QPushButton::clicked, this, &P_StateNodePart::btn_editTagTank);
	connect(ui.pushButton_relationTable, &QPushButton::clicked, this, &P_StateNodePart::btn_RelationTable);

	// > 播放方式
	connect(ui.comboBox_playType, &QComboBox::currentTextChanged, this, &P_StateNodePart::playTypeChanged);
	connect(ui.pushButton_moveToLeft, &QPushButton::clicked, this, &P_StateNodePart::btn_moveToLeft);
	connect(ui.pushButton_moveToRight, &QPushButton::clicked, this, &P_StateNodePart::btn_moveToRight);
	connect(ui.pushButton_moveUp, &QPushButton::clicked, this, &P_StateNodePart::btn_moveUp);
	connect(ui.pushButton_moveDown, &QPushButton::clicked, this, &P_StateNodePart::btn_moveDown);
	connect(ui.pushButton_editConfig, &QPushButton::clicked, this, &P_StateNodePart::btn_editConfig);

	// > 表单变化绑定
	connect(ui.lineEdit_name, &QLineEdit::textEdited, this->m_table, &P_RadioTable::modifyText_Selected);

}

P_StateNodePart::~P_StateNodePart(){
}


/*-------------------------------------------------
		控件 - 修改标签列表
*/
void P_StateNodePart::btn_editTagTank(){
	W_QStringListEditor d(this);
	d.setDataInModifyMode(this->m_curTagTank);
	if (d.exec() == QDialog::Accepted){
		this->m_curTagTank = d.getData();
		this->refreshTagTank();
	}
}
/*-------------------------------------------------
		控件 - 打开状态节点关系表
*/
void P_StateNodePart::btn_RelationTable(){
	W_StateNodeRelationTable d(this);
	d.setData_StateData(this->m_P_StatePart->getData());
	d.setData_StateNodeData(this->getData());
	d.exec();
}
/*-------------------------------------------------
		控件 - 刷新标签显示
*/
void P_StateNodePart::refreshTagTank(){
	QString context;
	context.append("[");
	context.append(this->m_curTagTank.join(","));
	context.append("]");
	ui.label_tagTank->setText(context);
}

/*-------------------------------------------------
		控件 - 获取节点名称
*/
QStringList P_StateNodePart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->m_stateNodeDataList.count(); i++){
		result.append(this->m_stateNodeDataList.at(i).value("节点名称").toString());
	}
	return result;
}

/*-------------------------------------------------
		控件 - 节点切换
*/
void P_StateNodePart::currentIndexChanged(int index){
	if (this->m_slotBlock_source == true){ return; }

	// > 旧的内容存储
	this->local_saveCurIndexData();

	// > 填入新的内容
	this->local_loadIndexData(index);
}


/*-------------------------------------------------
		状态节点配置 - 播放方式变化
*/
void P_StateNodePart::playTypeChanged(){
	QString text = ui.comboBox_playType->currentText();
	if (text == "随机播放状态元" || text == "顺序播放状态元"){
		this->m_P_TwoTableStringFiller->setLeftTitle("已有状态元");
		this->m_P_TwoTableStringFiller->setRightTitle("当前配置");
		QStringList name_list = this->m_P_StatePart->getNameList();
		TTool::_QStringList_clearEmptyRows_(&name_list);
		this->m_P_TwoTableStringFiller->setLeftString(name_list);
		this->m_P_TwoTableStringFiller->setRightString(this->m_curStateNameList);
		this->m_P_TwoTableStringFiller->refreshTable();
	}
	if (text == "随机播放嵌套集合" || text == "顺序播放嵌套集合"){
		this->m_P_TwoTableStringFiller->setLeftTitle("已有状态节点");
		this->m_P_TwoTableStringFiller->setRightTitle("当前配置");
		QStringList name_list = this->getNameList();//节点名称列表
		TTool::_QStringList_clearEmptyRows_(&name_list);
		this->m_P_TwoTableStringFiller->setLeftString(name_list);
		this->m_P_TwoTableStringFiller->setRightString(this->m_curNodeNameList);
		this->m_P_TwoTableStringFiller->refreshTable();
	}

	if (text == "随机播放状态元" || text == "随机播放嵌套集合"){
		ui.widget_randomMax->setVisible(true);
		this->m_P_TwoTableStringFiller->setRightRowHeaderVisible(false);
	}else{
		ui.widget_randomMax->setVisible(false);
		this->m_P_TwoTableStringFiller->setRightRowHeaderVisible(true);
	}
}
/*-------------------------------------------------
		状态节点配置 - 左移
*/
void P_StateNodePart::btn_moveToLeft(){
	this->m_P_TwoTableStringFiller->moveSelectedToLeft();
	this->configNameDataChanged();
}
/*-------------------------------------------------
		状态节点配置 - 右移
*/
void P_StateNodePart::btn_moveToRight(){
	this->m_P_TwoTableStringFiller->moveSelectedToRight();
	this->configNameDataChanged();
}
/*-------------------------------------------------
		状态节点配置 - 上移
*/
void P_StateNodePart::btn_moveUp(){
	this->m_P_TwoTableStringFiller->moveUpRightString();
	this->configNameDataChanged();
}
/*-------------------------------------------------
		状态节点配置 - 下移
*/
void P_StateNodePart::btn_moveDown(){
	this->m_P_TwoTableStringFiller->moveDownRightString();
	this->configNameDataChanged();
}
/*-------------------------------------------------
		状态节点配置 - 手动配置
*/
void P_StateNodePart::btn_editConfig(){
	W_QStringListEditor d(this);
	d.setParamShowingName("当前配置");
	d.setDataInModifyMode(this->m_P_TwoTableStringFiller->getRightString());
	if (d.exec() == QDialog::Accepted){
		this->m_P_TwoTableStringFiller->setRightString(d.getData());
		this->configNameDataChanged();
	}
}
/*-------------------------------------------------
		状态节点配置 - 刷新配置
*/
void P_StateNodePart::configNameDataChanged(){

	// > 变化的数据存储
	QString text = ui.comboBox_playType->currentText();
	if (text == "随机播放状态元" || text == "顺序播放状态元"){
		this->m_curStateNameList = this->m_P_TwoTableStringFiller->getRightString();
		TTool::_QStringList_clearEmptyRows_(&this->m_curStateNameList);
	}
	if (text == "随机播放嵌套集合" || text == "顺序播放嵌套集合"){
		this->m_curNodeNameList = this->m_P_TwoTableStringFiller->getRightString();
		TTool::_QStringList_clearEmptyRows_(&this->m_curNodeNameList);
	}
}


/*-------------------------------------------------
		快捷键 - 事件
*/
void P_StateNodePart::keyPressEvent(QKeyEvent *event){
	if (event->modifiers() & Qt::ControlModifier){
		if (event->key() == Qt::Key_C){
			this->shortcut_copyData();
		}
		if (event->key() == Qt::Key_V){
			this->shortcut_pasteData();
		}
	}
	if (event->key() == Qt::Key_Delete){
		this->shortcut_clearData();
	}
}
/*-------------------------------------------------
		操作 - 替换
*/
void P_StateNodePart::op_replace(int index, QJsonObject state){
	if (index < 0){ return; }
	if (index >= this->m_table->count()){ return; }
	if (state.isEmpty()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	this->m_stateNodeDataList.replace(index, state);
	this->local_loadIndexData(index);

	// > 更新表格的名称
	this->m_table->modifyText_Selected(ui.lineEdit_name->text());
}
/*-------------------------------------------------
		操作 - 清空
*/
void P_StateNodePart::op_clear(int index){
	if (index < 0){ return; }
	if (index >= this->m_table->count()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	this->m_stateNodeDataList.replace(index, QJsonObject());
	this->local_loadIndexData(index);

	// > 更新表格的名称
	this->m_table->modifyText_Selected(ui.lineEdit_name->text());
}
/*-------------------------------------------------
		快捷键 - 复制
*/
void P_StateNodePart::shortcut_copyData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1 ){ return; }
	this->m_copyed_data = this->m_stateNodeDataList.at(m_last_index);
	this->m_table->setItemOuterControl_PasteActive(true);		//激活粘贴
}
/*-------------------------------------------------
		快捷键 - 粘贴
*/
void P_StateNodePart::shortcut_pasteData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1){ return; }
	this->op_replace(this->m_last_index, this->m_copyed_data);
}
/*-------------------------------------------------
		快捷键 - 清空
*/
void P_StateNodePart::shortcut_clearData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1){ return; }
	this->op_clear(this->m_last_index);
}


/*-------------------------------------------------
		数据 - 保存本地数据
*/
void P_StateNodePart::local_saveCurIndexData(){
	if (this->m_last_index < 0){ return; }
	if (this->m_last_index >= this->m_stateNodeDataList.count()){ return; }

	// > 表单数据
		QJsonObject obj_edit;
		obj_edit.insert("节点名称", ui.lineEdit_name->text());
		obj_edit.insert("节点标签", TTool::_JSON_stringify_(this->m_curTagTank));
		obj_edit.insert("节点优先级", QString::number(ui.spinBox_priority->value()));
		obj_edit.insert("节点权重", QString::number(ui.spinBox_proportion->value()));
		obj_edit.insert("可被动作元打断", ui.checkBox_canBeInterrupted->isChecked() ? "true" : "false");
		obj_edit.insert("备注", ui.plainTextEdit_note->toPlainText());
	QJsonObject obj_org = this->m_stateNodeDataList.at(this->m_last_index);
	TTool::_QJsonObject_put_(&obj_org, obj_edit);

	// > 播放方式
		QJsonObject obj_play;
		obj_play.insert("播放方式", ui.comboBox_playType->currentText());
		obj_play.insert("随机播放状态元", TTool::_JSON_stringify_(this->m_curStateNameList));
		obj_play.insert("顺序播放状态元", TTool::_JSON_stringify_(this->m_curStateNameList));
		obj_play.insert("随机播放嵌套集合", TTool::_JSON_stringify_(this->m_curNodeNameList));
		obj_play.insert("顺序播放嵌套集合", TTool::_JSON_stringify_(this->m_curNodeNameList));
		obj_play.insert("随机播放的次数上限", QString::number(ui.spinBox_randomMax->value()));
	TTool::_QJsonObject_put_(&obj_org, obj_play);

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	this->m_stateNodeDataList.replace(this->m_last_index, obj_org);
}
/*-------------------------------------------------
		数据 - 读取本地数据
*/
void P_StateNodePart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index >= this->m_stateNodeDataList.count()){ return; }

	// > 表单数据
	QJsonObject obj_data = this->m_stateNodeDataList.at(index);
	//qDebug() << obj_data;
		ui.lineEdit_name->setText(obj_data.value("节点名称").toString());
		this->m_curTagTank = TTool::_JSON_parse_To_QListQString_(obj_data.value("节点标签").toString());
		this->refreshTagTank();
		ui.spinBox_priority->setValue(obj_data.value("节点优先级").toString().toInt());
		ui.spinBox_proportion->setValue(obj_data.value("节点权重").toString().toInt());
		ui.checkBox_canBeInterrupted->setChecked(obj_data.value("可被动作元打断").toString() == "true");
		ui.plainTextEdit_note->setPlainText(obj_data.value("备注").toString());

	// > 播放方式
		ui.comboBox_playType->setCurrentText(obj_data.value("播放方式").toString());

		QStringList arr = TTool::_JSON_parse_To_QListQString_(obj_data.value("随机播放状态元").toString());
		if (arr.isEmpty()){ arr = TTool::_JSON_parse_To_QListQString_(obj_data.value("顺序播放状态元").toString()); }
		this->m_curStateNameList = arr;
		TTool::_QStringList_clearEmptyRows_(&this->m_curStateNameList);
		
		QStringList arr2 = TTool::_JSON_parse_To_QListQString_(obj_data.value("随机播放嵌套集合").toString());
		if (arr2.isEmpty()){ arr2 = TTool::_JSON_parse_To_QListQString_(obj_data.value("顺序播放嵌套集合").toString()); }
		this->m_curNodeNameList = arr2;
		TTool::_QStringList_clearEmptyRows_(&this->m_curNodeNameList);
		this->playTypeChanged();
		
		ui.spinBox_randomMax->setValue(obj_data.value("随机播放的次数上限").toString().toInt());

	this->m_last_index = index;
}



/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_StateNodePart::setData(QList<QJsonObject> stateNodeDataList) {
	this->m_slotBlock_source = true;

	// > 状态节点表格
	this->m_stateNodeDataList = stateNodeDataList;
	if (this->m_stateNodeDataList.count() == 0){
		int data_count = S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_stateNode;
		for (int i = 0; i < data_count; i++){
			this->m_stateNodeDataList.append(QJsonObject());
		}
	}

	this->putDataToUi();
	this->m_slotBlock_source = false;
	this->m_table->selectStart();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QList<QJsonObject> P_StateNodePart::getData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->m_stateNodeDataList;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_StateNodePart::putDataToUi() {

	// > 名称列表
	this->m_table->setSource(this->getNameList());

	// > 当前选中的数据
	this->local_loadIndexData(this->m_last_index);

	// > 强制刷新一次（防止空白节点不刷新）
	this->playTypeChanged();
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_StateNodePart::putUiToData() {

	// > 保存当前数据
	this->local_saveCurIndexData();

}
