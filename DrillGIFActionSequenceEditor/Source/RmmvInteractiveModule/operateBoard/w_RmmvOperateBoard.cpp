#include "stdafx.h"
#include "W_RmmvOperateBoard.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		������壨���ڣ�.cpp
		����ģ�飺	����ģ��
		���ܣ�		�������Ĵ��ڡ�
					
		ʹ�÷�����
				>�򿪴���
					W_RmmvOperateBoard d;
					d.exec();

-----==========================================================-----
*/
W_RmmvOperateBoard::W_RmmvOperateBoard(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	
	//-----------------------------------
	//----��ͼ��ʼ��
	this->m_P_RmmvOperateBoard = new P_RmmvOperateBoard(parent);
	QHBoxLayout* l = new QHBoxLayout(ui.widget);
	ui.widget->setLayout(l);
	l->addWidget(this->m_P_RmmvOperateBoard);

	//-----------------------------------
	//----�¼���


	//-----------------------------------
	//----��ʼ��ui
	TTool::_chinese_(ui.buttonBox);

}

W_RmmvOperateBoard::~W_RmmvOperateBoard(){
}

