#include "stdafx.h"
#include "p_MouseResizeController.h"

#include "Source/Utils/common/TTool.h"


/*
-----==========================================================-----
		�ࣺ		������ſ�����.cpp
		�汾��		v1.01
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ÿ���������װ�� QGraphicsView ��ʹ���ܹ�֧�� ����������+�����ק ���ܡ�
					
		ʹ�÷�����	
				> ��ʼ��
					this->m_p_MouseResizeController = new P_MouseResizeController();
					this->m_p_MouseResizeController->initGraphicsView(this);
					//���������������������ؿ��ƣ��������ȥִ�У�
			
				> �¼���Ҫ�ӿ�
					//����ϸ���Լ� i_SPVView �ĵ��ã�	

				> �ⲿ�ź�
					connect(this->m_p_MouseResizeController, &P_MouseResizeController::scaleChanged, this, &I_SPVView::scaleChanged_view);

-----==========================================================-----
*/
P_MouseResizeController::P_MouseResizeController(){
}
P_MouseResizeController::~P_MouseResizeController(){
}

/*-------------------------------------------------
		��ʼ�� - ������ͼ
*/
void P_MouseResizeController::initGraphicsView(QGraphicsView* view){
	this->m_view = view;

	// > ���׷�ٿ���
	this->m_view->setMouseTracking(true);
	this->m_view->setCursor(QCursor(Qt::PointingHandCursor));		//��������

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
		���� - ��С
*/
void P_MouseResizeController::zoomOut(){
	if (this->m_scale <= 0.05){ return; }

	this->m_lastScale = this->m_scale;
	this->zoomResetPrivate();
	this->m_scale = this->m_lastScale - this->m_lastScale * 0.1;
	if (this->m_scale <= 0.05){ this->m_scale = 0.05; }

	this->m_view->scale(this->m_scale, this->m_scale);
	emit scaleChanged(this->m_scale);
}
/*-------------------------------------------------
		���� - �Ŵ�
*/
void P_MouseResizeController::zoomIn(){
	if (this->m_scale >= 5.00){ return; }
		
	this->m_lastScale = this->m_scale;
	this->zoomResetPrivate();
	this->m_scale = this->m_scale + 0.1;
	if (this->m_scale >= 5.00){ this->m_scale = 5.00; }

	this->m_view->scale(this->m_scale, this->m_scale);
	emit scaleChanged(this->m_scale);
}
/*-------------------------------------------------
		���� - ��С����
*/
void P_MouseResizeController::zoomReset(){
	this->zoomResetPrivate();
	this->m_scale = 1;
	emit scaleChanged(this->m_scale);
}
void P_MouseResizeController::zoomResetPrivate(){
	QMatrix org_matrix = this->m_view->matrix();	//��ֱ�ӵ�ȡ����ԭ��
	QMatrix tar_matrix;
	tar_matrix.setMatrix(1, org_matrix.m12(), org_matrix.m21(), 1, org_matrix.dx(), org_matrix.dy());
	this->m_view->setMatrix(tar_matrix, false);
}
/*-------------------------------------------------
		���� - ��ȡ����ֵ
*/
double P_MouseResizeController::getScale(){
	return this->m_scale;
}
/*-------------------------------------------------
		���� - ���ù����������η�
*/
void P_MouseResizeController::setScaleWheelModifier(QString charModifier){
	this->m_scaleWheelModifier = charModifier;
}


/*-------------------------------------------------
		�����¼� - ��갴��
*/
void P_MouseResizeController::event_mousePressEvent(QMouseEvent *event){

	// > ������±��
	if (event->button() == Qt::LeftButton){	
		this->m_mousePressed = true;
		this->m_mousePos = event->pos();
		this->m_mousePosLast = event->pos();
	}
}
/*-------------------------------------------------
		�����¼� - ���̧��
*/
void P_MouseResizeController::event_mouseReleaseEvent(QMouseEvent *event){

	// > �������
	this->m_mousePressed = false;
}
/*-------------------------------------------------
		�����¼� - ����ƶ�
*/
void P_MouseResizeController::event_mouseMoveEvent(QMouseEvent *event){
	if (this->m_mousePressed == false){ return; }
	this->m_mousePos = event->pos();

	// > �����ק���
	bool drag_enabled = false;
	if (this->m_scaleWheelModifier == "ֻ����"){ drag_enabled = true; }
	if (this->m_scaleWheelModifier == "ctrl" && (event->modifiers() & Qt::ControlModifier)){ drag_enabled = true; }
	if (this->m_scaleWheelModifier == "alt" && (event->modifiers() & Qt::AltModifier)){ drag_enabled = true; }

	if (drag_enabled == false){ return; }

	// > �����ק
	QPointF pf = this->m_mousePos - this->m_mousePosLast;
	if (this->m_view->horizontalScrollBar()->isVisible() ||
		this->m_view->verticalScrollBar()->isVisible()){

		int h_value = this->m_view->horizontalScrollBar()->value();
		int v_value = this->m_view->verticalScrollBar()->value();
		h_value = h_value - pf.x();
		v_value = v_value - pf.y();
		this->m_view->horizontalScrollBar()->setValue(h_value);
		this->m_view->verticalScrollBar()->setValue(v_value);
	}

	this->m_mousePosLast = this->m_mousePos;
}
/*-------------------------------------------------
		�����¼� - ������
*/
bool P_MouseResizeController::event_wheelEvent(QWheelEvent *e){

	// > ���ֿ��Ʊ��
	bool wheel_enabled = false;
	if (this->m_scaleWheelModifier == "ֻ����"){ wheel_enabled = true; }
	if (this->m_scaleWheelModifier == "ctrl" && (e->modifiers() & Qt::ControlModifier)){ wheel_enabled = true; }
	if (this->m_scaleWheelModifier == "alt" && (e->modifiers() & Qt::AltModifier)){ wheel_enabled = true; }

	if (wheel_enabled){
		if (e->delta() > 0){
			this->zoomIn();		//�������� - ��С
		}else{
			this->zoomOut();	//�������� - ��С
		}
		e->accept();
	}
	return wheel_enabled;
}