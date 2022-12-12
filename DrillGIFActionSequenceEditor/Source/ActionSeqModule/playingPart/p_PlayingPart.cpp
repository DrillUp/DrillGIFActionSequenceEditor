﻿#include "stdafx.h"
#include "p_PlayingPart.h"

#include "defaultStateGroup/w_DefaultStateGroupEdit.h"

#include "Source/ActionSeqModule/actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		放映区.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		该部分提供动作元编辑功能。
		
		目标：		->控件
						->单选表格（动作元）
						->单选表格（状态元）
						->快速表单
					->控件（动画帧）
						->动画帧播放器
						->图片查看块
					->本地数据
						->窗口交互

		使用方法：
				>初始化

-----==========================================================-----
*/
P_PlayingPart::P_PlayingPart(P_StatePart* state_part, P_StateNodePart* stateNode_part, P_ActionPart* action_part, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_actionSeq_curIndex = 0;
	this->m_statePart = state_part;
	this->m_stateNodePart = stateNode_part;
	this->m_actionPart = action_part;

	// > 参数值
	this->m_playing = false;					//正在播放
	this->m_timer = new QTimer();				//计时器
	connect(this->m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));

	//-----------------------------------
	//----初始化ui

	// > 图片查看块
	this->m_p_AnimPictureViewer = new P_AnimPictureViewer(ui.widget_view);
	this->m_p_AnimPictureViewer->rebuildUI();

	// > 操作台 - 编辑表格
	this->m_table_action = new P_RadioTable(ui.tableWidget_action);
	this->m_table_state = new P_RadioTable(ui.tableWidget_state);
	C_RaTConfig rat_config = C_RaTConfig();
	rat_config.zeroFillCount = 2;
	rat_config.rowHeight = 22;
	this->m_table_action->setConfigParam(rat_config);	//固定参数
	this->m_table_action->setItemOuterControlEnabled(false);
	C_RaTConfig rat_config2 = C_RaTConfig();
	rat_config2.zeroFillCount = 2;
	rat_config2.rowHeight = 22;
	rat_config2.isMultiSelect = true;
	this->m_table_state->setConfigParam(rat_config2);
	this->m_table_state->setItemOuterControlEnabled(false);


	//-----------------------------------
	//----事件绑定
	connect(ui.toolButton_open, &QToolButton::clicked, this, &P_PlayingPart::btn_play);
	connect(ui.toolButton_playDefault, &QToolButton::clicked, this, &P_PlayingPart::btn_playDefaultState);
	connect(ui.toolButton_playSimple, &QToolButton::clicked, this, &P_PlayingPart::btn_playSimpleStateNode);
	connect(ui.toolButton_playState, &QToolButton::clicked, this, &P_PlayingPart::btn_playStateNode);
	connect(ui.toolButton_playAction, &QToolButton::clicked, this, &P_PlayingPart::btn_playAction);
	connect(ui.toolButton_editGroup, &QToolButton::clicked, this, &P_PlayingPart::editDefaultState);
	connect(ui.toolButton_editSimple, &QToolButton::clicked, this, &P_PlayingPart::editSimpleState);

	// > 图片查看块 - 缩放
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomReset);
	connect(this->m_p_AnimPictureViewer, &P_AnimPictureViewer::scaleChanged, this, &P_PlayingPart::zoomValueChanged);

}

P_PlayingPart::~P_PlayingPart(){
}


/*-------------------------------------------------
		控件 - 根据文件名获取资源文件
*/
QFileInfo P_PlayingPart::getSrcFileByName(QString file_name){
	return QFileInfo(S_ActionSeqDataContainer::getInstance()->getActionSeqDir() + "/" + this->m_COAS_mainController._drill_curBitmapName + ".png");
}
/*-------------------------------------------------
		控件 - 缩放比例切换
*/
void P_PlayingPart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}


/*-------------------------------------------------
		操作台 - 编辑状态元集合
*/
void P_PlayingPart::editDefaultState(){

	// > 未播放时，刷新资源
	if (this->isPlaying() == false){
		this->refreshCurActionSeq();
	}

	// > 资源检查
	QStringList state_list = this->getStateNameList();
	TTool::_QStringList_clearEmptyRows_(&state_list);
	if (state_list.count() == 0){
		QMessageBox::information(this, "提示", "该动画序列状态元全部为空。你需要配置至少一个 状态元 才能编辑。", QMessageBox::Yes);
		return;
	}

	// > 窗口
	W_DefaultStateGroupEdit d(this);
	d.setWindowName("默认的状态元集合");
	d.setData(state_list, this->local_defaultSeq);
	if (d.exec() == QDialog::Accepted){
		this->local_defaultSeq = d.getData();
		this->putDataToUi();
	}
};
/*-------------------------------------------------
		操作台 - 编辑临时的简单状态元
*/
void P_PlayingPart::editSimpleState(){
	
	// > 资源检查
	QStringList state_list = this->getStateNameList();
	TTool::_QStringList_clearEmptyRows_(&state_list);
	if (state_list.count() == 0){
		QMessageBox::information(this, "提示", "该动画序列状态元全部为空。你需要配置至少一个 状态元 才能编辑。", QMessageBox::Yes);
		return;
	}

	// > 窗口
	W_DefaultStateGroupEdit d(this);
	d.setWindowName("临时的简单状态元");
	d.setData(state_list, this->m_temp_simpleState);
	if (d.exec() == QDialog::Accepted){
		this->m_temp_simpleState = d.getData();
		this->refreshTable();
	}
}
/*-------------------------------------------------
		操作台 - 获取名称
*/
QStringList P_PlayingPart::getStateNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		result.append(this->local_stateDataList.at(i).value("状态元名称").toString());
	}
	return result;
}
QStringList P_PlayingPart::getStateNodeNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_stateNodeDataList.count(); i++){
		result.append(this->local_stateNodeDataList.at(i).value("节点名称").toString());
	}
	return result;
}
QStringList P_PlayingPart::getActionNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		result.append(this->local_actionDataList.at(i).value("动作元名称").toString());
	}
	return result;
}
/*-------------------------------------------------
		操作台 - 获取全部关联文件名（去重）
*/
QStringList P_PlayingPart::getRelatFileNameList(){
	QStringList result = QStringList();

	// > 动作元资源
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		QJsonObject obj_action = this->local_actionDataList.at(i);
		QString str_src = obj_action.value("资源-动作元").toString();
		QStringList src_list = TTool::_JSON_parse_To_QListQString_(str_src);
		result.append(src_list);
	}

	// > 状态元资源
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		QJsonObject obj_state = this->local_stateDataList.at(i);
		QString str_src = obj_state.value("资源-状态元").toString();
		QStringList src_list = TTool::_JSON_parse_To_QListQString_(str_src);
		result.append(src_list);
	}

	// > 状态节点资源
	//（无资源）

	// > 去重
	result = result.toSet().toList();

	return result;
}
/*-------------------------------------------------
		操作台 - 刷新表格
*/
void P_PlayingPart::refreshTable(){

	// > 默认的状态元集合
	QString text_default = "";
	for (int i = 0; i < this->local_defaultSeq.count(); i++){
		text_default.append("◆ ");
		text_default.append(this->local_defaultSeq.at(i));
		if (i < this->local_defaultSeq.count() - 1){
			text_default.append("\n");
		}
	}
	if (this->local_defaultSeq.count() == 0){ text_default = "无"; }
	ui.label_defaultState->setText(text_default);

	// > 临时的简单状态元集合
	QString text_simple = "";
	for (int i = 0; i < this->m_temp_simpleState.count(); i++){
		text_simple.append("◆ ");
		text_simple.append(this->m_temp_simpleState.at(i));
		if (i < this->m_temp_simpleState.count() - 1){
			text_simple.append("\n");
		}
	}
	if (this->m_temp_simpleState.count() == 0){ text_simple = "无"; }
	ui.label_simpleState->setText(text_simple);

	// > 状态节点 列表名
	this->m_table_state->setSource(this->getStateNodeNameList());

	// > 动作元 列表名
	this->m_table_action->setSource(this->getActionNameList());
}


/*-------------------------------------------------
		动画帧 - 帧刷新
*/
void P_PlayingPart::updateFrame(){
	if (this->m_playing == false){ return; }


	// > 帧刷新
	this->m_COAS_mainController.drill_COAS_update();


	// > 播放图片（每帧）
	QString bitmap_name = this->m_COAS_mainController._drill_curBitmapName;
	this->m_p_AnimPictureViewer->setAnimFile(this->getSrcFileByName(bitmap_name));
	this->m_p_AnimPictureViewer->setTint(this->m_COAS_mainController._drill_curBitmapTint);
	//qDebug() << bitmap_name;
	

	// > 显示当前状态
	ui.label_actionSeqId->setText(QString::number( this->m_COAS_mainController._drill_data["id"].toInt() ));
	ui.label_curStateName->setText(this->m_COAS_mainController.drill_COAS_getCurStateName());
	ui.label_curActionName->setText(this->m_COAS_mainController.drill_COAS_getCurActName());
	ui.label_allRoot->setText(this->m_COAS_mainController.drill_COAS_getCurStateName_AllRoot());

}
/*-------------------------------------------------
		动画帧 - 开始
*/
void P_PlayingPart::startFrame(){

	// > 开启计时器
	this->m_timer->start(16);		//（由于不是生成gif模式，所以这里播放帧速度固定）
	this->m_playing = true;

	// > 数据初始化
	this->refreshCurActionSeq();
	QStringList src_name = this->getRelatFileNameList();
	QList<QFileInfo> scr_infoList = QList<QFileInfo>();
	for (int i = 0; i < src_name.count(); i++){
		scr_infoList.append(this->getSrcFileByName(src_name.at(i)));
	}
	this->m_p_AnimPictureViewer->setSource(scr_infoList);

	// > ui变化
	ui.groupBox_seq->setEnabled(true);
	ui.widget_operateFrame->setEnabled(true);
	ui.toolButton_playDefault->setEnabled(true);
	ui.toolButton_playSimple->setEnabled(true);
	this->updateIcon();
	emit playStarted();
}
/*-------------------------------------------------
		动画帧 - 暂停
*/
void P_PlayingPart::stopFrame(){
	if (this->isPlaying() == false){ return; }

	// > 关闭计时器
	this->m_timer->stop();
	this->m_playing = false;

	// > ui变化
	ui.groupBox_seq->setEnabled(false);
	ui.widget_operateFrame->setEnabled(false);
	ui.toolButton_playDefault->setEnabled(false);
	ui.toolButton_playSimple->setEnabled(false);
	this->updateIcon();
}
/*-------------------------------------------------
		动画帧 - 正在播放
*/
bool P_PlayingPart::isPlaying(){
	return this->m_playing;
}


/*-------------------------------------------------
		按钮 - 播放
*/
void P_PlayingPart::btn_play(){
	if (this->isPlaying() == true){
		this->stopFrame();
		return;
	}
	this->startFrame();
}
/*-------------------------------------------------
		按钮 - 播放默认的状态元集合
*/
void P_PlayingPart::btn_playDefaultState(){
	this->m_COAS_mainController.drill_COAS_setStateNodeDefault();
}
/*-------------------------------------------------
		按钮 - 播放简单状态元
*/
void P_PlayingPart::btn_playSimpleStateNode(){
	this->m_COAS_mainController.drill_COAS_setSimpleStateNode(this->m_temp_simpleState);
}
/*-------------------------------------------------
		按钮 - 播放状态节点
*/
void P_PlayingPart::btn_playStateNode(){
	QString node_name = this->m_table_state->getSelectedText();
	this->m_COAS_mainController.drill_COAS_setStateNode(node_name);
}
/*-------------------------------------------------
		按钮 - 播放动作元
*/
void P_PlayingPart::btn_playAction(){
	QString action_name = this->m_table_action->getSelectedText();
	this->m_COAS_mainController.drill_COAS_setAct(action_name);
}
/*-------------------------------------------------
		按钮 - 刷新播放图标
*/
void P_PlayingPart::updateIcon(){
	
	// > 启动播放时
	if (this->m_playing == true){
		ui.toolButton_open->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Pause.png"));
		ui.widget_playingPart->setStyleSheet("#widget_playingPart{ background-color: rgb(255, 255, 255); }");
	
	// > 暂停播放时
	}else{
		ui.toolButton_open->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Run.png"));
		ui.widget_playingPart->setStyleSheet("#widget_playingPart{ background-color: rgb(242, 242, 242); }");
		ui.label_actionSeqId->setText("");
		ui.label_curStateName->setText("");
		ui.label_curActionName->setText("");
		ui.label_allRoot->setText("");
	}
}



/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_PlayingPart::refreshCurActionSeq(){
	this->local_stateDataList = this->m_statePart->getData();
	this->local_stateNodeDataList = this->m_stateNodePart->getData();
	this->local_actionDataList = this->m_actionPart->getData();

	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_PlayingPart::setData_DefaultSeq(QStringList defaultSeq) {
	this->local_defaultSeq = defaultSeq;
}
void P_PlayingPart::setData_CurIndex(int index) {
	this->m_actionSeq_curIndex = index;
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QStringList P_PlayingPart::getData_DefaultSeq(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_defaultSeq;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_PlayingPart::putDataToUi() {


	// > 动画序列数据 - 初始化
	QJsonObject data = QJsonObject();
	data["id"] = this->m_actionSeq_curIndex;
	data["visible"] = true;
	data["pause"] = false;
	data["waitForPreload"] = true;

	// > 动画序列数据 - 容器 - 状态元序列
	QJsonArray state_tank = QJsonArray();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		state_tank.append(Drill_COAS_Init::drill_COAS_initState(this->local_stateDataList.at(i)));
	}
	data["state_tank"] = state_tank;

	// > 动画序列数据 - 容器 - 状态节点序列
	QJsonArray stateNode_tank = QJsonArray();
	for (int i = 0; i < this->local_stateNodeDataList.count(); i++){
		stateNode_tank.append(Drill_COAS_Init::drill_COAS_initStateNode(this->local_stateNodeDataList.at(i)));
	}
	data["stateNode_tank"] = stateNode_tank;

	// > 动画序列数据 - 容器 - 动作元序列
	QJsonArray act_tank = QJsonArray();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		act_tank.append(Drill_COAS_Init::drill_COAS_initAct(this->local_actionDataList.at(i)));
	}
	data["act_tank"] = act_tank;

	// > 动画序列数据 - 容器 - 默认的状态元集合
	data["state_default_randomSeq"] = TTool::_QJsonArray_QStringToA_(this->local_defaultSeq);

	// > 动画序列数据 - 打包并放置在全局参数中
	Drill_COAS_Init::getInstance()->setCOASDataByIndex(this->m_actionSeq_curIndex, data);


	// > 动画序列核心初始化
	this->m_COAS_mainController = Drill_COAS_MainController(data);
	this->m_COAS_mainController.drill_COAS_update();
	this->m_p_AnimPictureViewer->clearSource();
	this->m_p_AnimPictureViewer->setAnimFile(this->getSrcFileByName(this->m_COAS_mainController._drill_curBitmapName));
	this->m_p_AnimPictureViewer->setTint(this->m_COAS_mainController._drill_curBitmapTint);

	// > 刷新表格
	this->refreshTable();
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_PlayingPart::putUiToData() {
	

}
