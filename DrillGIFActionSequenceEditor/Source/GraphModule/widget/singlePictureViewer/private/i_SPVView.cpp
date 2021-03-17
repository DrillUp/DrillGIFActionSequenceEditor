#include "stdafx.h"
#include "i_SPVView.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴��-��ͼ ��ͼ.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���ӿؼ��볡�����м��࣬���ƹ����������Ź��ܡ�
					
		�ӹ��ܣ�	->������ʼ�����������ܣ�
					->����֡���������ܣ�
					->����
						->��С/�Ŵ�
					->�����ק

-----==========================================================-----
*/
I_SPVView::I_SPVView(QWidget *parent)
	: QGraphicsView(parent)
{
	this->init();
}
I_SPVView::~I_SPVView(){
}
/*-------------------------------------------------
		��ʼ��
*/
void I_SPVView::init(){

	// > ���׷�ٿ���
	this->setMouseTracking(true);
	this->setCursor(QCursor(Qt::PointingHandCursor));		//��������

	// > ������
	connect(this->horizontalScrollBar(), &QScrollBar::valueChanged, this, &I_SPVView::scrollValueChanged);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &I_SPVView::scrollValueChanged);

	// > ��������
	this->m_scene = new I_SPVScene();
	this->setScene(m_scene);

	// > ����
	this->m_scale = 1.00;
	this->m_lastScale = 1.00;
	this->m_scaleWheelModifier = "ֻ����";

	// > �����ק
	this->m_mousePressed = false;
	this->m_mousePos = QPoint();
	this->m_mousePosLast = QPoint();

}


/*-------------------------------------------------
		���� - ��ȡ����
*/
I_SPVScene* I_SPVView::getScene(){
	return this->m_scene;
}



/*-------------------------------------------------
		���� - ��С
*/
void I_SPVView::zoomOut(){
	if (this->m_scale <= 0.05){ return; }

	this->m_lastScale = this->m_scale;
	this->zoomResetPrivate();
	this->m_scale = this->m_lastScale - this->m_lastScale * 0.1;
	if (this->m_scale <= 0.05){ this->m_scale = 0.05; }

	scale(this->m_scale, this->m_scale);
	emit scaleChanged(this->m_scale);
}
/*-------------------------------------------------
		���� - �Ŵ�
*/
void I_SPVView::zoomIn(){
	if (this->m_scale >= 5.00){ return; }
		
	this->m_lastScale = this->m_scale;
	this->zoomResetPrivate();
	this->m_scale = this->m_scale + 0.1;
	if (this->m_scale >= 5.00){ this->m_scale = 5.00; }

	scale(this->m_scale, this->m_scale);
	emit scaleChanged(this->m_scale);
}
/*-------------------------------------------------
		���� - ��С����
*/
void I_SPVView::zoomReset(){
	this->zoomResetPrivate();
	this->m_scale = 1;
	emit scaleChanged(this->m_scale);
}
void I_SPVView::zoomResetPrivate(){
	QMatrix q;
	q.setMatrix(1, this->matrix().m12(), this->matrix().m21(), 1, this->matrix().dx(), this->matrix().dy());
	this->setMatrix(q, false);
}
/*-------------------------------------------------
		���� - ��ȡ����ֵ
*/
double I_SPVView::getScale(){
	return this->m_scale;
}
/*-------------------------------------------------
		���� - ���ù����������η�
*/
void I_SPVView::setScaleWheelModifier(QString charModifier){
	this->m_scaleWheelModifier = charModifier;
}


/*-------------------------------------------------
		������ - ����������
*/
void I_SPVView::scrollValueChanged(){
	this->update();		//������ˢ
}


/*-------------------------------------------------
		�����¼� - ��갴��
*/
void I_SPVView::mousePressEvent(QMouseEvent *event){
	QGraphicsView::mousePressEvent(event);	//�̳и��ຯ��

	if (event->button() == Qt::LeftButton){
		this->m_mousePressed = true;
		this->m_mousePos = event->pos();
		this->m_mousePosLast = event->pos();
	}
}
/*-------------------------------------------------
		�����¼� - ���̧��
*/
void I_SPVView::mouseReleaseEvent(QMouseEvent *event){
	QGraphicsView::mouseReleaseEvent(event);	//�̳и��ຯ��
	this->m_mousePressed = false;
}
/*-------------------------------------------------
		�����¼� - ����ƶ�
*/
void I_SPVView::mouseMoveEvent(QMouseEvent *event){
	QGraphicsView::mouseMoveEvent(event);	//�̳и��ຯ��

	if (this->m_mousePressed == false){ return; }
	this->m_mousePos = event->pos();

	// > �����ק
	QPointF pf = this->m_mousePos - this->m_mousePosLast;
	if (this->horizontalScrollBar()->isVisible() || 
		this->verticalScrollBar()->isVisible()){

		int h_value = this->horizontalScrollBar()->value();
		int v_value = this->verticalScrollBar()->value();
		h_value = h_value - pf.x();
		v_value = v_value - pf.y();
		this->horizontalScrollBar()->setValue(h_value);
		this->verticalScrollBar()->setValue(v_value);
	}

	this->m_mousePosLast = this->m_mousePos;
}
/*-------------------------------------------------
		�����¼� - ������
*/
void I_SPVView::wheelEvent(QWheelEvent *e){

	bool wheel_enabled = false;
	if (this->m_scaleWheelModifier == "ֻ����"){ wheel_enabled = true; }
	if (this->m_scaleWheelModifier == "ctrl" && (e->modifiers() & Qt::ControlModifier)){ wheel_enabled = true; }
	if (this->m_scaleWheelModifier == "alt" && (e->modifiers() & Qt::AltModifier)){ wheel_enabled = true; }

	if (wheel_enabled){
		if (e->delta() > 0){
			zoomIn();	//�������� - ��С
		}else{
			zoomOut();	//�������� - ��С
		}
		e->accept();
	}else{
		QGraphicsView::wheelEvent(e);
	}
}