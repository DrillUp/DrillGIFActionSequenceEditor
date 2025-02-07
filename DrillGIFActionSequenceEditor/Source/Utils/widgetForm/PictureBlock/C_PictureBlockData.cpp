#include "stdafx.h"
#include "C_PictureBlockData.h"

#include "Source/Utils/WidgetForm/PictureBitmapCache/S_PictureBitmapCache.h"

/*
-----==========================================================-----
		类：		图片单块 数据类.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		数据类的实体类。
					【该类不含id，因为在编辑块中，数据会被频繁交换】
					【该类参数 全开放 ，不要写封装函数】
					【图片单块控件可以直接修改到数据；但如果直接修改了数据，则要手动刷新 图片单块控件 】
-----==========================================================-----
*/

C_PictureBlockData::C_PictureBlockData(){

	// > 画布
	this->m_width = 0;
	this->m_height = 0;

	// > 标题
	this->m_titleNum = -2;					//标题编号
	this->m_title = "";						//标题文本
	this->m_titleColorType = "Normal";		//标题颜色类型

	// > 图片
	this->m_bitmapPath = "";				//资源路径

	// > 马赛克背景
	this->m_maskEnabled = true;				//马赛克 开关
	this->m_maskBorderEnabled = true;		//马赛克边线 开关
	this->m_maskWidth = 24;					//马赛克 块宽
	this->m_maskHeight = 24;				//马赛克 块高

}
C_PictureBlockData::~C_PictureBlockData(){
}


/*-------------------------------------------------
		画布 - 初始化
*/
void C_PictureBlockData::init(int width, int height, int titleNum, QString bitmapPath){
	this->m_width = width;
	this->m_height = height;
	this->m_titleNum = titleNum;
	this->m_bitmapPath = bitmapPath;
}
/*-------------------------------------------------
		控件 - 获取贴图
*/
QPixmap C_PictureBlockData::getBitmap(){
	return S_PictureBitmapCache::getInstance()->getBitmapByPath(this->m_bitmapPath);
}


/*-------------------------------------------------
		复制对象
*/
void C_PictureBlockData::copyFrom(C_PictureBlockData* a){

	// > 画布
	this->m_width = a->m_width;
	this->m_height = a->m_height;

	// > 标题
	this->m_titleNum = a->m_titleNum;
	this->m_title = a->m_title;
	this->m_titleColorType = a->m_titleColorType;

	// > 图片
	this->m_bitmapPath = a->m_bitmapPath;

	// > 马赛克背景
	this->m_maskEnabled = a->m_maskEnabled;
	this->m_maskBorderEnabled = a->m_maskBorderEnabled;
	this->m_maskWidth = a->m_maskWidth;
	this->m_maskHeight = a->m_maskHeight;
}
/*-------------------------------------------------
		空判断
*/
bool C_PictureBlockData::isNull(){
	if (this->m_width == 0){ return true; }
	if (this->m_height == 0){ return true; }
	if (this->m_titleNum == -2){ return true; }
	return false;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_PictureBlockData::getJsonObject(){
	QJsonObject obj;

	// > 画布
	obj.insert("m_width", this->m_width);
	obj.insert("m_height", this->m_height);

	// > 标题
	obj.insert("m_titleNum", this->m_titleNum);
	obj.insert("m_title", this->m_title);
	obj.insert("m_titleColorType", this->m_titleColorType);

	// > 图片
	obj.insert("index_BitmapPath", this->m_bitmapPath);

	// > 马赛克背景
	obj.insert("m_maskEnabled", this->m_maskEnabled);
	obj.insert("m_maskBorderEnabled", this->m_maskBorderEnabled);
	obj.insert("m_maskWidth", this->m_maskWidth);
	obj.insert("m_maskHeight", this->m_maskHeight);
	
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_PictureBlockData::setJsonObject(QJsonObject obj){

	// > 画布
	this->m_width = obj.value("m_width").toInt(0);
	this->m_height = obj.value("m_height").toInt(0);

	// > 标题
	this->m_titleNum = obj.value("m_titleNum").toInt(-1);
	this->m_title = obj.value("m_title").toString();
	this->m_titleColorType = obj.value("m_titleColorType").toString("Normal");

	// > 图片
	this->m_bitmapPath = obj.value("index_BitmapPath").toString();

	// > 马赛克背景
	this->m_maskEnabled = obj.value("m_maskEnabled").toBool(true);
	this->m_maskBorderEnabled = obj.value("m_maskBorderEnabled").toBool(true);
	this->m_maskWidth = obj.value("m_maskWidth").toInt();
	this->m_maskHeight = obj.value("m_maskHeight").toInt();
}