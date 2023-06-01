#include "stdafx.h"
#include "I_APVScene.h"

#include <QPixmap>

/*
-----==========================================================-----
		类：		图片查看块-动图 场景.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		查看变化动图的画布。

		子功能：	->场景初始化
						->宽度、高度、出血线
						->颜色
						->背景
					->动画帧
						->设置图片资源
						->切换帧
					->缩放（视图功能）
					->鼠标拖移（视图功能）

-----==========================================================-----
*/
I_APVScene::I_APVScene(QWidget *parent)
	: QGraphicsScene(parent)
{
	this->init();
}
I_APVScene::~I_APVScene(){
}

/*-------------------------------------------------
		初始化
*/
void I_APVScene::init(){

	// > 属性（默认）
	this->m_canvasWidth = this->getMaxWidth();
	this->m_canvasHeight = this->getMaxHeight();
	this->m_canvasThickness = 40;
	this->m_pixelWidth = 24;
	this->m_pixelHeight = 24;

	// > 辅助对象
	this->m_gridLineColor = QColor(255, 0, 0);
	this->m_backgroundColor = QColor(255, 255, 255);
	this->m_P_GridLineItem = new P_GridLineItem(this);
	this->m_maskBackground = nullptr;

	// > 部件
	this->m_animList.clear();
	this->m_curFrame = -1;
	this->m_curTint = 0;

	// > 场景初始化
	this->m_rebuildBlock = false;
	int ww = this->m_pixelWidth;
	int hh = this->m_pixelHeight;
	this->setSceneRect(-1 * this->m_canvasThickness, -1 * this->m_canvasThickness, this->m_canvasWidth + this->m_canvasThickness * 2, this->m_canvasHeight + this->m_canvasThickness * 2);
	this->setBackgroundBrush(QBrush(QColor(255, 255, 255, 255), Qt::BrushStyle::SolidPattern));
	this->refreshBackground();

}
/*-------------------------------------------------
		辅助 - 设置网格线
*/
void I_APVScene::setGridLine(int column, int row){

	// > 建立网格线
	this->m_P_GridLineItem->rebuildGrid(this->m_canvasWidth, this->m_canvasHeight, column, row, this->m_gridLineColor);
	
	// > 添加
	this->m_P_GridLineItem->addItemsToScene();
}
/*-------------------------------------------------
		辅助 - 清空网格线
*/
void I_APVScene::clearGridLine(){
	this->m_P_GridLineItem->clearAllItem();
}
/*-------------------------------------------------
		辅助 - 刷新背景
*/
void I_APVScene::refreshBackground(){
	if (this->m_maskBackground == nullptr){
		this->m_maskBackground = new I_MaskBackgroundItem();
		this->addItem(this->m_maskBackground);
	}
	this->m_maskBackground->setBackground_oneColor(this->m_canvasWidth, this->m_canvasHeight, this->m_pixelWidth, this->m_pixelHeight, this->m_backgroundColor);
	this->m_maskBackground->setPos(0, 0);
	this->m_maskBackground->setZValue(-10000);
}

/*-------------------------------------------------
		监听事件 - 鼠标按下
*/
void I_APVScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mousePressEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标抬起
*/
void I_APVScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标移动
*/
void I_APVScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标双击
*/
void I_APVScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	//（无操作函数）
}



/*-------------------------------------------------
		资源 - 设置资源
*/
void I_APVScene::setSource(QList<QFileInfo> file_list){
	QList<QPixmap> bitmap_list;
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo info = file_list.at(i);
		QImage image = QImage(info.absoluteFilePath());
		QPixmap pixmap = QPixmap::fromImage(image);
		bitmap_list.append(pixmap);
	}
	this->m_fileList = file_list;
	this->m_bitmapList = bitmap_list;
	this->rebuildScene();
}
/*-------------------------------------------------
		资源 - 设置资源
*/
QList<QFileInfo> I_APVScene::getSource(){
	return this->m_fileList;
}
/*-------------------------------------------------
		资源 - 清除资源
*/
void I_APVScene::clearSource(){
	this->m_fileList.clear();
	this->m_bitmapList.clear();
	this->rebuildScene();
}
/*-------------------------------------------------
		资源 - 添加资源（私有）
*/
void I_APVScene::addSource(QFileInfo file){
	this->m_fileList.append(file);
	QImage image = QImage(file.absoluteFilePath());
	QPixmap pixmap = QPixmap::fromImage(image);
	this->m_bitmapList.append(pixmap);
	this->rebuildScene();		//（重建）
}
/*-------------------------------------------------
		资源 - 重建场景
*/
void I_APVScene::rebuildScene(){
	if (this->m_rebuildBlock == true){ return; }
	this->m_rebuildBlock = true;

	// > 去除旧图片
	for (int i = 0; i < this->m_animList.count(); i++){
		this->removeItem(this->m_animList.at(i));
	}
	this->m_animList.clear();

	// > 高宽适配
	this->m_canvasWidth = this->getMaxWidth();
	this->m_canvasHeight = this->getMaxHeight();
	this->setSceneRect(-1 * this->m_canvasThickness, -1 * this->m_canvasThickness, this->m_canvasWidth + this->m_canvasThickness * 2, this->m_canvasHeight + this->m_canvasThickness * 2);
	this->refreshBackground();

	// > 放置图片
	for (int i = 0; i < this->m_bitmapList.count(); i++){
		QPixmap bitmap = this->m_bitmapList.at(i);
		QGraphicsPixmapItem* item = new QGraphicsPixmapItem();
		QPixmap result_pixmap = this->rotateColor(bitmap, this->m_curTint);

		double xx = (this->m_canvasWidth - result_pixmap.width())*0.5;
		double yy = (this->m_canvasHeight - result_pixmap.height())*0.5;
		item->setPixmap(result_pixmap);
		item->setPos(xx, yy);
		this->addItem(item);

		this->m_animList.append(item);
	}
	if (this->m_animList.count() > 0 && this->m_curFrame == -1){
		this->m_curFrame = 0;
	}

	// > 刷新帧
	this->refreshFrame();
	this->m_rebuildBlock = false;
}


/*-------------------------------------------------
		着色器 - 修改色调
*/
void I_APVScene::setTint(int rotate_offset){
	if (this->m_rebuildBlock == true){ return; }
	if (this->m_curTint == rotate_offset){ return; }
	this->m_curTint = rotate_offset;
	this->rebuildScene();
}
/*-------------------------------------------------
		着色器 - 获取色调
*/
int I_APVScene::getTint(){
	return this->m_curTint;
}
/*-------------------------------------------------
		着色器 - 旋转色彩
*/
QPixmap I_APVScene::rotateColor(QPixmap pixmap, int rotate_offset){
	if (pixmap.isNull()){ return pixmap; }
	if (rotate_offset == 0){ return pixmap; }
	rotate_offset = ((rotate_offset % 360) + 360) % 360;

	// > 格式转换（固定为rgba）
	QImage img = pixmap.toImage();
	if (img.format() != QImage::Format_RGBA8888){
		img = img.convertToFormat(QImage::Format_RGBA8888);
	}

	// > 图片颜色矩阵转换
	//		参考：https://blog.csdn.net/qq_43081702/article/details/110656227 
	uint8_t* rgb = img.bits();
	double* temp_arr = new double[3];
	int size = img.width()*img.height() * 4;
	for (int i = 0; i < size; i += 4){
		this->rgbToHsl(rgb[i + 0], rgb[i + 1], rgb[i + 2], temp_arr);
		double h = fmod((temp_arr[0] + rotate_offset), 360);
		double s = temp_arr[1];
		double l = temp_arr[2];
		this->hslToRgb(h, s, l, temp_arr);
		rgb[i + 0] = temp_arr[0];
		rgb[i + 1] = temp_arr[1];
		rgb[i + 2] = temp_arr[2];
	}
	delete temp_arr;
	//（原函数：Bitmap.prototype.rotateHue）
	//var context = this._context;
	//var imageData = context.getImageData(0, 0, this.width, this.height);
	//var pixels = imageData.data;
	//for (var i = 0; i < pixels.length; i += 4){
	//	var hsl = rgbToHsl(pixels[i + 0], pixels[i + 1], pixels[i + 2]);
	//	var h = (hsl[0] + offset) % 360;
	//	var s = hsl[1];
	//	var l = hsl[2];
	//	var rgb = hslToRgb(h, s, l);
	//	pixels[i + 0] = rgb[0];
	//	pixels[i + 1] = rgb[1];
	//	pixels[i + 2] = rgb[2];
	//}
	//context.putImageData(imageData, 0, 0);

	// > 放置图片
	QPixmap result_pixmap = QPixmap::fromImage(img);
	return result_pixmap;
}
/*-------------------------------------------------
		着色器 - rgb模式转hsl模式
*/
void I_APVScene::rgbToHsl(int r, int g, int b, double* result_arr){
	double cmin = qMin(r, qMin(g, b));
	double cmax = qMax(r, qMax(g, b));
	double h = 0;
	double s = 0;
	double l = (cmin + cmax) * 0.5;
	double delta = cmax - cmin;

	if (delta > 0){
		if (r == cmax){
			h = 60 * fmod(((g - b) / delta + 6), 6);
		}
		else if (g == cmax){
			h = 60 * ((b - r) / delta + 2);
		}
		else{
			h = 60 * ((r - g) / delta + 4);
		}
		s = delta / (255 - qAbs(2 * l - 255));
	}
	result_arr[0] = h;
	result_arr[1] = s;
	result_arr[2] = l;
}
/*-------------------------------------------------
		着色器 - hsl模式转rgb模式
*/
void I_APVScene::hslToRgb(double h, double s, double l, double* result_arr){
	int c = (255 - qAbs(2 * l - 255)) * s;
	int x = c * (1 - qAbs(fmod(h / 60, 2) - 1));
	int m = l - c / 2;
	int cm = c + m;
	int xm = x + m;

	if (h < 60){
		result_arr[0] = cm;
		result_arr[1] = xm;
		result_arr[2] = m;
		return;
	}
	else if (h < 120){
		result_arr[0] = xm; 
		result_arr[1] = cm; 
		result_arr[2] = m;
		return;
	}
	else if (h < 180){
		result_arr[0] = m; 
		result_arr[1] = cm; 
		result_arr[2] = xm;
		return;
	}
	else if (h < 240){
		result_arr[0] = m; 
		result_arr[1] = xm; 
		result_arr[2] = cm;
		return;
	}
	else if (h < 300){
		result_arr[0] = xm; 
		result_arr[1] = m; 
		result_arr[2] = cm;
		return;
	}
	else{
		result_arr[0] = cm; 
		result_arr[1] = m; 
		result_arr[2] = xm;
		return;
	}
}


/*-------------------------------------------------
		部件 - 切换帧（根据索引）
*/
void I_APVScene::setAnimFrame(int index){
	if (this->m_rebuildBlock == true){ return; }
	this->m_curFrame = index;
	this->refreshFrame();
}
/*-------------------------------------------------
		部件 - 切换帧（根据资源名称）
*/
void I_APVScene::setAnimName(QFileInfo file){
	if (this->m_rebuildBlock == true){ return; }

	// > 没有资源时
	int index = this->m_fileList.indexOf(file);
	if (index == -1){
		if (file.exists() == false){ return; }
		this->addSource(file);
		this->m_curFrame = this->m_fileList.indexOf(file);
		this->refreshFrame();
		return;
	}

	// > 资源播放
	this->m_curFrame = index;
	this->refreshFrame();
}
/*-------------------------------------------------
		部件 - 刷新帧显示
*/
void I_APVScene::refreshFrame(){
	if (this->m_curFrame < 0){ this->m_curFrame = -1; }
	if (this->m_curFrame >= this->m_animList.count()){ this->m_curFrame = -1; }

	for (int i = 0; i < this->m_animList.count(); i++){
		if (i == this->m_curFrame){
			this->m_animList.at(i)->setVisible(true);
		}else{
			this->m_animList.at(i)->setVisible(false);
		}
	}
}
/*-------------------------------------------------
		部件 - 资源高度
*/
int I_APVScene::getMaxHeight(){
	if (this->m_bitmapList.count() == 0){ return 600; }	//（默认）
	int result = 0;
	for (int i = 0; i < this->m_bitmapList.count(); i++){
		int hh = this->m_bitmapList.at(i).height();
		if (result < hh){
			result = hh;
		}
	}
	return result;
}
/*-------------------------------------------------
		部件 - 资源宽度
*/
int I_APVScene::getMaxWidth(){
	if (this->m_bitmapList.count() == 0){ return 800; }	//（默认）
	int result = 0;
	for (int i = 0; i < this->m_bitmapList.count(); i++){
		int ww = this->m_bitmapList.at(i).width();
		if (result < ww){
			result = ww;
		}
	}
	return result;
}

