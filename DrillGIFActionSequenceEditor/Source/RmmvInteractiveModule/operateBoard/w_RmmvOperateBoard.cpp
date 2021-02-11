#include "stdafx.h"
#include "W_RmmvOperateBoard.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		操作面板（窗口）.cpp
		所属模块：	交互模块
		功能：		操作面板的窗口。
					
		使用方法：
				>打开窗口
					W_RmmvOperateBoard d;
					d.exec();

-----==========================================================-----
*/
W_RmmvOperateBoard::W_RmmvOperateBoard(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	
	//-----------------------------------
	//----视图初始化
	this->m_P_RmmvOperateBoard = new P_RmmvOperateBoard(parent);
	QHBoxLayout* l = new QHBoxLayout(ui.widget);
	ui.widget->setLayout(l);
	l->addWidget(this->m_P_RmmvOperateBoard);

	//-----------------------------------
	//----事件绑定


	//-----------------------------------
	//----初始化ui
	TTool::_chinese_(ui.buttonBox);

}

W_RmmvOperateBoard::~W_RmmvOperateBoard(){
}

