#include "stdafx.h"
#include "I_MaskBackgroundItem.h"


/*
-----==========================================================-----
		�ࣺ		�����˱���.cpp
		�汾��		v1.04
		���ߣ�		drill_up
		����ģ�飺	ͼ��ģ��
		���ܣ�		�ṩһ��ֱ�����ɱ�����item����

		ʹ�÷�����	
				> ��ʼ����
					this->m_MaskBackgroundItem = new I_MaskBackgroundItem(this);
					this->m_MaskBackgroundItem->setPixelGrid(m_pixelWidth, m_pixelHeight);

-----==========================================================-----
*/
I_MaskBackgroundItem::I_MaskBackgroundItem()
	: QGraphicsPixmapItem()
{
	this->init();
}
I_MaskBackgroundItem::~I_MaskBackgroundItem(){
}

/*-------------------------------------------------
		��ʼ��
*/
void I_MaskBackgroundItem::init(){

	// > ������ʼ��
	this->m_blockWidth = 24;
	this->m_blockHeight = 24;

	// > �������Գ�ʼ��
	this->setFlag(QGraphicsItem::ItemIsMovable, false);
	this->setFlag(QGraphicsItem::ItemIsSelectable, false);
	this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
}

/*-------------------------------------------------
		���� - ���ñ�������ɫ��
*/
void I_MaskBackgroundItem::setBackground_oneColor(int width, int height, int block_width, int block_height, QColor color, int opacity){
	this->m_blockWidth = block_width;
	this->m_blockHeight = block_height;
	QPixmap bitmap = this->getBackgroundPixmap_oneColor(width, height, block_width, block_height, color, opacity);
	this->setPixmap(bitmap);
}
/*-------------------------------------------------
		���� - ���ñ�����˫ɫ��
*/
void I_MaskBackgroundItem::setBackground_twoColor(int width, int height, int block_width, int block_height, QColor color1, QColor color2){
	this->m_blockWidth = block_width;
	this->m_blockHeight = block_height;
	QPixmap bitmap = this->getBackgroundPixmap_twoColor(width, height, block_width, block_height, color1, color2);
	this->setPixmap(bitmap);
}

/*-------------------------------------------------
		���� - ��ȡͼ�񣨵�ɫ��
*/
QPixmap I_MaskBackgroundItem::getBackgroundPixmap_oneColor(int ww, int hh, int pw, int ph, QColor color, int opacity){

	QPixmap bitmap = QPixmap(ww, hh);				//����
	QPainter painter(&bitmap);						//����
	bitmap.fill(color);								//����ɫ

	painter.setPen(QPen(QColor(0, 0, 0, 0)));
	painter.setBrush(QBrush(QColor(0, 0, 0, opacity), Qt::BrushStyle::SolidPattern));
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

	return bitmap;
}
/*-------------------------------------------------
		���� - ��ȡͼ��˫ɫ��
*/
QPixmap I_MaskBackgroundItem::getBackgroundPixmap_twoColor(int ww, int hh, int pw, int ph, QColor color1, QColor color2){

	QPixmap bitmap = QPixmap(ww, hh);
	bitmap.setMask(QBitmap(bitmap));				//��alpha��
	bitmap.fill(Qt::transparent);					//͸������
	QPainter painter(&bitmap);						//����

	int i_count = qCeil(ww / (double)pw);
	int j_count = qCeil(hh / (double)ph);
	painter.setPen(QPen(QColor(0, 0, 0, 0)));
	painter.setBrush(QBrush(color1, Qt::BrushStyle::SolidPattern));
	for (int i = 0; i < i_count; i++){
		for (int j = 0; j < j_count; j++){
			painter.drawRect(i*pw + 0, j*ph + 0, pw * 0.5, ph * 0.5);
			painter.drawRect(i*pw + pw*0.5, j*ph + ph * 0.5, pw * 0.5, ph * 0.5);
		}
	}
	painter.setBrush(QBrush(color2, Qt::BrushStyle::SolidPattern));
	for (int i = 0; i < i_count; i++){
		for (int j = 0; j < j_count; j++){
			painter.drawRect(i*pw + 0, j*ph + ph * 0.5, pw * 0.5, ph * 0.5);
			painter.drawRect(i*pw + pw*0.5, j*ph + 0, pw * 0.5, ph * 0.5);
		}
	}
	painter.end();

	return bitmap;
}
