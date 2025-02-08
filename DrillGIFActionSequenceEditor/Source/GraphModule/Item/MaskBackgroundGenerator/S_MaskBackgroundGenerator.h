#pragma once
#include "stdafx.h"
#include <QObject>

#include "C_MaskBackgroundGeneratorConfig.h"

/*
-----==========================================================-----
		类：		马赛克生成器.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	图形模块
		功能：		根据配置生成马赛克贴图。
					内置图形缓冲池，相同的配置可以多次调用。
-----==========================================================-----
*/
class S_MaskBackgroundGenerator : public QObject
{
	Q_OBJECT

	protected:
		S_MaskBackgroundGenerator();
		~S_MaskBackgroundGenerator();
	public:
		static S_MaskBackgroundGenerator* cur_manager;			//单例
		static S_MaskBackgroundGenerator* getInstance();		//单例，获取自己
	
	//-----------------------------------
	//----生成器
	public:
											//生成器 - 获取背景（单色）
											//		【说明】：单色即先背景涂满底色(back_color)，再涂黑色块(blackBlock_opacity)，透明度为黑色块的透明度。
		QPixmap getMaskBackground_OneColor(int width, int height, int block_width, int block_height);
		QPixmap getMaskBackground_OneColor(int width, int height, int block_width, int block_height, QColor background_color);
		QPixmap getMaskBackground_OneColor(int width, int height, int block_width, int block_height, QColor background_color, int blackBlock_opacity);
											//生成器 - 获取背景（双色）
											//		【说明】：双色即颜色A和颜色B交替填涂。
		QPixmap getMaskBackground_TwoColor(int width, int height, int block_width, int block_height, QColor a_color, QColor b_color );
											//生成器 - 获取背景（高级配置）
											//		【说明】：其它马赛克背景的配置，使用对象类赋值。
		QPixmap getMaskBackground(C_MaskBackgroundGeneratorConfig config);
		
	//-----------------------------------
	//----缓冲池
	protected:
		QPixmap m_emptyPixmap;
		QList<C_MaskBackgroundGeneratorConfig> m_configTank;
		QList<QPixmap> m_pixmapTank;
	protected:
											//缓冲池 - 创建马赛克贴图
		QPixmap createMaskBackground(C_MaskBackgroundGeneratorConfig config);

};

