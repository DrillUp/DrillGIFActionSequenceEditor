﻿#include "stdafx.h"
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
	this->m_playing = false;					//正在播放
	this->m_backRun = false;					//是否倒放
	this->m_timer = new QTimer();				//计时器
	this->m_curFrame = 0;						//当前时间帧
	this->m_IndexFrame;			//动画帧的时间帧数
	this->m_IndexFrameCount = 0;				//总时间帧数
	this->m_timerInterval = 10;					//播放帧间隔
	connect(this->m_timer, SIGNAL(timeout()), this, SLOT(updateFrame()));	
	this->m_iconPlaying = false;

	this->m_animEditor = nullptr;
	this->m_slotBlock = false;

	//-----------------------------------
	//----初始化ui
	ui.combo->clear();
	ui.combo->addItems(QStringList() << "播放一次" << "播放二次" << "播放三次" << "播放四次" << "播放五次" << "永久循环");

	ui.toolButton_jumpToStart->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Start.png"));
	ui.toolButton_last->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Last.png"));
	ui.toolButton_play->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Run.png"));
	ui.toolButton_next->setIcon(QIcon(QRC_IconSrcPath + "/player/Play_Next.png"));
	ui.toolButton_new->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_New.png"));
	ui.toolButton_ExportGIF->setIcon(QIcon(QRC_IconSrcPath + "/menu/ExportGIF.png"));
	ui.toolButton_setting->setIcon(QIcon(QRC_IconSrcPath + "/menu/Common_Setting.png"));

	//-----------------------------------
	//----事件绑定
	connect(ui.toolButton_jumpToStart, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_homing);
	connect(ui.toolButton_last, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_last);
	connect(ui.toolButton_play, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_play);
	connect(ui.toolButton_next, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_next);
	connect(ui.toolButton_new, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_new);
	connect(ui.toolButton_ExportGIF, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_exportGIF);
	connect(ui.toolButton_setting, &QToolButton::clicked, this, &P_AnimationListPlayer::btn_setting);

	ui.toolButton_ExportGIF->setVisible(false);
}

P_AnimationListPlayer::~P_AnimationListPlayer(){
}


/*-------------------------------------------------
		动画帧 - 帧刷新
*/
void P_AnimationListPlayer::updateFrame(){
	if (this->m_IndexFrameCount == 0){ this->stopFrame(); return; }

	// > 切换选项
	int index = this->m_curFrame % this->m_IndexFrameCount;
	if (this->m_backRun == false){

		// > 正向播放
		for (int i = 0; i < this->m_IndexFrame.count(); i++){
			int i_count = this->m_IndexFrame.at(i);
			if (index < i_count){

				// > 信号变化
				emit frameIndexChanged(i);

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
				emit frameIndexChanged(i);

				break;
			}
			index -= i_count;
		}
	}


	// > 固定次数暂停
	if ( (this->m_backRun == false && index == 0) ||
		 (this->m_backRun == true && index == this->m_IndexFrame.count()-1) ){
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
		动画帧 - 开始
*/
void P_AnimationListPlayer::startFrame(){
	this->m_timer->start(this->m_timerInterval);
	this->m_curFrame = 0; 
	this->m_playing = true;
	this->updateIcon();
	emit frameButton_started();
}
/*-------------------------------------------------
		动画帧 - 暂停
*/
void P_AnimationListPlayer::stopFrame(){
	this->m_timer->stop();
	this->m_curFrame = 0;
	this->m_playing = false;
	this->updateIcon();
}
/*-------------------------------------------------
		动画帧 - 正在播放
*/
bool P_AnimationListPlayer::isPlaying(){
	return this->m_playing;
}
/*-------------------------------------------------
		动画帧 - 设置播放类型
*/
void P_AnimationListPlayer::setPlayType(QStringList play_type){
	ui.combo->clear();
	ui.combo->addItems(play_type);
}
/*-------------------------------------------------
		动画帧 - 设置时间帧列表（每个动画帧占用的时间帧数）
*/
void P_AnimationListPlayer::setPlayFrame(QList<int> indexFrame){
	this->m_IndexFrame = indexFrame;
	this->m_IndexFrameCount = 0;
	for (int i = 0; i < this->m_IndexFrame.count(); i++){
		this->m_IndexFrameCount += this->m_IndexFrame.at(i);
	}
}
/*-------------------------------------------------
		动画帧 - 设置倒放
*/
void P_AnimationListPlayer::setPlayBackRun(bool backRun){
	this->m_backRun = backRun;
}
/*-------------------------------------------------
		动画帧 - 设置播放间隔
*/
void P_AnimationListPlayer::setPlayTimerInterval(int timerInterval){
	this->m_timerInterval = timerInterval;
}



/*-------------------------------------------------
		按钮 - 刷新播放图标
*/
void P_AnimationListPlayer::updateIcon(){
	if (this->m_iconPlaying == this->m_playing){ return; }
	this->m_iconPlaying = this->m_playing;
	
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
	emit frameButton_homing();
}
/*-------------------------------------------------
		按钮 - 上一帧
*/
void P_AnimationListPlayer::btn_last(){
	this->btn_pause();
	emit frameButton_last();
}
/*-------------------------------------------------
		按钮 - 播放
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
		按钮 - 下一帧
*/
void P_AnimationListPlayer::btn_next(){
	this->btn_pause();
	emit frameButton_next();
}
/*-------------------------------------------------
		按钮 - 新建帧
*/
void P_AnimationListPlayer::btn_new(){
	this->btn_pause();
	emit frameButton_new();
}
/*-------------------------------------------------
		按钮 - 播放设置
*/
void P_AnimationListPlayer::btn_setting(){
	this->btn_pause();
	emit frameButton_setting();
}
/*-------------------------------------------------
		按钮 - 导出GIF（必须要编辑器导出）
*/
void P_AnimationListPlayer::btn_exportGIF(){
	if (this->m_animEditor == nullptr){ return; }
	
	this->m_animEditor->op_exportAll_GIFInAction();
}



/*-------------------------------------------------
		编辑器 - 设置
*/
void P_AnimationListPlayer::setAnimationListEditor(P_ALE_Editor* animEditor){
	this->m_animEditor = animEditor;
	ui.toolButton_ExportGIF->setVisible(true);

	if (this->m_animEditor->getUnit() == C_ALE_DataSet::SecondUnit){
		this->m_timerInterval = 10;
	}
	if (this->m_animEditor->getUnit() == C_ALE_DataSet::FrameUnit){
		this->m_timerInterval = 16;
	}

	connect(this, &P_AnimationListPlayer::frameIndexChanged, this->m_animEditor, &P_ALE_Editor::selectIndex_Single);					//播放器 索引 信号连接
	connect(this, &P_AnimationListPlayer::frameButton_homing, this->m_animEditor, &P_ALE_Editor::selectStart);					//播放器 切至首帧 信号连接
	connect(this, &P_AnimationListPlayer::frameButton_last, this->m_animEditor, &P_ALE_Editor::selectLast);					//播放器 上一帧 信号连接
	connect(this, &P_AnimationListPlayer::frameButton_next, this->m_animEditor, &P_ALE_Editor::selectNext);					//播放器 下一帧 信号连接
	connect(this, &P_AnimationListPlayer::frameButton_new, this->m_animEditor, &P_ALE_Editor::op_appendInAction);				//播放器 新建 信号连接
	connect(this, &P_AnimationListPlayer::frameButton_setting, this->m_animEditor, &P_ALE_Editor::openWindow_setConfigParam);	//播放器 ui设置 信号连接

	connect(this, &P_AnimationListPlayer::frameButton_started, this, &P_AnimationListPlayer::animEditor_started);				//播放器 开始播放 信号连接
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
	this->setPlayFrame(this->m_animEditor->getSource().getData_IntervalTank());		//在开始播放后，立即设置时间帧（因为时间帧会变）
}
