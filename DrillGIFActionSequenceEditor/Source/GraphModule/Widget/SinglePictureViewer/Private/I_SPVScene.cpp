#include "stdafx.h"
#include "I_SPVScene.h"

#include <QPixmap>

/*
-----==========================================================-----
		类：		图片查看块-单图 场景.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		查看单图的画布。

		子功能：	->场景初始化
						->宽度、高度、出血线
						->颜色
						->背景
					->单图
						->设置图片资源
					->缩放（视图功能）
					->鼠标拖移（视图功能）

-----==========================================================-----
*/
I_SPVScene::I_SPVScene(QWidget *parent)
	: QGraphicsScene(parent)
{
	this->init();
}
I_SPVScene::~I_SPVScene(){
}

/*-------------------------------------------------
		初始化
*/
void I_SPVScene::init(){

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
	this->m_bitmapItem = nullptr;

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
void I_SPVScene::setGridLine(int column, int row){

	// > 建立网格线
	this->m_P_GridLineItem->rebuildGrid(this->m_canvasWidth, this->m_canvasHeight, column, row, this->m_gridLineColor);
	
	// > 添加
	this->m_P_GridLineItem->addItemsToScene();
}
/*-------------------------------------------------
		辅助 - 清空网格线
*/
void I_SPVScene::clearGridLine(){
	this->m_P_GridLineItem->clearAllItem();
}
/*-------------------------------------------------
		辅助 - 刷新背景
*/
void I_SPVScene::refreshBackground(){
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
void I_SPVScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mousePressEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标抬起
*/
void I_SPVScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标移动
*/
void I_SPVScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	//（无操作函数）
}
/*-------------------------------------------------
		监听事件 - 鼠标双击
*/
void I_SPVScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	//（无操作函数）
}



/*-------------------------------------------------
		资源 - 设置资源
*/
void I_SPVScene::setSource(QPixmap bitmap){
	this->m_bitmap = bitmap;
	this->rebuildScene();
}
/*-------------------------------------------------
		资源 - 清除资源
*/
void I_SPVScene::clearSource(){
	this->m_bitmap = QPixmap();
	this->rebuildScene();
}
/*-------------------------------------------------
		资源 - 重建场景
*/
void I_SPVScene::rebuildScene(){
	if (this->m_rebuildBlock == true){ return; }
	this->m_rebuildBlock = true;
	
	// > 去除旧图片
	if (this->m_bitmapItem != nullptr){
		this->removeItem(this->m_bitmapItem);
	}

	// > 高宽适配
	this->m_canvasWidth = this->getMaxWidth();
	this->m_canvasHeight = this->getMaxHeight();
	this->setSceneRect(-1 * this->m_canvasThickness, -1 * this->m_canvasThickness, this->m_canvasWidth + this->m_canvasThickness * 2, this->m_canvasHeight + this->m_canvasThickness * 2);
	this->refreshBackground();

	// > 放置图片
	this->m_bitmapItem = new QGraphicsPixmapItem();
	QPixmap result_pixmap = this->rotateColor(this->m_bitmap, 0);

	double xx = (this->m_canvasWidth - result_pixmap.width())*0.5;
	double yy = (this->m_canvasHeight - result_pixmap.height())*0.5;
	this->m_bitmapItem->setPixmap(result_pixmap);
	this->m_bitmapItem->setPos(xx, yy);
	this->addItem(this->m_bitmapItem);

	this->m_rebuildBlock = false;
}


/*-------------------------------------------------
		着色器 - 修改色调
*/
void I_SPVScene::setTint(int rotate_offset){
	if (this->m_rebuildBlock == true){ return; }
	if (this->m_curTint == rotate_offset){ return; }
	this->m_curTint = rotate_offset;
	this->rebuildScene();
}
/*-------------------------------------------------
		着色器 - 获取色调
*/
int I_SPVScene::getTint(){
	return this->m_curTint;
}
/*-------------------------------------------------
		着色器 - 旋转色彩
*/
QPixmap I_SPVScene::rotateColor(QPixmap pixmap, int rotate_offset){
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
	uint8_t* rgba = img.bits();
	double* temp_arr = new double[3];
	int size = img.width()*img.height() * 4;
	for (int i = 0; i < size; i += 4){
		this->rgbToHsl(rgba[i + 0], rgba[i + 1], rgba[i + 2], temp_arr);
		double h = (int)(temp_arr[0] + rotate_offset) % 360;
		double s = temp_arr[1];
		double l = temp_arr[2];
		this->hslToRgb(h, s, l, temp_arr);
		rgba[i + 0] = temp_arr[0];
		rgba[i + 1] = temp_arr[1];
		rgba[i + 2] = temp_arr[2];
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
void I_SPVScene::rgbToHsl(int r, int g, int b, double* result_arr){
	double cmin = qMin(r, qMin(g, b));
	double cmax = qMax(r, qMax(g, b));
	double h = 0;
	double s = 0;
	double l = (cmin + cmax) * 0.5;
	double delta = cmax - cmin;

	if (delta > 0){
		if (r == cmax){
			h = 60 * ((int)((g - b) / delta + 6) % 6);
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
void I_SPVScene::hslToRgb(double h, double s, double l, double* result_arr){
	int c = (255 - qAbs(2 * l - 255)) * s;
	int x = c * (1 - qAbs((int)(h / 60) % 2 - 1));
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
		图片 - 最大高度
*/
int I_SPVScene::getMaxHeight(){
	if (this->m_bitmap.height() == 0){ return 600; }	//（默认）
	return this->m_bitmap.height();
}
/*-------------------------------------------------
		图片 - 最大宽度
*/
int I_SPVScene::getMaxWidth(){
	if (this->m_bitmap.width() == 0){ return 800; }		//（默认）
	return this->m_bitmap.width();
}

