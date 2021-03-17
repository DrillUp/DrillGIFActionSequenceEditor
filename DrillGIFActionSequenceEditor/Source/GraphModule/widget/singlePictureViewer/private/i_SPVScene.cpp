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
						->��ȡ��߶ȡ���Ѫ��
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
	this->m_backgroundColor = QColor(255, 255, 255);
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
		���� - ˢ�±���
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
		���� - ��ȡ��������ͼ
*/
QPixmap I_SPVScene::getBitmapMaskBackground(){
	int ww = this->m_canvasWidth;
	int hh = this->m_canvasHeight;
	int pw = this->m_pixelWidth;
	int ph = this->m_pixelHeight;

	QPixmap img = QPixmap(ww, hh);				//����
	QPainter painter(&img);						//����
	img.fill(this->m_backgroundColor);			//����ɫ

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
	//��ע�⣬painter.fillRect��������Ч�ģ���

	return img;
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
	QGraphicsPixmapItem* item = new QGraphicsPixmapItem();
	double xx = (this->m_canvasWidth - this->m_bitmap.width())*0.5;
	double yy = (this->m_canvasHeight - this->m_bitmap.height())*0.5;
	item->setPixmap(this->m_bitmap);
	item->setPos(xx, yy);
	this->addItem(item);

}

/*-------------------------------------------------
		ͼƬ - ���߶�
*/
int I_SPVScene::getMaxHeight(){
	if (this->m_bitmap.height() != 0){ return 600; }	//��Ĭ�ϣ�
	return this->m_bitmap.height();
}
/*-------------------------------------------------
		ͼƬ - �����
*/
int I_SPVScene::getMaxWidth(){
	if (this->m_bitmap.width() != 0){ return 800; }		//��Ĭ�ϣ�
	return this->m_bitmap.width();
}

