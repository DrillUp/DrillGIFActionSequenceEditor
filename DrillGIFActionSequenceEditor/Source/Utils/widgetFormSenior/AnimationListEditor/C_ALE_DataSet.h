#pragma once
#include "stdafx.h"
#include <QList>
#include "Source/Utils/WidgetForm/PictureListEditor/C_PLE_DataSet.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 数据类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧编辑块 的数据类。
					【该数据类只用于传值，所以不需要 ID或名称 的标识】
					（详细见cpp）
-----==========================================================-----
*/

class C_ALE_DataSet : public C_PLE_DataSet
{

	public:
		C_ALE_DataSet();
		~C_ALE_DataSet();
		

	//-----------------------------------
	//----主流程
	public:
											//主流程 - 设置资源
											//		【参数1】：文件父目录
											//		【参数2】：文件名（不带文件后缀，文件可重名）
											//		【说明】：此流程会赋值 父路径 和 资源文件 的参数。
		void setSource(QString pic_file, QList<QString> pic_name_list);
											//主流程 - 设置帧间隔数据
											//		【参数1】：默认帧间隔
											//		【参数2】：帧间隔明细表
											//		【说明】：此流程会赋值 默认帧间隔 和 帧间隔明细表 的参数。需要在setSource或setPicList之后执行。
		void setInterval(int gif_interval, QList<int> gif_intervalTank);


	//-----------------------------------
	//----父路径（全局数据，继承于PLE）
	//
	//		（暂无子类函数）


	//-----------------------------------
	//----动画帧单位（全局数据）
	public:
		enum DATA_UNIT{	
			FrameUnit,		//帧单位（1秒60帧）
			SecondUnit,		//秒单位（1秒100帧）
		};
	protected:
		DATA_UNIT m_unit;
	public:
											//动画帧单位 - 设置
											//		【说明】：数据单位以编辑器的为准。单位变化并不影响实际存储的帧间隔。
		void setData_Unit(C_ALE_DataSet::DATA_UNIT unit);
											//动画帧单位 - 获取
											//		【说明】：数据单位以编辑器的为准。单位变化并不影响实际存储的帧间隔。
		C_ALE_DataSet::DATA_UNIT getData_Unit();
		

	//-----------------------------------
	//----默认帧间隔（全局数据）
	protected:
		int m_gif_interval;
	public:
											//默认帧间隔 - 设置
		void setData_IntervalDefault(int gif_interval);
											//默认帧间隔 - 获取
		int getData_IntervalDefault();
											//默认帧间隔 - 修改默认帧间隔
											//		【说明】：与原默认帧间隔一样的值，会被统一改变。
											//		【说明】：此处为 改数据 中的默认帧间隔。改Cell的见P_ALE_Editor。
		void setIntervalDefaultInAll(int gif_interval);
		
		
	//-----------------------------------
	//----帧间隔明细表
	protected:
		QList<int> m_gif_intervalTank;
	public:
											//帧间隔明细表 - 设置（原数据）
		void setData_IntervalTank(QList<int> gif_intervalTank);
											//帧间隔明细表 - 获取（原数据）
		QList<int> getData_IntervalTank();
	public:
											//帧间隔明细表 - 自适应 - 获取（含自适应）
											//		【说明】：明细表 原数据的数组长度 可能与 图片资源长度 不一致，所以需要获取自适应数据。
		QList<int> getData_IntervalTank_WithFit();
											//帧间隔明细表 - 自适应 - 执行自适应转换
		static QList<int> converterFit_getIntervalList(QList<int> intervalValueList, int default_interval, int pic_count);
		
	public:
											//帧间隔明细表 - 单位 - 获取（含单位转换）
		double getData_IntervalTankOne_WithUnit(int index);
		QList<double> getData_IntervalTankList_WithUnit(QList<int> index_list);
		QList<double> getData_IntervalTankAll_WithUnit();
											//帧间隔明细表 - 单位 - 执行单位转换
		static double converterUnit_getInterval(int intervalValue, C_ALE_DataSet::DATA_UNIT unit);
		static QList<double> converterUnit_getIntervalList(QList<int> intervalValueList, C_ALE_DataSet::DATA_UNIT unit);

											//帧间隔明细表 - 单位 - 获取描述文本（"0.01"，实际帧为 0.01666 * n ）
		QString getDescriptionString(int intervalValue);
											//帧间隔明细表 - 单位 - 执行描述文本转换
		static QString converterUnit_getDescriptionString(int intervalValue, C_ALE_DataSet::DATA_UNIT unit);


	//-----------------------------------
	//----类属性
	public:
												//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject() override;
												//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj) override;
};