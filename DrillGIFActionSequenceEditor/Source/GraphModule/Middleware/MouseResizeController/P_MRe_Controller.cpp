#include "stdafx.h"
#include "P_MRe_Controller.h"

#include "Source/Utils/Common/TTool.h"


/*
-----==========================================================-----
		类：		鼠标缩放控制器.cpp
		缩写：		MRe（MouseResize）
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		该控制器用于装饰 QGraphicsView ，使其能够支持 鼠标控制缩放+鼠标拖拽 功能。
					
		子功能：	->鼠标滚轮缩放
					->鼠标拖拽
					->鼠标手形变化

		使用方法：	
				> 初始化
					this->m_p_MRe_Controller = new P_MRe_Controller();
					this->m_p_MRe_Controller->initGraphicsView(this);
					//（接下来所有鼠标缩放相关控制，用这个类去执行）
			
				> 事件必要接口
					//（详细可以见 i_SPVView 的调用）	

				> 外部信号
					connect(this->m_p_MRe_Controller, &P_MRe_Controller::scaleChanged, this, &I_SPVView::scaleChanged_view);

-----==========================================================-----
*/
P_MRe_Controller::P_MRe_Controller(){
}
P_MRe_Controller::~P_MRe_Controller(){
}

/*-------------------------------------------------
		初始化 - 设置视图
*/
void P_MRe_Controller::initGraphicsView(QGraphicsView* view){
	this->m_view = view;

	// > 鼠标追踪开启
	this->m_view->setMouseTracking(true);
	this->m_cursor = this->m_view->cursor();

	// > 缩放
	this->m_scale = 1.00;
	this->m_lastScale = 1.00;
	this->m_scaleWheelModifier = "只滚轮";

	// > 鼠标拖拽
	this->m_mousePressed = false;
	this->m_mousePos = QPoint(0, 0);
	this->m_mousePosLast = QPoint(0, 0);
}

/*-------------------------------------------------
		缩放 - 缩小
*/
void P_MRe_Controller::zoomOut(){
	if (this->m_scale <= 0.05){ return; }

	this->m_lastScale = this->m_scale;
	this->zoomResetPrivate();
	this->m_scale = this->m_lastScale - this->m_lastScale * 0.1;
	if (this->m_scale <= 0.05){ this->m_scale = 0.05; }

	this->m_view->scale(this->m_scale, this->m_scale);
	emit scaleChanged(this->m_scale);
}
/*-------------------------------------------------
		缩放 - 放大
*/
void P_MRe_Controller::zoomIn(){
	if (this->m_scale >= 5.00){ return; }
		
	this->m_lastScale = this->m_scale;
	this->zoomResetPrivate();
	this->m_scale = this->m_scale + 0.1;
	if (this->m_scale >= 5.00){ this->m_scale = 5.00; }

	this->m_view->scale(this->m_scale, this->m_scale);
	emit scaleChanged(this->m_scale);
}
/*-------------------------------------------------
		缩放 - 大小重置
*/
void P_MRe_Controller::zoomReset(){
	this->zoomResetPrivate();
	this->m_scale = 1;
	emit scaleChanged(this->m_scale);
}
void P_MRe_Controller::zoomResetPrivate(){
	QMatrix org_matrix = this->m_view->matrix();	//（直接调取矩阵还原）
	QMatrix tar_matrix;
	tar_matrix.setMatrix(1, org_matrix.m12(), org_matrix.m21(), 1, org_matrix.dx(), org_matrix.dy());
	this->m_view->setMatrix(tar_matrix, false);
}
/*-------------------------------------------------
		缩放 - 获取缩放值
*/
double P_MRe_Controller::getScale(){
	return this->m_scale;
}
/*-------------------------------------------------
		缩放 - 设置滚轮缩放修饰符
*/
void P_MRe_Controller::setScaleWheelModifier(QString charModifier){
	this->m_scaleWheelModifier = charModifier;
}


/*-------------------------------------------------
		监听事件 - 鼠标按下
*/
void P_MRe_Controller::event_mousePressEvent(QMouseEvent *event){

	// > 鼠标拖拽标记
	bool drag_enabled = false;
	if (this->m_scaleWheelModifier == "只滚轮"){ drag_enabled = true; }
	if (this->m_scaleWheelModifier == "ctrl" && (event->modifiers() & Qt::ControlModifier)){ drag_enabled = true; }
	if (this->m_scaleWheelModifier == "alt" && (event->modifiers() & Qt::AltModifier)){ drag_enabled = true; }

	// > 左键按下标记
	if (event->button() == Qt::LeftButton){	
		this->m_mousePressed = true;
		this->m_mousePos = event->pos();
		this->m_mousePosLast = event->pos();
	}

	// > 鼠标变化
	if (this->m_mousePressed == true && drag_enabled == true){
		this->m_view->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));		//闭合手形
	}
}
/*-------------------------------------------------
		监听事件 - 鼠标抬起
*/
void P_MRe_Controller::event_mouseReleaseEvent(QMouseEvent *event){

	// > 鼠标拖拽标记
	bool drag_enabled = false;
	if (this->m_scaleWheelModifier == "只滚轮"){ drag_enabled = true; }
	if (this->m_scaleWheelModifier == "ctrl" && (event->modifiers() & Qt::ControlModifier)){ drag_enabled = true; }
	if (this->m_scaleWheelModifier == "alt" && (event->modifiers() & Qt::AltModifier)){ drag_enabled = true; }

	// > 标记清理
	if (event->button() == Qt::LeftButton){
		this->m_mousePressed = false;
	}

	// > 鼠标变化
	if (drag_enabled == false){
		this->m_view->setCursor(this->m_cursor);								//默认鼠标形状
		return;
	}
	if (this->m_mousePressed == false){
		this->m_view->setCursor(QCursor(Qt::CursorShape::OpenHandCursor));		//展开手形
		return;
	}
}
/*-------------------------------------------------
		监听事件 - 鼠标移动
*/
void P_MRe_Controller::event_mouseMoveEvent(QMouseEvent *event){
	this->m_mousePos = event->pos();

	// > 鼠标拖拽标记
	bool drag_enabled = false;
	if (this->m_scaleWheelModifier == "只滚轮"){ drag_enabled = true; }
	if (this->m_scaleWheelModifier == "ctrl" && (event->modifiers() & Qt::ControlModifier)){ drag_enabled = true; }
	if (this->m_scaleWheelModifier == "alt" && (event->modifiers() & Qt::AltModifier)){ drag_enabled = true; }

	// > 鼠标变化
	if (drag_enabled == false){
		this->m_view->setCursor(this->m_cursor);								//默认鼠标形状
		return; 
	}
	if (this->m_mousePressed == false){ 
		this->m_view->setCursor(QCursor(Qt::CursorShape::OpenHandCursor));		//展开手形
		return; 
	}
	this->m_view->setCursor(QCursor(Qt::CursorShape::ClosedHandCursor));		//闭合手形

	// > 鼠标拖拽
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
		监听事件 - 鼠标滚轮
*/
bool P_MRe_Controller::event_wheelEvent(QWheelEvent *e){

	// > 滚轮控制标记
	bool wheel_enabled = false;
	if (this->m_scaleWheelModifier == "只滚轮"){ wheel_enabled = true; }
	if (this->m_scaleWheelModifier == "ctrl" && (e->modifiers() & Qt::ControlModifier)){ wheel_enabled = true; }
	if (this->m_scaleWheelModifier == "alt" && (e->modifiers() & Qt::AltModifier)){ wheel_enabled = true; }

	if (wheel_enabled){
		if (e->delta() > 0){
			this->zoomIn();		//滚轮向上 - 缩小
		}else{
			this->zoomOut();	//滚轮向上 - 缩小
		}
		e->accept();
	}
	return wheel_enabled;
}