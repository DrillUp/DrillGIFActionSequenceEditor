#ifndef P_GridLineItem_H
#define P_GridLineItem_H
#include "stdafx.h"

#include <QGraphicsLineItem>

/*
-----==========================================================-----
		类：		网格线.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	图形模块
		功能：		能够根据行列设置，生成线对象组。
					（详细见.cpp）
-----==========================================================-----
*/
class P_GridLineItem : public QObject
{
	Q_OBJECT

	public:
		P_GridLineItem(QGraphicsScene* scene);		//构造函数
		~P_GridLineItem();							//析构函数
		
	//-----------------------------------
	//----网格线
	private:
		QGraphicsScene* m_scene;				//父控件
		QList<QGraphicsLineItem*> m_itemTank;	//网格列表
	public:
										//网格线 - 建立网格
		void rebuildGrid(int width, int height, int column, int row, QColor color);
										//网格线 - 获取对象组
		QList<QGraphicsLineItem*> getItems();
										//网格线 - 添加到场景
		void addItemsToScene();
										//网格线 - 清空对象
		void clearAllItem();

};

#endif // P_GridLineItem_H