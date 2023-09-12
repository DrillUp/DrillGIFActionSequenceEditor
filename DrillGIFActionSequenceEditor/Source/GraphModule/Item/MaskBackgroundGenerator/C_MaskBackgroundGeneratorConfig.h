#pragma once
#include "stdafx.h"

#include <QObject>
#include <QJsonObject>
#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		马赛克生成器 配置数据.h
		版本：		v1.02
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
		
	//-----------------------------------
	//----填涂
	public:
		int mode;						//填涂模式（0:单色，1:双色）
		QColor background_color;		//单色 - 背景色
		int blackBlock_opacity;			//单色 - 黑色块透明度
		QColor a_color;					//双色 - 颜色A
		QColor b_color;					//双色 - 颜色B
	public:
										//填涂 - 设置（单色）
										//		【说明】：单色即先背景涂满底色(back_color)，再涂黑色块(blackBlock_opacity)，透明度为黑色块的透明度。
		void setMaskBackground_OneColor(int width, int height, int block_width, int block_height);
		void setMaskBackground_OneColor(int width, int height, int block_width, int block_height, QColor background_color);
		void setMaskBackground_OneColor(int width, int height, int block_width, int block_height, QColor background_color, int blackBlock_opacity);
										//填涂 - 设置（双色）
										//		【说明】：双色即颜色A和颜色B交替填涂。
		void setMaskBackground_TwoColor(int width, int height, int block_width, int block_height, QColor a_color, QColor b_color);
		

	//-----------------------------------
	//----描边（只外框描边）
	public:
		bool stroke_enabled;			//描边 - 开关
		QColor stroke_color;			//描边 - 线颜色
		int stroke_width;				//描边 - 线厚度
	public:
										//描边 - 设置
										//		【说明】：给贴图的最外层进行一层描边。
		void setMaskBackground_Stroke(QColor stroke_color, int stroke_width);


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

