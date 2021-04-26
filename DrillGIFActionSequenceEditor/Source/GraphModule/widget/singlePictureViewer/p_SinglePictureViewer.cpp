#include "stdafx.h"
#include "P_SinglePictureViewer.h"

#include "private/i_SPVView.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴��-��ͼ ��װ��.cpp
		�汾��		v1.01
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		ͨ��new���Զ���һ��QWidget�����ٳ� ͼƬ�鿴�� ��QGraphView��
					
		ע�⣺		1.ֻ��һ��View�Ĺ�����װ�壬�ṩ��View��ͼֱ�ӽ����Ľӿڡ�
					  ��Ҫ������View��

-----==========================================================-----
*/

P_SinglePictureViewer::P_SinglePictureViewer(QWidget* _parent)
{
	this->initWidgetAndLayout(_parent);			//���ؼ�
	this->m_GraphView = nullptr;				//��ͼ
}
P_SinglePictureViewer::~P_SinglePictureViewer(){
}

/*-------------------------------------------------
		���ؼ���ʼ��
*/
void P_SinglePictureViewer::initWidgetAndLayout(QWidget* _parent){
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
void P_SinglePictureViewer::rebuildUI(){

	// > ����ɾ��
	this->clearUI();

	// > �½�
	this->m_GraphView = new I_SPVView(this->m_parent);
	this->m_GraphView->show();
	this->m_layout->addWidget(this->m_GraphView);
	connect(this->m_GraphView->getMouseResizeController(), &P_MouseResizeController::scaleChanged, this, &P_SinglePictureViewer::scaleChanged_view);

}
/*-------------------------------------------------
		�ؼ� - ����UI
*/
void P_SinglePictureViewer::clearUI(){

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
void P_SinglePictureViewer::setSource(QFileInfo file){
	QImage image = QImage(file.absoluteFilePath());
	QPixmap pixmap = QPixmap::fromImage(image);
	this->m_GraphView->getScene()->setSource(pixmap);
}
void P_SinglePictureViewer::setSource(QPixmap pixmap){
	this->m_GraphView->getScene()->setSource(pixmap);
}


/*-------------------------------------------------
		���� - ����������
*/
void P_SinglePictureViewer::setGridLine(int column, int row){
	this->m_GraphView->getScene()->setGridLine(column,row);
}
/*-------------------------------------------------
		���� - ���������
*/
void P_SinglePictureViewer::clearGridLine(){
	this->m_GraphView->getScene()->clearGridLine();
}


/*-------------------------------------------------
		���� - ��ȡ������
*/
P_MouseResizeController* P_SinglePictureViewer::getResizeController(){
	return this->m_GraphView->getMouseResizeController();
}
/*-------------------------------------------------
		���� - ��С
*/
void P_SinglePictureViewer::zoomIn(){
	this->m_GraphView->getMouseResizeController()->zoomIn();
}
/*-------------------------------------------------
		���� - �Ŵ�
*/
void P_SinglePictureViewer::zoomOut(){
	this->m_GraphView->getMouseResizeController()->zoomOut();
}
/*-------------------------------------------------
		���� - ��С����
*/
void P_SinglePictureViewer::zoomReset(){
	this->m_GraphView->getMouseResizeController()->zoomReset();
}
/*-------------------------------------------------
		���� - ��ȡ����ֵ
*/
double P_SinglePictureViewer::getScale(){
	return this->m_GraphView->getMouseResizeController()->getScale();
}
/*-------------------------------------------------
		���� - ���ù����������η�
*/
void P_SinglePictureViewer::setScaleWheelModifier(QString charModifier){
	this->m_GraphView->getMouseResizeController()->setScaleWheelModifier(charModifier);
}
/*-------------------------------------------------
		���� - ����ֵ�ı䣨��view������
*/
void P_SinglePictureViewer::scaleChanged_view(double scale){
	emit scaleChanged(scale);
}
