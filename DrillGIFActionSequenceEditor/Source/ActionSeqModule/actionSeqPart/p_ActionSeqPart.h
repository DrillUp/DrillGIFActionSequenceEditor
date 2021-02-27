#ifndef P_ActionSeqPart_H
#define P_ActionSeqPart_H

#include <QtWidgets>
#include "ui_P_ActionSeqPart.h"

#include "Source/ActionSeqModule/actionPart/p_ActionPart.h"
#include "Source/ActionSeqModule/statePart/p_StatePart.h"
#include "Source/ActionSeqModule/playingPart/p_PlayingPart.h"

#include "Source/Utils/widgetForm/foldableTabWindow/p_FoldableTabRelater.h"
#include "Source/Utils/widgetFormSenior/flexibleClassificationTree/p_FlexibleClassificationTree.h"

/*
-----==========================================================-----
		类：		动作序列块.h
		作者：		drill_up
		所属模块：	动作序列模块
		功能：		该部分提供动作序列放映，功能，并嵌套状态元、动作元的块。
					（详细见cpp）
-----==========================================================-----
*/
class P_ActionSeqPart : public QWidget
{
	Q_OBJECT

	public:
		P_ActionSeqPart(QWidget *parent = 0);
		~P_ActionSeqPart();
		
	//-----------------------------------
	//----动作序列 全数据
	protected:
		P_FlexibleClassificationTree* m_p_tree;		//动作序列显示树

		int m_realLen_actionSeq;					//长度 - 动作序列
		int m_realLen_action;						//长度 - 动作元
		int m_realLen_state;						//长度 - 状态元

	public slots:
										//全数据 - 树选择变化
		void currentActionSeqChanged(QTreeWidgetItem* item, int id, QString name);
		
	//-----------------------------------
	//----本地数据的索引
	protected:
		int m_cur_actionSeqIndex;				//当前动作序列索引
		QJsonObject m_cur_actionSeq;			//当前动作序列
		QList<QJsonObject> m_cur_actionTank;	//当前动作元容器
		QList<QJsonObject> m_cur_stateTank;		//当前状态元容器
	public:
									//数据 - 保存本地数据
		void local_saveCurIndexData();
									//数据 - 读取本地数据
		void local_loadIndexData(int index);

	//-----------------------------------
	//----大控件
	protected:
		P_FoldableTabRelater* m_p_FoldableTabRelater;	//可折叠选项卡
		P_ActionPart* m_actionPart;						//动作元块
		P_StatePart* m_statePart;						//状态元块
		P_PlayingPart* m_playingPart;					//放映区

	//-----------------------------------
	//----窗口
	public:
		bool m_slotBlock_source;
		QJsonObject local_actionSeq;				//动作序列数据
		QList<QJsonObject> m_actionSeq_list;		//动作序列列表（临时）
	public:
										//窗口 - 设置数据
		void setData(QJsonObject actionSeq);
										//窗口 - 取出数据
		QJsonObject getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	private:
		Ui::P_ActionSeqPart ui;

};

#endif // P_ActionSeqPart_H
