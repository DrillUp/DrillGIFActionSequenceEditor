#ifndef P_ActionSeqPart_H
#define P_ActionSeqPart_H

#include <QtWidgets>
#include "ui_P_ActionSeqPart.h"

#include "Source/ActionSeqModule/actionPart/p_ActionPart.h"
#include "Source/ActionSeqModule/statePart/p_StatePart.h"

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
	//----控件
	protected:
		P_ActionPart* m_actionPart;
		P_StatePart* m_statePart;
	public:
		

	//-----------------------------------
	//----窗口
	public:
		QJsonObject local_actionSeq;
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
