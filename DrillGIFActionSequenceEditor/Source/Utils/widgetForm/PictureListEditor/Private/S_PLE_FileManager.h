#pragma once
#include "stdafx.h"
#include <QObject>

/*
-----==========================================================-----
		类：		动画帧编辑块 文件夹管理器.cpp
		所属模块：	示例模块
		功能：		动画帧编辑块 的文件夹管理器。
					【父路径直接为一个文件夹，通过判断文件夹内是否存在文件，来进行覆盖提示。所有 添加文件 的操作都会用到该管理器。】
-----==========================================================-----
*/

class S_PLE_FileManager : public QObject{

	Q_OBJECT

	protected:
		S_PLE_FileManager();
		~S_PLE_FileManager();
	public:
		static S_PLE_FileManager* cur_manager;			//单例
		static S_PLE_FileManager* getInstance();		//单例，获取自己（必须要拿到全局配置才能进行计算）
		

	//-----------------------------------
	//----文件夹管理器
	public:
										//管理器 - 添加 单个图片文件 到文件夹
										//		【参数1】：资源文件全路径
										//		【参数2】：目标文件夹的全路径
										//		【返回】：新加文件的全路径，如果没成功则返回空字符串
		QString addPicFile_ToDir(QString fileFullPath, QString parentDir);

										//管理器 - 资源父路径下是否有文件
										//		【说明】：fileBaseName为不需要加后缀的文件名，该函数不考虑多后缀名情况。（如aaa.png和aaa.txt）
		virtual bool hasFile_InDir(QString fileBaseName, QString parentDir);
										//管理器 - 获取资源父路径的文件
										//		【说明】：fileBaseName为不需要加后缀的文件名，该函数不考虑多后缀名情况。（如aaa.png和aaa.txt）
		virtual QString getFile_InDir(QString fileBaseName, QString parentDir);
		

	//-----------------------------------
	//----文件变化信号
	public:
	signals:
										//变化信号 - 文件被添加
		void signal_picAdded(QString pic_name);
	public:
	signals:
										//变化信号 - 文件被覆盖
		void signal_picCovered(QString pic_name);
	public:
	signals:
										//变化信号 - 文件被移除（暂时还没有）
		void signal_picRemoved(QString pic_name);

};

