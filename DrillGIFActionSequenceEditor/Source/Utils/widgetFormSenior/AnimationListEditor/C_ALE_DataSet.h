#pragma once
#include "stdafx.h"
#include <QList>

/*
-----==========================================================-----
		类：		动画帧 数据类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧 的数据类。
					注意，只有五项。如果有其他数据合并，想办法另起一个类。
					（详细见cpp）
-----==========================================================-----
*/
class C_ALE_DataSet {

	public:
		C_ALE_DataSet();
		~C_ALE_DataSet();
		
	//-----------------------------------
	//----数据
	public:
		enum DATA_UNIT{	
			FrameUnit,		//帧单位（1秒60帧）
			SecondUnit,		//秒单位（1秒100帧）
		};
	protected:
		int id;								//标识
		DATA_UNIT m_unit;					//单位

		QList<QString> gif_src;				//资源文件名
		QString gif_src_file;				//资源文件夹
		QList<int> gif_intervalTank;		//帧间隔-明细表
		int gif_interval;					//帧间隔
		
	//-----------------------------------
	//----设置
	public:
											//访问器 - 设置标识
		void setData_Id(int id);
											//访问器 - 设置文件父目录
		void setData_ParentFile(QString gif_src_file);
											//访问器 - 设置默认帧间隔
		void setData_IntervalDefault(int gif_interval);
											//访问器 - 设置单位
											//		【说明】：数据单位以编辑器的为准。单位变化并不影响实际存储的帧间隔。
		void setData_Unit(C_ALE_DataSet::DATA_UNIT unit);

											//接口 - 设置资源
											//		【参数1】：文件父目录
											//		【参数2】：不带文件后缀，文件可重名
		void setSource(QString gif_src_file, QList<QString> gif_src);

											//接口 - 设置帧间隔
											//		【参数1】：默认帧间隔
											//		【参数2】：帧间隔 明细表
		void setInterval(int gif_interval, QList<int> gif_intervalTank);
											//接口 - 设置默认帧间隔
											//		【参数】：默认帧间隔
											//		【说明】：与原默认帧间隔一样的值，会被统一改变。
		void setIntervalDefaultWithFit(int gif_interval);


	protected:
		void checkInterval();
		void checkIntervalValue();

	//-----------------------------------
	//----获取
	public:
											//访问器 - 获取标识
		int getData_Id();
											//访问器 - 获取默认帧间隔
		int getData_IntervalDefault();
											//访问器 - 获取帧间隔明细表
		QList<int> getData_IntervalTank();
											//访问器 - 单位
											//		【说明】：数据单位以编辑器的为准。单位变化并不影响实际存储的帧间隔。
		C_ALE_DataSet::DATA_UNIT getData_Unit();

											//接口 - 获取文件
		QFileInfo getFile(int index);
											//接口 - 获取文件（多个）
		QList<QFileInfo> getFile_Multi(QList<int> index_list);
											//接口 - 获取全部文件
		QList<QFileInfo> getAllFile();
											//接口 - 获取文件数量
		int getFileCount();
											//接口 - 获取文件路径（F:/aaa/vvv ）
		QString getFileRoot();
											//接口 - 检查文件（不带文件后缀）
		bool hasFileName(QString file_name);

											//接口 - 获取默认帧间隔（含单位转换）
		double getIntervalDefaultWithUnit();
											//接口 - 获取帧间隔（含单位转换）
		double getIntervalWithUnit(int index);
		QList<double> getIntervalWithUnit_Multi(QList<int> index_list);
											//接口 - 获取帧间隔明细表（含单位转换）
		QList<double> getIntervalTankWithUnit();
											//接口 - 获取帧间隔文本（"0.01"，实际帧为 0.01666 * n ）
		QString getIntervalString(int index);

	protected:
		double intervalUnitTransform(int interval);
		

	//-----------------------------------
	//----操作
	public:
									//操作 - 添加
		void op_append(QString gif_src);
		void op_insert(int index, QString gif_src);
		void op_insert(int index, QStringList gif_src_list, QList<int> interval_list = QList<int>());
									//操作 - 替换
		void op_replace(int index, QString gif_src);
		void op_replace_interval(int index, int interval);
		void op_replace_interval(QList<int> index, int interval);
									//操作 - 移除
		void op_remove(int index);
									//操作 - 交换位置
		void op_swap(int index_a, int index_b);
	public:
									//复制文件
		static bool copyFile(QString filePath_from, QString filePath_to);

	//-----------------------------------
	//----类属性
	public:
									//空判断
		bool isNull();
									//实体类 -> QJsonObject
		QJsonObject getJsonObject();
									//QJsonObject -> 实体类
		void setJsonObject(QJsonObject obj);
};