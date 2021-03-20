#include "stdafx.h"
#include "p_MatrixBlockSelector.h"

#include "Source/Utils/common/TTool.h"


/*
-----==========================================================-----
		�ࣺ		�������е�ѡ��.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ÿ���������װ�� QGraphicsView ��ʹ���ܹ�֧�� �������й�ѡ�л� + ��ɫ���л�/ʸ��ͼ�л� ���ܡ�
					
		ʹ�÷�����	
				> ��ʼ��
					this->m_p_MatrixBlockSelector = new P_MatrixBlockSelector();
					this->m_p_MatrixBlockSelector->setBlockSize(ww, hh);
					this->m_p_MatrixBlockSelector->initBlockColor(QList<QColor>() << QColor(255, 255, 255, 0) << QColor(0, 0, 255, 50));
					this->m_p_MatrixBlockSelector->setGraphicsView(this);
					connect(this->m_p_MatrixBlockSelector, &P_MatrixBlockSelector::newBlockAdded, this, &I_FixedAreaView::blockAdded);
					connect(this->m_p_MatrixBlockSelector, &P_MatrixBlockSelector::oldBlockClearBefore, this, &I_FixedAreaView::clearBlockBefore);
					//�����������з��鹴ѡ��ؿ��ƣ��������ȥִ�У�
			
				> �¼���Ҫ�ӿ�
					//����ϸ���Լ� i_SPVView �ĵ��ã�	

				> �ⲿ�ź�
					connect(this->m_p_MatrixBlockSelector, &P_MatrixBlockSelector::scaleChanged, this, &I_SPVView::scaleChanged_view);

-----==========================================================-----
*/
P_MatrixBlockSelector::P_MatrixBlockSelector(){
	this->m_blockWidth = 24;
	this->m_blockHeight = 24;
	this->m_blockColorTank = QList<QColor>() << QColor(255, 255, 255, 0) << QColor(0, 0, 255, 50);
}
P_MatrixBlockSelector::~P_MatrixBlockSelector(){
}

/*-------------------------------------------------
		��ʼ�� - ���÷����С
*/
void P_MatrixBlockSelector::initBlockSize(int width, int height){
	this->m_blockWidth = width;
	this->m_blockHeight = height;
}
/*-------------------------------------------------
		��ʼ�� - ���÷�����ɫ
*/
void P_MatrixBlockSelector::initBlockColor(QList<QColor> color_list){
	this->m_blockColorTank = color_list;
}
/*-------------------------------------------------
		��ʼ�� - ����ʸ��ͼ�б�
*/
void P_MatrixBlockSelector::initBlockSvgIcon(QList<QFileInfo> svg_file_list){
	for (int i = 0; i < svg_file_list.count(); i++){
		QFileInfo svg_file = svg_file_list.at(i);
		QSvgRenderer* render = new QSvgRenderer(svg_file.absoluteFilePath());
		this->m_rendererTank.append(render);
	}
}
/*-------------------------------------------------
		��ʼ�� - ������ͼ
*/
void P_MatrixBlockSelector::initGraphicsView(QGraphicsView* view){
	this->m_view = view;

	// > ���׷�ٿ���
	this->m_view->setMouseTracking(true);
	this->m_view->setCursor(QCursor(Qt::PointingHandCursor));		//��������

	// > ѡ�з��� ����
	this->m_blockTank = QList<I_MBSBlock*>();
	this->m_cur_mouseState = 0;
	this->m_mousePressed = false;
}
/*-------------------------------------------------
		���� - ���÷���
*/
void P_MatrixBlockSelector::rebuildBlock(int width, int height){
	this->clearAllBlock();		//����շ��飩
	this->m_blockWidth = width;
	this->m_blockHeight = height;
}
void P_MatrixBlockSelector::rebuildBlock_double(double width, double height){
	this->clearAllBlock();		//����շ��飩
	this->m_blockWidth = width;
	this->m_blockHeight = height;
}


/*-------------------------------------------------
		���� - ��ȡȫ������
*/
QList<I_MBSBlock*> P_MatrixBlockSelector::getAllBlock(){
	return this->m_blockTank;
}
/*-------------------------------------------------
		���� - ��ȡ������󣨵�λͼ�飩
*/
QRect P_MatrixBlockSelector::getBlockMatrix(){
	QRectF scene_rect = this->m_view->sceneRect();
		
	// > �ܱ��������������ֵ
	int left = qCeil(scene_rect.left() / this->m_blockWidth);
	int top = qCeil(scene_rect.top() / this->m_blockHeight);
	int right = qFloor(scene_rect.right() / this->m_blockWidth) -1;	//������صķ��鲻�㣩
	int bottom = qFloor(scene_rect.bottom() / this->m_blockHeight) -1;
	return QRect(QPoint(left,top),QPoint(right,bottom));
}
/*-------------------------------------------------
		���� - ���ȫ�����顢ʸ�����
*/
void P_MatrixBlockSelector::clearAllBlock(){
	emit oldBlockClearBefore(this->m_blockTank, this->m_svgTank);
	this->m_cur_mouseState = 0;
	this->m_blockTank.clear();
	this->m_svgTank.clear();
	this->m_coveredPointTank.clear();
}
/*-------------------------------------------------
		���� - ��ӷ���
*/
void P_MatrixBlockSelector::setBlock(QPoint matrix_pos, int state){

	// > �������ʱ
	int index = this->m_coveredPointTank.indexOf(matrix_pos);
	if (index != -1 ){
		this->m_blockTank.at(index)->setState(state);
		if (this->m_rendererTank.count() != 0){
			this->m_svgTank.at(index)->setSharedRenderer(this->m_rendererTank.at(state));
		}
		return;
	}

	// > �½�����
	I_MBSBlock* item = new I_MBSBlock();
	item->setBlockSize(this->m_blockWidth, this->m_blockHeight);
	item->setColorTank(this->m_blockColorTank);
	item->setMatrixPosition(matrix_pos.x(), matrix_pos.y());
	item->setState(state);
	this->m_blockTank.push_back(item);
	this->m_coveredPointTank.push_back(matrix_pos);

	QGraphicsSvgItem* svgItem = new QGraphicsSvgItem();
	svgItem->setPos(matrix_pos.x()*this->m_blockWidth, matrix_pos.y()*this->m_blockHeight);
	if (this->m_rendererTank.count() != 0){
		svgItem->setSharedRenderer(this->m_rendererTank.at(state));
	}
	this->m_svgTank.push_back(svgItem);

	emit newBlockAdded(item, svgItem);
}


/*-------------------------------------------------
		��Ϳ - ��ȡָ��״̬λ��ȫ����
*/
QList<QPoint> P_MatrixBlockSelector::getPointList(int state){
	QList<QPoint> result_list = QList<QPoint>();

	// > ״̬λ =0 ʱ
	if (state == 0){

		// > ���� �յ�
		QRect rect = this->getBlockMatrix();
		if ( this->m_blockTank.count() < rect.width()* rect.height() ){
			for (int y = rect.y(); y < rect.height(); y++){
				for (int x = rect.x(); x < rect.width(); x++){	//���Ⱥ���������
					QPoint pos = QPoint(x, y);
					if (this->m_coveredPointTank.contains(pos) == false){
						result_list.append(pos);
					}
				}
			}
		}

		// > ���� ״̬λ=0 �ĵ�
		for (int i = 0; i < this->m_blockTank.count(); i++){
			I_MBSBlock* item = this->m_blockTank.at(i);
			if (item->getState() == state){
				result_list.append(QPoint(item->getMatrixX(), item->getMatrixY()));
			}
		}

	// > ���� ״̬λ ʱ
	}else{
		for (int i = 0; i < this->m_blockTank.count(); i++){
			I_MBSBlock* item = this->m_blockTank.at(i);
			if (item->getState() == state){
				result_list.append(QPoint(item->getMatrixX(), item->getMatrixY()));
			}
		}
	}
	return result_list;
}
/*-------------------------------------------------
		��Ϳ - ��Ϳȫ��״̬λ
*/
void P_MatrixBlockSelector::fillAll(int state){

	QRect rect = this->getBlockMatrix();
	for (int y = rect.y(); y < rect.height(); y++){
		for (int x = rect.x(); x < rect.width(); x++){	//���Ⱥ���������
			QPoint pos = QPoint(x, y);
			this->setBlock(pos, state);
		}
	}
}


/*-------------------------------------------------
		�����¼� - ��갴��
*/
void P_MatrixBlockSelector::event_mousePressEvent(QMouseEvent *event){

	// > ������±��
	this->m_mousePressed = true;
	QPointF widget_point = event->pos();
	QPointF scene_point = this->m_view->mapToScene(widget_point.rx(), widget_point.ry());

	// > ��ת��
	int xx = qFloor(scene_point.rx() / this->m_blockWidth);
	int yy = qFloor(scene_point.ry() / this->m_blockHeight);
	QPoint p = QPoint(xx, yy);

	// > ��Ϳ״̬�л�
	bool changed = false;
	for (int i = 0; i < this->m_blockTank.count(); i++){
		I_MBSBlock* item = this->m_blockTank.at(i);
		if (item->getMatrixX() == p.x() && item->getMatrixY() == p.y()){

			int state = item->getState();
			state += 1;
			state %= this->m_blockColorTank.count();
			item->setState(state);
			this->m_cur_mouseState = state;

			changed = true;
			break;
		}
	}
	if (changed == false){		//�޷���ʱ��Ĭ�����1״̬
		this->m_cur_mouseState = 1;
	}

	// > ˢ�·��鼯
	this->setBlock(p, this->m_cur_mouseState);
}
/*-------------------------------------------------
		�����¼� - ���̧��
*/
void P_MatrixBlockSelector::event_mouseReleaseEvent(QMouseEvent *event){

	// > �������
	this->m_mousePressed = false;
}
/*-------------------------------------------------
		�����¼� - ����ƶ�
*/
void P_MatrixBlockSelector::event_mouseMoveEvent(QMouseEvent *event){
	if (this->m_mousePressed == false){ return; }
	QPointF widget_point = event->pos();
	QPointF scene_point = this->m_view->mapToScene(widget_point.rx(), widget_point.ry());

	// > ��ת��
	int xx = qFloor(scene_point.rx() / this->m_blockWidth);
	int yy = qFloor(scene_point.ry() / this->m_blockHeight);
	QPoint p = QPoint(xx, yy);

	// > ˢ�·��鼯
	this->setBlock(p, this->m_cur_mouseState);
}
