#include "stdafx.h"
#include "P_ALEBlock.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		����֡��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		һ��������ͼƬ�飬���Լ̳и��࣬�������QWidget��

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_ALEBlock::P_ALEBlock(int width, int height, QWidget *parent)
	: P_PiSBlock(width, height, parent)
{
	this->_init2();			//������ĺ���_init�̳�û�ã�
}
P_ALEBlock::~P_ALEBlock(){
}

/*-------------------------------------------------
		��ʼ��
*/
void P_ALEBlock::_init2(){

	// > ��������
	QLabel* label = new QLabel(this);
	label->setText("");
	label->setGeometry(this->m_width - 51, this->m_height - 21, 46, 18);
	label->setStyleSheet("border: 1px solid #000000; background-color: #ffffff; color:#000000; font-size:13px; ");
	label->setAlignment(Qt::AlignCenter);
	label->stackUnder(ui.label);
	this->m_FrameLabel = label;

}

/*-------------------------------------------------
		�ؼ� - ����֡�ı�
*/
void P_ALEBlock::setFrameText(QString name){
	if ( this->m_FrameLabel == nullptr){ return; }
	this->m_FrameLabel->setText(name);
}
