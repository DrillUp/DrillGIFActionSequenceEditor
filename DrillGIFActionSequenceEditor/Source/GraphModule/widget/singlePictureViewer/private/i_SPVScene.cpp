#include "stdafx.h"
#include "i_SPVScene.h"

#include <QPixmap>

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴��-��ͼ ����.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�鿴��ͼ�Ļ�����

		�ӹ��ܣ�	->������ʼ��
						->���ȡ��߶ȡ���Ѫ��
						->��ɫ
						->����
					->��ͼ
						->����ͼƬ��Դ
					->���ţ���ͼ���ܣ�
					->������ƣ���ͼ���ܣ�

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
		��ʼ��
*/
void I_SPVScene::init(){

	// > ���ԣ�Ĭ�ϣ�
	this->m_canvasWidth = this->getMaxWidth();
	this->m_canvasHeight = this->getMaxHeight();
	this->m_canvasThickness = 40;
	this->m_pixelWidth = 24;
	this->m_pixelHeight = 24;

	// > ��������
	this->m_gridLineColor = QColor(255, 0, 0);
	this->m_backgroundColor = QColor(255, 255, 255);
	this->m_P_GridLineItem = new P_GridLineItem(this);
	this->m_maskBackground = nullptr;

	// > ����
	this->m_bitmapItem = nullptr;

	// > ������ʼ��
	int ww = this->m_pixelWidth;
	int hh = this->m_pixelHeight;
	this->setSceneRect(-1 * this->m_canvasThickness, -1 * this->m_canvasThickness, this->m_canvasWidth + this->m_canvasThickness * 2, this->m_canvasHeight + this->m_canvasThickness * 2);
	this->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255), Qt::BrushStyle::SolidPattern));
	this->refreshBackground();

}
/*-------------------------------------------------
		���� - ����������
*/
void I_SPVScene::setGridLine(int column, int row){

	// > ����������
	this->m_P_GridLineItem->rebuildGrid(this->m_canvasWidth, this->m_canvasHeight, column, row, this->m_gridLineColor);
	
	// > ����
	this->m_P_GridLineItem->addItemsToScene();
}
/*-------------------------------------------------
		���� - ���������
*/
void I_SPVScene::clearGridLine(){
	this->m_P_GridLineItem->clearAllItem();
}
/*-------------------------------------------------
		���� - ˢ�±���
*/
void I_SPVScene::refreshBackground(){
	if (this->m_maskBackground == nullptr){
		this->m_maskBackground = new I_MaskBackgroundItem();
		this->addItem(this->m_maskBackground);
	}
	this->m_maskBackground->setBackground_oneColor(this->m_canvasWidth, this->m_canvasHeight, this->m_pixelWidth, this->m_pixelHeight, this->m_backgroundColor);
	this->m_maskBackground->setPos(0, 0);
	this->m_maskBackground->setZValue(-10000);
}

/*-------------------------------------------------
		�����¼� - ��갴��
*/
void I_SPVScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mousePressEvent(mouseEvent);
	//���޲���������
}
/*-------------------------------------------------
		�����¼� - ���̧��
*/
void I_SPVScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	//���޲���������
}
/*-------------------------------------------------
		�����¼� - ����ƶ�
*/
void I_SPVScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	//���޲���������
}
/*-------------------------------------------------
		�����¼� - ���˫��
*/
void I_SPVScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	//���޲���������
}



/*-------------------------------------------------
		��Դ - ������Դ
*/
void I_SPVScene::setSource(QPixmap bitmap){
	this->m_bitmap = bitmap;
	this->rebuildScene();
}
/*-------------------------------------------------
		��Դ - �����Դ
*/
void I_SPVScene::clearSource(){
	this->m_bitmap = QPixmap();
	this->rebuildScene();
}
/*-------------------------------------------------
		��Դ - �ؽ�����
*/
void I_SPVScene::rebuildScene(){
	
	// > ȥ����ͼƬ
	if (this->m_bitmapItem != nullptr){
		this->removeItem(this->m_bitmapItem);
	}

	// > �߿�����
	this->m_canvasWidth = this->getMaxWidth();
	this->m_canvasHeight = this->getMaxHeight();
	this->setSceneRect(-1 * this->m_canvasThickness, -1 * this->m_canvasThickness, this->m_canvasWidth + this->m_canvasThickness * 2, this->m_canvasHeight + this->m_canvasThickness * 2);
	this->refreshBackground();

	// > ����ͼƬ
	this->m_bitmapItem = new QGraphicsPixmapItem();
	double xx = (this->m_canvasWidth - this->m_bitmap.width())*0.5;
	double yy = (this->m_canvasHeight - this->m_bitmap.height())*0.5;
	this->m_bitmapItem->setPixmap(this->m_bitmap);
	this->m_bitmapItem->setPos(xx, yy);
	this->addItem(this->m_bitmapItem);

}

/*-------------------------------------------------
		ͼƬ - ���߶�
*/
int I_SPVScene::getMaxHeight(){
	if (this->m_bitmap.height() == 0){ return 600; }	//��Ĭ�ϣ�
	return this->m_bitmap.height();
}
/*-------------------------------------------------
		ͼƬ - ������
*/
int I_SPVScene::getMaxWidth(){
	if (this->m_bitmap.width() == 0){ return 800; }		//��Ĭ�ϣ�
	return this->m_bitmap.width();
}
