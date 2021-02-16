#ifndef P_PlayingPart_H
#define P_PlayingPart_H

#include <QtWidgets>
#include "ui_P_PlayingPart.h"

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
										//控件 - 开始播放
		void startPlay();
										//控件 - 结束播放
		void endPlay();

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
		Ui::P_PlayingPart ui;

};

#endif // P_PlayingPart_H
