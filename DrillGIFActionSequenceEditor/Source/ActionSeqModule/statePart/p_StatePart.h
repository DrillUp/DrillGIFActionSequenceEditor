#ifndef P_StatePart_H
#define P_StatePart_H

#include <QtWidgets>
#include "ui_P_StatePart.h"

/*
-----==========================================================-----
		类：		状态元块.h
		作者：		drill_up
		所属模块：	状态元模块
		功能：		该部分提供状态元编辑功能。
					（详细见cpp）
-----==========================================================-----
*/
class P_StatePart : public QWidget
{
	Q_OBJECT

	public:
		P_StatePart(QWidget *parent = 0);
		~P_StatePart();
		
	//-----------------------------------
	//----控件
	public:
		

	//-----------------------------------
	//----窗口
	public:
		QJsonObject local_state;
	public:
										//窗口 - 设置数据
		void setData(QJsonObject state);
										//窗口 - 取出数据
		QJsonObject getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	private:
		Ui::P_StatePart ui;

};

#endif // P_StatePart_H
