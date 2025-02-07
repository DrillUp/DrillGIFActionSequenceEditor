#pragma once
#include "stdafx.h"

/*
-----==========================================================-----
		类：		图片单块 数据类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		数据类的实体类。
					【该类不含id，因为在编辑块中，数据会被频繁交换】
					【该类参数 全开放 ，不要写封装函数】
					【图片单块控件可以直接修改到数据；但如果直接修改了数据，则要手动刷新 图片单块控件 】
					（详细见cpp）
-----==========================================================-----
*/

class C_PictureBlockData{

	public:
		C_PictureBlockData();				//构造函数
		~C_PictureBlockData();				//析构函数
		

	//-----------------------------------
	//----画布
	public:
		int m_width;
		int m_height;
	public:
											//画布 - 初始化
											//		【说明】：创建数据类时，需要执行的初始化。
		void init(int width, int height, int titleNum = -1, QString bitmapPath = "");


	//-----------------------------------
	//----标题
	public:
		int m_titleNum;						//标题编号（-2为未赋值，-1为不使用，0以上为索引编号）
		QString m_title;					//标题文本
		QString m_titleColorType;			//标题颜色类型（Normal/Changed）


	//-----------------------------------
	//----图片
	public:
		QString m_bitmapPath;				//资源路径（完整的全路径）
	public:
											//图片 - 获取贴图
		QPixmap getBitmap();


	//-----------------------------------
	//----马赛克背景
	public:
		bool m_maskEnabled;					//马赛克 开关
		bool m_maskBorderEnabled;			//马赛克边线 开关
		int m_maskWidth;					//马赛克 块宽
		int m_maskHeight;					//马赛克 块高
		

	//-----------------------------------
	//----类属性
	public:
									//复制对象
		void copyFrom(C_PictureBlockData* a);
									//空判断
		bool isNull();
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj);

};
typedef QSharedPointer<C_PictureBlockData> C_PictureBlockDataPtr;
