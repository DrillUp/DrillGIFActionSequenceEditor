#include "stdafx.h"
#include "P_PictureBlock.h"

#include "Source/GraphModule/Item/MaskBackgroundGenerator/S_MaskBackgroundGenerator.h"
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		图片单块.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		单独的图片块，作为列表中显示的一个基本块对象。

-----==========================================================-----
*/

P_PictureBlock::P_PictureBlock(int width, int height, QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);


	//-----------------------------------
	//----参数初始化

	// > 控件
	this->m_width = width;
	this->m_height = height;

	// > 马赛克背景
	this->m_maskEnabled = true;					//马赛克 绘制
	this->m_maskWidth = 24;						//马赛克 块宽
	this->m_maskHeight = 24;					//马赛克 块高
	this->m_maskColor = QColor(245, 245, 245);	//马赛克 颜色


	//-----------------------------------
	//----ui初始化

	// > 标题
	ui.label_title->setText("");
	ui.label_title->move(3, 3);

	// > 获取画布高宽
	int ww = this->getDrawingWidth();
	int hh = this->getDrawingHeight();

	ui.label_picture->setFixedWidth(ww);
	ui.label_picture->setMinimumWidth(ww);
	ui.label_picture->setMaximumWidth(ww);
	ui.label_picture->setFixedHeight(hh);
	ui.label_picture->setMinimumHeight(hh);
	ui.label_picture->setMaximumHeight(hh);
	ui.label_picture->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	ui.label_picture->setGeometry(5, 5, ww, hh);

	ui.label_background->setFixedWidth(ww);
	ui.label_background->setMinimumWidth(ww);
	ui.label_background->setMaximumWidth(ww);
	ui.label_background->setFixedHeight(hh);
	ui.label_background->setMinimumHeight(hh);
	ui.label_background->setMaximumHeight(hh);
	ui.label_background->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	ui.label_background->setGeometry(5, 5, ww, hh);

	ui.label_picture->stackUnder(ui.label_title);
	ui.label_background->stackUnder(ui.label_picture);

	// > 刷新 - 图片
	this->refreshPixmap();
	// > 刷新 - 马赛克背景
	this->refreshMask();
}
P_PictureBlock::~P_PictureBlock(){
}


/*-------------------------------------------------
		控件 - 设置标题
*/
void P_PictureBlock::setTitle(QString title){
	ui.label_title->setText(title);
}
QString P_PictureBlock::getTitle(){
	return ui.label_title->text();
}
/*-------------------------------------------------
		控件 - 获取画布高宽
*/
int P_PictureBlock::getDrawingWidth(){
	return this->m_width - 10;
}
int P_PictureBlock::getDrawingHeight(){
	return this->m_height - 10;
}

/*-------------------------------------------------
		图片 - 设置
*/
void P_PictureBlock::setPixmap(QPixmap pixmap){
	this->m_pixmapOrg = pixmap;
	this->refreshPixmap();
}
QPixmap P_PictureBlock::getPixmap(){
	return this->m_pixmapOrg;
}
/*-------------------------------------------------
		图片 - 刷新
*/
void P_PictureBlock::refreshPixmap(){
	int ww = this->getDrawingWidth();
	int hh = this->getDrawingHeight();

	// > 按比例缩放
	QPixmap scaledPixmap = this->m_pixmapOrg.scaled(QSize(ww, hh), Qt::KeepAspectRatio);

	ui.label_picture->setPixmap(scaledPixmap);
}

/*-------------------------------------------------
		马赛克背景 - 设置
*/
void P_PictureBlock::setMaskEnabled(bool enabled){
	if (this->m_maskEnabled == enabled){ return; }
	this->m_maskEnabled = enabled;
	this->refreshMask();
}
/*-------------------------------------------------
		马赛克背景 - 刷新
*/
void P_PictureBlock::refreshMask(){
	int ww = this->getDrawingWidth();
	int hh = this->getDrawingHeight();

	C_MaskBackgroundGeneratorConfig config;
	config.setMaskBackground_OneColor(ww, hh, this->m_maskWidth, this->m_maskHeight);
	config.setMaskBackground_Stroke(QColor(0, 0, 0), 2);
	QPixmap backgroundPixmap = S_MaskBackgroundGenerator::getInstance()->getMaskBackground(config);

	ui.label_background->setPixmap(backgroundPixmap);
}

