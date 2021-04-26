#include "stdafx.h"
#include "P_PPaViewer.h"

#include "i_PPaView.h"
#include "Source/Utils/common/TTool.h"


/*
-----==========================================================-----
		类：		切割帧选择器 组装体.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		通过new，自动将一个QWidget，开辟成 切割帧选择器 的QGraphView。
					
		注意：		1.只是一个View的过渡组装体，提供与View视图直接交互的接口。
					  主要本体是View。

-----==========================================================-----
*/

P_PPaViewer::P_PPaViewer(QWidget* _parent)
{
	this->initWidgetAndLayout(_parent);			//父控件
	this->m_GraphView = nullptr;				//视图
}
P_PPaViewer::~P_PPaViewer(){
}

/*-------------------------------------------------
		父控件初始化
*/
void P_PPaViewer::initWidgetAndLayout(QWidget* _parent){
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
void P_PPaViewer::rebuildUI(){

	// > 内容删除
	this->clearUI();

	// > 新建
	this->m_GraphView = new I_PPaView(this->m_parent);
	this->m_GraphView->show();
	this->m_layout->addWidget(this->m_GraphView);
	connect(this->m_GraphView->getMouseResizeController(), &P_MouseResizeController::scaleChanged, this, &P_PPaViewer::scaleChanged_view);

}
/*-------------------------------------------------
		控件 - 清理UI
*/
void P_PPaViewer::clearUI(){

	// > 去掉视图
	if (this->m_GraphView != nullptr){
		m_layout->removeWidget(this->m_GraphView);
		this->m_GraphView->setParent(nullptr);
		this->m_GraphView->deleteLater();
		this->m_GraphView = nullptr;
	}
}


/*-------------------------------------------------
		图片 - 设置 图片资源
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
		辅助 - 设置网格线和方块划分
*/
void P_PPaViewer::setGridLineAndBlockMatrix(int column, int row){
	this->m_GraphView->getScene()->setGridLine(column,row);
	
	// > 方块阵列初始化（不规则小数显示，显示的实际情况自动 qFloor）
	double ww = this->m_GraphView->getScene()->width() / column;
	double hh = this->m_GraphView->getScene()->height() / row;
	this->m_GraphView->m_block_width = ww;
	this->m_GraphView->m_block_height = hh;
	this->m_GraphView->getMatrixBlockSelector()->rebuildBlock_double(ww,hh);

	// > 填充
	this->m_GraphView->getMatrixBlockSelector()->fillAll(1);
}
/*-------------------------------------------------
		辅助 - 清空网格线
*/
void P_PPaViewer::clearGridLine(){
	this->m_GraphView->getScene()->clearGridLine();
}
/*-------------------------------------------------
		辅助 - 获取切割的图片
*/
QList<QPixmap> P_PPaViewer::getCutBitmap(){
	return this->m_GraphView->getCutBitmap();
}


/*-------------------------------------------------
		缩放 - 获取控制器
*/
P_MouseResizeController* P_PPaViewer::getResizeController(){
	return this->m_GraphView->getMouseResizeController();
}
/*-------------------------------------------------
		缩放 - 缩小
*/
void P_PPaViewer::zoomIn(){
	this->m_GraphView->getMouseResizeController()->zoomIn();
}
/*-------------------------------------------------
		缩放 - 放大
*/
void P_PPaViewer::zoomOut(){
	this->m_GraphView->getMouseResizeController()->zoomOut();
}
/*-------------------------------------------------
		缩放 - 大小重置
*/
void P_PPaViewer::zoomReset(){
	this->m_GraphView->getMouseResizeController()->zoomReset();
}
/*-------------------------------------------------
		缩放 - 获取缩放值
*/
double P_PPaViewer::getScale(){
	return this->m_GraphView->getMouseResizeController()->getScale();
}
/*-------------------------------------------------
		缩放 - 设置滚轮缩放修饰符
*/
void P_PPaViewer::setScaleWheelModifier(QString charModifier){
	this->m_GraphView->getMouseResizeController()->setScaleWheelModifier(charModifier);
}
/*-------------------------------------------------
		缩放 - 缩放值改变（与view交互）
*/
void P_PPaViewer::scaleChanged_view(double scale){
	emit scaleChanged(scale);
}
