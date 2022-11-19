#include "stdafx.h"
#include "I_APVView.h"

/*
-----==========================================================-----
		类：		图片查看块-动图 视图.cpp
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
I_APVView::I_APVView(QWidget *parent)
	: QGraphicsView(parent)
{
	this->init();
}
I_APVView::~I_APVView(){
}
/*-------------------------------------------------
		初始化
*/
void I_APVView::init(){

	// > 滚动条
	connect(this->horizontalScrollBar(), &QScrollBar::valueChanged, this, &I_APVView::scrollValueChanged);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &I_APVView::scrollValueChanged);

	// > 场景建立
	this->m_scene = new I_APVScene();
	this->setScene(m_scene);

	// > 鼠标缩放控制器
	this->m_p_MouseResizeController = new P_MRe_Controller();
	this->m_p_MouseResizeController->initGraphicsView(this);
}


/*-------------------------------------------------
		部件 - 获取场景
*/
I_APVScene* I_APVView::getScene(){
	return this->m_scene;
}
/*-------------------------------------------------
		部件 - 获取鼠标缩放控制器
*/
P_MRe_Controller* I_APVView::getMouseResizeController(){
	return this->m_p_MouseResizeController;
}


/*-------------------------------------------------
		滚动条 - 滚动条滚动
*/
void I_APVView::scrollValueChanged(){
	this->update();		//场景重刷
}


/*-------------------------------------------------
		监听事件 - 鼠标按下
*/
void I_APVView::mousePressEvent(QMouseEvent *event){
	QGraphicsView::mousePressEvent(event);	//继承父类函数

	// > 鼠标缩放控制 事件
	this->m_p_MouseResizeController->event_mousePressEvent(event);
}
/*-------------------------------------------------
		监听事件 - 鼠标抬起
*/
void I_APVView::mouseReleaseEvent(QMouseEvent *event){
	QGraphicsView::mouseReleaseEvent(event);	//继承父类函数

	// > 鼠标缩放控制 事件
	this->m_p_MouseResizeController->event_mouseReleaseEvent(event);
}
/*-------------------------------------------------
		监听事件 - 鼠标移动
*/
void I_APVView::mouseMoveEvent(QMouseEvent *event){
	QGraphicsView::mouseMoveEvent(event);	//继承父类函数

	// > 鼠标缩放控制 事件
	this->m_p_MouseResizeController->event_mouseMoveEvent(event);
}
/*-------------------------------------------------
		监听事件 - 鼠标滚轮
*/
void I_APVView::wheelEvent(QWheelEvent *event){

	// > 鼠标缩放控制 事件
	bool blocked = this->m_p_MouseResizeController->event_wheelEvent(event);
	if (blocked == true){ return; }

	QGraphicsView::wheelEvent(event);
}