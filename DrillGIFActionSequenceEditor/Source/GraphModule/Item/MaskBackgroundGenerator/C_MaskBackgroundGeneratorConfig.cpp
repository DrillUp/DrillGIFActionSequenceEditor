#include "stdafx.h"
#include "C_MaskBackgroundGeneratorConfig.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		马赛克生成器 配置数据.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		马赛克生成器执行的 配置设置。
-----==========================================================-----
*/

C_MaskBackgroundGeneratorConfig::C_MaskBackgroundGeneratorConfig(){

	// > 数据
	this->width = 0;						//贴图宽度
	this->height = 0;						//贴图高度
	this->blockWidth = 0;					//马赛克方块宽度
	this->blockHeight = 0;					//马赛克方块高度

	// > 填涂
	this->mode = 0;									//填涂模式（0:单色，1:双色）
	this->background_color = QColor(255,255,255);	//单色 - 背景色
	this->blackBlock_opacity = 30;					//单色 - 黑色块透明度
	this->a_color = QColor();						//双色 - 颜色A
	this->b_color = QColor();						//双色 - 颜色B

	// > 描边
	this->stroke_enabled = false;			//描边 - 开关
	this->stroke_color = QColor(0,0,0);		//描边 - 线颜色
	this->stroke_width = 2;					//描边 - 线厚度
}
C_MaskBackgroundGeneratorConfig::~C_MaskBackgroundGeneratorConfig() {
}


/*-------------------------------------------------
		背景 - 设置背景（单色）
*/
void C_MaskBackgroundGeneratorConfig::setMaskBackground_OneColor(int width, int height, int block_width, int block_height){
	this->setMaskBackground_OneColor(width, height, block_width, block_height, QColor(255, 255, 255), 30);
}
void C_MaskBackgroundGeneratorConfig::setMaskBackground_OneColor(int width, int height, int block_width, int block_height, QColor background_color){
	this->setMaskBackground_OneColor(width, height, block_width, block_height, background_color, 30);
}
void C_MaskBackgroundGeneratorConfig::setMaskBackground_OneColor(int width, int height, int block_width, int block_height, QColor background_color, int blackBlock_opacity){

	// > 数据
	this->width = width;
	this->height = height;
	this->blockWidth = block_width;
	this->blockHeight = block_height;

	// > 填涂
	this->mode = 0;
	this->background_color = background_color;
	this->blackBlock_opacity = blackBlock_opacity;
}
/*-------------------------------------------------
		背景 - 设置背景（双色）
*/
void C_MaskBackgroundGeneratorConfig::setMaskBackground_TwoColor(int width, int height, int block_width, int block_height, QColor a_color, QColor b_color){

	// > 数据
	this->width = width;
	this->height = height;
	this->blockWidth = block_width;
	this->blockHeight = block_height;

	// > 填涂
	this->mode = 1;
	this->a_color = a_color;
	this->b_color = b_color;
}


/*-------------------------------------------------
		描边 - 设置
*/
void C_MaskBackgroundGeneratorConfig::setMaskBackground_Stroke(QColor stroke_color, int stroke_width){
	this->stroke_enabled = true;
	this->stroke_color = stroke_color;
	this->stroke_width = stroke_width;
}


/*-------------------------------------------------
		运算符重载
*/
const bool C_MaskBackgroundGeneratorConfig::operator== (const C_MaskBackgroundGeneratorConfig& a)const {

	// > 数据
	if (this->width != a.width){ return false; }
	if (this->height != a.height){ return false; }
	if (this->blockWidth != a.blockWidth){ return false; }
	if (this->blockHeight != a.blockHeight){ return false; }

	// > 填涂
	if (this->mode != a.mode){ return false; }
	if (this->mode == 0){
		if (this->background_color != a.background_color){ return false; }
		if (this->blackBlock_opacity != a.blackBlock_opacity){ return false; }
	}
	if (this->mode == 1){
		if (this->a_color != a.a_color){ return false; }
		if (this->b_color != a.b_color){ return false; }
	}

	// > 描边
	if (this->stroke_enabled != a.stroke_enabled){ return false; }
	if (this->stroke_enabled == true){
		if (this->stroke_color != a.stroke_color){ return false; }
		if (this->stroke_width != a.stroke_width){ return false; }
	}

	return true;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_MaskBackgroundGeneratorConfig::getJsonObject() {
	QJsonObject obj;

	// > 数据
	obj.insert("width", this->width);
	obj.insert("height", this->height);
	obj.insert("blockWidth", this->blockWidth);
	obj.insert("blockHeight", this->blockHeight);

	// > 填涂
	obj.insert("mode", this->mode);
	obj.insert("background_color", TTool::_to_HtmlColor_rgb_(this->background_color));
	obj.insert("blackBlock_opacity", this->blackBlock_opacity);
	obj.insert("a_color", TTool::_to_HtmlColor_rgb_(this->a_color) );
	obj.insert("b_color", TTool::_to_HtmlColor_rgb_(this->b_color) );

	// > 描边
	obj.insert("stroke_enabled", this->stroke_enabled);
	obj.insert("stroke_color", TTool::_to_HtmlColor_rgb_(this->stroke_color));
	obj.insert("stroke_width", this->stroke_width);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_MaskBackgroundGeneratorConfig::setJsonObject(QJsonObject obj) {

	// > 数据
	if (obj.value("width").isUndefined() == false) { this->width = obj.value("width").toInt(); }
	if (obj.value("height").isUndefined() == false) { this->height = obj.value("height").toInt(); }
	if (obj.value("blockWidth").isUndefined() == false) { this->blockWidth = obj.value("blockWidth").toInt(); }
	if (obj.value("blockHeight").isUndefined() == false) { this->blockHeight = obj.value("blockHeight").toInt(); }

	// > 填涂
	if (obj.value("mode").isUndefined() == false) { this->mode = obj.value("mode").toInt(); }
	if (obj.value("background_color").isUndefined() == false && obj.value("background_color").toString() != "") { this->background_color = QColor(obj.value("background_color").toString()); }
	if (obj.value("blackBlock_opacity").isUndefined() == false) { this->blackBlock_opacity = obj.value("blackBlock_opacity").toInt(); }
	if (obj.value("a_color").isUndefined() == false && obj.value("a_color").toString() != "") { this->a_color = QColor(obj.value("a_color").toString()); }
	if (obj.value("b_color").isUndefined() == false && obj.value("b_color").toString() != "") { this->b_color = QColor(obj.value("b_color").toString()); }

	// > 描边
	if (obj.value("stroke_enabled").isUndefined() == false) { this->stroke_enabled = obj.value("stroke_enabled").toBool(); }
	if (obj.value("stroke_color").isUndefined() == false && obj.value("stroke_color").toString() != "") { this->stroke_color = QColor(obj.value("stroke_color").toString()); }
	if (obj.value("stroke_width").isUndefined() == false) { this->stroke_width = obj.value("stroke_width").toInt(); }

}