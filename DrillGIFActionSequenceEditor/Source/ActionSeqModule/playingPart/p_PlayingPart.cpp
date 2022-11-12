#include "stdafx.h"
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
P_PlayingPart::P_PlayingPart(P_StatePart* state_part, P_ActionPart* action_part, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数
	this->m_statePart = state_part;
	this->m_actionPart = action_part;

	// > 参数值
	this->m_playing = false;					//正在播放
	this->m_timer = new QTimer();				//计时器
	connect(this->m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));

	//-----------------------------------
	//----初始化ui

	// > 编辑表格
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

	// > 图片查看块
	this->m_p_AnimPictureViewer = new P_AnimPictureViewer(ui.widget_view);
	this->m_p_AnimPictureViewer->rebuildUI();


	//-----------------------------------
	//----事件绑定
	connect(ui.toolButton_open, &QToolButton::clicked, this, &P_PlayingPart::btn_play);
	connect(ui.toolButton_playDefault, &QToolButton::clicked, this, &P_PlayingPart::btn_playDefault);
	connect(ui.toolButton_playState, &QToolButton::clicked, this, &P_PlayingPart::btn_playState);
	connect(ui.toolButton_playAction, &QToolButton::clicked, this, &P_PlayingPart::btn_playAction);
	connect(ui.toolButton_editGroup, &QToolButton::clicked, this, &P_PlayingPart::editDefaultStateGroup);

	// > 图片查看块 - 缩放
	connect(ui.toolButton_zoom_in, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomIn);
	connect(ui.toolButton_zoom_out, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomOut);
	connect(ui.toolButton_zoom_regular, &QPushButton::clicked, this->m_p_AnimPictureViewer, &P_AnimPictureViewer::zoomReset);
	connect(this->m_p_AnimPictureViewer, &P_AnimPictureViewer::scaleChanged, this, &P_PlayingPart::zoomValueChanged);

}

P_PlayingPart::~P_PlayingPart(){
}

/*-------------------------------------------------
		控件 - 编辑状态元集合
*/
void P_PlayingPart::editDefaultStateGroup(){
	// > 未播放时，刷新资源
	if (this->isPlaying() == false){
		this->refreshSource();
	}

	// > 资源检查
	QStringList state_list = this->getStateNameListWithoutEmpty();
	if (state_list.count() == 0){
		QMessageBox::information(this, "提示", "你需要配置至少一个 状态元 才能编辑。", QMessageBox::Yes);
		return;
	}

	// > 窗口
	W_DefaultStateGroupEdit d(this);
	d.setData(state_list, this->local_defaultStateList);
	if (d.exec() == QDialog::Accepted){
		this->local_defaultStateList = d.getData();
		this->putDataToUi();
	}

}
/*-------------------------------------------------
		控件 - 根据文件名获取资源文件
*/
QFileInfo P_PlayingPart::getSrcFileByName(QString file_name){
	return QFileInfo(S_ActionSeqDataContainer::getInstance()->getActionSeqDir() + "/" + this->m_COAS_mainController._drill_curBitmapName + ".png");
}
/*-------------------------------------------------
		控件 - 获取名称
*/
QStringList P_PlayingPart::getStateNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		result.append(this->local_stateDataList.at(i).value("状态元名称").toString());
	}
	return result;
}
QStringList P_PlayingPart::getStateNameListWithoutEmpty(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		QString name = this->local_stateDataList.at(i).value("状态元名称").toString();
		if (name == ""){ continue; }
		result.append(name);
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
		控件 - 获取全部关联文件名（去重）
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

	// > 去重
	result = result.toSet().toList();

	return result;
}
/*-------------------------------------------------
		控件 - 缩放比例切换
*/
void P_PlayingPart::zoomValueChanged(double value){
	ui.label_zoomValue->setText( QString::number(value * 100)+"%" );
}


/*-------------------------------------------------
		动画帧 - 帧刷新
*/
void P_PlayingPart::updateFrame(){
	if (this->m_playing == false){ return; }

	// > 帧刷新
	this->m_COAS_mainController.drill_COAS_update();

	// > 播放图片
	this->m_p_AnimPictureViewer->setAnimFile(this->getSrcFileByName(this->m_COAS_mainController._drill_curBitmapName));
	
	// > 显示当前状态
	//QString cur_state = this->m_COAS_data._drill_state_curCom;
	//if (this->m_COAS_data.drill_COAS_isPlayingAct()){
	//	ui.label_playingName->setText(QString("动作元【")+this->m_COAS_data._drill_act_curCom+"】");
	//}else{
	//	if (cur_state != ""){		//（cur_state为空时不会变化资源图片）
	//		ui.label_playingName->setText(QString("状态元【") + cur_state + "】");
	//	}
	//}
	//if (cur_state != ""){ ui.label_playingState->setText(cur_state); }
	//ui.label_playingAction->setText(this->m_COAS_data._drill_act_curCom);
	//ui.label_stateSeq->setText(this->m_COAS_data._drill_state_curSeq.join(","));

}
/*-------------------------------------------------
		动画帧 - 开始
*/
void P_PlayingPart::startFrame(){

	// > 开启计时器
	this->m_timer->start(16);		//（由于不是生成gif模式，所以这里播放帧速度固定）
	this->m_playing = true;

	// > 数据初始化
	this->refreshSource();
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
	this->updateIcon();
}
/*-------------------------------------------------
		动画帧 - 正在播放
*/
bool P_PlayingPart::isPlaying(){
	return this->m_playing;
}

/*-------------------------------------------------
		放映区 - 播放
*/
void P_PlayingPart::btn_play(){
	if (this->isPlaying() == true){
		this->stopFrame();
		return;
	}
	this->startFrame();
}
/*-------------------------------------------------
		按钮 - 加入默认状态元
*/
void P_PlayingPart::btn_playDefault(){
	this->m_COAS_mainController.drill_COAS_setStateNodeDefault();
}
/*-------------------------------------------------
		按钮 - 加入状态元
*/
void P_PlayingPart::btn_playState(){
	QStringList state_list = this->m_table_state->getSelectedText_Multi();
	this->m_COAS_mainController.drill_COAS_setSimpleStateNode(state_list);
}
/*-------------------------------------------------
		按钮 - 加入动作元
*/
void P_PlayingPart::btn_playAction(){
	QString action_name = this->m_table_action->getSelectedText();
	this->m_COAS_mainController.drill_COAS_setAct(action_name);
}
/*-------------------------------------------------
		放映区 - 刷新播放图标
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
		ui.label_playingAction->setText("");
		ui.label_playingName->setText("");
		ui.label_playingState->setText("");
		ui.label_stateSeq->setText("");
	}
}



/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_PlayingPart::refreshSource(){
	this->local_stateDataList = this->m_statePart->getData();
	this->local_actionDataList = this->m_actionPart->getData();
	this->putDataToUi();
}
/*-------------------------------------------------
		窗口 - 设置数据
*/
void P_PlayingPart::setDefaultStateData(QStringList defaultStateList) {
	this->local_defaultStateList = defaultStateList;
}
/*-------------------------------------------------
		窗口 - 取出数据
*/
QStringList P_PlayingPart::getDefaultStateData(){
	this->putUiToData();

	// > 校验
	//...
	
	return this->local_defaultStateList;
}
/*-------------------------------------------------
		窗口 - 本地数据 -> ui数据
*/
void P_PlayingPart::putDataToUi() {

	// > 表格选择
	this->m_table_action->setSource(this->getActionNameList());
	this->m_table_state->setSource(this->getStateNameList());
	
	// > 状态元
	QString text = "";
	for (int i = 0; i < this->local_defaultStateList.count(); i++){
		text = text + "◆ " + this->local_defaultStateList.at(i);
		if (i < this->local_defaultStateList.count()-1){
			text = text + "\n";
		}
	}
	if (this->local_defaultStateList.count() == 0){ text = "无"; }
	ui.label_defaultState->setText(text);

	// > 解析动画序列（直接根据 默认集合+状态元列表+动作元列表 设置动画序列）
	QJsonObject data = QJsonObject();
	data["state_default_randomSeq"] = TTool::_QJsonArray_QStringToA_(this->local_defaultStateList);
	QJsonArray state_tank = QJsonArray();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		state_tank.append(Drill_COAS_Init::drill_COAS_initState(this->local_stateDataList.at(i)));
	}
	data["state_tank"] = state_tank;
	QJsonArray act_tank = QJsonArray();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		act_tank.append(Drill_COAS_Init::drill_COAS_initAct(this->local_actionDataList.at(i)));
	}
	data["act_tank"] = act_tank;

	// > 动画序列核心初始化
	this->m_COAS_mainController = Drill_COAS_MainController(data);
	this->m_COAS_mainController.drill_COAS_update();
	this->m_p_AnimPictureViewer->clearSource();
	this->m_p_AnimPictureViewer->setAnimFile(this->getSrcFileByName(this->m_COAS_mainController._drill_curBitmapName));
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_PlayingPart::putUiToData() {
	

}
