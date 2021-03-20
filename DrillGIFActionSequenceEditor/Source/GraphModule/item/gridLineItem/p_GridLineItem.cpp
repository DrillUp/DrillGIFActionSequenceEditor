#include "stdafx.h"
#include "P_GridLineItem.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		网格线.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	图形模块
		功能：		能够根据行列设置，生成线对象组。
					
		注意：		只是一个生成器（也可以当容器用），生成后，可以手动获取，也可以直接执行其中的快捷函数。

		使用方法：
				> 建立网格线
					this->m_P_GridLineItem = new P_GridLineItem(this);
					this->m_P_GridLineItem->rebuildGrid(this->m_canvasWidth, this->m_canvasHeight, column, row, this->m_gridLineColor);
				> 添加到场景
					this->m_P_GridLineItem->addItemsToScene();

-----==========================================================-----
*/

P_GridLineItem::P_GridLineItem(QGraphicsScene* scene){
	this->m_scene = scene;
}
P_GridLineItem::~P_GridLineItem(){
}

/*-------------------------------------------------
		网格线 - 建立网格
*/
void P_GridLineItem::rebuildGrid(int width, int height, int column, int row, QColor color){
	double ww = width / (double)column;
	double hh = height / (double)row;

	this->clearAllItem();

	// > 竖线
	for (int i = 0; i <= column; i++){
		QGraphicsLineItem* line = new QGraphicsLineItem();
		line->setLine(QLineF(ww*i, 0, ww*i, height));
		line->setPen(color);
		this->m_itemTank.append(line);
	}
	// > 横线
	for (int i = 0; i <= row; i++){
		QGraphicsLineItem* line = new QGraphicsLineItem();
		line->setLine(QLineF(0, hh*i, width, hh*i));
		line->setPen(color);
		this->m_itemTank.append(line);
	}

}

/*-------------------------------------------------
		网格线 - 获取对象组
*/
QList<QGraphicsLineItem*> P_GridLineItem::getItems(){
	return this->m_itemTank;
}

/*-------------------------------------------------
		网格线 - 清空对象
*/
void P_GridLineItem::addItemsToScene(){
	for (int i = 0; i < this->m_itemTank.count(); i++){
		this->m_scene->addItem(this->m_itemTank.at(i));
	}
}
/*-------------------------------------------------
		网格线 - 清空对象
*/
void P_GridLineItem::clearAllItem(){

	// > 从场景中移除
	for (int i = 0; i < this->m_itemTank.count(); i++){
		this->m_scene->removeItem(this->m_itemTank.at(i));
	}

	// > 清空容器
	this->m_itemTank.clear();
}