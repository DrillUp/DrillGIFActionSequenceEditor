#ifndef P_ActionPart_H
#define P_ActionPart_H

#include <QtWidgets>
#include "ui_P_ActionPart.h"

/*
-----==========================================================-----
		类：		动作元块.h
		作者：		drill_up
		所属模块：	动作元模块
		功能：		该部分提供动作元的编辑。
					（详细见cpp）
-----==========================================================-----
*/
class P_ActionPart : public QWidget
{
	Q_OBJECT

	public:
		P_ActionPart(QWidget *parent = 0);
		~P_ActionPart();
		
	//-----------------------------------
	//----控件
	public:
		

	//-----------------------------------
	//----窗口
	public:
		QJsonObject local_actionData;
	public:
										//窗口 - 设置数据
		void setData(QJsonObject actionData);
										//窗口 - 取出数据
		QJsonObject getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	private:
		Ui::P_ActionPart ui;

};

#endif // P_ActionPart_H
