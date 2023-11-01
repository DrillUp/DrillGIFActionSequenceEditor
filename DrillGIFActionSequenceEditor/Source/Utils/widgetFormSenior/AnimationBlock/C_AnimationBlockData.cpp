#include "stdafx.h"
#include "C_AnimationBlockData.h"

#include "Source/Utils/WidgetForm/PictureBitmapCache/S_PictureBitmapCache.h"

/*
-----==========================================================-----
		类：		动画帧单块 数据类（继承于 图片单块 数据类）.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧单块的数据类。
					【该类不含id，因为在编辑块中，数据会被频繁交换】
					【该类参数 全开放 ，不要写封装函数】
					【动画帧单块控件可以直接修改到数据；但如果直接修改了数据，则要手动刷新 动画帧单块控件 】
-----==========================================================-----
*/

C_AnimationBlockData::C_AnimationBlockData()
	: C_PictureBlockData()
{

	// > 帧数
	this->m_intervalValue = 4;

}
C_AnimationBlockData::~C_AnimationBlockData(){
}


/*-------------------------------------------------
		复制对象
*/
void C_AnimationBlockData::copyFrom(C_AnimationBlockData* a){
	C_PictureBlockData::copyFrom(a);

	// > 帧数
	this->m_intervalValue = a->m_intervalValue;

}
/*-------------------------------------------------
		实体类 -> QJsonObject
*/
QJsonObject C_AnimationBlockData::getJsonObject(){
	QJsonObject obj = C_PictureBlockData::getJsonObject();

	// > 帧数
	obj.insert("m_frameValue", this->m_intervalValue);
	
	return obj;
}
/*-------------------------------------------------
		QJsonObject -> 实体类
*/
void C_AnimationBlockData::setJsonObject(QJsonObject obj){
	C_PictureBlockData::setJsonObject(obj);

	// > 帧数
	this->m_intervalValue = obj.value("m_frameValue").toInt(4);

}