#pragma once
#include "stdafx.h"

/*
-----==========================================================-----
		类：		图片缓存容器.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		缓存容器，资源路径名称 与 资源图片 一对一。
					（详细见cpp）
-----==========================================================-----
*/

class S_PictureBitmapCache{

	public:
		S_PictureBitmapCache();
		~S_PictureBitmapCache();
		static S_PictureBitmapCache* cur_manager;		//单例
		static S_PictureBitmapCache* getInstance();		//单例，获取自己
		
	//-----------------------------------
	//----容器
	protected:
		QStringList m_pathList;			//资源路径名称
		QList<QPixmap> m_bitmapList;	//资源图片
	public:
										//容器 - 清理全部
		void clearAll();
										//容器 - 添加路径
										//		【说明】：此函数重复调用会刷新QPixmap。因此需要先用hasPath判断一下再添加。
		void addPath(QString path);
										//容器 - 是否含有路径
		bool hasPath(QString path);

										//容器 - 刷新路径
										//		【说明】：如果资源图片文件发生了变化，执行此函数。
		void refreshPath(QString path);
										//容器 - 获取资源图片
										//		【说明】：如果不含指定路径，则返回空图片对象。
		QPixmap getBitmapByPath(QString path);

};

