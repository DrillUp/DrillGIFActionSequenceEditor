#ifndef P_MouseResizeButtonPart_H
#define P_MouseResizeButtonPart_H

#include <QtWidgets>
#include "ui_P_MouseResizeButtonPart.h"
#include "../p_MouseResizeController.h"

/*
-----==========================================================-----
		类：		鼠标缩放 按钮块.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	图形模块
		功能：		提供鼠标缩放的 按钮控件块。
					（详细见cpp）
-----==========================================================-----
*/
class P_MouseResizeButtonPart : public QWidget
{
	Q_OBJECT

	public:
		P_MouseResizeButtonPart(QWidget *parent = 0);
		~P_MouseResizeButtonPart();

	//-----------------------------------
	//----控件
	public slots:
										//控件 - 变化
		void zoomValueChanged(double value);
		
	//-----------------------------------
	//----控制器
	protected:
		P_MouseResizeController* m_p_MouseResizeController;
	public slots:
										//控制器 - 初始化
		void initWidget(P_MouseResizeController* controller);
		
	//-----------------------------------
	//----块
	private:
		Ui::P_MouseResizeButtonPart ui;

};

#endif // P_MouseResizeButtonPart_H
