#include "stdafx.h"
#include "P_PPaViewer.h"

#include "i_PPaView.h"
#include "Source/Utils/common/TTool.h"


/*
-----==========================================================-----
		�ࣺ		�и�֡ѡ���� ��װ��.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		ͨ��new���Զ���һ��QWidget�����ٳ� �и�֡ѡ���� ��QGraphView��
					
		ע�⣺		1.ֻ��һ��View�Ĺ�����װ�壬�ṩ��View��ͼֱ�ӽ����Ľӿڡ�
					  ��Ҫ������View��

-----==========================================================-----
*/

P_PPaViewer::P_PPaViewer(QWidget* _parent)
{
	this->initWidgetAndLayout(_parent);			//���ؼ�
	this->m_GraphView = nullptr;				//��ͼ
}
P_PPaViewer::~P_PPaViewer(){
}

/*-------------------------------------------------
		���ؼ���ʼ��
*/
void P_PPaViewer::initWidgetAndLayout(QWidget* _parent){
	if (_parent->layout() == nullptr){
		this->m_layout = new QVBoxLayout(_parent);
		_parent->setLayout(this->m_layout);
	}
	QString layout_name = _parent->layout()->metaObject()->className();
	Q_ASSERT(layout_name == "QVBoxLayout");		//������QVBoxLayout
	this->m_parent = _parent;
	this->m_layout = qobject_cast<QVBoxLayout*>(_parent->layout());
	this->m_layout->setMargin(0);
}


/*-------------------------------------------------
		�ؼ� - �ؽ��ؼ�
*/
void P_PPaViewer::rebuildUI(){

	// > ����ɾ��
	this->clearUI();

	// > �½�
	this->m_GraphView = new I_PPaView(this->m_parent);
	this->m_GraphView->show();
	this->m_layout->addWidget(this->m_GraphView);
	connect(this->m_GraphView->getMouseResizeController(), &P_MouseResizeController::scaleChanged, this, &P_PPaViewer::scaleChanged_view);

}
/*-------------------------------------------------
		�ؼ� - ����UI
*/
void P_PPaViewer::clearUI(){

	// > ȥ����ͼ
	if (this->m_GraphView != nullptr){
		m_layout->removeWidget(this->m_GraphView);
		this->m_GraphView->setParent(nullptr);
		this->m_GraphView->deleteLater();
		this->m_GraphView = nullptr;
	}
}


/*-------------------------------------------------
		ͼƬ - ���� ͼƬ��Դ
*/
void P_PPaViewer::setSource(QFileInfo file){
	QImage image = QImage(file.absoluteFilePath());
	QPixmap pixmap = QPixmap::fromImage(image);
	this->m_GraphView->getScene()->setSource(pixmap);
}
void P_PPaViewer::setSource(QPixmap pixmap){
	this->m_GraphView->getScene()->setSource(pixmap);
}


/*-------------------------------------------------
		���� - ���������ߺͷ��黮��
*/
void P_PPaViewer::setGridLineAndBlockMatrix(int column, int row){
	this->m_GraphView->getScene()->setGridLine(column,row);
	
	// > �������г�ʼ����������С����ʾ����ʾ��ʵ������Զ� qFloor��
	double ww = this->m_GraphView->getScene()->width() / column;
	double hh = this->m_GraphView->getScene()->height() / row;
	this->m_GraphView->m_block_width = ww;
	this->m_GraphView->m_block_height = hh;
	this->m_GraphView->getMatrixBlockSelector()->rebuildBlock_double(ww,hh);

	// > ���
	this->m_GraphView->getMatrixBlockSelector()->fillAll(1);
}
/*-------------------------------------------------
		���� - ���������
*/
void P_PPaViewer::clearGridLine(){
	this->m_GraphView->getScene()->clearGridLine();
}
/*-------------------------------------------------
		���� - ��ȡ�и��ͼƬ
*/
QList<QPixmap> P_PPaViewer::getCutBitmap(){
	return this->m_GraphView->getCutBitmap();
}


/*-------------------------------------------------
		���� - ��С
*/
void P_PPaViewer::zoomIn(){
	this->m_GraphView->getMouseResizeController()->zoomIn();
}
/*-------------------------------------------------
		���� - �Ŵ�
*/
void P_PPaViewer::zoomOut(){
	this->m_GraphView->getMouseResizeController()->zoomOut();
}
/*-------------------------------------------------
		���� - ��С����
*/
void P_PPaViewer::zoomReset(){
	this->m_GraphView->getMouseResizeController()->zoomReset();
}
/*-------------------------------------------------
		���� - ��ȡ����ֵ
*/
double P_PPaViewer::getScale(){
	return this->m_GraphView->getMouseResizeController()->getScale();
}
/*-------------------------------------------------
		���� - ���ù����������η�
*/
void P_PPaViewer::setScaleWheelModifier(QString charModifier){
	this->m_GraphView->getMouseResizeController()->setScaleWheelModifier(charModifier);
}
/*-------------------------------------------------
		���� - ����ֵ�ı䣨��view������
*/
void P_PPaViewer::scaleChanged_view(double scale){
	emit scaleChanged(scale);
}
