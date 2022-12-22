#include "stdafx.h"
#include "P_COAS_StateNodePart.h"

#include "../DataPart/P_COAS_DataPart.h"
#include "../DataPart/DefaultRandomSeq/W_COAS_DefaultRandomSeq.h"

#include "../Data/S_ActionSeqDataContainer.h"
#include "RelationTable/W_COAS_StateNodeRelationTable.h"
#include "Source/ProjectModule/S_ProjectManager.h"
#include "Source/Utils/WidgetForm/QStringListEditor/W_QStringListEditor.h"
#include "Source/Utils/Common/TTool.h"

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
						
		使用方法：	见 P_COAS_DataPart 动画序列块对此块的初始化。
-----==========================================================-----
*/
P_COAS_StateNodePart::P_COAS_StateNodePart(P_COAS_StatePart* statePart, QWidget *parent)
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
	this->m_parentPart = nullptr;
	this->m_P_COAS_StatePart = statePart;

	// > 双表格
	this->m_P_TwoTableStringFiller = new P_TwoTableStringFiller(ui.tableWidget_left, ui.tableWidget_right);

	//-----------------------------------
	//----事件绑定
	connect(this->m_table, &P_RadioTable::currentIndexChanged, this, &P_COAS_StateNodePart::currentIndexChanged);
	connect(this->m_table, &P_RadioTable::menuPasteItemTriggered, this, &P_COAS_StateNodePart::shortcut_pasteData);
	connect(this->m_table, &P_RadioTable::menuCopyItemTriggered, this, &P_COAS_StateNodePart::shortcut_copyData);
	connect(this->m_table, &P_RadioTable::menuClearItemTriggered, this, &P_COAS_StateNodePart::shortcut_clearData);

	// > 标签列表编辑
	connect(ui.pushButton_tagTank, &QPushButton::clicked, this, &P_COAS_StateNodePart::btn_editTagTank);
	connect(ui.pushButton_checkLoop, &QPushButton::clicked, this, &P_COAS_StateNodePart::btn_checkData);
	connect(ui.pushButton_relationTable, &QPushButton::clicked, this, &P_COAS_StateNodePart::btn_RelationTable);
	connect(ui.pushButton_relationTable_2, &QPushButton::clicked, this, &P_COAS_StateNodePart::btn_RelationTable);
	connect(ui.pushButton_editDefaultRandomSeq, &QPushButton::clicked, this, &P_COAS_StateNodePart::btn_editDefaultRandomSeq);
	connect(ui.pushButton_editDefaultRandomSeq_2, &QPushButton::clicked, this, &P_COAS_StateNodePart::btn_editDefaultRandomSeq);
	
	// > 播放方式
	connect(ui.comboBox_playType, &QComboBox::currentTextChanged, this, &P_COAS_StateNodePart::playTypeChanged);
	connect(ui.pushButton_moveToLeft, &QPushButton::clicked, this, &P_COAS_StateNodePart::btn_moveToLeft);
	connect(ui.pushButton_moveToRight, &QPushButton::clicked, this, &P_COAS_StateNodePart::btn_moveToRight);
	connect(ui.pushButton_moveUp, &QPushButton::clicked, this, &P_COAS_StateNodePart::btn_moveUp);
	connect(ui.pushButton_moveDown, &QPushButton::clicked, this, &P_COAS_StateNodePart::btn_moveDown);
	connect(ui.pushButton_editConfig, &QPushButton::clicked, this, &P_COAS_StateNodePart::btn_editConfig);

	// > 表单变化绑定
	connect(ui.lineEdit_name, &QLineEdit::textEdited, this, &P_COAS_StateNodePart::nameEdited);

}

P_COAS_StateNodePart::~P_COAS_StateNodePart(){
}


/*-------------------------------------------------
		控件 - 表单变化
*/
void P_COAS_StateNodePart::nameEdited(QString name){

	// > 状态节点列表变化
	this->m_table->modifyText_Selected(name);

	// > 可选的状态节点变化
	QString text = ui.comboBox_playType->currentText();
	if (text == "随机播放嵌套集合" || text == "顺序播放嵌套集合"){
		QStringList name_list = this->m_table->getAllText();//节点名称列表
		TTool::_QStringList_clearEmptyRows_(&name_list);
		this->m_P_TwoTableStringFiller->setLeftString(name_list);
		this->m_P_TwoTableStringFiller->refreshTable();
	}
}
/*-------------------------------------------------
		控件 - 修改标签列表
*/
void P_COAS_StateNodePart::btn_editTagTank(){
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
void P_COAS_StateNodePart::btn_RelationTable(){
	QList<C_COAS_StatePtr> stateData = this->m_P_COAS_StatePart->getData();
	QList<C_COAS_StateNodePtr> stateNodeData = this->getData();
	this->m_P_COAS_StatePart->checkData_StateDataList(stateData);
	this->checkData_StateNodeDataList(stateData, stateNodeData);
	QStringList error_state = this->m_P_COAS_StatePart->checkData_getErrorMessage();
	QStringList error_stateNode = this->checkData_getErrorMessage();

	// > 没有数据问题时
	if (error_state.count() == 0 && error_stateNode.count() == 0){
		W_COAS_StateNodeRelationTable d(this);
		d.setData_StateData(this->m_P_COAS_StatePart->getData());
		d.setData_StateNodeData(this->getData());
		d.exec();
		return;
	}

	// > 存在数据问题时
	QString context;
	context.append("关系表生成失败，存在以下数据问题。\n");
	context.append("状态元：");
	if (error_state.count() == 0){ 
		context.append("无\n"); 
	}else{
		context.append("\n"); 
		for (int i = 0; i < error_state.count(); i++){
			context.append("> ");
			context.append(error_state.at(i));
			context.append("\n");
		}
	}
	context.append("状态节点：");
	if (error_stateNode.count() == 0){
		context.append("无\n");
	}else{
		context.append("\n"); 
		for (int i = 0; i < error_stateNode.count(); i++){
			context.append("> ");
			context.append(error_stateNode.at(i));
			context.append("\n");
		}
	}
	QMessageBox::about(nullptr, "提示", context);
}
/*-------------------------------------------------
		控件 - 检查状态节点
*/
void P_COAS_StateNodePart::btn_checkData(){
	QList<C_COAS_StatePtr> stateData = this->m_P_COAS_StatePart->getData();
	QList<C_COAS_StateNodePtr> stateNodeData = this->getData();
	this->m_P_COAS_StatePart->checkData_StateDataList(stateData);
	this->checkData_StateNodeDataList(stateData, stateNodeData);
	QStringList error_state = this->m_P_COAS_StatePart->checkData_getErrorMessage();
	QStringList error_stateNode = this->checkData_getErrorMessage();

	// > 没有数据问题时
	if (error_state.count() == 0 && error_stateNode.count() == 0){
		QMessageBox::about(nullptr, "提示", "嵌套关系完整，暂无数据问题。"); 
		return;
	}

	// > 存在数据问题时
	QString context;
	context.append("嵌套关系存在问题，如下。\n");
	context.append("状态元：");
	if (error_state.count() == 0){ 
		context.append("无\n"); 
	}else{
		context.append("\n"); 
		for (int i = 0; i < error_state.count(); i++){
			context.append("> ");
			context.append(error_state.at(i));
			context.append("\n");
		}
	}
	context.append("状态节点：");
	if (error_stateNode.count() == 0){
		context.append("无\n");
	}else{
		context.append("\n"); 
		for (int i = 0; i < error_stateNode.count(); i++){
			context.append("> ");
			context.append(error_stateNode.at(i));
			context.append("\n");
		}
	}
	QMessageBox::about(nullptr, "提示", context);
}
/*-------------------------------------------------
		控件 - 刷新标签显示
*/
void P_COAS_StateNodePart::refreshTagTank(){
	QString context;
	context.append("[");
	context.append(this->m_curTagTank.join(","));
	context.append("]");
	ui.label_tagTank->setText(context);
}

/*-------------------------------------------------
		控件 - 获取节点名称
*/
QStringList P_COAS_StateNodePart::getNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->m_stateNodeDataList.count(); i++){
		C_COAS_StateNodePtr node_ptr = this->m_stateNodeDataList.at(i);
		if (node_ptr.isNull()){ continue; }
		result.append(node_ptr->name);
	}
	return result;
}

/*-------------------------------------------------
		控件 - 节点切换
*/
void P_COAS_StateNodePart::currentIndexChanged(int index){
	if (this->m_slotBlock_source == true){ return; }

	// > 旧的内容存储
	this->local_saveCurIndexData();

	// > 填入新的内容
	this->local_loadIndexData(index);
}


/*-------------------------------------------------
		状态节点配置 - 播放方式变化
*/
void P_COAS_StateNodePart::playTypeChanged(){
	QString text = ui.comboBox_playType->currentText();
	if (text == "随机播放状态元" || text == "顺序播放状态元"){
		this->m_P_TwoTableStringFiller->setLeftTitle("已有状态元");
		this->m_P_TwoTableStringFiller->setRightTitle("当前配置");
		QStringList name_list = this->m_P_COAS_StatePart->getNameList();
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
void P_COAS_StateNodePart::btn_moveToLeft(){
	this->m_P_TwoTableStringFiller->moveSelectedToLeft();
	this->configNameDataChanged();
}
/*-------------------------------------------------
		状态节点配置 - 右移
*/
void P_COAS_StateNodePart::btn_moveToRight(){
	this->m_P_TwoTableStringFiller->moveSelectedToRight();
	this->configNameDataChanged();
}
/*-------------------------------------------------
		状态节点配置 - 上移
*/
void P_COAS_StateNodePart::btn_moveUp(){
	this->m_P_TwoTableStringFiller->moveUpRightString();
	this->configNameDataChanged();
}
/*-------------------------------------------------
		状态节点配置 - 下移
*/
void P_COAS_StateNodePart::btn_moveDown(){
	this->m_P_TwoTableStringFiller->moveDownRightString();
	this->configNameDataChanged();
}
/*-------------------------------------------------
		状态节点配置 - 手动配置
*/
void P_COAS_StateNodePart::btn_editConfig(){
	W_QStringListEditor d(this);
	d.setParamShowingName("当前配置");
	d.setDataInModifyMode(this->m_P_TwoTableStringFiller->getRightString());
	if (d.exec() == QDialog::Accepted){
		this->m_P_TwoTableStringFiller->setRightString(d.getData());
		this->configNameDataChanged();
	}
}
/*-------------------------------------------------
		状态节点配置 - 播放默认的状态元集合
*/
void P_COAS_StateNodePart::btn_editDefaultRandomSeq(){
	C_COAS_DataPtr data_ptr = this->m_parentPart->getCurrentData();
	if (data_ptr.isNull()){ return; }

	// > 资源检查
	QStringList state_list = data_ptr->getNameList_State();
	TTool::_QStringList_clearEmptyRows_(&state_list);
	if (state_list.count() == 0){
		QMessageBox::information(this, "提示", "该动画序列状态元全部为空。你需要配置至少一个 状态元 才能编辑。", QMessageBox::Yes);
		return;
	}

	// > 窗口
	W_COAS_DefaultRandomSeq d(this);
	d.setWindowName("默认的状态元集合");
	d.setData(state_list, data_ptr->m_state_default_randomSeq);
	if (d.exec() == QDialog::Accepted){
		data_ptr->m_state_default_randomSeq = d.getData();
		this->m_parentPart->refreshPlayingPartTable();	//（刷新放映区表格内容）
	}
}
/*-------------------------------------------------
		状态节点配置 - 刷新配置
*/
void P_COAS_StateNodePart::configNameDataChanged(){

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
void P_COAS_StateNodePart::keyPressEvent(QKeyEvent *event){
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
void P_COAS_StateNodePart::op_replace(int index, QJsonObject state){
	if (index < 0){ return; }
	if (index >= this->m_table->count()){ return; }
	if (state.isEmpty()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	C_COAS_StateNodePtr node_ptr = this->m_stateNodeDataList.at(index);
	node_ptr->setJsonObject_Chinese(state, index);
	this->local_loadIndexData(index);

	// > 更新表格的名称
	this->m_table->modifyText_Selected(ui.lineEdit_name->text());
}
/*-------------------------------------------------
		操作 - 清空
*/
void P_COAS_StateNodePart::op_clear(int index){
	if (index < 0){ return; }
	if (index >= this->m_table->count()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	C_COAS_StateNodePtr node_ptr = this->m_stateNodeDataList.at(index);
	node_ptr->clearData();
	this->local_loadIndexData(index);

	// > 更新表格的名称
	this->m_table->modifyText_Selected(ui.lineEdit_name->text());
}
/*-------------------------------------------------
		快捷键 - 复制
*/
void P_COAS_StateNodePart::shortcut_copyData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1 ){ return; }
	C_COAS_StateNodePtr node_ptr = this->m_stateNodeDataList.at(this->m_last_index);
	this->m_copyed_data = node_ptr->getJsonObject_Chinese();
	this->m_table->setItemOuterControl_PasteActive(true);		//激活粘贴
}
/*-------------------------------------------------
		快捷键 - 粘贴
*/
void P_COAS_StateNodePart::shortcut_pasteData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1){ return; }
	this->op_replace(this->m_last_index, this->m_copyed_data);
}
/*-------------------------------------------------
		快捷键 - 清空
*/
void P_COAS_StateNodePart::shortcut_clearData(){
	if (ui.tableWidget->hasFocus() == false){ return; }
	if (this->m_last_index == -1){ return; }
	this->op_clear(this->m_last_index);
}


/*-------------------------------------------------
		数据 - 保存本地数据
*/
void P_COAS_StateNodePart::local_saveCurIndexData(){
	if (this->m_last_index < 0){ return; }
	if (this->m_last_index >= this->m_stateNodeDataList.count()){ return; }

	// > 直接对数据指针进行赋值
	C_COAS_StateNodePtr node_ptr = this->m_stateNodeDataList.at(this->m_last_index);

	// > 常规
	node_ptr->name = ui.lineEdit_name->text();
	node_ptr->tag_tank = this->m_curTagTank;
	node_ptr->priority = ui.spinBox_priority->value();
	node_ptr->proportion = ui.spinBox_proportion->value();
	node_ptr->canBeInterrupted = ui.checkBox_canBeInterrupted->isChecked();

	// > 播放列表
	node_ptr->play_type = ui.comboBox_playType->currentText();
	node_ptr->play_randomStateSeq = this->m_curStateNameList;
	node_ptr->play_plainStateSeq = this->m_curStateNameList;
	node_ptr->play_randomNodeSeq = this->m_curNodeNameList;
	node_ptr->play_plainNodeSeq = this->m_curNodeNameList;
	node_ptr->play_randomMax = ui.spinBox_randomMax->value();

	// > 杂项
	node_ptr->note = ui.plainTextEdit_note->toPlainText();


	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();
}
/*-------------------------------------------------
		数据 - 读取本地数据
*/
void P_COAS_StateNodePart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index >= this->m_stateNodeDataList.count()){ return; }

	// > 直接数据指针中读取值
	C_COAS_StateNodePtr node_ptr = this->m_stateNodeDataList.at(index);

	// > 常规
	ui.lineEdit_name->setText(node_ptr->name);
	this->m_curTagTank = node_ptr->tag_tank;
	this->refreshTagTank();
	ui.spinBox_priority->setValue(node_ptr->priority);
	ui.spinBox_proportion->setValue(node_ptr->proportion);
	ui.checkBox_canBeInterrupted->setChecked(node_ptr->canBeInterrupted);

	// > 播放列表
	ui.comboBox_playType->setCurrentText(node_ptr->play_type);

	QStringList name_list = node_ptr->play_randomStateSeq;
	if (name_list.isEmpty()){ name_list = node_ptr->play_plainStateSeq; }
	this->m_curStateNameList = name_list;
	TTool::_QStringList_clearEmptyRows_(&this->m_curStateNameList);

	QStringList name_list2 = node_ptr->play_randomNodeSeq;
	if (name_list2.isEmpty()){ name_list2 = node_ptr->play_plainNodeSeq; }
	this->m_curNodeNameList = name_list2;
	TTool::_QStringList_clearEmptyRows_(&this->m_curNodeNameList);

	this->playTypeChanged();	//（刷新内容）

	ui.spinBox_randomMax->setValue(node_ptr->play_randomMax);

	// > 杂项
	ui.plainTextEdit_note->setPlainText(node_ptr->note);
	
	this->m_last_index = index;
}



/*-------------------------------------------------
		数据检查 - 执行检查
*/
void P_COAS_StateNodePart::checkData_StateNodeDataList(QList<C_COAS_StatePtr> stateDataList, QList<C_COAS_StateNodePtr> stateNodeDataList){
	this->m_errorMessage.clear();
	this->m_errorInRecursion = false;
	this->m_temp_stateNodeDataList = stateNodeDataList;

	// > 空校验
	// （不校验）

	// > 重名校验
	QStringList name_list;
	QStringList repeatName_list;
	for (int i = 0; i < this->m_temp_stateNodeDataList.count(); i++){
		C_COAS_StateNodePtr node_ptr = this->m_temp_stateNodeDataList.at(i);
		QString name = node_ptr->name;
		if (name == ""){ continue; }
		if (name_list.contains(name)){
			if (repeatName_list.contains(name)){ continue; }
			repeatName_list.append(name);
		}
		name_list.append(name);
	}
	if (repeatName_list.count() > 0){
		this->m_errorMessage.append("存在重复的状态节点名称：" + repeatName_list.join(","));
	}

	// > 自连接校验
	QStringList connectSelfName_list;
	for (int i = 0; i < this->m_temp_stateNodeDataList.count(); i++){
		C_COAS_StateNodePtr node_ptr = this->m_temp_stateNodeDataList.at(i);
		QString name = node_ptr->name;
		if (name == ""){ continue; }
		QStringList n_name_list;
		if (node_ptr->play_type == "随机播放嵌套集合"){
			n_name_list = node_ptr->play_randomNodeSeq;
		}
		if (node_ptr->play_type == "顺序播放嵌套集合"){
			n_name_list = node_ptr->play_plainNodeSeq;
		}
		if (n_name_list.contains(name)){
			if (connectSelfName_list.contains(name)){ continue; }
			connectSelfName_list.append(name);
		}
	}
	if (connectSelfName_list.count() > 0){
		this->m_errorMessage.append("存在自我嵌套的状态节点：" + connectSelfName_list.join(","));
	}

	// > 状态元对应校验
	QStringList stateName_list;
	for (int i = 0; i < stateDataList.count(); i++){
		C_COAS_StatePtr stateData = stateDataList.at(i);
		QString name = stateData->name;
		if (name == ""){ continue; }
		stateName_list.append(name);
	}
	for (int i = 0; i < this->m_temp_stateNodeDataList.count(); i++){
		C_COAS_StateNodePtr node_ptr = this->m_temp_stateNodeDataList.at(i);
		QString name = node_ptr->name;
		if (name == ""){ continue; }
		QStringList missName_list;
		QStringList s_name_list;
		if (node_ptr->play_type == "随机播放状态元"){
			s_name_list = node_ptr->play_randomStateSeq;
		}
		if (node_ptr->play_type == "顺序播放状态元"){
			s_name_list = node_ptr->play_plainStateSeq;
		}
		for (int j = 0; j < s_name_list.count(); j++){
			QString s_name = s_name_list.at(j);
			if (s_name == ""){ continue; }
			if (stateName_list.contains(s_name) == false){
				missName_list.append(s_name);
			}
		}
		if (missName_list.count() > 0){
			this->m_errorMessage.append("状态节点[" + name + "]指向了并不存在的状态元：" + missName_list.join(","));
		}
	}

	// > 死循环校验
	for (int i = 0; i < this->m_temp_stateNodeDataList.count(); i++){
		C_COAS_StateNodePtr node_ptr = this->m_temp_stateNodeDataList.at(i);
		QString name = node_ptr->name;
		if (name == ""){ continue; }
		qDebug() << "死循环校验-状态节点：" << name;

		// > 递归添加节点
		this->searchNode_Recursion(node_ptr, 1);
	}

}
/*-------------------------------------------------
		数据检查 - 递归检查节点
*/
void P_COAS_StateNodePart::searchNode_Recursion(C_COAS_StateNodePtr nodeData_ptr, int layer_deep){
	if (this->m_errorInRecursion == true){ return; }
	if (layer_deep > 20){
		this->m_errorMessage.append("状态节点的嵌套中存在死循环，请重新检查状态节点的嵌套情况。");
		this->m_errorInRecursion = true;
		return;
	}

	// > 子节点为状态元
	if (nodeData_ptr->play_type == "随机播放状态元" || nodeData_ptr->play_type == "顺序播放状态元"){
		return;
	}

	// > 子节点为状态节点
	QStringList n_name_list;
	if (nodeData_ptr->play_type == "随机播放嵌套集合"){
		n_name_list = nodeData_ptr->play_randomNodeSeq;
	}
	if (nodeData_ptr->play_type == "顺序播放嵌套集合"){
		n_name_list = nodeData_ptr->play_plainNodeSeq;
	}
	for (int i = 0; i < n_name_list.count(); i++){
		QString n_name = n_name_list.at(i);
		for (int j = 0; j < this->m_temp_stateNodeDataList.count(); j++){
			C_COAS_StateNodePtr next_ptr = this->m_temp_stateNodeDataList.at(j);
			if (n_name == next_ptr->name){
				this->searchNode_Recursion(next_ptr, layer_deep + 1);
				break;
			}
		}
	}
}
/*-------------------------------------------------
		数据检查 - 获取检查信息
*/
QStringList P_COAS_StateNodePart::checkData_getErrorMessage(){
	return this->m_errorMessage;
}



/*-------------------------------------------------
		父窗口 - 设置父窗口
*/
void P_COAS_StateNodePart::setParentPart(P_COAS_DataPart* part){
	this->m_parentPart = part;
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_COAS_StateNodePart::setData(QList<C_COAS_StateNodePtr> stateNodeDataList) {
	this->m_slotBlock_source = true;

	// > 状态节点表格
	this->m_stateNodeDataList = stateNodeDataList;
	if (this->m_stateNodeDataList.count() == 0){
		int data_count = S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_stateNode;
		for (int i = 0; i < data_count; i++){
			this->m_stateNodeDataList.append(C_COAS_StateNodePtr());
		}
	}

	this->putDataToUi();
	this->m_slotBlock_source = false;
	this->m_table->selectStart();
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QList<C_COAS_StateNodePtr> P_COAS_StateNodePart::getData(){
	this->putUiToData();
	return this->m_stateNodeDataList;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_COAS_StateNodePart::putDataToUi() {

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
void P_COAS_StateNodePart::putUiToData() {

	// > 保存当前数据
	this->local_saveCurIndexData();

}
