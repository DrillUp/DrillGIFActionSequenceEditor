#include "stdafx.h"
#include "I_SPVView.h"

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

	// > 滚动条
	connect(this->horizontalScrollBar(), &QScrollBar::valueChanged, this, &I_SPVView::scrollValueChanged);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &I_SPVView::scrollValueChanged);

	// > 场景建立
	this->m_scene = new I_SPVScene();
	this->setScene(m_scene);

	// > 鼠标缩放控制器
	this->m_p_MouseResizeController = new P_MRe_Controller();
	this->m_p_MouseResizeController->initGraphicsView(this);
}


/*-------------------------------------------------
		部件 - 获取场景
*/
I_SPVScene* I_SPVView::getScene(){
	return this->m_scene;
}
/*-------------------------------------------------
		部件 - 获取鼠标缩放控制器
*/
P_MRe_Controller* I_SPVView::getMouseResizeController(){
	return this->m_p_MouseResizeController;
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

	// > 鼠标缩放控制 事件
	this->m_p_MouseResizeController->event_mousePressEvent(event);
}
/*-------------------------------------------------
		监听事件 - 鼠标抬起
*/
void I_SPVView::mouseReleaseEvent(QMouseEvent *event){
	QGraphicsView::mouseReleaseEvent(event);	//继承父类函数

	// > 鼠标缩放控制 事件
	this->m_p_MouseResizeController->event_mouseReleaseEvent(event);
}
/*-------------------------------------------------
		监听事件 - 鼠标移动
*/
void I_SPVView::mouseMoveEvent(QMouseEvent *event){
	QGraphicsView::mouseMoveEvent(event);	//继承父类函数

	// > 鼠标缩放控制 事件
	this->m_p_MouseResizeController->event_mouseMoveEvent(event);
}
/*-------------------------------------------------
		监听事件 - 鼠标滚轮
*/
void I_SPVView::wheelEvent(QWheelEvent *event){

	// > 鼠标缩放控制 事件
	bool blocked = this->m_p_MouseResizeController->event_wheelEvent(event);
	if ( blocked == true){ return; }

	QGraphicsView::wheelEvent(event);
}