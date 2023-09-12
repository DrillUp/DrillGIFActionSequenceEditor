#ifndef P_AnimationListPlayer_H
#define P_AnimationListPlayer_H
#include "stdafx.h"

#include <QtWidgets>
#include "ui_P_AnimationListPlayer.h"

#include "Source/Utils/WidgetFormSenior/AnimationListEditor/P_ALE_Editor.h"

/*
-----==========================================================-----
		类：		动画帧播放器.h
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		该模块能够控制动画帧播放，相当于 动画帧编辑块 的一个扩展粘合剂。
					【播放器 与 编辑器 是独立的，播放器发出信号，编辑器接收并处理】
					【具体去看看编辑器接收信号处理方式，可以此使播放器连接其它的控件】
					（详细见cpp）
-----==========================================================-----
*/
class P_AnimationListPlayer : public QWidget
{
	Q_OBJECT

	public:
		P_AnimationListPlayer(QWidget *parent = 0);
		~P_AnimationListPlayer();
		
	//-----------------------------------
	//----动画帧
	private:
		bool m_playing;					//正在播放
		bool m_backRun;					//是否倒放
		QTimer* m_timer;				//计时器
		int m_curFrame;					//当前时间帧
		QList<int> m_IndexFrame;		//动画帧的时间帧数
		int m_IndexFrameCount;			//总时间帧数
		int m_timerInterval;			//播放间隔

	public:
										//动画帧 - 开始
		void startFrame();
										//动画帧 - 暂停
		void stopFrame();
										//动画帧 - 正在播放
		bool isPlaying();
	public slots:
										//动画帧 - 设置播放类型
		void setPlayType(QStringList play_type);
										//动画帧 - 设置时间帧列表（每个动画帧占用的时间帧数）
		void setPlayFrame(QList<int> indexFrame);
										//动画帧 - 设置倒放
		void setPlayBackRun(bool backRun);
										//动画帧 - 设置播放间隔
		void setPlayTimerInterval(int timerInterval);

	private slots:
										//动画帧 - 帧刷新
		void updateFrame();
		
	//-----------------------------------
	//----对外交互
	signals:
										//对外交互 - 图片切换（信号）
		void frameIndexChanged(int index);
										//对外交互 - 切至首帧（信号）
		void frameButton_homing();
										//对外交互 - 上一帧（信号）
		void frameButton_last();
										//对外交互 - 开始播放（信号）
		void frameButton_started();
										//对外交互 - 下一帧（信号）
		void frameButton_next();
										//对外交互 - 新建帧（信号）
		void frameButton_new();
										//对外交互 - 播放设置（信号）
		void frameButton_setting();
		
	//-----------------------------------
	//----按钮
	private:
		QString m_iconSrcPath;
		bool m_iconPlaying;
	public:
										//按钮 - 切至首帧
		void btn_homing();
										//按钮 - 上一帧
		void btn_last();
										//按钮 - 播放
		void btn_play();
		void btn_pause();
										//按钮 - 下一帧
		void btn_next();
										//按钮 - 新建帧
		void btn_new();
										//按钮 - 导出GIF
		void btn_exportGIF();
										//按钮 - 播放设置
		void btn_setting();
	private:
										//按钮 - 刷新播放图标
		void updateIcon();

	//-----------------------------------
	//----编辑器
	private:
		P_ALE_Editor* m_animEditor;
	public:
										//编辑器 - 设置
		void setAnimationListEditor(P_ALE_Editor* animEditor);
										//编辑器 - 获取
		P_ALE_Editor* getAnimationListEditor();
	public slots:
										//编辑器 - 开始播放
		void animEditor_started();


	//-----------------------------------
	//----窗口
	public:
		bool m_slotBlock;
	private:
		Ui::P_AnimationListPlayer ui;

};

#endif // W_ImportMeshDialog_H
