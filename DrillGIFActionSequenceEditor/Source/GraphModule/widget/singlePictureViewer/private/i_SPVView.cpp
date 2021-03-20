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

	// > ������
	connect(this->horizontalScrollBar(), &QScrollBar::valueChanged, this, &I_SPVView::scrollValueChanged);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &I_SPVView::scrollValueChanged);

	// > ��������
	this->m_scene = new I_SPVScene();
	this->setScene(m_scene);

	// > ������ſ�����
	this->m_p_MouseResizeController = new P_MouseResizeController();
	this->m_p_MouseResizeController->initGraphicsView(this);
}


/*-------------------------------------------------
		���� - ��ȡ����
*/
I_SPVScene* I_SPVView::getScene(){
	return this->m_scene;
}
/*-------------------------------------------------
		���� - ��ȡ������ſ�����
*/
P_MouseResizeController* I_SPVView::getMouseResizeController(){
	return this->m_p_MouseResizeController;
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

	// > ������ſ��� �¼�
	this->m_p_MouseResizeController->event_mousePressEvent(event);
}
/*-------------------------------------------------
		�����¼� - ���̧��
*/
void I_SPVView::mouseReleaseEvent(QMouseEvent *event){
	QGraphicsView::mouseReleaseEvent(event);	//�̳и��ຯ��

	// > ������ſ��� �¼�
	this->m_p_MouseResizeController->event_mouseReleaseEvent(event);
}
/*-------------------------------------------------
		�����¼� - ����ƶ�
*/
void I_SPVView::mouseMoveEvent(QMouseEvent *event){
	QGraphicsView::mouseMoveEvent(event);	//�̳и��ຯ��

	// > ������ſ��� �¼�
	this->m_p_MouseResizeController->event_mouseMoveEvent(event);
}
/*-------------------------------------------------
		�����¼� - ������
*/
void I_SPVView::wheelEvent(QWheelEvent *event){

	// > ������ſ��� �¼�
	bool blocked = this->m_p_MouseResizeController->event_wheelEvent(event);
	if ( blocked == true){ return; }

	QGraphicsView::wheelEvent(event);
}