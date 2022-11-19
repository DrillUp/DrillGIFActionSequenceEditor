#include "stdafx.h"
#include "P_MBS_Selector.h"

#include "Source/Utils/Common/TTool.h"


/*
-----==========================================================-----
		类：		方块阵列点选器.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		该控制器用于装饰 QGraphicsView ，使其能够支持 方块阵列勾选切换 + 颜色块切换/矢量图切换 功能。
					
		使用方法：	
				> 初始化
					this->m_p_MBS_Controller = new P_MBS_Controller();
					this->m_p_MBS_Controller->setBlockSize(ww, hh);
					this->m_p_MBS_Controller->initBlockColor(QList<QColor>() << QColor(255, 255, 255, 0) << QColor(0, 0, 255, 50));
					this->m_p_MBS_Controller->setGraphicsView(this);
					connect(this->m_p_MBS_Controller, &P_MBS_Controller::newBlockAdded, this, &I_FixedAreaView::blockAdded);
					connect(this->m_p_MBS_Controller, &P_MBS_Controller::oldBlockClearBefore, this, &I_FixedAreaView::clearBlockBefore);
					//（接下来所有方块勾选相关控制，用这个类去执行）
			
				> 事件必要接口
					//（详细可以见 i_SPVView 的调用）	

				> 外部信号
					connect(this->m_p_MBS_Controller, &P_MBS_Controller::scaleChanged, this, &I_SPVView::scaleChanged_view);

-----==========================================================-----
*/
P_MBS_Selector::P_MBS_Selector(){
	this->m_blockWidth = 24;
	this->m_blockHeight = 24;
	this->m_blockColorTank = QList<QColor>() << QColor(255, 255, 255, 0) << QColor(0, 0, 255, 50);
}
P_MBS_Selector::~P_MBS_Selector(){
}

/*-------------------------------------------------
		初始化 - 设置方块大小
*/
void P_MBS_Selector::initBlockSize(int width, int height){
	this->m_blockWidth = width;
	this->m_blockHeight = height;
}
/*-------------------------------------------------
		初始化 - 设置方块颜色
*/
void P_MBS_Selector::initBlockColor(QList<QColor> color_list){
	this->m_blockColorTank = color_list;
}
/*-------------------------------------------------
		初始化 - 设置矢量图列表
*/
void P_MBS_Selector::initBlockSvgIcon(QList<QFileInfo> svg_file_list){
	for (int i = 0; i < svg_file_list.count(); i++){
		QFileInfo svg_file = svg_file_list.at(i);
		QSvgRenderer* render = new QSvgRenderer(svg_file.absoluteFilePath());
		this->m_rendererTank.append(render);
	}
}
/*-------------------------------------------------
		初始化 - 设置视图
*/
void P_MBS_Selector::initGraphicsView(QGraphicsView* view){
	this->m_view = view;

	// > 鼠标追踪开启
	this->m_view->setMouseTracking(true);
	this->m_view->setCursor(QCursor(Qt::PointingHandCursor));		//设置手形

	// > 选中方块 部件
	this->m_blockTank = QList<I_MBS_Block*>();
	this->m_cur_mouseState = 0;
	this->m_mousePressed = false;
}
/*-------------------------------------------------
		重置 - 重置方块
*/
void P_MBS_Selector::rebuildBlock(int width, int height){
	this->clearAllBlock();		//（清空方块）
	this->m_blockWidth = width;
	this->m_blockHeight = height;
}
void P_MBS_Selector::rebuildBlock_double(double width, double height){
	this->clearAllBlock();		//（清空方块）
	this->m_blockWidth = width;
	this->m_blockHeight = height;
}


/*-------------------------------------------------
		方块 - 获取全部方块
*/
QList<I_MBS_Block*> P_MBS_Selector::getAllBlock(){
	return this->m_blockTank;
}
/*-------------------------------------------------
		方块 - 获取方块矩阵（单位图块）
*/
QRect P_MBS_Selector::getBlockMatrix(){
	QRectF scene_rect = this->m_view->sceneRect();
		
	// > 能被整除的坐标矩阵值
	int left = qCeil(scene_rect.left() / this->m_blockWidth);
	int top = qCeil(scene_rect.top() / this->m_blockHeight);
	int right = qFloor(scene_rect.right() / this->m_blockWidth) -1;	//（最边沿的方块不算）
	int bottom = qFloor(scene_rect.bottom() / this->m_blockHeight) -1;
	return QRect(QPoint(left,top),QPoint(right,bottom));
}
/*-------------------------------------------------
		方块 - 清除全部方块、矢量标记
*/
void P_MBS_Selector::clearAllBlock(){
	emit oldBlockClearBefore(this->m_blockTank, this->m_svgTank);
	this->m_cur_mouseState = 0;
	this->m_blockTank.clear();
	this->m_svgTank.clear();
	this->m_coveredPointTank.clear();
}
/*-------------------------------------------------
		方块 - 添加方块
*/
void P_MBS_Selector::setBlock(QPoint matrix_pos, int state){

	// > 方块存在时
	int index = this->m_coveredPointTank.indexOf(matrix_pos);
	if (index != -1 ){
		this->m_blockTank.at(index)->setState(state);
		if (this->m_rendererTank.count() != 0){
			this->m_svgTank.at(index)->setSharedRenderer(this->m_rendererTank.at(state));
		}
		return;
	}

	// > 新建方块
	I_MBS_Block* item = new I_MBS_Block();
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
		填涂 - 获取指定状态位的全部点
*/
QList<QPoint> P_MBS_Selector::getPointList(int state){
	QList<QPoint> result_list = QList<QPoint>();

	// > 状态位 =0 时
	if (state == 0){

		// > 包括 空点
		QRect rect = this->getBlockMatrix();
		if ( this->m_blockTank.count() < rect.width()* rect.height() ){
			for (int y = rect.y(); y < rect.height(); y++){
				for (int x = rect.x(); x < rect.width(); x++){	//（先横向再纵向）
					QPoint pos = QPoint(x, y);
					if (this->m_coveredPointTank.contains(pos) == false){
						result_list.append(pos);
					}
				}
			}
		}

		// > 包括 状态位=0 的点
		for (int i = 0; i < this->m_blockTank.count(); i++){
			I_MBS_Block* item = this->m_blockTank.at(i);
			if (item->getState() == state){
				result_list.append(QPoint(item->getMatrixX(), item->getMatrixY()));
			}
		}

	// > 其他 状态位 时
	}else{
		for (int i = 0; i < this->m_blockTank.count(); i++){
			I_MBS_Block* item = this->m_blockTank.at(i);
			if (item->getState() == state){
				result_list.append(QPoint(item->getMatrixX(), item->getMatrixY()));
			}
		}
	}
	return result_list;
}
/*-------------------------------------------------
		填涂 - 填涂全部状态位
*/
void P_MBS_Selector::fillAll(int state){

	QRect rect = this->getBlockMatrix();
	for (int y = rect.y(); y < rect.height(); y++){
		for (int x = rect.x(); x < rect.width(); x++){	//（先横向再纵向）
			QPoint pos = QPoint(x, y);
			this->setBlock(pos, state);
		}
	}
}


/*-------------------------------------------------
		监听事件 - 鼠标按下
*/
void P_MBS_Selector::event_mousePressEvent(QMouseEvent *event){

	// > 左键按下标记
	this->m_mousePressed = true;
	QPointF widget_point = event->pos();
	QPointF scene_point = this->m_view->mapToScene(widget_point.rx(), widget_point.ry());

	// > 点转换
	int xx = qFloor(scene_point.rx() / this->m_blockWidth);
	int yy = qFloor(scene_point.ry() / this->m_blockHeight);
	QPoint p = QPoint(xx, yy);

	// > 填涂状态切换
	bool changed = false;
	for (int i = 0; i < this->m_blockTank.count(); i++){
		I_MBS_Block* item = this->m_blockTank.at(i);
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
	if (changed == false){		//无方块时，默认填充1状态
		this->m_cur_mouseState = 1;
	}

	// > 刷新方块集
	this->setBlock(p, this->m_cur_mouseState);
}
/*-------------------------------------------------
		监听事件 - 鼠标抬起
*/
void P_MBS_Selector::event_mouseReleaseEvent(QMouseEvent *event){

	// > 标记清理
	this->m_mousePressed = false;
}
/*-------------------------------------------------
		监听事件 - 鼠标移动
*/
void P_MBS_Selector::event_mouseMoveEvent(QMouseEvent *event){
	if (this->m_mousePressed == false){ return; }
	QPointF widget_point = event->pos();
	QPointF scene_point = this->m_view->mapToScene(widget_point.rx(), widget_point.ry());

	// > 点转换
	int xx = qFloor(scene_point.rx() / this->m_blockWidth);
	int yy = qFloor(scene_point.ry() / this->m_blockHeight);
	QPoint p = QPoint(xx, yy);

	// > 刷新方块集
	this->setBlock(p, this->m_cur_mouseState);
}
