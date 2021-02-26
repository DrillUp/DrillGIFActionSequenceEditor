#include "stdafx.h"
#include "i_APVScene.h"

#include <QPixmap>

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴�� ����.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�����������е���Ӻ������ӡ�

		�ӹ��ܣ�	->������ʼ��
						->��ȡ��߶ȡ���Ѫ��
						->��ɫ
						->����
					->����֡
						->����ͼƬ��Դ
						->�л�֡
					->���ţ���ͼ���ܣ�
					->������ƣ���ͼ���ܣ�

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
		��ʼ��
*/
void I_APVScene::init(){

	// > ���ԣ�Ĭ�ϣ�
	this->m_canvasWidth = 400;
	this->m_canvasHeight = 300;
	this->m_canvasThickness = 40;
	this->m_pixelWidth = 24;
	this->m_pixelHeight = 24;
	this->m_backgroundColor = QColor(255, 255, 255);
	this->m_maskBackground = nullptr;

	// > ����
	this->m_animList = QList<QGraphicsPixmapItem*>();
	this->m_curFrame = -1;

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
		���� - ��ȡ��������ͼ
*/
QPixmap I_APVScene::getBitmapMaskBackground(){
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
void I_APVScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mousePressEvent(mouseEvent);
	//���޲���������
}
/*-------------------------------------------------
		�����¼� - ���̧��
*/
void I_APVScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	//���޲���������
}
/*-------------------------------------------------
		�����¼� - ����ƶ�
*/
void I_APVScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	//���޲���������
}
/*-------------------------------------------------
		�����¼� - ���˫��
*/
void I_APVScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	//���޲���������
}



/*-------------------------------------------------
		��Դ - ������Դ
*/
void I_APVScene::setSource(QList<QPixmap> bitmap_list){
	this->m_bitmapList = bitmap_list;
	this->rebuildScene();
}
/*-------------------------------------------------
		��Դ - �ؽ�����
*/
void I_APVScene::rebuildScene(){
	
	// > ȥ����ͼƬ
	for (int i = 0; i < this->m_animList.count(); i++){
		this->removeItem(this->m_animList.at(i));
	}
	this->m_animList.clear();

	// > �߿�����
	this->m_canvasWidth = this->getMaxWidth();
	this->m_canvasHeight = this->getMaxHeight();
	this->setSceneRect(-1 * this->m_canvasThickness, -1 * this->m_canvasThickness, this->m_canvasWidth + this->m_canvasThickness * 2, this->m_canvasHeight + this->m_canvasThickness * 2);
	this->refreshBackground();

	// > ����ͼƬ
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
	if (this->m_animList.count() > 0 && this->m_curFrame == -1){
		this->m_curFrame = 0;
	}

	// > ˢ��֡
	this->refreshFrame();
}

/*-------------------------------------------------
		���� - �л�֡
*/
void I_APVScene::setAnimFrame(int index){
	this->m_curFrame = index;
	this->refreshFrame();
}
/*-------------------------------------------------
		���� - ˢ��֡��ʾ
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
		���� - ��Դ�߶�
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
		���� - ��Դ���
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

