#include "stdafx.h"
#include "i_PPaView.h"

/*
-----==========================================================-----
		�ࣺ		�и�֡ѡ���� ��ͼ.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		���ӿؼ��볡�����м��࣬���ƹ����������Ź��ܡ�
					
		�ӹ��ܣ�	->������ʼ�����������ܣ�
					->����֡���������ܣ�
					->������ſ�����
					->�������е�ѡ��

-----==========================================================-----
*/
I_PPaView::I_PPaView(QWidget *parent)
	: QGraphicsView(parent)
{
	this->init();
}
I_PPaView::~I_PPaView(){
}
/*-------------------------------------------------
		��ʼ��
*/
void I_PPaView::init(){

	// > ������
	connect(this->horizontalScrollBar(), &QScrollBar::valueChanged, this, &I_PPaView::scrollValueChanged);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &I_PPaView::scrollValueChanged);

	// > ��������
	this->m_scene = new I_PPaScene();
	this->setScene(m_scene);

	// > ������ſ�����
	this->m_p_MouseResizeController = new P_MouseResizeController();
	this->m_p_MouseResizeController->initGraphicsView(this);

	// > �����ѡ��
	this->m_p_MatrixBlockSelector = new P_MatrixBlockSelector();
	this->m_p_MatrixBlockSelector->initBlockColor(QList<QColor>() << QColor(255, 255, 255, 0) << QColor(0, 0, 0, 120) );
	this->m_p_MatrixBlockSelector->initBlockSvgIcon(
		QList<QFileInfo>() << QFileInfo("") << QFileInfo(QRC_IconSrcPath + "/view/Icon_Selected.svg")
	);
	this->m_p_MatrixBlockSelector->initGraphicsView(this);
	connect(this->m_p_MatrixBlockSelector, &P_MatrixBlockSelector::newBlockAdded, this, &I_PPaView::blockAdded);
	connect(this->m_p_MatrixBlockSelector, &P_MatrixBlockSelector::oldBlockClearBefore, this, &I_PPaView::blockClear);
}


/*-------------------------------------------------
		���� - ��ȡ����
*/
I_PPaScene* I_PPaView::getScene(){
	return this->m_scene;
}
/*-------------------------------------------------
		���� - ��ȡ������ſ�����
*/
P_MouseResizeController* I_PPaView::getMouseResizeController(){
	return this->m_p_MouseResizeController;
}
/*-------------------------------------------------
		���� - �������е�ѡ��
*/
P_MatrixBlockSelector* I_PPaView::getMatrixBlockSelector(){
	return this->m_p_MatrixBlockSelector;
}


/*-------------------------------------------------
		��ѡ�� - ��ȡ�иͼƬ
*/
QList<QPixmap> I_PPaView::getCutBitmap(){
	QList<QPixmap> result_list = QList<QPixmap>();
	QPixmap src_bitmap = this->m_scene->m_bitmap;
	QList<QPoint> p_list = this->m_p_MatrixBlockSelector->getPointList(1);	//��ȡ��ѡ��״̬λ
	for (int i = 0; i < p_list.count(); i++){
		QPoint p = p_list.at(i);
		int xx = qFloor(p.x()*this->m_block_width);		//������ʱ��С���������ȫ��ȡ����
		int yy = qFloor(p.y()*this->m_block_height);
		int ww = qFloor(this->m_block_width);
		int hh = qFloor(this->m_block_height);
		QPixmap bitmap = src_bitmap.copy(xx,yy,ww,hh);
		result_list.append(bitmap);
	}
	return result_list;
}
/*-------------------------------------------------
		���� - �������
*/
void I_PPaView::blockAdded(I_MBSBlock* block, QGraphicsSvgItem* item){
	this->scene()->addItem(block);
	this->scene()->addItem(item);

	// > ���㹴ѡ�������
	double size = 24;
	if (this->m_block_height > this->m_block_width){
		size = this->m_block_width /24 / 2;	//��24��ͼ��Ĵ�С��
	}else{
		size = this->m_block_height / 24 / 2;	//��24��ͼ��Ĵ�С��
	}
	item->setScale(size);
}
/*-------------------------------------------------
		���� - �������
*/
void I_PPaView::blockClear(QList<I_MBSBlock*> block_list, QList<QGraphicsSvgItem*> item_list){
	for (int i = 0; i < block_list.count(); i++ ){
		this->scene()->removeItem(block_list.at(i));
	}
	for (int i = 0; i < item_list.count(); i++){
		this->scene()->removeItem(item_list.at(i));
	}
}



/*-------------------------------------------------
		������ - ����������
*/
void I_PPaView::scrollValueChanged(){
	this->update();		//������ˢ
}


/*-------------------------------------------------
		�����¼� - ��갴��
*/
void I_PPaView::mousePressEvent(QMouseEvent *event){
	QGraphicsView::mousePressEvent(event);	//�̳и��ຯ��

	// > ������ſ��� �¼�
	this->m_p_MouseResizeController->event_mousePressEvent(event);

	// > �����ѡ�� �¼�
	this->m_p_MatrixBlockSelector->event_mousePressEvent(event);
}
/*-------------------------------------------------
		�����¼� - ���̧��
*/
void I_PPaView::mouseReleaseEvent(QMouseEvent *event){
	QGraphicsView::mouseReleaseEvent(event);	//�̳и��ຯ��

	// > ������ſ��� �¼�
	this->m_p_MouseResizeController->event_mouseReleaseEvent(event);

	// > �����ѡ�� �¼�
	this->m_p_MatrixBlockSelector->event_mouseReleaseEvent(event);
}
/*-------------------------------------------------
		�����¼� - ����ƶ�
*/
void I_PPaView::mouseMoveEvent(QMouseEvent *event){
	QGraphicsView::mouseMoveEvent(event);	//�̳и��ຯ��

	// > ������ſ��� �¼�
	this->m_p_MouseResizeController->event_mouseMoveEvent(event);

	// > �����ѡ�� �¼�
	this->m_p_MatrixBlockSelector->event_mouseMoveEvent(event);
}
/*-------------------------------------------------
		�����¼� - ������
*/
void I_PPaView::wheelEvent(QWheelEvent *event){

	// > ������ſ��� �¼�
	bool blocked = this->m_p_MouseResizeController->event_wheelEvent(event);
	if ( blocked == true){ return; }

	QGraphicsView::wheelEvent(event);
}