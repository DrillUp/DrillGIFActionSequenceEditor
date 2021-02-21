#include "stdafx.h"
#include "P_PiSBlock.h"

#include "Source/Utils/common/TTool.h"
#include "i_PiSPicture.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		һ��������ͼƬ�飬���Լ̳и��࣬�������QWidget��

		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
P_PiSBlock::P_PiSBlock(int width, int height, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	this->m_width = width;
	this->m_height = height;
	this->_init();
}
P_PiSBlock::~P_PiSBlock(){
}

/*-------------------------------------------------
		��ʼ��
*/
void P_PiSBlock::_init(){

	// > ͼƬ����
	this->m_i_PiSPicture = new I_PiSPicture( this->m_width - 10, this->m_height - 10, this );
	this->m_i_PiSPicture->move(5, 6);
	this->m_i_PiSPicture->stackUnder(ui.label);

	// > ��������
	ui.label->setText("");
	ui.label->move(2, 3);
}

/*-------------------------------------------------
		�ؼ� - �����ı�
*/
void P_PiSBlock::setCountText(QString name){
	ui.label->setText(name);
}
/*-------------------------------------------------
		�ؼ� - ��ȡ�ı�
*/
QString P_PiSBlock::getCountText(){
	return ui.label->text();
}
/*-------------------------------------------------
		�ؼ� - ����ͼƬ
*/
void P_PiSBlock::setPixmap(QPixmap pixmap){
	this->m_i_PiSPicture->setPixmap(pixmap);
}
