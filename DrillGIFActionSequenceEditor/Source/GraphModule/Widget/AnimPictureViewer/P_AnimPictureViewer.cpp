#include "stdafx.h"
#include "P_AnimPictureViewer.h"

#include "Private/I_APVView.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		图片查看块-动图 组装体.cpp
		版本：		v1.05
		作者：		drill_up
		所属模块：	工具模块
		功能：		通过new，自动将一个QWidget，开辟成 图片查看块 的QGraphView。
					
		注意：		1.只是一个View的过渡组装体，提供与View视图直接交互的接口。
					  主要本体是View。

-----==========================================================-----
*/

P_AnimPictureViewer::P_AnimPictureViewer(QWidget* _parent)
{
	this->initWidgetAndLayout(_parent);			//父控件
	this->m_GraphView = nullptr;				//视图
}
P_AnimPictureViewer::~P_AnimPictureViewer(){
}

/*-------------------------------------------------
		父控件初始化
*/
void P_AnimPictureViewer::initWidgetAndLayout(QWidget* _parent){
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
void P_AnimPictureViewer::rebuildUI(){

	// > 内容删除
	this->clearUI();

	// > 新建
	this->m_GraphView = new I_APVView(this->m_parent);
	this->m_GraphView->show();
	this->m_layout->addWidget(this->m_GraphView);
	connect(this->m_GraphView->getMouseResizeController(), &P_MRe_Controller::scaleChanged, this, &P_AnimPictureViewer::scaleChanged_view);

}
/*-------------------------------------------------
		控件 - 清理UI
*/
void P_AnimPictureViewer::clearUI(){

	// > 去掉视图
	if (this->m_GraphView != nullptr){
		m_layout->removeWidget(this->m_GraphView);
		this->m_GraphView->setParent(nullptr);
		this->m_GraphView->deleteLater();
		this->m_GraphView = nullptr;
	}
}




/*-------------------------------------------------
		动画帧 - 设置 图片资源
*/
void P_AnimPictureViewer::setSource(QList<QFileInfo> file_list){
	this->m_GraphView->getScene()->setSource(file_list);
}
/*-------------------------------------------------
		动画帧 - 获取 图片资源
*/
QList<QFileInfo> P_AnimPictureViewer::getSource(){
	return this->m_GraphView->getScene()->getSource();
}
/*-------------------------------------------------
		动画帧 - 清除资源
*/
void P_AnimPictureViewer::clearSource(){
	return this->m_GraphView->getScene()->clearSource();
}
/*-------------------------------------------------
		动画帧 - 切换帧（根据索引）
*/
void P_AnimPictureViewer::setAnimFrame(int index){
	this->m_GraphView->getScene()->setAnimFrame(index);
}
/*-------------------------------------------------
		动画帧 - 切换帧（根据资源名称）
*/
void P_AnimPictureViewer::setAnimFile(QFileInfo file){
	this->m_GraphView->getScene()->setAnimName(file);
}



/*-------------------------------------------------
		辅助 - 设置网格线
*/
void P_AnimPictureViewer::setGridLine(int column, int row){
	this->m_GraphView->getScene()->setGridLine(column,row);
}
/*-------------------------------------------------
		辅助 - 清空网格线
*/
void P_AnimPictureViewer::clearGridLine(){
	this->m_GraphView->getScene()->clearGridLine();
}


/*-------------------------------------------------
		着色器 - 修改色调
*/
void P_AnimPictureViewer::setTint(int rotate_offset){
	this->m_GraphView->getScene()->setTint(rotate_offset);
}
/*-------------------------------------------------
		着色器 - 获取色调
*/
int P_AnimPictureViewer::getTint(){
	return this->m_GraphView->getScene()->getTint();
}


/*-------------------------------------------------
		缩放 - 获取控制器
*/
P_MRe_Controller* P_AnimPictureViewer::getResizeController(){
	return this->m_GraphView->getMouseResizeController();
}
/*-------------------------------------------------
		缩放 - 缩小
*/
void P_AnimPictureViewer::zoomIn(){
	this->m_GraphView->getMouseResizeController()->zoomIn();
}
/*-------------------------------------------------
		缩放 - 放大
*/
void P_AnimPictureViewer::zoomOut(){
	this->m_GraphView->getMouseResizeController()->zoomOut();
}
/*-------------------------------------------------
		缩放 - 大小重置
*/
void P_AnimPictureViewer::zoomReset(){
	this->m_GraphView->getMouseResizeController()->zoomReset();
}
/*-------------------------------------------------
		缩放 - 获取缩放值
*/
double P_AnimPictureViewer::getScale(){
	return this->m_GraphView->getMouseResizeController()->getScale();
}
/*-------------------------------------------------
		缩放 - 设置滚轮缩放修饰符
*/
void P_AnimPictureViewer::setScaleWheelModifier(QString charModifier){
	this->m_GraphView->getMouseResizeController()->setScaleWheelModifier(charModifier);
}
/*-------------------------------------------------
		缩放 - 缩放值改变（与view交互）
*/
void P_AnimPictureViewer::scaleChanged_view(double scale){
	emit scaleChanged(scale);
}