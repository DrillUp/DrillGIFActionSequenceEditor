#ifndef DRILLGIFACTIONSEQUENCEEDITOR_H
#define DRILLGIFACTIONSEQUENCEEDITOR_H

#include <QtWidgets/QDialog>
#include "ui_drillgifactionsequenceeditor.h"

/*
-----==========================================================-----
		类：		主窗体.h
		作者：		drill_up
		所属模块：	主窗体模块
		功能：		程序进入后的主控制窗口。
					（详细见cpp）
-----==========================================================-----
*/
class P_ActionSeqPart;
class W_RmmvOperateBoard;
class DrillGIFActionSequenceEditor : public QDialog
{
	Q_OBJECT

	public:
		DrillGIFActionSequenceEditor(QWidget *parent = 0);
		~DrillGIFActionSequenceEditor();
		static DrillGIFActionSequenceEditor* cur_instance;		//单例
		static DrillGIFActionSequenceEditor* getInstance();		//单例，获取自己
		void _init();											//初始化
		
	//-----------------------------------
	//----控件（rmmv交互数据）
	public:
		W_RmmvOperateBoard* m_w_RmmvOperateBoard;
	public:
								//控件 - 打开rmmv交互窗口
		void openWindowRmmvInteractive();
	public slots:
								//控件 - rmmv交互数据已读取
		void rmmvInteractiveDataLoaded();

	//-----------------------------------
	//----控件（动作序列交互数据）
	public:
		P_ActionSeqPart* m_p_ActionSeqPart;
	public slots:
								//控件 - 动作序列已读取
		void actionDataLoaded();
		
	//-----------------------------------
	//----窗口
	private:
		Ui::DrillGIFActionSequenceEditorClass ui;
};

#endif // DRILLGIFACTIONSEQUENCEEDITOR_H
