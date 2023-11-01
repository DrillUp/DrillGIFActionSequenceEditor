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
	//----播放器
	protected:
		QTimer* m_timer = nullptr;		//计时器
		int m_timerInterval;			//计时器间隔
		int m_curFrame;					//当前时间帧
		QList<int> m_IndexFrame;		//动画帧的时间帧数
		int m_IndexFrameCount;			//总时间帧数
		bool m_isPlaying;				//是否正在播放
	public:
										//播放器 - 开始
		void startFrame();
										//播放器 - 暂停
		void stopFrame();
										//播放器 - 是否正在播放
		bool isPlaying();
	public slots:
										//播放器 - 设置播放类型
		void setPlayType(QStringList play_type);
										//播放器 - 设置时间帧列表（每个动画帧占用的时间帧数）
		void setPlayFrame(QList<int> indexFrame);
										//播放器 - 设置播放间隔
		void setPlayTimerInterval(int timerInterval);
	protected slots:
										//播放器 - 帧刷新（私有）
		void updateFrame();
		

	//-----------------------------------
	//----倒放
	protected:
		bool m_isBackRun;				//是否倒放
	public slots:
										//倒放 - 设置倒放
		void setBackRun(bool backRun);
	public:
	signals:
										//倒放 - 倒放切换（信号）
		void signal_backRunChanged(bool backRun);


	//-----------------------------------
	//----按钮
	protected:
		bool m_iconPlaying;
	public:
										//按钮 - 切至首帧
		void btn_homing();
										//按钮 - 上一帧
		void btn_last();
										//按钮 - 下一帧
		void btn_next();
										//按钮 - 播放/暂停
		void btn_play();
		void btn_pause();
										//按钮 - 正向播放/倒放
		void btn_switchRunDirection();
	protected:
										//按钮 - 刷新播放图标
		void updateIcon();
		
	public:
										//附加按钮 - 新建帧
		void btn_new();
										//附加按钮 - 导出GIF
		void btn_exportGIF();
										//附加按钮 - 播放设置
		void btn_setting();


	//-----------------------------------
	//----编辑块
	protected:
		P_ALE_Editor* m_animEditor = nullptr;
	public:
										//编辑块 - 设置
		void setAnimationListEditor(P_ALE_Editor* animEditor);
										//编辑块 - 获取
		P_ALE_Editor* getAnimationListEditor();
	protected slots:
										//编辑块 - 开始播放
		void animEditor_started();
		
	//-----------------------------------
	//----编辑块交互
	signals:
										//编辑块交互 - 图片切换（信号）
		void signal_frameIndexChanged(int index);
										//编辑块交互 - 切至首帧（信号）
		void signal_frameButton_homing();
										//编辑块交互 - 上一帧（信号）
		void signal_frameButton_last();
										//编辑块交互 - 下一帧（信号）
		void signal_frameButton_next();
										//编辑块交互 - 开始播放（信号）
		void signal_frameButton_started();


	//-----------------------------------
	//----块
	private:
		Ui::P_AnimationListPlayer ui;

};

#endif // W_ImportMeshDialog_H
