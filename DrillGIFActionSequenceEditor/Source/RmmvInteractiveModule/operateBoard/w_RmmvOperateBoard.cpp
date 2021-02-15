#include "stdafx.h"
#include "W_RmmvOperateBoard.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		������壨���ڣ�.cpp
		�汾��		v1.01
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

/*-------------------------------------------------
		���� - ��������
*/
void W_RmmvOperateBoard::setData(C_RmmvProjectData actionData) {
	this->m_P_RmmvOperateBoard->setData(actionData);
}
/*-------------------------------------------------
		���� - ȡ������
*/
C_RmmvProjectData W_RmmvOperateBoard::getData(){
	return this->m_P_RmmvOperateBoard->getData();
}
