#include "stdafx.h"
#include "I_PPaScene.h"

#include <QPixmap>

/*
-----==========================================================-----
		类：		切割帧选择器 场景.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		使得可以在场景中点击勾选切割帧。

		子功能：	->场景初始化
						->宽度、高度、出血线
						->颜色
						->背景
					->单图
						->设置图片资源
					->缩放（视图功能）
					->鼠标拖移（视图功能）

-----==========================================================-----
*/
I_PPaScene::I_PPaScene(QWidget *parent)
	: QGraphicsScene(parent)
{
	this->init();
}
I_PPaScene::~I_PPaScene(){
}

/*-------------------------------------------------
		初始化
*/
void I_PPaScene::init(){

	// > 属性（默认）
	this->m_canvasWidth = this->getMaxWidth();
	this->m_canvasHeight = this->getMaxHeight();
	this->m_canvasThickness = 0;		//（不允许出现边缘线）
	this->m_pixelWidth = 24;
	this->m_pixelHeight = 24;

	// > 辅助对象
	this->m_gridLineColor = QColor(255, 0, 0);
	this->m_backgroundColor = QColor(255, 255, 255);
	this->m_P_GridLineItem = new P_GridLineItem(this);
	this->m_maskBackground = nullptr;

	// > 部件
	this->m_bitmapItem = nullptr;

	// > 场景初始化
	int ww = this->m_pixelWidth;
	int hh = this->m_pixelHeight;
	this->setSceneRect(-1 * this->m_canvasThickness, -1 * this->m_canvasThickness, this->m_canvasWidth + this->m_canvasThickness * 2, this->m_canvasHeight + this->m_canvasThickness * 2);
	this->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255), Qt::BrushStyle::SolidPattern));
	this->refreshBackground();

}
/*-------------------------------------------------
		辅助 - 设置网格线
*/
void I_PPaScene::setGridLine(int column, int row){

	// > 建立网格线
	this->m_P_GridLineItem->rebuildGrid(this->m_canvasWidth, this->m_canvasHeight, column, row, this->m_gridLineColor);
	
	// > 添加
	this->m_P_GridLineItem->addItemsToScene();
}
/*-------------------------------------------------
		辅助 - 清空网格线
*/
void I_PPaScene::clearGridLine(){
	this->m_P_GridLineItem->clearAllItem();
}
/*-------------------------------------------------
		辅助 - 刷新背景
*/
void I_PPaScene::refreshBackground(){
	if (this->m_maskBackground == nullptr){
		this->m_maskBackground = new I_MaskBackgroundItem();
		this->addItem(this->m_maskBackground);
	}
	this->m_maskBackground->setBackground_oneColor(this->m_canvasWidth, this->m_canvasHeight, this->m_pixelWidth, this->m_pixelHeight, this->m_backgroundColor);
	this->m_maskBackground->setPos(0, 0);
	this->m_maskBackground->setZValue(-10000);
}

/*-------------------------------------------------
		监听事件 - 鼠标按下
*/
void I_PPaScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mousePressEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标抬起
*/
void I_PPaScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标移动
*/
void I_PPaScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标双击
*/
void I_PPaScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	//（无操作函数）
}



/*-------------------------------------------------
		资源 - 设置资源
*/
void I_PPaScene::setSource(QPixmap bitmap){
	this->m_bitmap = bitmap;
	this->rebuildScene();
}
/*-------------------------------------------------
		资源 - 清除资源
*/
void I_PPaScene::clearSource(){
	this->m_bitmap = QPixmap();
	this->rebuildScene();
}
/*-------------------------------------------------
		资源 - 重建场景
*/
void I_PPaScene::rebuildScene(){
	
	// > 去除旧图片
	if (this->m_bitmapItem != nullptr){
		this->removeItem(this->m_bitmapItem);
	}

	// > 高宽适配
	this->m_canvasWidth = this->getMaxWidth();
	this->m_canvasHeight = this->getMaxHeight();
	this->setSceneRect(-1 * this->m_canvasThickness, -1 * this->m_canvasThickness, this->m_canvasWidth + this->m_canvasThickness * 2, this->m_canvasHeight + this->m_canvasThickness * 2);
	this->refreshBackground();

	// > 放置图片
	this->m_bitmapItem = new QGraphicsPixmapItem();
	double xx = (this->m_canvasWidth - this->m_bitmap.width())*0.5;
	double yy = (this->m_canvasHeight - this->m_bitmap.height())*0.5;
	this->m_bitmapItem->setPixmap(this->m_bitmap);
	this->m_bitmapItem->setPos(xx, yy);
	this->addItem(this->m_bitmapItem);

}

/*-------------------------------------------------
		图片 - 最大高度
*/
int I_PPaScene::getMaxHeight(){
	if (this->m_bitmap.height() == 0){ return 600; }	//（默认）
	return this->m_bitmap.height();
}
/*-------------------------------------------------
		图片 - 最大宽度
*/
int I_PPaScene::getMaxWidth(){
	if (this->m_bitmap.width() == 0){ return 800; }		//（默认）
	return this->m_bitmap.width();
}

