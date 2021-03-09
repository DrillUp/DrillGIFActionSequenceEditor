#ifndef P_PlayingPart_H
#define P_PlayingPart_H

#include <QtWidgets>
#include "ui_P_PlayingPart.h"

#include "Source/ActionSeqModule/actionPart/p_ActionPart.h"
#include "Source/ActionSeqModule/statePart/p_StatePart.h"
#include "Source/Utils/widgetForm/radioTable/p_RadioTable.h"

/*
-----==========================================================-----
		类：		放映区.h
		作者：		drill_up
		所属模块：	动作序列模块
		功能：		该部分提供动作序列的放映。
					（详细见cpp）
-----==========================================================-----
*/
class P_PlayingPart : public QWidget
{
	Q_OBJECT

	public:
		P_PlayingPart(P_StatePart* state_part, P_ActionPart* action_part, QWidget *parent = 0);
		~P_PlayingPart();
		
	//-----------------------------------
	//----控件
	public:
		P_ActionPart* m_actionPart;
		P_StatePart* m_statePart;
		P_RadioTable* m_table_action;
		P_RadioTable* m_table_state;
		P_AnimPictureViewer* m_p_AnimPictureViewer;
	public slots:
										//控件 - 编辑状态元集合
		void editDefaultStateGroup();
										//控件 - 获取名称
		QStringList getActionNameList();
		QStringList getStateNameList();
		QStringList getStateNameListWithoutEmpty();

	//-----------------------------------
	//----按钮
	protected:
		QString m_iconSrcPath;
	public slots:
										//按钮 - 播放
		void btn_play();
	private:
										//按钮 - 刷新播放图标
		void updateIcon();
		
	//-----------------------------------
	//----动画帧
	private:
		bool m_playing;					//正在播放
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

	private slots:
										//动画帧 - 帧刷新
		void updateFrame();
	signals:
										//动画帧 - 开始播放（信号）
		void playStarted();

	//-----------------------------------
	//----资源列表
	private:
		QStringList m_srcTank;
		QList<QPixmap> m_bitmapTank;

	//-----------------------------------
	//----窗口
	public:
		QStringList local_defaultStateList;
		QList<QJsonObject> local_stateDataList;
		QList<QJsonObject> local_actionDataList;
	public:
										//窗口 - 刷新基于的数据
		void refreshSource();
										//窗口 - 设置数据
		void setDefaultStateData(QStringList defaultStateList);
										//窗口 - 取出数据
		QStringList getDefaultStateData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	private:
		Ui::P_PlayingPart ui;

};

#endif // P_PlayingPart_H
