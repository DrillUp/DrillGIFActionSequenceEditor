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

P_PictureBlock::P_PictureBlock(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----ui初始化

	// > 标题
	ui.label_title->setText("");
	ui.label_title->move(3, 3);

}
P_PictureBlock::~P_PictureBlock(){
}


/*-------------------------------------------------
		控件 - 设置数据
*/
void P_PictureBlock::setDataPtr(C_PictureBlockDataPtr data){
	Q_ASSERT(data->isNull() == false);
	Q_ASSERT(data.isNull() == false);
	this->m_dataPtr = data;
	this->refreshUI();
}
/*-------------------------------------------------
		控件 - 获取数据
*/
C_PictureBlockDataPtr P_PictureBlock::getDataPtr(){
	return this->m_dataPtr;
}

/*-------------------------------------------------
		控件 - 刷新UI
*/
void P_PictureBlock::refreshUI(){
	Q_ASSERT(this->m_dataPtr->isNull() == false);
	Q_ASSERT(this->m_dataPtr.isNull() == false);

	// > 刷新 - 画布
	this->refreshSize();

	// > 刷新 - 标题
	this->refreshTitle();
	this->refreshTitleColor();

	// > 刷新 - 图片
	this->refreshPixmap();

	// > 刷新 - 马赛克背景
	this->refreshMask();
};


/*-------------------------------------------------
		画布 - 设置高宽
*/
void P_PictureBlock::setSize(int width, int height){
	this->m_dataPtr->m_width = width;
	this->m_dataPtr->m_height = height;
	this->refreshUI();		//修改高宽后，必然执行 刷新UI。
}
/*-------------------------------------------------
		画布 - 获取高宽
*/
int P_PictureBlock::getDrawingWidth(){
	return this->m_dataPtr->m_width - 10;
}
int P_PictureBlock::getDrawingHeight(){
	return this->m_dataPtr->m_height - 10;
}
/*-------------------------------------------------
		画布 - 刷新
*/
void P_PictureBlock::refreshSize(){
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
}


/*-------------------------------------------------
		标题 - 设置
*/
void P_PictureBlock::setTitle(QString title){
	this->m_dataPtr->m_title = title;
	this->m_dataPtr->m_titleNum = -1;
	this->refreshTitle();
}
void P_PictureBlock::setTitleNum(int title_num){
	this->m_dataPtr->m_title = QString::number(title_num + 1);
	this->m_dataPtr->m_titleNum = title_num;
	this->refreshTitle();
}
void P_PictureBlock::setTitleNumWithZeroFill(int title_num, int zeroFillCount, QChar zeroFillChar){
	this->m_dataPtr->m_title = TTool::_zeroFill_(title_num + 1, zeroFillCount, QLatin1Char(zeroFillChar.toLatin1()));
	this->m_dataPtr->m_titleNum = title_num;
	this->refreshTitle();
}
QString P_PictureBlock::getTitle(){
	return this->m_dataPtr->m_title;
}
int P_PictureBlock::getTitleNum(){
	return this->m_dataPtr->m_titleNum;
}
/*-------------------------------------------------
		标题 - 刷新
*/
void P_PictureBlock::refreshTitle(){
	ui.label_title->setText(this->m_dataPtr->m_title);
}

/*-------------------------------------------------
		标题 - 设置颜色
*/
void P_PictureBlock::setTitleColorType(QString type){
	this->m_dataPtr->m_titleColorType = type;
	this->refreshTitleColor();
}
QString P_PictureBlock::getTitleColorType(){
	return this->m_dataPtr->m_titleColorType;
}
/*-------------------------------------------------
		标题 - 修改颜色
*/
void P_PictureBlock::setTitleColor(QColor color_a, QColor color_b){
	QString style = "";
	style.append("border: 1px solid #ffffff;");
	style.append("border-radius: 3px;");
	style.append("color:#ffffff;");
	style.append("font-size:18px;");
	style.append("font-weight:bold;");

	style.append("background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:0 ");
	style.append(TTool::_to_HtmlColor_rgb_(color_a));
	style.append(", stop:1 ");
	style.append(TTool::_to_HtmlColor_rgb_(color_b));
	style.append(");");

	ui.label_title->setStyleSheet(style);
}
/*-------------------------------------------------
		标题 - 刷新颜色
*/
void P_PictureBlock::refreshTitleColor(){

	if (this->m_dataPtr->m_titleColorType == "Changed"){
		this->setTitleColor(QColor("#e447e1"), QColor("#ba2fb7"));
	}else{
		this->setTitleColor(QColor("#8ad8ff"), QColor("#5fc2ff"));
	}
}


/*-------------------------------------------------
		图片 - 设置
*/
void P_PictureBlock::setBitmapPath(QString bitmapPath){
	this->m_dataPtr->m_bitmapPath = bitmapPath;
	this->refreshPixmap();
}
QString P_PictureBlock::getBitmapPath(){
	return this->m_dataPtr->m_bitmapPath;
}
/*-------------------------------------------------
		图片 - 刷新
*/
void P_PictureBlock::refreshPixmap(){
	int ww = this->getDrawingWidth();
	int hh = this->getDrawingHeight();

	// > 按比例缩放
	QPixmap orgPixmap = this->m_dataPtr->getBitmap();
	QPixmap scaledPixmap = orgPixmap.scaled(QSize(ww, hh), Qt::KeepAspectRatio);
	ui.label_picture->setPixmap(scaledPixmap);

	// > 图片居中
	int xx = 5;
	int yy = 5;
	if (scaledPixmap.height()+1 >= hh){		//水平位移
		xx += (ww - scaledPixmap.width())*0.5;
	}else{									//垂直位移
		//yy += (hh - scaledPixmap.height())*0.5;	//（不需要位移）
	}
	ui.label_picture->setGeometry(xx, yy, scaledPixmap.width(), scaledPixmap.height());
}


/*-------------------------------------------------
		马赛克背景 - 设置
*/
void P_PictureBlock::setMaskConfig(bool maskEnabled, bool borderEnabled){
	if (this->m_dataPtr->m_maskEnabled == maskEnabled &&
		this->m_dataPtr->m_maskBorderEnabled == borderEnabled){
		return;
	}
	this->m_dataPtr->m_maskEnabled = maskEnabled;
	this->m_dataPtr->m_maskBorderEnabled = borderEnabled;
	this->refreshMask();
}
/*-------------------------------------------------
		马赛克背景 - 刷新
*/
void P_PictureBlock::refreshMask(){
	int ww = this->getDrawingWidth();
	int hh = this->getDrawingHeight();

	C_MaskBackgroundGeneratorConfig config;
	if (this->m_dataPtr->m_maskEnabled == true){
		config.setMaskBackground_OneColor(ww, hh, this->m_dataPtr->m_maskWidth, this->m_dataPtr->m_maskHeight);
	}else{
		//（双色都为白色的马赛克背景）
		config.setMaskBackground_TwoColor(ww, hh, this->m_dataPtr->m_maskWidth, this->m_dataPtr->m_maskHeight, QColor(255, 255, 255), QColor(255, 255, 255));
	}
	if (this->m_dataPtr->m_maskBorderEnabled == true){
		config.setMaskBackground_Stroke(QColor(0, 0, 0), 2);
	}else{
		config.setMaskBackground_Stroke(QColor(255, 255, 255), 0);
	}
	QPixmap backgroundPixmap = S_MaskBackgroundGenerator::getInstance()->getMaskBackground(config);

	ui.label_background->setPixmap(backgroundPixmap);
}

