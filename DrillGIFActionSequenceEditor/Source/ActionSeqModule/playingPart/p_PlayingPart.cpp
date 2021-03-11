#include "stdafx.h"
#include "p_PlayingPart.h"

#include "defaultStateGroup/w_DefaultStateGroupEdit.h"
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
	this->m_iconSrcPath = ":/DrillGIFActionSequenceEditor/Resources/icons";
	this->m_playing = false;					//正在播放
	this->m_timer = new QTimer();				//计时器
	this->m_curFrame = 0;						//当前时间帧
	this->m_IndexFrame = QList<int>();			//动画帧的时间帧数
	this->m_IndexFrameCount = 0;				//总时间帧数
	this->m_timerInterval = 10;					//播放帧间隔
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
	connect(ui.toolButton_editGroup, &QToolButton::clicked, this, &P_PlayingPart::editDefaultStateGroup);

}

P_PlayingPart::~P_PlayingPart(){
}


/*-------------------------------------------------
		动画帧 - 帧刷新
*/
void P_PlayingPart::updateFrame(){
	if (this->m_IndexFrameCount == 0){ this->stopFrame(); return; }

	// > 切换选项
	int index = this->m_curFrame % this->m_IndexFrameCount;

	// > 播放
	for (int i = 0; i < this->m_IndexFrame.count(); i++){
		int i_count = this->m_IndexFrame.at(i);
		if (index < i_count){

			// > 图片变化
			//...

			break;
		}
		index -= i_count;
	}

	// > 帧数+1
	this->m_curFrame += 1;
}
/*-------------------------------------------------
		动画帧 - 开始
*/
void P_PlayingPart::startFrame(){

	// > 开启计时器
	this->m_timer->start(this->m_timerInterval);
	this->m_curFrame = 0; 
	this->m_playing = true;

	// > 数据初始化
	this->m_IndexFrameCount = 10;
	this->refreshSource();

	// > ui变化
	ui.widget_viewFrame->setEnabled(true);
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
	this->m_curFrame = 0;
	this->m_playing = false;

	// > ui变化
	ui.widget_viewFrame->setEnabled(false);
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
	QMessageBox::information(this, "提示", "放映区功能还未完成，按钮点击没有效果。", QMessageBox::Yes);
	if (this->isPlaying() == true){
		this->stopFrame();
		return;
	}
	this->startFrame();
}
/*-------------------------------------------------
		放映区 - 刷新播放图标
*/
void P_PlayingPart::updateIcon(){
	
	if (this->m_playing == true){
		ui.toolButton_open->setIcon(QIcon(this->m_iconSrcPath + "/player/Play_Pause.png"));
	}else{
		ui.toolButton_open->setIcon(QIcon(this->m_iconSrcPath + "/player/Play_Run.png"));
	}
}


/*-------------------------------------------------
		控件 - 编辑状态元集合
*/
void P_PlayingPart::editDefaultStateGroup(){
	QStringList state_list = this->getStateNameListWithoutEmpty();
	if (state_list.count() == 0){
		QMessageBox::information(this, "提示", "你需要配置至少一个 状态元 才能编辑。", QMessageBox::Yes);
		return;
	}

	W_DefaultStateGroupEdit d(this);
	d.setData(state_list, this->local_defaultStateList);
	if (d.exec() == QDialog::Accepted){
		this->local_defaultStateList = d.getData();
		this->putDataToUi();
	}

}
/*-------------------------------------------------
		控件 - 获取状态元名称
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
/*-------------------------------------------------
		控件 - 获取动作元名称
*/
QStringList P_PlayingPart::getActionNameList(){
	QStringList result = QStringList();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		result.append(this->local_actionDataList.at(i).value("动作元名称").toString());
	}
	return result;
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

	// > 解析动作序列（直接根据 默认集合+状态元列表+动作元列表 设置动作序列）
	QJsonObject data = QJsonObject();
	data["state_default_randomSeq"] = TTool::_QJsonArray_QStringToA_(this->local_defaultStateList);
	QJsonArray state_tank = QJsonArray();
	for (int i = 0; i < this->local_stateDataList.count(); i++){
		state_tank[i] = Drill_COAS_Init::drill_COAS_initState(this->local_stateDataList.at(i));
	}
	data["state_tank"] = state_tank;
	QJsonArray act_tank = QJsonArray();
	for (int i = 0; i < this->local_actionDataList.count(); i++){
		act_tank[i] = Drill_COAS_Init::drill_COAS_initState(this->local_actionDataList.at(i));
	}
	data["act_tank"] = act_tank;

	this->m_COAS_data = Drill_COAS_Data(data);
}
/*-------------------------------------------------
		窗口 - ui数据 -> 本地数据
*/
void P_PlayingPart::putUiToData() {
	

}
