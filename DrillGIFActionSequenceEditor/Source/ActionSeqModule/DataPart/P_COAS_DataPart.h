#ifndef P_COAS_DataPart_H
#define P_COAS_DataPart_H

#include <QtWidgets>
#include "ui_P_COAS_DataPart.h"

#include "C_COAS_Data.h"
#include "Source/ActionSeqModule/ActionPart/P_COAS_ActionPart.h"
#include "Source/ActionSeqModule/StatePart/P_COAS_StatePart.h"
#include "Source/ActionSeqModule/StateNodePart/P_COAS_StateNodePart.h"
#include "Source/ActionSeqModule/PlayingPart/P_COAS_PlayingPart.h"
#include "Source/ActionSeqModule/Data/LengthData/C_COAS_Length.h"

#include "Source/Utils/WidgetForm/FoldableTabWindow/P_FoldableTabRelater.h"
#include "Source/Utils/WidgetFormSenior/FlexibleClassificationTree/P_FlexibleClassificationTree.h"

/*
-----==========================================================-----
		类：		动画序列块.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		描述动画序列的块，嵌套了状态元、状态节点、动作元、放映区的块。
					（详细见cpp）
-----==========================================================-----
*/
class P_COAS_DataPart : public QWidget
{
	Q_OBJECT

	public:
		P_COAS_DataPart(QWidget *parent = 0);
		~P_COAS_DataPart();
		
	//-----------------------------------
	//----动画序列 全数据
	protected:
		P_FlexibleClassificationTree* m_p_tree;		//动画序列显示树
	public slots:
										//全数据 - 树选择变化
		void currentActionSeqChanged(QTreeWidgetItem* item, int id, QString name);
		
	//-----------------------------------
	//----本地数据的索引
	protected:
		int m_cur_actionSeqIndex;				//当前动画序列索引
	public:
									//数据 - 获取当前动画序列
		C_COAS_DataPtr getCurrentData();
									//数据 - 动画序列
		QList<C_COAS_DataPtr> getActionSeqList();
	public:
									//数据 - 保存本地数据
		void local_saveCurIndexData();
									//数据 - 读取本地数据
		void local_loadIndexData(int index);
		
	//-----------------------------------
	//----快捷键
	protected:
		QJsonObject m_copyed_actionSeq;
	protected:
									//操作 - 替换
		void op_replace(int index, QJsonObject actiong_seq);
									//操作 - 清空
		void op_clear(int index);
	public:
									//快捷键 - 事件
		void keyPressEvent(QKeyEvent *event);
									//快捷键 - 复制
		void shortcut_copyData();
									//快捷键 - 粘贴
		void shortcut_pasteData();
									//快捷键 - 清空
		void shortcut_clearData();

	//-----------------------------------
	//----大控件
	protected:
		P_FoldableTabRelater* m_p_FoldableTabRelater;		//可折叠选项卡
		P_COAS_ActionPart* m_actionPart;					//动作元块
		P_COAS_StatePart* m_statePart;						//状态元块
		P_COAS_StateNodePart* m_stateNodePart;				//状态节点块
		P_COAS_PlayingPart* m_playingPart;					//放映区
	public:
										//大控件 - 置灰
										//		【说明】：选择一个动画序列后，置灰才会被取消。
		void setPartGray();
										//大控件 - 停止播放
		void stopPlaying();
										//大控件 - 刷新表格
		void refreshPlayingPartTable();
										//大控件 - 任意大控件被点击时
		void partFocusIn(QString part_name);

	//-----------------------------------
	//----窗口
	public:
		bool m_slotBlock_source;		//加载时阻塞
	public:
										//窗口 - 设置数据
		void setData(QList<C_COAS_DataPtr> actionSeq, C_COAS_Length length);
										//窗口 - 取出数据
		QList<C_COAS_DataPtr> getData_ActionSeqData();
		C_COAS_Length getData_ActionSeqLength();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
	public:
										//窗口 - 修改长度
		void modifyDataLengthInAction();
		
	public:
										//窗口 - 用户自定义UI读取
		void ui_loadConfig();
										//窗口 - 用户自定义UI存储
		void ui_saveConfig();
	private:
		Ui::P_COAS_DataPart ui;

};

#endif // P_COAS_DataPart_H
