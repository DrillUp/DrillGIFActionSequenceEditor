#include "stdafx.h"
#include "I_PPaView.h"

/*
-----==========================================================-----
		类：		切割帧选择器 视图.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		连接控件与场景的中间类，控制滚动条与缩放功能。
					
		子功能：	->场景初始化（场景功能）
					->动画帧（场景功能）
					->鼠标缩放控制器
					->方块阵列点选器

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
		初始化
*/
void I_PPaView::init(){

	// > 滚动条
	connect(this->horizontalScrollBar(), &QScrollBar::valueChanged, this, &I_PPaView::scrollValueChanged);
	connect(this->verticalScrollBar(), &QScrollBar::valueChanged, this, &I_PPaView::scrollValueChanged);

	// > 场景建立
	this->m_scene = new I_PPaScene();
	this->setScene(m_scene);

	// > 鼠标缩放控制器
	this->m_p_MouseResizeController = new P_MRe_Controller();
	this->m_p_MouseResizeController->initGraphicsView(this);

	// > 方块点选器
	this->m_p_MatrixBlockSelector = new P_MBS_Selector();
	this->m_p_MatrixBlockSelector->initBlockColor(QList<QColor>() << QColor(255, 255, 255, 0) << QColor(0, 0, 0, 120) );
	this->m_p_MatrixBlockSelector->initBlockSvgIcon(
		QList<QFileInfo>() << QFileInfo("") << QFileInfo(QRC_IconSrcPath + "/view/Icon_Selected.svg")
	);
	this->m_p_MatrixBlockSelector->initGraphicsView(this);
	connect(this->m_p_MatrixBlockSelector, &P_MBS_Selector::newBlockAdded, this, &I_PPaView::blockAdded);
	connect(this->m_p_MatrixBlockSelector, &P_MBS_Selector::oldBlockClearBefore, this, &I_PPaView::blockClear);
}


/*-------------------------------------------------
		部件 - 获取场景
*/
I_PPaScene* I_PPaView::getScene(){
	return this->m_scene;
}
/*-------------------------------------------------
		部件 - 获取鼠标缩放控制器
*/
P_MRe_Controller* I_PPaView::getMouseResizeController(){
	return this->m_p_MouseResizeController;
}
/*-------------------------------------------------
		部件 - 方块阵列点选器
*/
P_MBS_Selector* I_PPaView::getMatrixBlockSelector(){
	return this->m_p_MatrixBlockSelector;
}


/*-------------------------------------------------
		点选器 - 获取切割都图片
*/
QList<QPixmap> I_PPaView::getCutBitmap(){
	QList<QPixmap> result_list = QList<QPixmap>();
	QPixmap src_bitmap = this->m_scene->m_bitmap;
	QList<QPoint> p_list = this->m_p_MatrixBlockSelector->getPointList(1);	//获取勾选的状态位
	for (int i = 0; i < p_list.count(); i++){
		QPoint p = p_list.at(i);
		int xx = qFloor(p.x()*this->m_block_width);		//（计算时用小数，计算后全部取整）
		int yy = qFloor(p.y()*this->m_block_height);
		int ww = qFloor(this->m_block_width);
		int hh = qFloor(this->m_block_height);
		QPixmap bitmap = src_bitmap.copy(xx,yy,ww,hh);
		result_list.append(bitmap);
	}
	return result_list;
}
/*-------------------------------------------------
		部件 - 方块添加
*/
void I_PPaView::blockAdded(I_MBS_Block* block, QGraphicsSvgItem* item){
	this->scene()->addItem(block);
	this->scene()->addItem(item);

	// > 计算勾选标记缩放
	double size = 24;
	if (this->m_block_height > this->m_block_width){
		size = this->m_block_width /24 / 2;	//（24是图标的大小）
	}else{
		size = this->m_block_height / 24 / 2;	//（24是图标的大小）
	}
	item->setScale(size);
}
/*-------------------------------------------------
		部件 - 方块清除
*/
void I_PPaView::blockClear(QList<I_MBS_Block*> block_list, QList<QGraphicsSvgItem*> item_list){
	for (int i = 0; i < block_list.count(); i++ ){
		this->scene()->removeItem(block_list.at(i));
	}
	for (int i = 0; i < item_list.count(); i++){
		this->scene()->removeItem(item_list.at(i));
	}
}



/*-------------------------------------------------
		滚动条 - 滚动条滚动
*/
void I_PPaView::scrollValueChanged(){
	this->update();		//场景重刷
}


/*-------------------------------------------------
		监听事件 - 鼠标按下
*/
void I_PPaView::mousePressEvent(QMouseEvent *event){
	QGraphicsView::mousePressEvent(event);	//继承父类函数

	// > 鼠标缩放控制 事件
	this->m_p_MouseResizeController->event_mousePressEvent(event);

	// > 方块点选器 事件
	this->m_p_MatrixBlockSelector->event_mousePressEvent(event);
}
/*-------------------------------------------------
		监听事件 - 鼠标抬起
*/
void I_PPaView::mouseReleaseEvent(QMouseEvent *event){
	QGraphicsView::mouseReleaseEvent(event);	//继承父类函数

	// > 鼠标缩放控制 事件
	this->m_p_MouseResizeController->event_mouseReleaseEvent(event);

	// > 方块点选器 事件
	this->m_p_MatrixBlockSelector->event_mouseReleaseEvent(event);
}
/*-------------------------------------------------
		监听事件 - 鼠标移动
*/
void I_PPaView::mouseMoveEvent(QMouseEvent *event){
	QGraphicsView::mouseMoveEvent(event);	//继承父类函数

	// > 鼠标缩放控制 事件
	this->m_p_MouseResizeController->event_mouseMoveEvent(event);

	// > 方块点选器 事件
	this->m_p_MatrixBlockSelector->event_mouseMoveEvent(event);
}
/*-------------------------------------------------
		监听事件 - 鼠标滚轮
*/
void I_PPaView::wheelEvent(QWheelEvent *event){

	// > 鼠标缩放控制 事件
	bool blocked = this->m_p_MouseResizeController->event_wheelEvent(event);
	if ( blocked == true){ return; }

	QGraphicsView::wheelEvent(event);
}