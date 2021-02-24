#include "stdafx.h"
#include "i_APVScene.h"

#include <QPixmap>

/*
-----==========================================================-----
		类：		图片查看块 场景.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		可以用鼠标进行点添加和线连接。

		子功能：	->场景初始化
						->宽度、高度、出血线
						->颜色
						->背景
					->动画帧
						->设置图片资源
						->切换帧
					->缩放（视图功能）
					->鼠标拖移（视图功能）

-----==========================================================-----
*/
I_APVScene::I_APVScene(QWidget *parent)
	: QGraphicsScene(parent)
{
	this->init();
}
I_APVScene::~I_APVScene(){
}

/*-------------------------------------------------
		初始化
*/
void I_APVScene::init(){

	// > 属性（默认）
	this->m_canvasWidth = 400;
	this->m_canvasHeight = 300;
	this->m_canvasThickness = 40;
	this->m_pixelWidth = 24;
	this->m_pixelHeight = 24;
	this->m_backgroundColor = QColor(255, 255, 255);
	this->m_maskBackground = nullptr;

	// > 部件
	this->m_animList = QList<QGraphicsPixmapItem*>();
	this->m_curFrame = -1;

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
void I_APVScene::refreshBackground(){
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
QPixmap I_APVScene::getBitmapMaskBackground(){
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
void I_APVScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mousePressEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标抬起
*/
void I_APVScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标移动
*/
void I_APVScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标双击
*/
void I_APVScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	//（无操作函数）
}



/*-------------------------------------------------
		资源 - 设置资源
*/
void I_APVScene::setSource(QList<QPixmap> bitmap_list){
	this->m_bitmapList = bitmap_list;
	this->rebuildScene();
}
/*-------------------------------------------------
		资源 - 重建场景
*/
void I_APVScene::rebuildScene(){
	
	// > 去除旧图片
	for (int i = 0; i < this->m_animList.count(); i++){
		this->removeItem(this->m_animList.at(i));
	}
	this->m_animList.clear();

	// > 高宽适配
	this->m_canvasWidth = this->getMaxWidth();
	this->m_canvasHeight = this->getMaxHeight();
	this->setSceneRect(-1 * this->m_canvasThickness, -1 * this->m_canvasThickness, this->m_canvasWidth + this->m_canvasThickness * 2, this->m_canvasHeight + this->m_canvasThickness * 2);
	this->refreshBackground();

	// > 放置图片
	for (int i = 0; i < this->m_bitmapList.count(); i++){
		QPixmap bitmap = this->m_bitmapList.at(i);
		QGraphicsPixmapItem* item = new QGraphicsPixmapItem();
		double xx = (this->m_canvasWidth - bitmap.width())*0.5;
		double yy = (this->m_canvasHeight - bitmap.height())*0.5;
		item->setPixmap(bitmap);
		item->setPos(xx,yy);
		this->addItem(item);
		this->m_animList.append(item);
	}

	// > 刷新帧
	this->refreshFrame();
}

/*-------------------------------------------------
		部件 - 切换帧
*/
void I_APVScene::setAnimFrame(int index){
	this->m_curFrame = index;
	this->refreshFrame();
}
/*-------------------------------------------------
		部件 - 刷新帧显示
*/
void I_APVScene::refreshFrame(){
	if (this->m_curFrame < 0){ this->m_curFrame = -1; }
	if (this->m_curFrame >= this->m_animList.count()){ this->m_curFrame = -1; }

	for (int i = 0; i < this->m_animList.count(); i++){
		if (i == this->m_curFrame){
			this->m_animList.at(i)->setVisible(true);
		}else{
			this->m_animList.at(i)->setVisible(false);
		}
	}
}
/*-------------------------------------------------
		部件 - 资源高度
*/
int I_APVScene::getMaxHeight(){
	int result = 0;
	for (int i = 0; i < this->m_bitmapList.count(); i++){
		int hh = this->m_bitmapList.at(i).height();
		if (result < hh){
			result = hh;
		}
	}
	return result;
}
/*-------------------------------------------------
		部件 - 资源宽度
*/
int I_APVScene::getMaxWidth(){
	int result = 0;
	for (int i = 0; i < this->m_bitmapList.count(); i++){
		int ww = this->m_bitmapList.at(i).width();
		if (result < ww){
			result = ww;
		}
	}
	return result;
}

