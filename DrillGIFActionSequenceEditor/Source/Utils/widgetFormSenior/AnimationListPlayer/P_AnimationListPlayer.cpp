#include "stdafx.h"
#include "P_AnimationListPlayer.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		动画帧播放器.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		该模块能够控制动画帧播放，相当于 动画帧编辑块 的一个扩展粘合剂。
					【播放器 与 编辑器 是独立的，播放器发出信号，编辑器接收并处理】
					【具体去看看编辑器接收信号处理方式，可以此使播放器连接其它的控件】

		子功能：	-> 动画帧
						-> 切换选项（信号）
						-> 按钮点击（信号）
						-> 固定次数暂停 / 永久循环
						-> 时间帧初始化
					-> 控件控制
						-> 控制 动画帧编辑块

		说明：		> 通过连接 信号 来实现播放效果，需要连接到子控件的槽。
					> 时间帧需要初始化

		使用方法：
				> 初始化
					this->m_p_AnimationListPlayer = new P_AnimationListPlayer(this);
					this->m_p_AnimationListPlayer->setAnimationListEditor(m_p_AnimationListEditor);

					QHBoxLayout* layout = new QHBoxLayout(ui.widget_player);
					ui.widget_player->setLayout(layout);
					layout->setMargin(0);
					layout->addWidget(this->m_p_AnimationListPlayer);

-----==========================================================-----
*/
P_AnimationListPlayer::P_AnimationListPlayer(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----初始化参数

	// > 播放器
	this->m_timer = new QTimer();				//计时器
	connect(this->m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
	this->m_timerInterval = 10;					//计时器间隔
	this->m_curFrame = 0;						//当前时间帧
	this->m_IndexFrame.clear();					//动画帧的时间帧数
	this->m_IndexFrameCount = 0;				//总时间帧数
	this->m_isPlaying = false;					//是否正在播放

	// > 下拉框高度
	TTool::_ChangeCombox_itemHeight_(ui.combo, 26);

	// > 倒放
	this->m_isBackRun = false;					//是否倒放

	// > 按钮
	this->m_iconPlaying = false;

	// > 编辑块
	this->m_animEditor = nullptr;


	//-----------------------------------
	//----初始化ui

	// > 播放类型
	ui.combo->clear();
	ui.combo->addItems(QStringList() << "播放一次" << "播放二次" << "播放三次" << "播放四次" << "播放五次" << "永久循环");

	// > 按钮
	ui.toolButton_jumpToStart->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Start.png"));
	ui.toolButton_last->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Last.png"));
	ui.toolButton_next->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Next.png"));
	ui.toolButton_play->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Run.png"));
	ui.toolButton_backRun->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Right.png"));

	ui.toolButton_new->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_New.png"));
	ui.toolButton_ExportGIF->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportGIF.png"));
	ui.toolButton_setting->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Setting.png"));

	//-----------------------------------
	//----事件绑定

	// > 按钮
	connect(ui.toolButton_jumpToStart, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_homing);
	connect(ui.toolButton_last, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_last);
	connect(ui.toolButton_next, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_next);
	connect(ui.toolButton_play, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_play);
	connect(ui.toolButton_backRun, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_switchRunDirection);

	connect(ui.toolButton_new, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_new);
	connect(ui.toolButton_ExportGIF, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_exportGIF);
	connect(ui.toolButton_setting, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_setting);


	// > 初始隐藏
	ui.toolButton_ExportGIF->setVisible(false);
}
P_AnimationListPlayer::~P_AnimationListPlayer(){
}


/*-------------------------------------------------
		播放器 - 帧刷新（私有）
*/
void P_AnimationListPlayer::updateFrame(){
	if (this->m_IndexFrameCount == 0){ this->stopFrame(); return; }

	// > 切换选项
	int index = this->m_curFrame % this->m_IndexFrameCount;
	if (this->m_isBackRun == false){

		// > 正向播放
		for (int i = 0; i < this->m_IndexFrame.count(); i++){
			int i_count = this->m_IndexFrame.at(i);
			if (index < i_count){

				// > 信号变化
				emit signal_frameIndexChanged(i);

				break;
			}
			index -= i_count;
		}
	
	}else{

		// > 倒放
		for (int i = this->m_IndexFrame.count()-1; i >=0; i--){
			int i_count = this->m_IndexFrame.at(i);
			if (index < i_count){

				// > 信号变化
				emit signal_frameIndexChanged(i);

				break;
			}
			index -= i_count;
		}
	}


	// > 固定次数暂停
	if ( (this->m_isBackRun == false && index == 0) ||
	 	 (this->m_isBackRun == true && index == 0)){
		int loop_time = qFloor(this->m_curFrame / this->m_IndexFrameCount);
		
		if (loop_time >= 1 && ui.combo->currentText() == "播放一次"){ this->stopFrame(); return; }
		if (loop_time >= 2 && ui.combo->currentText() == "播放二次"){ this->stopFrame(); return; }
		if (loop_time >= 2 && ui.combo->currentText() == "播放两次"){ this->stopFrame(); return; }
		if (loop_time >= 3 && ui.combo->currentText() == "播放三次"){ this->stopFrame(); return; }
		if (loop_time >= 4 && ui.combo->currentText() == "播放四次"){ this->stopFrame(); return; }
		if (loop_time >= 5 && ui.combo->currentText() == "播放五次"){ this->stopFrame(); return; }

	}

	// > 帧数+1
	this->m_curFrame += 1;
}


/*-------------------------------------------------
		播放器 - 开始
*/
void P_AnimationListPlayer::startFrame(){
	this->m_timer->start(this->m_timerInterval);
	this->m_curFrame = 0; 
	this->m_isPlaying = true;
	this->updateIcon();
	emit signal_frameButton_started();
}
/*-------------------------------------------------
		播放器 - 暂停
*/
void P_AnimationListPlayer::stopFrame(){
	this->m_timer->stop();
	this->m_curFrame = 0;
	this->m_isPlaying = false;
	this->updateIcon();
}
/*-------------------------------------------------
		播放器 - 是否正在播放
*/
bool P_AnimationListPlayer::isPlaying(){
	return this->m_isPlaying;
}
/*-------------------------------------------------
		播放器 - 设置播放类型
*/
void P_AnimationListPlayer::setPlayType(QStringList play_type){
	ui.combo->clear();
	ui.combo->addItems(play_type);
}
/*-------------------------------------------------
		播放器 - 设置时间帧列表（每个动画帧占用的时间帧数）
*/
void P_AnimationListPlayer::setPlayFrame(QList<int> indexFrame){
	this->m_IndexFrame = indexFrame;
	this->m_IndexFrameCount = 0;
	for (int i = 0; i < this->m_IndexFrame.count(); i++){
		this->m_IndexFrameCount += this->m_IndexFrame.at(i);
	}
}
/*-------------------------------------------------
		播放器 - 设置播放间隔
*/
void P_AnimationListPlayer::setPlayTimerInterval(int timerInterval){
	this->m_timerInterval = timerInterval;
}


/*-------------------------------------------------
		倒放 - 设置倒放
*/
void P_AnimationListPlayer::setBackRun(bool backRun){
	if (this->m_isBackRun == backRun){ return; }
	this->m_isBackRun = backRun;
	this->updateIcon();
	emit signal_backRunChanged(backRun);
}


/*-------------------------------------------------
		按钮 - 刷新播放图标
*/
void P_AnimationListPlayer::updateIcon(){
	
	// > 正向播放
	if (this->m_isBackRun == false){
		ui.toolButton_backRun->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Right.png"));

	// > 倒放
	}else{
		ui.toolButton_backRun->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Left.png"));
	}

	if (this->m_iconPlaying == this->m_isPlaying){ return; }
	this->m_iconPlaying = this->m_isPlaying;
	
	// > 启动播放时
	if (this->m_iconPlaying == true){
		ui.toolButton_play->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Pause.png"));
		ui.combo->setEnabled(false);
		//（蓝光效果）
		ui.widget_controlArea->setStyleSheet("\n #widget_controlArea{ background-color: qlineargradient(spread:pad, x1:0, y1:1, x2:0, y2:0, stop:0 rgba(138,255,168,105), stop:0.8 rgba(138,255,168,0), stop:1 rgba(255,255,255,0) ); }");
		
	// > 暂停播放时
	}else{
		ui.toolButton_play->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Run.png"));
		ui.combo->setEnabled(true);
		ui.widget_controlArea->setStyleSheet("");
	}
}
/*-------------------------------------------------
		按钮 - 切至首帧
*/
void P_AnimationListPlayer::btn_homing(){
	this->btn_pause();
	emit signal_frameButton_homing();
}
/*-------------------------------------------------
		按钮 - 上一帧
*/
void P_AnimationListPlayer::btn_last(){
	this->btn_pause();
	emit signal_frameButton_last();
}
/*-------------------------------------------------
		按钮 - 下一帧
*/
void P_AnimationListPlayer::btn_next(){
	this->btn_pause();
	emit signal_frameButton_next();
}
/*-------------------------------------------------
		按钮 - 播放/暂停
*/
void P_AnimationListPlayer::btn_play(){
	if (this->isPlaying() == true){
		this->btn_pause();	//（二次点击为暂停）
		return;
	}
	this->startFrame();
}
void P_AnimationListPlayer::btn_pause(){
	this->stopFrame();
}
/*-------------------------------------------------
		按钮 - 正向播放/倒放
*/
void P_AnimationListPlayer::btn_switchRunDirection(){
	if (this->m_isBackRun == true){
		this->setBackRun(false);	//（倒放按钮点击后，变正向播放）
	}else{
		this->setBackRun(true);		//（正向播放按钮点击后，变倒放）
	}
}

/*-------------------------------------------------
		附加按钮 - 新建帧
*/
void P_AnimationListPlayer::btn_new(){
	if (this->m_animEditor == nullptr){ return; }
	this->btn_pause();
	this->m_animEditor->action_append();
}
/*-------------------------------------------------
		附加按钮 - 播放设置
*/
void P_AnimationListPlayer::btn_setting(){
	if (this->m_animEditor == nullptr){ return; }
	this->btn_pause();
	this->m_animEditor->openWindow_setConfigParam_ALE();
}
/*-------------------------------------------------
		附加按钮 - 导出GIF
*/
void P_AnimationListPlayer::btn_exportGIF(){
	if (this->m_animEditor == nullptr){ return; }
	this->btn_pause();
	this->m_animEditor->action_exportGIF_All();
}



/*-------------------------------------------------
		编辑器 - 设置
*/
void P_AnimationListPlayer::setAnimationListEditor(P_ALE_Editor* animEditor){
	this->m_animEditor = animEditor;

	// > 按钮显示
	ui.toolButton_ExportGIF->setVisible(true);

	// > 计时器间隔设置
	if (this->m_animEditor->getCurrentData_Unit() == C_ALE_DataSet::SecondUnit){
		this->m_timerInterval = 10;
	}
	if (this->m_animEditor->getCurrentData_Unit() == C_ALE_DataSet::FrameUnit){
		this->m_timerInterval = 16;
	}

	// > 按钮连接
	connect(this, &P_AnimationListPlayer::signal_frameIndexChanged, this->m_animEditor, &P_ALE_Editor::selectIndex_Single);				//播放器 索引 信号连接
	connect(this, &P_AnimationListPlayer::signal_frameIndexChanged, this->m_animEditor, &P_ALE_Editor::scrollToSelected);				//播放器 滚动到索引 信号连接
	
	connect(this, &P_AnimationListPlayer::signal_frameButton_homing, this->m_animEditor, &P_ALE_Editor::selectStart);					//播放器 切至首帧 信号连接
	connect(this, &P_AnimationListPlayer::signal_frameButton_last, this->m_animEditor, &P_ALE_Editor::selectLast);						//播放器 上一帧 信号连接
	connect(this, &P_AnimationListPlayer::signal_frameButton_next, this->m_animEditor, &P_ALE_Editor::selectNext);						//播放器 下一帧 信号连接
	connect(this, &P_AnimationListPlayer::signal_frameButton_started, this, &P_AnimationListPlayer::animEditor_started);				//播放器 开始播放 信号连接
}
/*-------------------------------------------------
		编辑器 - 获取
*/
P_ALE_Editor* P_AnimationListPlayer::getAnimationListEditor(){
	return this->m_animEditor;
}
/*-------------------------------------------------
		编辑器 - 开始播放
*/
void P_AnimationListPlayer::animEditor_started(){
	if (this->m_animEditor == nullptr){ return; }
	this->setPlayFrame(this->m_animEditor->getCurrentData_IntervalList());		//在开始播放后，立即设置时间帧（因为时间帧会变）
}
