#pragma once
#include "stdafx.h"

#include <QObject>
#include <QJsonObject>
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		马赛克生成器 配置数据.h
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		马赛克生成器执行的 配置设置。
					（详细见cpp）
-----==========================================================-----
*/

class C_MaskBackgroundGeneratorConfig{

	public:
		C_MaskBackgroundGeneratorConfig();
		~C_MaskBackgroundGeneratorConfig();
		
	//-----------------------------------
	//----数据
	public:
		int width;						//贴图宽度
		int height;						//贴图高度
		int blockWidth;					//马赛克方块宽度
		int blockHeight;				//马赛克方块高度
		int mode;						//马赛克模式（0:单色，1:双色）
		
		QColor a_color;					//颜色A
		QColor b_color;					//颜色B
		int after_opacity;				//后加颜色的透明度
	public:
										//背景 - 设置背景（单色）
										//		【说明】：单色即先背景涂满颜色，再涂黑色块，透明度为黑色的透明度。
		void setMaskBackground_OneColor(int width, int height, int block_width, int block_height, QColor color = QColor(255, 255, 255), int opacity = 30);
										//背景 - 设置背景（双色）
										//		【说明】：双色即先背景涂满颜色A，再涂颜色B，透明度为颜色B的透明度。
		void setMaskBackground_TwoColor(int width, int height, int block_width, int block_height, QColor a_color, int a_opacity, QColor b_color, int b_opacity);

	//-----------------------------------
	//----类属性
	public:
												//运算符重载
		const bool operator== (const C_MaskBackgroundGeneratorConfig& a)const;
												//实体类 -> QJsonObject
		QJsonObject getJsonObject();
												//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);

};

