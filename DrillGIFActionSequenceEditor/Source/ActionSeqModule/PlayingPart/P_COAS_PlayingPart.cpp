#include "stdafx.h"
#include "P_COAS_PlayingPart.h"

#include "../DataPart/P_COAS_DataPart.h"
#include "../DataPart/DefaultRandomSeq/W_COAS_DefaultRandomSeq.h"

#include "Source/ActionSeqModule/Data/S_ActionSeqDataContainer.h"
#include "Source/ProjectModule/S_ProjectManager.h"
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
P_COAS_PlayingPart::P_COAS_PlayingPart(P_COAS_StatePart* state_part, P_COAS_StateNodePart* stateNode_part, P_COAS_ActionPart* action_part, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_statePart = state_part;
	this->m_stateNodePart = stateNode_part;
	this->m_actionPart = action_part;
	this->m_parentPart = nullptr;

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
	connect(ui.toolButton_open, &QToolButton::clicked, this, &P_COAS_PlayingPart::btn_play);
	connect(ui.toolButton_playDefault, &QToolButton::clicked, this, &P_COAS_PlayingPart::btn_playDefaultState);
	connect(ui.toolButton_playSimple, &QToolButton::clicked, this, &P_COAS_PlayingPart::btn_playSimpleStateNode);
	connect(ui.toolButton_playState, &QToolButton::clicked, this, &P_COAS_PlayingPart::btn_playStateNode);
	connect(ui.toolButton_playAction, &QToolButton::clicked, this, &P_COAS_PlayingPart::btn_playAction);
	connect(ui.toolButton_editGroup, &QToolButton::clicked, this, &P_COAS_PlayingPart::editDefaultState);
	connect(ui.toolButton_editSimple, &QToolButton::clicked, this, &P_COAS_PlayingPart::editSimpleState);

	// > 图片查看块 - 缩放
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomReset);
	connect(this->m_p_AnimPictureViewer, &P_AnimPictureViewer::scaleChanged, this, &P_COAS_PlayingPart::zoomValueChanged);

}

P_COAS_PlayingPart::~P_COAS_PlayingPart(){
}


/*-------------------------------------------------
		控件 - 根据文件名获取资源文件
*/
QFileInfo P_COAS_PlayingPart::getSrcFileByName(QString file_name){
	return QFileInfo(S_ActionSeqDataContainer::getInstance()->getActionSeqDir() + "/" + this->m_COAS_mainController._drill_curBitmapName + ".png");
}
/*-------------------------------------------------
		控件 - 缩放比例切换
*/
void P_COAS_PlayingPart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}


/*-------------------------------------------------
		操作台 - 编辑状态元集合
*/
void P_COAS_PlayingPart::editDefaultState(){
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
		this->refreshTable();	//（刷新放映区表格内容）
	}
};
/*-------------------------------------------------
		操作台 - 编辑临时的简单状态元
*/
void P_COAS_PlayingPart::editSimpleState(){
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
	d.setWindowName("临时的简单状态元");
	d.setData(state_list, this->m_temp_simpleState);
	if (d.exec() == QDialog::Accepted){
		this->m_temp_simpleState = d.getData();
		this->refreshTable();
	}
}
/*-------------------------------------------------
		操作台 - 清理临时的简单状态元
*/
void P_COAS_PlayingPart::clearSimpleState(){
	this->m_temp_simpleState.clear();
}
/*-------------------------------------------------
		操作台 - 刷新表格
*/
void P_COAS_PlayingPart::refreshTable(){
	C_COAS_DataPtr data_ptr = this->m_parentPart->getCurrentData();
	if (data_ptr.isNull()){ return; }

	// > 默认的状态元集合
	QString default_str = "";
	QStringList default_list = data_ptr->m_state_default_randomSeq;
	for (int i = 0; i < default_list.count(); i++){
		default_str.append("◆ ");
		default_str.append(default_list.at(i));
		if (i < default_list.count() - 1){
			default_str.append("\n");
		}
	}
	if (default_list.count() == 0){ default_str = "无"; }
	ui.label_defaultState->setText(default_str);

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
	this->m_table_state->setSource(data_ptr->getNameList_StateNode());

	// > 动作元 列表名
	this->m_table_action->setSource(data_ptr->getNameList_Action());
}


/*-------------------------------------------------
		动画帧 - 帧刷新
*/
void P_COAS_PlayingPart::updateFrame(){
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
		动画帧 - 数据检查
*/
bool P_COAS_PlayingPart::checkData(){
	C_COAS_DataPtr data_ptr = this->m_parentPart->getCurrentData();
	if (data_ptr.isNull()){ return false; }
	this->m_statePart->checkData_StateDataList(data_ptr->m_state_tank);
	this->m_stateNodePart->checkData_StateNodeDataList(data_ptr->m_state_tank, data_ptr->m_stateNode_tank);
	this->m_actionPart->checkData_ActionDataList(data_ptr->m_act_tank);
	QStringList error_state = this->m_statePart->checkData_getErrorMessage();
	QStringList error_stateNode = this->m_stateNodePart->checkData_getErrorMessage();
	QStringList error_action = this->m_actionPart->checkData_getErrorMessage();

	// > 没有数据问题时
	if (error_state.count() == 0 && error_stateNode.count() == 0 && error_action.count() == 0){
		return true;
	}

	// > 存在数据问题时
	QString context;
	context.append("放映区暂时不能启动，因为存在以下数据问题。\n");
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
	context.append("动作元：");
	if (error_action.count() == 0){
		context.append("无\n");
	}else{
		context.append("\n");
		for (int i = 0; i < error_action.count(); i++){
			context.append("> ");
			context.append(error_action.at(i));
			context.append("\n");
		}
	}
	QMessageBox::about(this, "提示", context);
	return false;
}
/*-------------------------------------------------
		动画帧 - 开始
*/
void P_COAS_PlayingPart::startFrame(){
	C_COAS_DataPtr data_ptr = this->m_parentPart->getCurrentData();
	if (data_ptr.isNull()){ return; }

	// > 开启计时器
	this->m_timer->start(16);		//（由于不是生成gif模式，所以这里播放帧速度固定）
	this->m_playing = true;

	// > 强制刷新内容
	this->putDataToUi();

	// > 数据初始化
	QStringList src_name = data_ptr->getFileNameList();
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
void P_COAS_PlayingPart::stopFrame(){
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
bool P_COAS_PlayingPart::isPlaying(){
	return this->m_playing;
}


/*-------------------------------------------------
		按钮 - 播放
*/
void P_COAS_PlayingPart::btn_play(){

	// > 暂停
	if (this->isPlaying() == true){
		this->stopFrame();
		return;
	}

	// > 启动前，强制保存父块的数据
	this->m_parentPart->local_saveCurIndexData();

	// > 校验数据
	if (this->checkData() == false){
		return;
	}

	this->startFrame();
}
/*-------------------------------------------------
		按钮 - 播放默认的状态元集合
*/
void P_COAS_PlayingPart::btn_playDefaultState(){
	this->m_COAS_mainController.drill_COAS_setStateNodeDefault();
}
/*-------------------------------------------------
		按钮 - 播放简单状态元
*/
void P_COAS_PlayingPart::btn_playSimpleStateNode(){
	this->m_COAS_mainController.drill_COAS_setSimpleStateNode(this->m_temp_simpleState);
}
/*-------------------------------------------------
		按钮 - 播放状态节点
*/
void P_COAS_PlayingPart::btn_playStateNode(){
	QString node_name = this->m_table_state->getSelectedText();
	this->m_COAS_mainController.drill_COAS_setStateNode(node_name);
}
/*-------------------------------------------------
		按钮 - 播放动作元
*/
void P_COAS_PlayingPart::btn_playAction(){
	QString action_name = this->m_table_action->getSelectedText();
	this->m_COAS_mainController.drill_COAS_setAct(action_name);
}
/*-------------------------------------------------
		按钮 - 刷新播放图标
*/
void P_COAS_PlayingPart::updateIcon(){
	
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
		父窗口 - 设置父窗口
*/
void P_COAS_PlayingPart::setParentPart(P_COAS_DataPart* part){
	this->m_parentPart = part;
}


/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_COAS_PlayingPart::putDataToUi() {


	// > 动画序列数据 - 打包并放置在全局参数中
	C_COAS_DataPtr data_ptr = this->m_parentPart->getCurrentData();
	if (data_ptr.isNull()){ return; }
	QJsonObject COAS_data_Chinese = data_ptr->getJsonObject_Chinese();
	QJsonObject COAS_data_English = Drill_COAS_Init::drill_COAS_initSequence(COAS_data_Chinese);
	Drill_COAS_Init::getInstance()->setCOASDataByIndex(data_ptr->m_COAS_id, COAS_data_English);
	//qDebug() << "动画序列数据【中文】：" << COAS_data_Chinese;
	//qDebug() << "动画序列数据【英文】：" << COAS_data_English;

	// > 动画序列核心初始化
	this->m_COAS_mainController = Drill_COAS_MainController(COAS_data_English);
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
void P_COAS_PlayingPart::putUiToData() {
	//（无）
}
