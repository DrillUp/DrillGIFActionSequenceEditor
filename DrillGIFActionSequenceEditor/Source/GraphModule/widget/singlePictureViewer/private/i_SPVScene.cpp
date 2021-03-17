#include "stdafx.h"
#include "i_SPVScene.h"

#include <QPixmap>

/*
-----==========================================================-----
		类：		图片查看块-单图 场景.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		查看单图的画布。

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
I_SPVScene::I_SPVScene(QWidget *parent)
	: QGraphicsScene(parent)
{
	this->init();
}
I_SPVScene::~I_SPVScene(){
}

/*-------------------------------------------------
		初始化
*/
void I_SPVScene::init(){

	// > 属性（默认）
	this->m_canvasWidth = this->getMaxWidth();
	this->m_canvasHeight = this->getMaxHeight();
	this->m_canvasThickness = 40;
	this->m_pixelWidth = 24;
	this->m_pixelHeight = 24;
	this->m_backgroundColor = QColor(255, 255, 255);
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
		属性 - 刷新背景
*/
void I_SPVScene::refreshBackground(){
	if (this->m_maskBackground == nullptr){
		this->m_maskBackground = new QGraphicsPixmapItem();
		this->addItem(this->m_maskBackground);
	}
	this->m_maskBackground->setPixmap(this->getBitmapMaskBackground());
	this->m_maskBackground->setPos(0, 0);
	this->m_maskBackground->setZValue(-10000);
}
/*-------------------------------------------------
		属性 - 获取马赛克贴图
*/
QPixmap I_SPVScene::getBitmapMaskBackground(){
	int ww = this->m_canvasWidth;
	int hh = this->m_canvasHeight;
	int pw = this->m_pixelWidth;
	int ph = this->m_pixelHeight;

	QPixmap img = QPixmap(ww, hh);				//画板
	QPainter painter(&img);						//画家
	img.fill(this->m_backgroundColor);			//填充底色

	painter.setPen(QPen(QColor(0, 0, 0, 0)));
	painter.setBrush(QBrush(QColor(0, 0, 0, 30), Qt::BrushStyle::SolidPattern));
	int i_count = qCeil(ww / (double)pw);
	int j_count = qCeil(hh / (double)ph);
	for (int i = 0; i < i_count; i++){
		for (int j = 0; j < j_count; j++){
			painter.drawRect(i*pw + 0, j*ph + 0, pw * 0.5, ph * 0.5);
			painter.drawRect(i*pw + pw*0.5, j*ph + ph * 0.5, pw * 0.5, ph * 0.5);
		}
	}
	painter.end();
	//（注意，painter.fillRect函数是无效的！）

	return img;
}

/*-------------------------------------------------
		监听事件 - 鼠标按下
*/
void I_SPVScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mousePressEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标抬起
*/
void I_SPVScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标移动
*/
void I_SPVScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标双击
*/
void I_SPVScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	//（无操作函数）
}



/*-------------------------------------------------
		资源 - 设置资源
*/
void I_SPVScene::setSource(QPixmap bitmap){
	this->m_bitmap = bitmap;
	this->rebuildScene();
}
/*-------------------------------------------------
		资源 - 清除资源
*/
void I_SPVScene::clearSource(){
	this->m_bitmap = QPixmap();
	this->rebuildScene();
}
/*-------------------------------------------------
		资源 - 重建场景
*/
void I_SPVScene::rebuildScene(){
	
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
	QGraphicsPixmapItem* item = new QGraphicsPixmapItem();
	double xx = (this->m_canvasWidth - this->m_bitmap.width())*0.5;
	double yy = (this->m_canvasHeight - this->m_bitmap.height())*0.5;
	item->setPixmap(this->m_bitmap);
	item->setPos(xx, yy);
	this->addItem(item);

}

/*-------------------------------------------------
		图片 - 最大高度
*/
int I_SPVScene::getMaxHeight(){
	if (this->m_bitmap.height() != 0){ return 600; }	//（默认）
	return this->m_bitmap.height();
}
/*-------------------------------------------------
		图片 - 最大宽度
*/
int I_SPVScene::getMaxWidth(){
	if (this->m_bitmap.width() != 0){ return 800; }		//（默认）
	return this->m_bitmap.width();
}

