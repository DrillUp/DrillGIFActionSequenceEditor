#include "stdafx.h"
#include "Drill_COAS_Data.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		类：		动画序列数据【复刻类】.cpp
		作者：		drill_up
		所属模块：	动画序列数据
		功能：		动作序列核心插件的复刻类。
		
		目标：		->控件
						->单选表格
						->快速表单

		使用方法：
				>初始化

-----==========================================================-----
*/
Drill_COAS_Data::Drill_COAS_Data(QJsonObject data, QObject *parent = 0)
	: QObject(parent)
{
	this->_drill_data = data;			//深拷贝数据
	this->drill_initData();				//初始化数据
}
Drill_COAS_Data::~Drill_COAS_Data(){
}

/*-------------------------------------------------
		创建 - 初始化
*/
void Drill_COAS_Data::drill_initData(){
	QJsonObject data = this->_drill_data;
	
	QString state_default_randomSeq_str = data.value("state_default_randomSeq").toString();
	if (){

	}
	QStringList


	this->_drill_time = 0;											//持续时间
	this->_drill_arrayCheck = true;									//检查数组元素
	this->_drill_bitmapName = "";									//当前的bitmap对象名
	this->_drill_bitmapPath = "";									//当前的bitmap路径
	this->_drill_bitmapTint = 0;										//当前的bitmap色调
	this->_drill_bitmapSmooth = false;								//当前的bitmap模糊
	this->_drill_state_curCom = "";									//状态元 - 当前状态
	this->_drill_state_curTime = 0;									//状态元 - 当前时间
	this->_drill_state_curSeq = data['state_default_randomSeq'];		//状态元 - 当前序列
	this->_drill_act_curCom = "";									//动作元 - 当前动作
	this->_drill_act_curTime = 0;									//动作元 - 当前时间
}
/*-------------------------------------------------
		数据 - 帧刷新（需要父类手动执行）
*/
void Drill_COAS_Data::update(){
	



}