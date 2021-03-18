#include "stdafx.h"
#include "P_SinglePictureViewer.h"

#include "private/i_SPVView.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		图片查看块-单图 组装体.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		通过new，自动将一个QWidget，开辟成 图片查看块 的QGraphView。
					
		注意：		1.只是一个View的过渡组装体，提供与View视图直接交互的接口。
					  主要本体是View。

-----==========================================================-----
*/

P_SinglePictureViewer::P_SinglePictureViewer(QWidget* _parent)
{
	this->initWidgetAndLayout(_parent);			//父控件
	this->m_GraphView = nullptr;				//视图
}
P_SinglePictureViewer::~P_SinglePictureViewer(){
}

/*-------------------------------------------------
		父控件初始化
*/
void P_SinglePictureViewer::initWidgetAndLayout(QWidget* _parent){
	if (_parent->layout() == nullptr){
		this->m_layout = new QVBoxLayout(_parent);
		_parent->setLayout(this->m_layout);
	}
	QString layout_name = _parent->layout()->metaObject()->className();
	Q_ASSERT(layout_name == "QVBoxLayout");		//必须是QVBoxLayout
	this->m_parent = _parent;
	this->m_layout = qobject_cast<QVBoxLayout*>(_parent->layout());
	this->m_layout->setMargin(0);
}


/*-------------------------------------------------
		控件 - 重建控件
*/
void P_SinglePictureViewer::rebuildUI(){

	// > 内容删除
	this->clearUI();

	// > 新建
	this->m_GraphView = new I_SPVView(this->m_parent);
	this->m_GraphView->show();
	this->m_layout->addWidget(this->m_GraphView);
	connect(this->m_GraphView, &I_SPVView::scaleChanged, this, &P_SinglePictureViewer::scaleChanged_view);

}
/*-------------------------------------------------
		控件 - 清理UI
*/
void P_SinglePictureViewer::clearUI(){

	// > 去掉视图
	if (this->m_GraphView != nullptr){
		m_layout->removeWidget(this->m_GraphView);
		this->m_GraphView->setParent(nullptr);
		this->m_GraphView->deleteLater();
		this->m_GraphView = nullptr;
	}
}


/*-------------------------------------------------
		缩放 - 缩小
*/
void P_SinglePictureViewer::zoomIn(){
	this->m_GraphView->zoomIn();
}
/*-------------------------------------------------
		缩放 - 放大
*/
void P_SinglePictureViewer::zoomOut(){
	this->m_GraphView->zoomOut();
}
/*-------------------------------------------------
		缩放 - 大小重置
*/
void P_SinglePictureViewer::zoomReset(){
	this->m_GraphView->zoomReset();
}
/*-------------------------------------------------
		缩放 - 获取缩放值
*/
double P_SinglePictureViewer::getScale(){
	return this->m_GraphView->getScale();
}
/*-------------------------------------------------
		缩放 - 设置滚轮缩放修饰符
*/
void P_SinglePictureViewer::setScaleWheelModifier(QString charModifier){
	this->m_GraphView->setScaleWheelModifier(charModifier);
}
/*-------------------------------------------------
		缩放 - 缩放值改变（与view交互）
*/
void P_SinglePictureViewer::scaleChanged_view(double scale){
	emit scaleChanged(scale);
}


/*-------------------------------------------------
		图片 - 设置 图片资源
*/
void P_SinglePictureViewer::setSource(QPixmap pixmap){
	this->m_GraphView->getScene()->setSource(pixmap);
}


/*-------------------------------------------------
		辅助 - 设置网格线
*/
void P_SinglePictureViewer::setGridLine(int column, int row){
	this->m_GraphView->getScene()->setGridLine(column,row);
}
/*-------------------------------------------------
		辅助 - 清空网格线
*/
void P_SinglePictureViewer::clearGridLine(){
	this->m_GraphView->getScene()->clearGridLine();
}