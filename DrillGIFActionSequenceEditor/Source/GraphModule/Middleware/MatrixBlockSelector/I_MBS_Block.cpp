#include "stdafx.h"
#include "I_MBS_Block.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		阵列方块.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		鼠标点击后，呈现不同显示状态的方块。
					【这个方块的x、y、w、h被完全锁定】
-----==========================================================-----
*/
I_MBS_Block::I_MBS_Block() : QGraphicsRectItem(){
}
I_MBS_Block::~I_MBS_Block(){
}
/*-------------------------------------------------
		初始化
*/
void I_MBS_Block::_init(){

	// > 私有属性初始化
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
		状态位 - 设置方块颜色容器
*/
void I_MBS_Block::setColorTank(QList<QColor> color_tank){
	this->m_blockColorTank = color_tank;
	this->setState(this->m_state);
}
/*-------------------------------------------------
		状态位 - 设置当前状态
*/
void I_MBS_Block::setState(int state){
	if (state < 0){ state = 0; }
	if (state > this->m_blockColorTank.count() - 1){ state = this->m_blockColorTank.count() - 1; }

	this->m_state = state;
	this->setPen(QPen(QColor(255, 255, 255, 0)));
	this->setBrush(QBrush(this->m_blockColorTank.at(state), Qt::BrushStyle::SolidPattern));
}
/*-------------------------------------------------
		状态位 - 获取当前状态
*/
int I_MBS_Block::getState(){
	return this->m_state;
}


/*-------------------------------------------------
		矩阵 - 设置方块大小
*/
void I_MBS_Block::setBlockSize(int width, int height){
	this->m_width = width;
	this->m_height = height;
	this->refreshRect();
}
/*-------------------------------------------------
		矩阵 - 设置位置（单位图块）
*/
void I_MBS_Block::setMatrixPosition(int x, int y){
	this->m_matrixX = x;
	this->m_matrixY = y;
	this->refreshRect();
}
/*-------------------------------------------------
		矩阵 - 获取矩阵位置X
*/
int I_MBS_Block::getMatrixX(){
	return this->m_matrixX;
}
/*-------------------------------------------------
		矩阵 - 获取矩阵位置Y
*/
int I_MBS_Block::getMatrixY(){
	return this->m_matrixY;
}
/*-------------------------------------------------
		矩阵 - 刷新方块绘制（私有）
*/
void I_MBS_Block::refreshRect(){
	int ww = this->m_width;
	int hh = this->m_height;
	int xx = ww * this->m_matrixX;
	int yy = hh * this->m_matrixY;
	this->setRect(xx, yy, ww, hh);
}

/*-------------------------------------------------
		运算符重载
*/
const bool I_MBS_Block::operator== (const I_MBS_Block& a)const {
	return this->m_matrixX == a.m_matrixX && this->m_matrixY == a.m_matrixY;
}