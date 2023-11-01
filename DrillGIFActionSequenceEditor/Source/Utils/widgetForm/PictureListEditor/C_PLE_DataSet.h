#pragma once
#include "stdafx.h"
#include <QList>
#include "Source/ProjectModule/Container/C_ContainerDataBase.h"

/*
-----==========================================================-----
		类：		图片编辑块 数据类.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		图片编辑块 的数据类。
					【该数据类只用于传值，所以不需要 ID或名称 的标识】
					（详细见cpp）
-----==========================================================-----
*/

class C_PLE_DataSet{

	public:
		C_PLE_DataSet();
		~C_PLE_DataSet();
		

	//-----------------------------------
	//----主流程
	public:
											//主流程 - 设置资源
											//		【参数1】：文件父目录
											//		【参数2】：文件名（不带文件后缀，文件可重名）
											//		【说明】：此流程会赋值 父路径 和 资源文件 的参数。
		void setPicList(QString pic_file, QStringList pic_name_list);


	//-----------------------------------
	//----资源文件
	protected:
		QList<QString> picPath_list;		//资源文件列表
	public:
											//资源文件 - 设置
											//		【说明】：纯粹设置数据的函数，不考虑父路径的情况。
		void setData_PicList(QList<QString> picPath_list);
											//资源文件 - 获取
		QList<QString> getData_PicList();
		int getData_PicListCount();

											//资源文件 - 获取 - 根据索引
		QString getPic_ByIndex(int index);
											//资源文件 - 获取 - 根据索引列表
											//		【说明】：若index越界，则返回空字符串。返回的列表长度与index列表长度一致。
		QList<QString> getPic_ByIndexList(QList<int> index_list);
											//资源文件 - 获取 - 全部文件是否在同一文件夹
		bool isPicList_AllInSameDir();
											//资源文件 - 获取 - 父文件目录
											//		【说明】：若不在同一文件夹，则返回空字符串。
		QString getPicList_ParentDir();
		

	//-----------------------------------
	//----父路径（全局数据）
	//				（原数据为字符串列表，即完整的文件路径）
	//				（这里以 父目录+文件名列表 的划分方式来建立函数，并不改变原数据的存储）
	//				【父路径直接为一个文件夹，通过判断文件夹内是否存在文件，来进行覆盖提示。所有 添加文件 的操作都会用到该管理器。】
	protected:
		QString pic_parentDir;
	public:
											//父路径 - 设置
											//		【说明】：设置时会自动去掉末尾的"/"。
		void setData_ParentDir(QString parentDir);
											//父路径 - 获取
		QString getData_ParentDir();
											//父路径 - 资源中是否有文件名
											//		【说明】：参数为不带文件后缀的文件名。
		bool hasPicName(QString pic_filename);


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