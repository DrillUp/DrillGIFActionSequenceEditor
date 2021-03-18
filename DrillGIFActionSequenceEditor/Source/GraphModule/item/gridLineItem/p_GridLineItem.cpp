#include "stdafx.h"
#include "P_GridLineItem.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		������.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	ͼ��ģ��
		���ܣ�		�ܹ������������ã������߶����顣
					
		ע�⣺		1.ֻ��һ�������������ɺ󣬿����ֶ���ȡ��Ҳ����ֱ��ִ�����еĿ�ݺ�����

		ʹ�÷�����
				> ����������
					this->m_P_GridLineItem = new P_GridLineItem(this);
					this->m_P_GridLineItem->rebuildGrid(this->m_canvasWidth, this->m_canvasHeight, column, row, this->m_gridLineColor);
				> ��ӵ�����
					this->m_P_GridLineItem->addItemsToScene();

-----==========================================================-----
*/

P_GridLineItem::P_GridLineItem(QGraphicsScene* scene){
	this->m_scene = scene;
}
P_GridLineItem::~P_GridLineItem(){
}

/*-------------------------------------------------
		������ - ��������
*/
void P_GridLineItem::rebuildGrid(int width, int height, int column, int row, QColor color){
	double ww = width / (double)column;
	double hh = height / (double)row;

	this->clearAllItem();

	// > ����
	for (int i = 0; i <= column; i++){
		QGraphicsLineItem* line = new QGraphicsLineItem();
		line->setLine(QLineF(ww*i, 0, ww*i, height));
		line->setPen(color);
		this->m_itemTank.append(line);
	}
	// > ����
	for (int i = 0; i <= row; i++){
		QGraphicsLineItem* line = new QGraphicsLineItem();
		line->setLine(QLineF(0, hh*i, width, hh*i));
		line->setPen(color);
		this->m_itemTank.append(line);
	}

}

/*-------------------------------------------------
		������ - ��ȡ������
*/
QList<QGraphicsLineItem*> P_GridLineItem::getItems(){
	return this->m_itemTank;
}

/*-------------------------------------------------
		������ - ��ն���
*/
void P_GridLineItem::addItemsToScene(){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		this->m_scene->addItem(this->m_itemTank.at(i));
	}
}
/*-------------------------------------------------
		������ - ��ն���
*/
void P_GridLineItem::clearAllItem(){

	// > �ӳ������Ƴ�
	for (int i = 0; i < this->m_itemTank.count(); i++){
		this->m_scene->removeItem(this->m_itemTank.at(i));
	}

	// > �������
	this->m_itemTank.clear();
}