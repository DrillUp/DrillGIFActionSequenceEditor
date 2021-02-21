#include "stdafx.h"
#include "I_PiSPicture.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ��.cpp
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		ʹ�ñ�ǩ���ж����ť����Щ��ť�൱�ڹ�ѡ��


		ʹ�÷�����
				>��ʼ��

-----==========================================================-----
*/
I_PiSPicture::I_PiSPicture(int width, int height, QWidget *parent)
	: QWidget(parent)
{
	//-----------------------------------
	//----������ʼ��
	this->m_pixmapWidth = width;
	this->m_pixmapHeight = height;

	this->m_maskEnabled = true;					//������ ����
	this->m_maskWidth = 24;						//������ ���
	this->m_maskHeight = 24;					//������ ���
	this->m_maskColor = QColor(245, 245, 245);	//������ ��ɫ

	//-----------------------------------
	//----ui��ʼ��
	this->setFixedWidth(this->m_pixmapWidth);
	this->setMinimumWidth(this->m_pixmapWidth);
	this->setMaximumWidth(this->m_pixmapWidth);
	this->setFixedHeight(this->m_pixmapHeight);
	this->setMinimumHeight(this->m_pixmapHeight);
	this->setMaximumHeight(this->m_pixmapHeight);
	this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

}
I_PiSPicture::~I_PiSPicture(){
}

/*-------------------------------------------------
		�ؼ� - ����ͼƬ
*/
void I_PiSPicture::setPixmap(QPixmap pixmap){
	this->m_pixmap = pixmap;
	this->update();
}
/*-------------------------------------------------
		�ؼ� - ������
*/
void I_PiSPicture::setMaskEnabled(bool maskEnabled){
	if (this->m_maskEnabled == maskEnabled){ return; }
	this->m_maskEnabled = maskEnabled;
	this->update();
}

/*-------------------------------------------------
		���� - ����ͼƬ
*/
void I_PiSPicture::paintEvent(QPaintEvent*){

	// > ����������
	QPixmap scaledPixmap = this->m_pixmap.scaled(QSize(this->m_pixmapWidth, this->m_pixmapHeight), Qt::KeepAspectRatio);

	// > ����
	QPainter* painter = new QPainter();
	painter->begin(this);

	//QPen pen;		//����һ��б�ߣ������ã�
	//pen.setWidthF(0.4);
	//pen.setColor(QColor(0, 0, 0));
	//painter->setPen(pen);
	//painter->drawLine(0, 0, 7, 7);

	// > ��������
	if (this->m_maskEnabled){
		painter->drawPixmap(QPoint(0, 0), this->getBitmapMaskBackground());
	}

	// > ���е�
	QPoint p = QPoint((this->m_pixmapWidth - scaledPixmap.width())*0.5, (this->m_pixmapHeight - scaledPixmap.height())*0.5);
	painter->drawPixmap(p, scaledPixmap);

	painter->end();
}
/*-------------------------------------------------
		���� - ���������˷���
*/
QPixmap I_PiSPicture::getBitmapMaskBackground(){
	int ww = this->m_pixmapWidth;
	int hh = this->m_pixmapHeight;
	int pw = this->m_maskWidth;
	int ph = this->m_maskHeight;

	QPixmap img = QPixmap(ww, hh);				//����
	QPainter painter(&img);						//����
	img.fill(this->m_maskColor);				//����ɫ

	painter.setPen(QPen(QColor(0, 0, 0, 0)));
	painter.setBrush(QBrush(QColor(0, 0, 0, 30), Qt::BrushStyle::SolidPattern));
	int i_count = qCeil(ww / (double)pw);
	int j_count = qCeil(hh / (double)ph);
	for (int i = 0; i < i_count; i++){
		for (int j = 0; j < j_count; j++){
			painter.drawRect(i*pw + 0, j*ph+ 0, pw * 0.5, ph * 0.5);
			painter.drawRect(i*pw + pw*0.5, j*ph + ph * 0.5, pw * 0.5, ph * 0.5);
		}
	}
	painter.end();
	//��ע�⣬painter.fillRect��������Ч�ģ���

	return img;
}