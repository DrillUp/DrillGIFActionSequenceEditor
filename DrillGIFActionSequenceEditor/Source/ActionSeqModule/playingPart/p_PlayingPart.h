#ifndef P_PlayingPart_H
#define P_PlayingPart_H

#include <QtWidgets>
#include "ui_P_PlayingPart.h"

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
		P_PlayingPart(QWidget *parent = 0);
		~P_PlayingPart();
		
	//-----------------------------------
	//----控件
	public:
		P_RadioTable* m_table_action;
		P_RadioTable* m_table_state;
	public:
										//控件 - 开始播放
		void openPlay();
										//控件 - 获取名称
		QStringList getStateNameList();
		QStringList getActionNameList();

	//-----------------------------------
	//----窗口
	public:
		QStringList local_defaultStateList;
		QList<QJsonObject> local_stateDataList;
		QList<QJsonObject> local_actionDataList;
	public:
										//窗口 - 设置数据
		void setSource(QList<QJsonObject> stateDataList, QList<QJsonObject> actionDataList);
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
