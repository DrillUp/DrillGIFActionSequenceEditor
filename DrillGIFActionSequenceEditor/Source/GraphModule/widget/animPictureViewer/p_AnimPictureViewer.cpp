#include "stdafx.h"
#include "p_AnimPictureViewer.h"

#include "private/i_APVView.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		ͼƬ�鿴�� ��װ��.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		ͨ��new���Զ���һ��QWidget�����ٳ� ͼƬ�鿴�� ��QGraphView��
					
		ע�⣺		1.ֻ��һ��View�Ĺ�����װ�壬�ṩ��View��ͼֱ�ӽ����Ľӿڡ�
					  ��Ҫ������View��

-----==========================================================-----
*/

P_AnimPictureViewer::P_AnimPictureViewer(QWidget* _parent)
{
	this->initWidgetAndLayout(_parent);			//���ؼ�
	this->m_GraphView = nullptr;				//��ͼ
}
P_AnimPictureViewer::~P_AnimPictureViewer(){
}

/*-------------------------------------------------
		���ؼ���ʼ��
*/
void P_AnimPictureViewer::initWidgetAndLayout(QWidget* _parent){
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
void P_AnimPictureViewer::rebuildUI(){

	// > ����ɾ��
	this->clearUI();

	// > �½�
	this->m_GraphView = new I_APVView(this->m_parent);
	this->m_GraphView->show();
	this->m_layout->addWidget(this->m_GraphView);
	connect(this->m_GraphView, &I_APVView::scaleChanged, this, &P_AnimPictureViewer::scaleChanged_view);

}
/*-------------------------------------------------
		�ؼ� - ����UI
*/
void P_AnimPictureViewer::clearUI(){

	// > ȥ����ͼ
	if (this->m_GraphView != nullptr){
		m_layout->removeWidget(this->m_GraphView);
		this->m_GraphView->setParent(nullptr);
		this->m_GraphView->deleteLater();
		this->m_GraphView = nullptr;
	}
}


/*-------------------------------------------------
		���� - ��С
*/
void P_AnimPictureViewer::zoomIn(){
	this->m_GraphView->zoomIn();
}
/*-------------------------------------------------
		���� - �Ŵ�
*/
void P_AnimPictureViewer::zoomOut(){
	this->m_GraphView->zoomOut();
}
/*-------------------------------------------------
		���� - ��С����
*/
void P_AnimPictureViewer::zoomReset(){
	this->m_GraphView->zoomReset();
}
/*-------------------------------------------------
		���� - ��ȡ����ֵ
*/
double P_AnimPictureViewer::getScale(){
	return this->m_GraphView->getScale();
}
/*-------------------------------------------------
		���� - ���ù����������η�
*/
void P_AnimPictureViewer::setScaleWheelModifier(QString charModifier){
	this->m_GraphView->setScaleWheelModifier(charModifier);
}
/*-------------------------------------------------
		���� - ����ֵ�ı䣨��view������
*/
void P_AnimPictureViewer::scaleChanged_view(double scale){
	emit scaleChanged(scale);
}


/*-------------------------------------------------
		����֡ - ���� ͼƬ��Դ
*/
void P_AnimPictureViewer::setSource(QList<QFileInfo> file_list){
	this->m_GraphView->getScene()->setSource(file_list);
}
/*-------------------------------------------------
		����֡ - ��ȡ ͼƬ��Դ
*/
QList<QFileInfo> P_AnimPictureViewer::getSource(){
	return this->m_GraphView->getScene()->getSource();
}
/*-------------------------------------------------
		����֡ - �����Դ
*/
void P_AnimPictureViewer::clearSource(){
	return this->m_GraphView->getScene()->clearSource();
}
/*-------------------------------------------------
		����֡ - �л�֡������������
*/
void P_AnimPictureViewer::setAnimFrame(int index){
	this->m_GraphView->getScene()->setAnimFrame(index);
}
/*-------------------------------------------------
		����֡ - �л�֡��������Դ���ƣ�
*/
void P_AnimPictureViewer::setAnimFile(QFileInfo file){
	this->m_GraphView->getScene()->setAnimName(file);
}
