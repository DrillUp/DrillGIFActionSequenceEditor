#pragma once
#include "stdafx.h"

#include "Source/Utils/WidgetForm/PictureBlock/C_PictureBlockData.h"

/*
-----==========================================================-----
		类：		动画帧单块 数据类（继承于 图片单块 数据类）.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧单块的数据类。
					【该类不含id，因为在编辑块中，数据会被频繁交换】
					【该类参数 全开放 ，不要写封装函数】
					【动画帧单块控件可以直接修改到数据；但如果直接修改了数据，则要手动刷新 动画帧单块控件 】
					（详细见cpp）
-----==========================================================-----
*/

class C_AnimationBlockData : public C_PictureBlockData
{

	public:
		C_AnimationBlockData();				//构造函数
		~C_AnimationBlockData();			//析构函数


	//-----------------------------------
	//----帧数
	public:
		int m_intervalValue;				//帧间隔值（无单位）
		

	//-----------------------------------
	//----类属性
	public:
									//复制对象
		void copyFrom(C_AnimationBlockData* a);
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject() override;
									//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj) override;

};
typedef QSharedPointer<C_AnimationBlockData> C_AnimationBlockDataPtr;
