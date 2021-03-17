#include "stdafx.h"
#include "i_SPVView.h"

/*
-----==========================================================-----
		类：		图片查看块-单图 视图.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		连接控件与场景的中间类，控制滚动条与缩放功能。
					
		子功能：	->场景初始化（场景功能）
					->动画帧（场景功能）
					->缩放
						->缩小/放大
					->鼠标拖拽

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
		初始化
*/
void I_SPVView::init(){

	// > 鼠标追踪开启
	this->setMouseTracking(true);
	this->setCursor(QCursor(Qt::PointingHandCursor));		//设置手形

	// > 滚动条
	connect(this->horizontalScrollBar(), &QScrollBar::valueChanged, this, &I_SPVView::scrollValueChanged);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &I_SPVView::scrollValueChanged);

	// > 场景建立
	this->m_scene = new I_SPVScene();
	this->setScene(m_scene);

	// > 缩放
	this->m_scale = 1.00;
	this->m_lastScale = 1.00;
	this->m_scaleWheelModifier = "只滚轮";

	// > 鼠标拖拽
	this->m_mousePressed = false;
	this->m_mousePos = QPoint();
	this->m_mousePosLast = QPoint();

}


/*-------------------------------------------------
		部件 - 获取场景
*/
I_SPVScene* I_SPVView::getScene(){
	return this->m_scene;
}



/*-------------------------------------------------
		缩放 - 缩小
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
		缩放 - 放大
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
		缩放 - 大小重置
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
		缩放 - 获取缩放值
*/
double I_SPVView::getScale(){
	return this->m_scale;
}
/*-------------------------------------------------
		缩放 - 设置滚轮缩放修饰符
*/
void I_SPVView::setScaleWheelModifier(QString charModifier){
	this->m_scaleWheelModifier = charModifier;
}


/*-------------------------------------------------
		滚动条 - 滚动条滚动
*/
void I_SPVView::scrollValueChanged(){
	this->update();		//场景重刷
}


/*-------------------------------------------------
		监听事件 - 鼠标按下
*/
void I_SPVView::mousePressEvent(QMouseEvent *event){
	QGraphicsView::mousePressEvent(event);	//继承父类函数

	if (event->button() == Qt::LeftButton){
		this->m_mousePressed = true;
		this->m_mousePos = event->pos();
		this->m_mousePosLast = event->pos();
	}
}
/*-------------------------------------------------
		监听事件 - 鼠标抬起
*/
void I_SPVView::mouseReleaseEvent(QMouseEvent *event){
	QGraphicsView::mouseReleaseEvent(event);	//继承父类函数
	this->m_mousePressed = false;
}
/*-------------------------------------------------
		监听事件 - 鼠标移动
*/
void I_SPVView::mouseMoveEvent(QMouseEvent *event){
	QGraphicsView::mouseMoveEvent(event);	//继承父类函数

	if (this->m_mousePressed == false){ return; }
	this->m_mousePos = event->pos();

	// > 鼠标拖拽
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
		监听事件 - 鼠标滚轮
*/
void I_SPVView::wheelEvent(QWheelEvent *e){

	bool wheel_enabled = false;
	if (this->m_scaleWheelModifier == "只滚轮"){ wheel_enabled = true; }
	if (this->m_scaleWheelModifier == "ctrl" && (e->modifiers() & Qt::ControlModifier)){ wheel_enabled = true; }
	if (this->m_scaleWheelModifier == "alt" && (e->modifiers() & Qt::AltModifier)){ wheel_enabled = true; }

	if (wheel_enabled){
		if (e->delta() > 0){
			zoomIn();	//滚轮向上 - 缩小
		}else{
			zoomOut();	//滚轮向上 - 缩小
		}
		e->accept();
	}else{
		QGraphicsView::wheelEvent(e);
	}
}