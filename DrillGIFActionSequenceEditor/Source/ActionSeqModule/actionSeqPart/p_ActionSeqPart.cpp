#include "stdafx.h"
#include "p_ActionSeqPart.h"

#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/ProjectModule/storageGlobal/s_IniManager.h"

#include "../actionSeqData/lengthData/w_ActionSeqLength.h"
#include "../actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/Utils/widgetFormSenior/flexibleClassificationTree/private/c_FCT_Config.h"

/*
-----==========================================================-----
		类：		动画序列块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供动画序列放映，功能，并嵌套状态元、动作元的块。
		
		目标：		1.

		使用方法：
				>初始化

-----==========================================================-----
*/
P_ActionSeqPart::P_ActionSeqPart(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_slotBlock_source = false;
	this->m_cur_actionSeqIndex = -1;
	this->m_cur_actionSeq = C_ActionSeq();

	//-----------------------------------
	//----初始化ui

	// > 放映区、动作元、状态元
	this->m_statePart = new P_StatePart(parent);
	this->m_stateNodePart = new P_StateNodePart(parent);
	this->m_actionPart = new P_ActionPart(parent);
	this->m_playingPart = new P_PlayingPart(this->m_statePart, this->m_actionPart, parent);

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
	connect(this->m_p_tree, &P_FlexibleClassificationTree::currentLeafChanged, this, &P_ActionSeqPart::currentActionSeqChanged);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::menuCopyLeafTriggered, this, &P_ActionSeqPart::shortcut_copyData);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::menuPasteLeafTriggered, this, &P_ActionSeqPart::shortcut_pasteData);
	connect(this->m_p_tree, &P_FlexibleClassificationTree::menuClearLeafTriggered, this, &P_ActionSeqPart::shortcut_clearData);

	// > 水平分割线
	ui.splitter->setStretchFactor(0, 4);		//（比例变化比较奇怪，暂时这样吧）
	ui.splitter->setStretchFactor(1, 5);
	ui.splitter->handle(1)->setAttribute(Qt::WA_Hover, true);

	//-----------------------------------
	//----事件绑定
	connect(ui.lineEdit, &QLineEdit::textEdited, this->m_p_tree, &P_FlexibleClassificationTree::outerModifySelectedLeafName);
	connect(ui.toolButton_modifyLength, &QToolButton::clicked, this, &P_ActionSeqPart::modifyDataLengthInAction);

}

P_ActionSeqPart::~P_ActionSeqPart(){
}


/*-------------------------------------------------
		全数据 - 树选择变化
*/
void P_ActionSeqPart::currentActionSeqChanged(QTreeWidgetItem* item, int id, QString name){
	if (id > this->m_actionSeq_list.length()){ return; }
	if (this->m_slotBlock_source == true){ return; }

	// > 旧数据存储
	this->local_saveCurIndexData();

	// > 新数据填充
	this->local_loadIndexData(id - 1);
	
}



/*-------------------------------------------------
		数据 - 保存本地数据
*/
void P_ActionSeqPart::local_saveCurIndexData(){
	if (this->m_cur_actionSeqIndex < 0){ return; }
	if (this->m_cur_actionSeqIndex >= this->m_actionSeq_list.count()){ return; }

	// > 子控件 - 标签
	this->m_cur_actionSeq.m_name = ui.lineEdit->text();

	// > 子控件 - 动作元块
	this->m_cur_actionSeq.m_act_tank = this->m_actionPart->getData();

	// > 子控件 - 状态元块
	this->m_cur_actionSeq.m_state_tank = this->m_statePart->getData();

	// > 子控件 - 状态节点块
	this->m_cur_actionSeq.m_stateNode_tank = this->m_stateNodePart->getData();

	// > 子控件 - 放映区
	this->m_cur_actionSeq.m_state_default_randomSeq = this->m_playingPart->getDefaultStateData();
	

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 保存数据
	QJsonObject obj_actionSeq = this->m_cur_actionSeq.getJsonObject();
	this->m_actionSeq_list.replace(this->m_cur_actionSeqIndex, obj_actionSeq);
}
/*-------------------------------------------------
		数据 - 读取本地数据
*/
void P_ActionSeqPart::local_loadIndexData(int index){
	if (index < 0){ return; }
	if (index >= this->m_actionSeq_list.count()){ return; }

	// > 读取数据
	QJsonObject obj_actionSeq = this->m_actionSeq_list.at(index);
	this->m_cur_actionSeq.setJsonObject(obj_actionSeq);

	// > 子控件 - 标签
	ui.widget_editPart->setEnabled(true);
	ui.lineEdit->setText(this->m_cur_actionSeq.m_name);
	
	// > 子控件 - 动作元块
	this->m_actionPart->setData(this->m_cur_actionSeq.m_act_tank);

	// > 子控件 - 状态元块
	this->m_statePart->setData(this->m_cur_actionSeq.m_state_tank);

	// > 子控件 - 状态节点块
	this->m_stateNodePart->setData(this->m_cur_actionSeq.m_stateNode_tank);

	// > 子控件 - 放映区
	this->m_playingPart->stopFrame();
	this->m_playingPart->setDefaultStateData(this->m_cur_actionSeq.m_state_default_randomSeq);
	this->m_playingPart->refreshSource();

	this->m_cur_actionSeqIndex = index;
}



/*-------------------------------------------------
		操作 - 替换
*/
void P_ActionSeqPart::op_replace(int index, QJsonObject actiong_seq){
	if (index < 0){ return; }
	if (index >= this->m_actionSeq_list.count()){ return; }
	if (actiong_seq.isEmpty()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	this->m_actionSeq_list.replace(index, actiong_seq);
	this->local_loadIndexData(index);

	// > 更新树的名称
	this->m_p_tree->outerModifySelectedLeafName(ui.lineEdit->text());
}
/*-------------------------------------------------
		操作 - 清空
*/
void P_ActionSeqPart::op_clear(int index){
	if (index < 0){ return; }
	if (index >= this->m_actionSeq_list.count()){ return; }

	// > 编辑标记
	S_ProjectManager::getInstance()->setDirty();

	// > 执行替换
	this->m_actionSeq_list.replace(index, QJsonObject());
	this->local_loadIndexData(index);

	// > 更新树的名称
	this->m_p_tree->outerModifySelectedLeafName(ui.lineEdit->text());
}
/*-------------------------------------------------
		快捷键 - 事件
*/
void P_ActionSeqPart::keyPressEvent(QKeyEvent *event){
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
void P_ActionSeqPart::shortcut_copyData(){
	if (ui.treeWidget_ActionSeq->hasFocus() == false){ return; }
	this->m_copyed_actionSeq = this->m_cur_actionSeq.getJsonObject();
	this->m_p_tree->setLeafOuterControl_PasteActive(true);		//激活粘贴
}
/*-------------------------------------------------
		快捷键 - 粘贴
*/
void P_ActionSeqPart::shortcut_pasteData(){
	if (ui.treeWidget_ActionSeq->hasFocus() == false){ return; }
	this->op_replace(this->m_cur_actionSeqIndex, this->m_copyed_actionSeq);
}
/*-------------------------------------------------
		快捷键 - 清空
*/
void P_ActionSeqPart::shortcut_clearData(){
	if (ui.treeWidget_ActionSeq->hasFocus() == false){ return; }
	this->op_clear(this->m_cur_actionSeqIndex);
}



/*-------------------------------------------------
		大控件 - 置灰
*/
void P_ActionSeqPart::setPartGray(){
	this->m_p_FoldableTabRelater->homingAllTab();
	ui.tabWidget->setCurrentIndex(0);
	this->m_cur_actionSeqIndex = -1;
	ui.widget_editPart->setEnabled(false);
}
/*-------------------------------------------------
		大控件 - 停止播放
*/
void P_ActionSeqPart::stopPlaying(){
	this->m_playingPart->stopFrame();
}


/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_ActionSeqPart::setData(QJsonObject actionSeq, C_ActionSeqLength length) {
	this->m_slotBlock_source = true;
	S_ActionSeqDataContainer::getInstance()->setActionSeqData(actionSeq);
	S_ActionSeqDataContainer::getInstance()->setActionSeqLength(length);	//（length取自容器，不会实时变化）
	this->putDataToUi();
	this->m_slotBlock_source = false;
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QJsonObject P_ActionSeqPart::getDataActionSeqData(){
	this->putUiToData();
	return S_ActionSeqDataContainer::getInstance()->getActionSeqData();
}
C_ActionSeqLength P_ActionSeqPart::getDataActionSeqLength(){
	this->putUiToData();
	return S_ActionSeqDataContainer::getInstance()->getActionSeqLength();
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_ActionSeqPart::putDataToUi() {

	// > 树结构初始化
	QJsonObject obj_treeData = S_ActionSeqDataContainer::getInstance()->getTreeData();
	C_FCT_Config* config = dynamic_cast<C_FCT_Config*>(this->m_p_tree->createConfigData());
	config->setJsonObject(obj_treeData, this->m_p_tree);		//（存储的数据需要在load前完成赋值）
	this->m_p_tree->setConfigEx(config);

	// > 分解数据（动画序列 --> 动画序列列表）
	int data_actionSeqCount = S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_actionSeq;
	QJsonObject data_actionSeq = S_ActionSeqDataContainer::getInstance()->getActionSeqData();

	this->m_actionSeq_list = QList<QJsonObject>();
	for (int i = 0; i < data_actionSeqCount; i++){

		// > 旧名字修改（动作序列 -> 动画序列）
		QJsonValue v = data_actionSeq.value("动作序列-" + QString::number(i + 1));
		if (v.isUndefined() == false){
			data_actionSeq.insert("动画序列-" + QString::number(i + 1), v);
			data_actionSeq.remove("动作序列-" + QString::number(i + 1));
		}

		// > 添加内容
		QJsonValue value = data_actionSeq.value("动画序列-" + QString::number(i + 1));
		if (value.isUndefined() == false){
			QJsonObject obj = TTool::_JSON_parse_To_Obj_(value.toString());
			obj.insert("COAS_id", i + 1);
			obj.insert("COAS_name", obj.value("标签").toString());
			this->m_actionSeq_list.append(obj);
		}
		else{
			QJsonObject obj = QJsonObject();
			obj.insert("COAS_id", i+1);
			obj.insert("COAS_name","");
			this->m_actionSeq_list.append(obj);
		}
	}
	this->m_p_tree->loadSource(this->m_actionSeq_list, "COAS_id", "COAS_name", "COAS_type");

	// > 编辑块置灰
	this->setPartGray();	//（需要玩家重新选择一个动画序列）
	
	// > 载入动画序列数据
	//qDebug() << data_actionSeq;

}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_ActionSeqPart::putUiToData() {

	// > 保存当前数据
	this->local_saveCurIndexData();

	// > 合并数据（动画序列列表 --> 动画序列 ）
	int data_actionSeqCount = S_ActionSeqDataContainer::getInstance()->getActionSeqLength().realLen_actionSeq;
	QJsonObject data_actionSeq = S_ActionSeqDataContainer::getInstance()->getActionSeqData();

	for (int i = 0; i < data_actionSeqCount; i++){
		QJsonObject data_obj = this->m_actionSeq_list.at(i);

		// > 树数据（合并到动画序列）
		int id = data_obj.value("COAS_id").toInt();
		data_obj.insert("COAS_type", this->m_p_tree->getLeafType(id));
		data_obj.remove("COAS_id");
		data_obj.remove("COAS_name");	//（去除id和name，type保留）

		QString data_str = TTool::_JSON_stringify_(data_obj);
		data_actionSeq.insert("动画序列-" + QString::number(i + 1), data_str);
		data_actionSeq.remove("动作序列-" + QString::number(i + 1));
	}

	S_ActionSeqDataContainer::getInstance()->setActionSeqData(data_actionSeq);
	S_ActionSeqDataContainer::getInstance()->modifyTreeData(this->m_p_tree->getConfigEx()->getJsonObject());
}


/*-------------------------------------------------
		窗口 - 修改长度
*/
void P_ActionSeqPart::modifyDataLengthInAction(){
	this->m_slotBlock_source = true;

	W_ActionSeqLength d(this);
	d.setDataInModifyMode(S_ActionSeqDataContainer::getInstance()->getActionSeqLength());
	if (d.exec() == QDialog::Accepted){

		this->putUiToData();
		C_ActionSeqLength result = d.getData();
		S_ActionSeqDataContainer::getInstance()->setActionSeqLength(result);
		this->putDataToUi();
	}
	this->m_slotBlock_source = false;

}


/*-------------------------------------------------
		窗口 - 用户自定义UI读取
*/
void P_ActionSeqPart::ui_loadConfig(){

	// > 分割条
	QString data = S_IniManager::getInstance()->getConfig("COAS_MainWindow_Spliter");
	if (data != ""){
		ui.splitter->setSizes(TTool::_QList_QStringToInt_(data.split(",")));
	}

}
/*-------------------------------------------------
		窗口 - 用户自定义UI存储
*/
void P_ActionSeqPart::ui_saveConfig(){

	// > 分割条
	QStringList data = TTool::_QList_IntToQString_( ui.splitter->sizes() );
	S_IniManager::getInstance()->setConfig("COAS_MainWindow_Spliter", data.join(","));
}