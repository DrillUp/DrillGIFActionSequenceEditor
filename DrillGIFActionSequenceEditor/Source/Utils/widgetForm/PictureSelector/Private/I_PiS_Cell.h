#pragma once
#include "stdafx.h"

#include "Source/Utils/WidgetForm/PictureBlock/P_PictureBlock.h"

/*
-----==========================================================-----
		类：		图片选择块 单元胞.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		图片选择块的元胞对象。
					（详细见cpp）
-----==========================================================-----
*/

class I_PiS_Cell{

	public:
		I_PiS_Cell();				//构造函数
		~I_PiS_Cell();				//析构函数
		
	//-----------------------------------
	//----数据
	public:
		int index_Title;					//编号
		int index_OrgBitmap;				//资源贴图编号
		
	//-----------------------------------
	//----控件
	public:
		QListWidgetItem* item = nullptr;	//项 对象
		P_PictureBlock* widget = nullptr;	//控件 对象
	public:
									//控件 - 获取贴图
									//		【说明】：资源数据这里只考虑复制自身，从已有的贴图列表中获取。
		QPixmap getQPixmap(QList<QPixmap>* org_bitmapList);
		
	//-----------------------------------
	//----类属性
	public:
									//空判断
		bool isNull();
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj);
};
