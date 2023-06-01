﻿#pragma once
#include "stdafx.h"
#include <QObject>

#include "C_MaskBackgroundGeneratorConfig.h"

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
class S_MaskBackgroundGenerator : public QObject
{
	Q_OBJECT

	protected:
		S_MaskBackgroundGenerator();
		~S_MaskBackgroundGenerator();
	public:
		static S_MaskBackgroundGenerator* cur_manager;			//单例
		static S_MaskBackgroundGenerator* getInstance();		//单例，获取自己（必须要拿到全局配置才能进行计算）
	
	//-----------------------------------
	//----生成器
	public:
											//生成器 - 设置背景（单色）
											//		【说明】：单色即先背景涂满颜色，再涂黑色块，透明度为黑色的透明度。
		QPixmap getMaskBackground_OneColor(int width, int height, int block_width, int block_height, QColor color = QColor(255, 255, 255), int opacity = 30);
											//生成器 - 设置背景（双色）
											//		【说明】：双色即先背景涂满颜色A，再涂颜色B，透明度为颜色B的透明度。
		QPixmap getMaskBackground_TwoColor(int width, int height, int block_width, int block_height, QColor a_color, int a_opacity, QColor b_color, int b_opacity);
											//生成器 - 获取马赛克贴图
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

