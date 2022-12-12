#ifndef P_PlayingPart_H
#define P_PlayingPart_H

#include <QtWidgets>
#include "ui_P_PlayingPart.h"

#include "Source/ActionSeqModule/statePart/p_StatePart.h"
#include "Source/ActionSeqModule/stateNodePart/p_StateNodePart.h"
#include "Source/ActionSeqModule/actionPart/p_ActionPart.h"
#include "Source/ActionSeqModule/reprintedCore/Drill_COAS_MainController.h"
#include "Source/ActionSeqModule/reprintedCore/Drill_COAS_Init.h"
#include "Source/Utils/widgetForm/radioTable/p_RadioTable.h"

/*
-----==========================================================-----
		类：		放映区.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		该部分提供动画序列的放映。
					（详细见cpp）
-----==========================================================-----
*/
class P_PlayingPart : public QWidget
{
	Q_OBJECT

	public:
		P_PlayingPart(P_StatePart* state_part, P_StateNodePart* stateNode_part, P_ActionPart* action_part, QWidget *parent = 0);
		~P_PlayingPart();
		
	//-----------------------------------
	//----控件
	public:
		P_StatePart* m_statePart;
		P_StateNodePart* m_stateNodePart;
		P_ActionPart* m_actionPart;
		P_AnimPictureViewer* m_p_AnimPictureViewer;
	public slots:
										//控件 - 根据文件名获取资源文件
		QFileInfo getSrcFileByName(QString file_name);
										//控件 - 缩放比例变化
		void zoomValueChanged(double value);
		

	//-----------------------------------
	//----操作台
	public:
		P_RadioTable* m_table_state;
		P_RadioTable* m_table_action;
		QStringList m_temp_simpleState;
	public slots:
										//操作台 - 编辑默认的状态元集合
		void editDefaultState();
										//操作台 - 编辑临时的简单状态元
		void editSimpleState();
										//操作台 - 获取全部关联文件名（去重）
		QStringList getRelatFileNameList();
										//操作台 - 获取名称
		QStringList getStateNameList();
		QStringList getStateNodeNameList();
		QStringList getActionNameList();
										//操作台 - 刷新表格
										//		【说明】：只刷新表格和显示名。
		void refreshTable();

	//-----------------------------------
	//----按钮
	public slots:
										//按钮 - 播放
		void btn_play();
										//按钮 - 播放默认的状态元集合
		void btn_playDefaultState();
										//按钮 - 播放简单状态元
		void btn_playSimpleStateNode();
										//按钮 - 播放状态节点
		void btn_playStateNode();
										//按钮 - 播放动作元
		void btn_playAction();
	private:
										//按钮 - 刷新播放图标
		void updateIcon();
		

	//-----------------------------------
	//----动画帧
	private:
		bool m_playing;					//正在播放
		QTimer* m_timer;				//计时器
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
		Drill_COAS_MainController m_COAS_mainController;		//主控制数据项


	//-----------------------------------
	//----窗口
	public:
		int m_actionSeq_curIndex;
		QStringList local_defaultSeq;
		QList<QJsonObject> local_stateDataList;
		QList<QJsonObject> local_stateNodeDataList;
		QList<QJsonObject> local_actionDataList;
	public:
										//窗口 - 刷新基于的数据
		void refreshCurActionSeq();
										//窗口 - 设置数据
		void setData_DefaultSeq(QStringList defaultSeq);
		void setData_CurIndex(int index);
										//窗口 - 取出数据
		QStringList getData_DefaultSeq();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	private:
		Ui::P_PlayingPart ui;

};

#endif // P_PlayingPart_H
