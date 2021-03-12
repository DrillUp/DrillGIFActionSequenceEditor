#ifndef P_ActionSeqPart_H
#define P_ActionSeqPart_H

#include <QtWidgets>
#include "ui_P_ActionSeqPart.h"

#include "Source/ActionSeqModule/actionPart/p_ActionPart.h"
#include "Source/ActionSeqModule/statePart/p_StatePart.h"
#include "Source/ActionSeqModule/playingPart/p_PlayingPart.h"
#include "Source/ActionSeqModule/actionSeqData/lengthData/c_ActionSeqLength.h"

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
		P_FoldableTabRelater* m_p_FoldableTabRelater;	//可折叠选项卡
		P_ActionPart* m_actionPart;						//动作元块
		P_StatePart* m_statePart;						//状态元块
		P_PlayingPart* m_playingPart;					//放映区
	public:
										//大控件 - 置灰
										//		【说明】：选择一个动作序列后，置灰才会被取消。
		void setPartGray();
										//大控件 - 停止播放
		void stopPlaying();

	//-----------------------------------
	//----窗口
	public:
		bool m_slotBlock_source;					//加载时阻塞
		QList<QJsonObject> m_actionSeq_list;		//动作序列列表（临时）
	public:
										//窗口 - 设置数据
		void setData(QJsonObject actionSeq, C_ActionSeqLength length);
										//窗口 - 取出数据
		QJsonObject getDataActionSeqData();
		C_ActionSeqLength getDataActionSeqLength();
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
		Ui::P_ActionSeqPart ui;

};

#endif // P_ActionSeqPart_H
