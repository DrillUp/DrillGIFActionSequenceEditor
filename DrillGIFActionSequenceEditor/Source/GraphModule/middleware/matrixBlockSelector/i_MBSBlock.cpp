#include "stdafx.h"
#include "I_MBSBlock.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		���з���.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		������󣬳��ֲ�ͬ��ʾ״̬�ķ��顣
					����������x��y��w��h����ȫ������
-----==========================================================-----
*/
I_MBSBlock::I_MBSBlock() : QGraphicsRectItem(){
}
I_MBSBlock::~I_MBSBlock(){
}
/*-------------------------------------------------
		��ʼ��
*/
void I_MBSBlock::_init(){

	// > ˽�����Գ�ʼ��
	this->m_matrixX = 0;
	this->m_matrixY = 0;
	this->m_width = 48;
	this->m_height = 48;
	this->m_blockColorTank = QList<QColor>() << QColor(255, 255, 255, 0) << QColor(0, 0, 255, 50);
	this->refreshRect();
	this->setState(0);

	this->setFlag(QGraphicsItem::ItemIsMovable, false);
	this->setFlag(QGraphicsItem::ItemIsSelectable, false);
	this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
}


/*-------------------------------------------------
		״̬λ - ���÷�����ɫ����
*/
void I_MBSBlock::setColorTank(QList<QColor> color_tank){
	this->m_blockColorTank = color_tank;
	this->setState(this->m_state);
}
/*-------------------------------------------------
		״̬λ - ���õ�ǰ״̬
*/
void I_MBSBlock::setState(int state){
	if (state < 0){ state = 0; }
	if (state > this->m_blockColorTank.count() - 1){ state = this->m_blockColorTank.count() - 1; }

	this->m_state = state;
	this->setPen(QPen(QColor(255, 255, 255, 0)));
	this->setBrush(QBrush(this->m_blockColorTank.at(state), Qt::BrushStyle::SolidPattern));
}
/*-------------------------------------------------
		״̬λ - ��ȡ��ǰ״̬
*/
int I_MBSBlock::getState(){
	return this->m_state;
}


/*-------------------------------------------------
		���� - ���÷����С
*/
void I_MBSBlock::setBlockSize(int width, int height){
	this->m_width = width;
	this->m_height = height;
	this->refreshRect();
}
/*-------------------------------------------------
		���� - ����λ�ã���λͼ�飩
*/
void I_MBSBlock::setMatrixPosition(int x, int y){
	this->m_matrixX = x;
	this->m_matrixY = y;
	this->refreshRect();
}
/*-------------------------------------------------
		���� - ��ȡ����λ��X
*/
int I_MBSBlock::getMatrixX(){
	return this->m_matrixX;
}
/*-------------------------------------------------
		���� - ��ȡ����λ��Y
*/
int I_MBSBlock::getMatrixY(){
	return this->m_matrixY;
}
/*-------------------------------------------------
		���� - ˢ�·�����ƣ�˽�У�
*/
void I_MBSBlock::refreshRect(){
	int ww = this->m_width;
	int hh = this->m_height;
	int xx = ww * this->m_matrixX;
	int yy = hh * this->m_matrixY;
	this->setRect(xx, yy, ww, hh);
}

/*-------------------------------------------------
		���������
*/
const bool I_MBSBlock::operator== (const I_MBSBlock& a)const {
	return this->m_matrixX == a.m_matrixX && this->m_matrixY == a.m_matrixY;
}