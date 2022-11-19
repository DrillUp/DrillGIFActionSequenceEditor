#ifndef P_MRe_ButtonPart_H
#define P_MRe_ButtonPart_H
#include "stdafx.h"

#include <QtWidgets>
#include "ui_P_MRe_ButtonPart.h"
#include "../P_MRe_Controller.h"

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
class P_MRe_ButtonPart : public QWidget
{
	Q_OBJECT

	public:
		P_MRe_ButtonPart(QWidget *parent = 0);
		~P_MRe_ButtonPart();

	//-----------------------------------
	//----控件
	public slots:
										//控件 - 变化
		void zoomValueChanged(double value);
		
	//-----------------------------------
	//----控制器
	protected:
		P_MRe_Controller* m_p_MouseResizeController;
	public slots:
										//控制器 - 初始化
		void initWidget(P_MRe_Controller* controller);
		
	//-----------------------------------
	//----块
	private:
		Ui::P_MRe_ButtonPart ui;

};

#endif // P_MRe_ButtonPart_H
