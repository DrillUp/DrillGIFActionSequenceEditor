#include "stdafx.h"
#include "C_MaskBackgroundGeneratorConfig.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		马赛克生成器 配置数据.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		马赛克生成器执行的 配置设置。
-----==========================================================-----
*/

C_MaskBackgroundGeneratorConfig::C_MaskBackgroundGeneratorConfig(){

	this->width = 0;					//贴图宽度
	this->height = 0;					//贴图高度
	this->blockWidth = 0;				//马赛克方块宽度
	this->blockHeight = 0;				//马赛克方块高度
	this->mode = 0;						//马赛克模式（0:单色，1:双色）

	this->a_color = QColor();			//颜色A
	this->b_color = QColor();			//颜色B
	this->after_opacity = 0;			//后加颜色的透明度
}
C_MaskBackgroundGeneratorConfig::~C_MaskBackgroundGeneratorConfig() {
}


/*-------------------------------------------------
		背景 - 设置背景（单色）
*/
void C_MaskBackgroundGeneratorConfig::setMaskBackground_OneColor(int width, int height, int block_width, int block_height, QColor color, int opacity){

	this->width = width;
	this->height = height;
	this->blockWidth = block_width;
	this->blockHeight = block_height;
	this->mode = 0;

	this->a_color = color;
	this->after_opacity = opacity;

}
/*-------------------------------------------------
		背景 - 设置背景（双色）
*/
void C_MaskBackgroundGeneratorConfig::setMaskBackground_TwoColor(int width, int height, int block_width, int block_height, QColor a_color, int a_opacity, QColor b_color, int b_opacity){

	this->width = width;
	this->height = height;
	this->blockWidth = block_width;
	this->blockHeight = block_height;
	this->mode = 0;

	this->a_color = a_color;
	this->b_color = b_color;
	this->after_opacity = a_opacity;
}



/*-------------------------------------------------
		运算符重载
*/
const bool C_MaskBackgroundGeneratorConfig::operator== (const C_MaskBackgroundGeneratorConfig& a)const {
	if (this->width != a.width){ return false; }
	if (this->height != a.height){ return false; }
	if (this->blockWidth != a.blockWidth){ return false; }
	if (this->blockHeight != a.blockHeight){ return false; }

	if (this->mode != a.mode){ return false; }
	if (this->mode == 0){
		if (this->a_color != a.a_color){ return false; }
		if (this->after_opacity != a.after_opacity){ return false; }
	}
	if (this->mode == 1){
		if (this->a_color != a.a_color){ return false; }
		if (this->b_color != a.b_color){ return false; }
		if (this->after_opacity != a.after_opacity){ return false; }
	}
	return true;
}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_MaskBackgroundGeneratorConfig::getJsonObject() {
	QJsonObject obj;

	obj.insert("width", this->width);
	obj.insert("height", this->height);
	obj.insert("blockWidth", this->blockWidth);
	obj.insert("blockHeight", this->blockHeight);
	obj.insert("mode", this->mode);

	obj.insert("a_color", TTool::_to_HtmlColor_rgb_(this->a_color) );
	obj.insert("b_color", TTool::_to_HtmlColor_rgb_(this->b_color) );
	obj.insert("after_opacity", this->after_opacity);

	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_MaskBackgroundGeneratorConfig::setJsonObject(QJsonObject obj) {

	if (obj.value("width").isUndefined() == false) { this->width = obj.value("width").toInt(); }
	if (obj.value("height").isUndefined() == false) { this->height = obj.value("height").toInt(); }
	if (obj.value("blockWidth").isUndefined() == false) { this->blockWidth = obj.value("blockWidth").toInt(); }
	if (obj.value("blockHeight").isUndefined() == false) { this->blockHeight = obj.value("blockHeight").toInt(); }
	if (obj.value("mode").isUndefined() == false) { this->mode = obj.value("mode").toInt(); }

	if (obj.value("a_color").isUndefined() == false && obj.value("a_color").toString() != "") { this->a_color = QColor(obj.value("a_color").toString()); }
	if (obj.value("b_color").isUndefined() == false && obj.value("b_color").toString() != "") { this->b_color = QColor(obj.value("b_color").toString()); }
	if (obj.value("after_opacity").isUndefined() == false) { this->after_opacity = obj.value("after_opacity").toInt(); }

}