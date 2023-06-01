#include "stdafx.h"
#include "S_MaskBackgroundGenerator.h"

/*
-----==========================================================-----
		类：		马赛克生成器.cpp
		版本：		v1.01
		作者：		drill_up
		所属模块：	图形模块
		功能：		根据配置生成马赛克贴图。
					内置图形缓冲池，相同的配置可以多次调用。
-----==========================================================-----
*/
S_MaskBackgroundGenerator::S_MaskBackgroundGenerator(){
	
	this->m_emptyPixmap = QPixmap();
	this->m_configTank.clear();
	this->m_pixmapTank.clear();
}
S_MaskBackgroundGenerator::~S_MaskBackgroundGenerator() {
}

/* --------------------------------------------------------------
----------单例
*/
S_MaskBackgroundGenerator* S_MaskBackgroundGenerator::cur_manager = nullptr;
S_MaskBackgroundGenerator* S_MaskBackgroundGenerator::getInstance() {
	if (cur_manager == nullptr) {
		cur_manager = new S_MaskBackgroundGenerator();
	}
	return cur_manager;
}


/*-------------------------------------------------
		生成器 - 设置背景（单色）
*/
QPixmap S_MaskBackgroundGenerator::getMaskBackground_OneColor(int width, int height, int block_width, int block_height, QColor color, int opacity){
	C_MaskBackgroundGeneratorConfig config;
	config.setMaskBackground_OneColor(width, height, block_width, block_height, color, opacity);
	return this->getMaskBackground(config);
}
/*-------------------------------------------------
		生成器 - 设置背景（双色）
*/
QPixmap S_MaskBackgroundGenerator::getMaskBackground_TwoColor(int width, int height, int block_width, int block_height, QColor a_color, int a_opacity, QColor b_color, int b_opacity){
	C_MaskBackgroundGeneratorConfig config;
	config.setMaskBackground_TwoColor(width, height, block_width, block_height, a_color, a_opacity, b_color, b_opacity);
	return this->getMaskBackground(config);
}
/*-------------------------------------------------
		生成器 - 马赛克贴图
*/
QPixmap S_MaskBackgroundGenerator::getMaskBackground(C_MaskBackgroundGeneratorConfig config){
	
	// > 已有的直接用
	int index = this->m_configTank.indexOf(config);
	if (index >= 0){
		return this->m_pixmapTank.at(index);
	}

	// > 没有的则创建
	QPixmap pixmap = this->createMaskBackground(config);
	this->m_configTank.append(config);
	this->m_pixmapTank.append(pixmap);
	return pixmap;
}

/*-------------------------------------------------
		缓冲池 - 创建马赛克贴图
*/
QPixmap S_MaskBackgroundGenerator::createMaskBackground(C_MaskBackgroundGeneratorConfig config){

	// > 无效贴图
	if (config.width == 0){ return this->m_emptyPixmap; }
	if (config.height == 0){ return this->m_emptyPixmap; }
	if (config.blockWidth == 0){ return this->m_emptyPixmap; }
	if (config.blockHeight == 0){ return this->m_emptyPixmap; }

	// > 准备参数
	int ww = config.width;
	int hh = config.height;
	int pw = config.blockWidth;
	int ph = config.blockHeight;

	// > 单色
	if (config.mode == 0){
		QColor a_color = config.a_color;
		a_color.setAlpha(255);

		// > 准备画布
		QPixmap pixmap = QPixmap(ww, hh);
		pixmap.fill(a_color);					//指定色背景
		QPainter painter(&pixmap);				//画家

		// > 开始绘制
		int i_count = qCeil(ww / (double)pw);
		int j_count = qCeil(hh / (double)ph);
		painter.setPen(QPen(QColor(0, 0, 0, 0)));
		painter.setBrush(QBrush(QColor(0, 0, 0, config.after_opacity), Qt::BrushStyle::SolidPattern));
		for (int i = 0; i < i_count; i++){
			for (int j = 0; j < j_count; j++){
				painter.drawRect(i*pw + 0, j*ph + 0, pw * 0.5, ph * 0.5);
				painter.drawRect(i*pw + pw*0.5, j*ph + ph * 0.5, pw * 0.5, ph * 0.5);
			}
		}
		painter.end();
		return pixmap;
	}

	// > 双色
	if (config.mode == 1){
		QColor a_color = config.a_color;
		a_color.setAlpha(255);
		QColor b_color = config.b_color;
		b_color.setAlpha(config.after_opacity);

		// > 准备画布
		QPixmap pixmap = QPixmap(ww, hh);
		pixmap.fill(a_color);					//指定色背景
		QPainter painter(&pixmap);				//画家

		// > 开始绘制
		int i_count = qCeil(ww / (double)pw);
		int j_count = qCeil(hh / (double)ph);
		painter.setPen(QPen(QColor(0, 0, 0, 0)));
		painter.setBrush(QBrush(b_color, Qt::BrushStyle::SolidPattern));
		for (int i = 0; i < i_count; i++){
			for (int j = 0; j < j_count; j++){
				painter.drawRect(i*pw + 0, j*ph + 0, pw * 0.5, ph * 0.5);
				painter.drawRect(i*pw + pw*0.5, j*ph + ph * 0.5, pw * 0.5, ph * 0.5);
			}
		}
		painter.end();
		return pixmap;
	}

	return this->m_emptyPixmap;
}