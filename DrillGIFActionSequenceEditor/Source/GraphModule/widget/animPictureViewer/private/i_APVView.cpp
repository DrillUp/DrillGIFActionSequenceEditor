#include "stdafx.h"
#include "i_APVView.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴��-��ͼ ��ͼ.cpp
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
I_APVView::I_APVView(QWidget *parent)
	: QGraphicsView(parent)
{
	this->init();
}
I_APVView::~I_APVView(){
}
/*-------------------------------------------------
		��ʼ��
*/
void I_APVView::init(){

	// > ������
	connect(this->horizontalScrollBar(), &QScrollBar::valueChanged, this, &I_APVView::scrollValueChanged);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &I_APVView::scrollValueChanged);

	// > ��������
	this->m_scene = new I_APVScene();
	this->setScene(m_scene);

	// > ������ſ�����
	this->m_p_MouseResizeController = new P_MouseResizeController();
	this->m_p_MouseResizeController->initGraphicsView(this);
}


/*-------------------------------------------------
		���� - ��ȡ����
*/
I_APVScene* I_APVView::getScene(){
	return this->m_scene;
}
/*-------------------------------------------------
		���� - ��ȡ������ſ�����
*/
P_MouseResizeController* I_APVView::getMouseResizeController(){
	return this->m_p_MouseResizeController;
}


/*-------------------------------------------------
		������ - ����������
*/
void I_APVView::scrollValueChanged(){
	this->update();		//������ˢ
}


/*-------------------------------------------------
		�����¼� - ��갴��
*/
void I_APVView::mousePressEvent(QMouseEvent *event){
	QGraphicsView::mousePressEvent(event);	//�̳и��ຯ��

	// > ������ſ��� �¼�
	this->m_p_MouseResizeController->event_mousePressEvent(event);
}
/*-------------------------------------------------
		�����¼� - ���̧��
*/
void I_APVView::mouseReleaseEvent(QMouseEvent *event){
	QGraphicsView::mouseReleaseEvent(event);	//�̳и��ຯ��

	// > ������ſ��� �¼�
	this->m_p_MouseResizeController->event_mouseReleaseEvent(event);
}
/*-------------------------------------------------
		�����¼� - ����ƶ�
*/
void I_APVView::mouseMoveEvent(QMouseEvent *event){
	QGraphicsView::mouseMoveEvent(event);	//�̳и��ຯ��

	// > ������ſ��� �¼�
	this->m_p_MouseResizeController->event_mouseMoveEvent(event);
}
/*-------------------------------------------------
		�����¼� - ������
*/
void I_APVView::wheelEvent(QWheelEvent *event){

	// > ������ſ��� �¼�
	bool blocked = this->m_p_MouseResizeController->event_wheelEvent(event);
	if (blocked == true){ return; }

	QGraphicsView::wheelEvent(event);
}