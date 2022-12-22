#include "stdafx.h"
#include "P_COAS_DataPart.h"

#include "Source/ProjectModule/S_ProjectManager.h"
#include "Source/ProjectModule/StorageGlobal/S_IniManager.h"

#include "../Data/LengthData/W_COAS_Length.h"
#include "../Data/S_ActionSeqDataContainer.h"
#include "Source/Utils/WidgetFormSenior/FlexibleClassificationTree/Private/C_FCT_Config.h"

/*
-----==========================================================-----
		类：		动画序列块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		描述动画序列的块，嵌套了状态元、状态节点、动作元、放映区的块。
-----==========================================================-----
*/
P_COAS_DataPart::P_COAS_DataPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_slotBlock_source = false;
	this->m_cur_actionSeqIndex = -1;

	//-----------------------------------
	//----初始化ui

	// > 放映区、动作元、状态元
	this->m_statePart = new P_COAS_StatePart(parent);
	this->m_stateNodePart = new P_COAS_StateNodePart(this->m_statePart, parent);
	this->m_actionPart = new P_COAS_ActionPart(parent);
	this->m_playingPart = new P_COAS_PlayingPart(this->m_statePart, this->m_stateNodePart, this->m_actionPart, parent);
	this->m_stateNodePart->setParentPart(this);
	this->m_playingPart->setParentPart(this);

	// > 可折叠选项卡
	this->m_p_FoldableTabRelater = new P_FoldableTabRelater(ui.tabWidget);	//（ui中的只是示意，该工具类会重建tab）
	this->m_p_FoldableTabRelater->addPart(" 放映区  ", this->m_playingPart);
	this->m_p_FoldableTabRelater->addPart(" 状态元  ", this->m_statePart);
	this->m_p_FoldableTabRelater->addPart(" 状态节点  ", this->m_stateNodePart);
	this->m_p_FoldableTabRelater->addPart(" 动作元  ", this->m_actionPart);
	this->setPartGray();

	// > 树
	this->m_p_tree = new P_FlexibleClassificationTree(ui.treeWidget_ActionSeq);
	C_FCT_Config* config = this->m_p_tree->getConfigEx();
	config->rowHeight = 32;
	config->setCurrentMode("自定义分支（按id递增排序）");
	this->m_p_tree->setConfigEx(config);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::currentLeafChanged, this, &P_COAS_DataPart::currentActionSeqChanged);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::menuCopyLeafTriggered, this, &P_COAS_DataPart::shortcut_copyData);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::menuPasteLeafTriggered, this, &P_COAS_DataPart::shortcut_pasteData);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::menuClearLeafTriggered, this, &P_COAS_DataPart::shortcut_clearData);

	// > 水平分割线
	ui.splitter->setStretchFactor(0, 4);		//（比例变化比较奇怪，暂时这样吧）
	ui.splitter->setStretchFactor(1, 5);
	ui.splitter->handle(1)->setAttribute(Qt::WA_Hover, true);

	//-----------------------------------
	//----事件绑定
	connect(ui.lineEdit, &QLineEdit::textEdited, this->m_p_tree, &P_FlexibleClassificationTree::outerModifySelectedLeafName);
	connect(ui.toolButton_modifyLength, &QToolButton::clicked, this, &P_COAS_DataPart::modifyDataLengthInAction);

}

P_COAS_DataPart::~P_COAS_DataPart(){
}


/*-------------------------------------------------
		全数据 - 树选择变化
*/
void P_COAS_DataPart::currentActionSeqChanged(QTreeWidgetItem* item, int id, QString name){
	if (this->m_slotBlock_source == true){ return; }
	if (id < 0){ return; }
	if (id >= this->getActionSeqList().count()){ return; }

	// > 旧数据存储
	this->local_saveCurIndexData();

	// > 新数据填充
	this->local_loadIndexData(id - 1);
	
	// > 树选择变化时，清理 放映区 的临时简单状态元
	this->m_playingPart->clearSimpleState();
}



/*-------------------------------------------------
		数据 - 获取当前动画序列
*/
C_COAS_DataPtr P_COAS_DataPart::getCurrentData(){
	if (this->m_cur_actionSeqIndex < 0){ return C_COAS_DataPtr(); }
	if (this->m_cur_actionSeqIndex >= this->getActionSeqList().count()){ return C_COAS_DataPtr(); }
	return this->getActionSeqList().at(this->m_cur_actionSeqIndex);
}
/*-------------------------------------------------
		数据 - 动画序列
*/
QList<C_COAS_DataPtr> P_COAS_DataPart::getActionSeqList(){
	return S_ActionSeqDataContainer::getInstance()->getActionSeqData();
}
/*-------------------------------------------------
		数据 - 保存本地数据
*/
void P_COAS_DataPart::local_saveCurIndexData(){
	C_COAS_DataPtr action_data = this->getCurrentData();
	if (action_data.isNull()){ return; }

	// > 子控件 - 标签
	action_data->m_COAS_name = ui.lineEdit->text();

	// > 子控件 - 动作元块
	action_data->m_act_tank = this->m_actionPart->getData();

	// > 子控件 - 状态元块
	action_data->m_state_tank = this->m_statePart->getData();

	// > 子控件 - 状态节点块
	action_data->m_stateNode_tank = this->m_stateNodePart->getData();

	// > 子控件 - 放映区
	//（无）
	

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();
}
/*-------------------------------------------------
		数据 - 读取本地数据
*/
void P_COAS_DataPart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index >= this->getActionSeqList().count()){ return; }

	// > 读取数据
	C_COAS_DataPtr action_data = this->getActionSeqList().at(index);
	this->m_cur_actionSeqIndex = index;
	if (action_data.isNull()){ return; }

	// > 子控件 - 标签
	ui.widget_editPart->setEnabled(true);
	ui.lineEdit->setText(action_data->m_COAS_name);
	
	// > 子控件 - 动作元块
	this->m_actionPart->setData(action_data->m_act_tank);

	// > 子控件 - 状态元块
	this->m_statePart->setData(action_data->m_state_tank);

	// > 子控件 - 状态节点块
	this->m_stateNodePart->setData(action_data->m_stateNode_tank);

	// > 子控件 - 放映区
	this->m_playingPart->stopFrame();
	this->m_playingPart->putDataToUi();

}



/*-------------------------------------------------
		操作 - 替换
*/
void P_COAS_DataPart::op_replace(int index, QJsonObject actiong_seq){
	if (index < 0){ return; }
	if (index >= this->getActionSeqList().count()){ return; }
	if (actiong_seq.isEmpty()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	this->getActionSeqList().at(index)
		->setJsonObject_Chinese(actiong_seq);
	this->local_loadIndexData(index);

	// > 更新树的名称
	this->m_p_tree->outerModifySelectedLeafName(ui.lineEdit->text());
}
/*-------------------------------------------------
		操作 - 清空
*/
void P_COAS_DataPart::op_clear(int index){
	if (index < 0){ return; }
	if (index >= this->getActionSeqList().count()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	this->getActionSeqList().at(index)->clearTankData();
	this->local_loadIndexData(index);

	// > 更新树的名称
	this->m_p_tree->outerModifySelectedLeafName(ui.lineEdit->text());
}
/*-------------------------------------------------
		快捷键 - 事件
*/
void P_COAS_DataPart::keyPressEvent(QKeyEvent *event){
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
		快捷键 - 复制
*/
void P_COAS_DataPart::shortcut_copyData(){
	if (ui.treeWidget_ActionSeq->hasFocus() == false){ return; }
	C_COAS_DataPtr action_data = this->getCurrentData();
	if (action_data.isNull()){ return; }
	this->m_copyed_actionSeq = action_data->getJsonObject_Chinese();
	this->m_p_tree->setLeafOuterControl_PasteActive(true);		//激活粘贴
}
/*-------------------------------------------------
		快捷键 - 粘贴
*/
void P_COAS_DataPart::shortcut_pasteData(){
	if (ui.treeWidget_ActionSeq->hasFocus() == false){ return; }
	this->op_replace(this->m_cur_actionSeqIndex, this->m_copyed_actionSeq);
}
/*-------------------------------------------------
		快捷键 - 清空
*/
void P_COAS_DataPart::shortcut_clearData(){
	if (ui.treeWidget_ActionSeq->hasFocus() == false){ return; }
	this->op_clear(this->m_cur_actionSeqIndex);
}



/*-------------------------------------------------
		大控件 - 置灰
*/
void P_COAS_DataPart::setPartGray(){
	this->m_p_FoldableTabRelater->homingAllTab();
	ui.tabWidget->setCurrentIndex(0);
	this->m_cur_actionSeqIndex = -1;
	ui.widget_editPart->setEnabled(false);
}
/*-------------------------------------------------
		大控件 - 停止播放
*/
void P_COAS_DataPart::stopPlaying(){
	this->m_playingPart->stopFrame();
}
/*-------------------------------------------------
		大控件 - 刷新表格
*/
void P_COAS_DataPart::refreshPlayingPartTable(){
	this->m_playingPart->refreshTable();
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_COAS_DataPart::setData(QList<C_COAS_DataPtr> actionSeq, C_COAS_Length length) {
	this->m_slotBlock_source = true;
	S_ActionSeqDataContainer::getInstance()->setActionSeqData(actionSeq);
	S_ActionSeqDataContainer::getInstance()->setActionSeqLength(length);	//（length取自容器，不会实时变化）
	this->putDataToUi();
	this->m_slotBlock_source = false;
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QList<C_COAS_DataPtr> P_COAS_DataPart::getData_ActionSeqData(){
	this->putUiToData();
	return S_ActionSeqDataContainer::getInstance()->getActionSeqData();
}
C_COAS_Length P_COAS_DataPart::getData_ActionSeqLength(){
	this->putUiToData();
	return S_ActionSeqDataContainer::getInstance()->getActionSeqLength();
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_COAS_DataPart::putDataToUi() {

	// > 树结构 - 初始化
	QJsonObject tree_config = S_ActionSeqDataContainer::getInstance()->getTreeConfig();
	C_FCT_Config* config = dynamic_cast<C_FCT_Config*>(this->m_p_tree->createConfigData());
	config->setJsonObject(tree_config, this->m_p_tree);		//（存储的数据需要在load前完成赋值）
	this->m_p_tree->setConfigEx(config);

	// > 树结构 - 数据赋值
	QList<QJsonObject> tree_data = S_ActionSeqDataContainer::getInstance()->getTreeData();
	this->m_p_tree->loadSource(tree_data, "COAS_id", "COAS_name", "COAS_type");


	// > 编辑块置灰
	this->setPartGray();	//（需要玩家重新选择一个动画序列）
	
	// > 载入动画序列数据
	//qDebug() << tree_data;
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_COAS_DataPart::putUiToData() {

	// > 保存当前数据
	this->local_saveCurIndexData();

	// > 树数据保存
	QList<C_COAS_DataPtr> data_list = this->getActionSeqList();
	for (int i = 0; i < data_list.count(); i++){
		C_COAS_DataPtr data = data_list.at(i);
		data->m_COAS_type = this->m_p_tree->getLeafType(data->m_COAS_id);
	}

	//S_ActionSeqDataContainer::getInstance()->setActionSeqData(data_actionSeq);（由于是指针，所以不需要再修改了）
	S_ActionSeqDataContainer::getInstance()->modifyTreeConfig(this->m_p_tree->getConfigEx()->getJsonObject());
}


/*-------------------------------------------------
		窗口 - 修改长度
*/
void P_COAS_DataPart::modifyDataLengthInAction(){
	this->m_slotBlock_source = true;

	W_COAS_Length d(this);
	d.setDataInModifyMode(S_ActionSeqDataContainer::getInstance()->getActionSeqLength());
	if (d.exec() == QDialog::Accepted){

		this->putUiToData();
		C_COAS_Length result = d.getData();
		S_ActionSeqDataContainer::getInstance()->setActionSeqLength(result);
		this->putDataToUi();
	}
	this->m_slotBlock_source = false;

}


/*-------------------------------------------------
		窗口 - 用户自定义UI读取
*/
void P_COAS_DataPart::ui_loadConfig(){

	// > 分割条
	QString data = S_IniManager::getInstance()->getConfig("COAS_MainWindow_Spliter");
	if (data != ""){
		ui.splitter->setSizes(TTool::_QList_QStringToInt_(data.split(",")));
	}

}
/*-------------------------------------------------
		窗口 - 用户自定义UI存储
*/
void P_COAS_DataPart::ui_saveConfig(){

	// > 分割条
	QStringList data = TTool::_QList_IntToQString_( ui.splitter->sizes() );
	S_IniManager::getInstance()->setConfig("COAS_MainWindow_Spliter", data.join(","));
}