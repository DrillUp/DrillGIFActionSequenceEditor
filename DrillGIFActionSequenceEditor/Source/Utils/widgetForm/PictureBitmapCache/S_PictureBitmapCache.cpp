#include "stdafx.h"
#include "S_PictureBitmapCache.h"

/*
-----==========================================================-----
		类：		图片缓存容器.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		缓存容器，资源全路径名称 与 资源图片 一对一。
-----==========================================================-----
*/

S_PictureBitmapCache::S_PictureBitmapCache(){
	this->clearAll();
}
S_PictureBitmapCache::~S_PictureBitmapCache() {
}

/* --------------------------------------------------------------
---------- 单例
*/
S_PictureBitmapCache* S_PictureBitmapCache::cur_manager = NULL;
S_PictureBitmapCache* S_PictureBitmapCache::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_PictureBitmapCache();
	}
	return cur_manager;
}


/*-------------------------------------------------
		容器 - 清理全部
*/
void S_PictureBitmapCache::clearAll(){
	this->m_pathList.clear();
	this->m_bitmapList.clear();
}
/*-------------------------------------------------
		容器 - 添加路径
*/
void S_PictureBitmapCache::addPath(QString path){
	
	// > 若已存在，则刷新资源
	if (this->m_pathList.contains(path)){
		this->refreshPath(path);
		return;
	}

	// > 添加路径
	QImage image = QImage(path);
	QPixmap pixmap = QPixmap::fromImage(image);
	this->m_pathList.append(path);
	this->m_bitmapList.append(pixmap);
}
/*-------------------------------------------------
		容器 - 是否含有路径
*/
bool S_PictureBitmapCache::hasPath(QString path){
	return this->m_pathList.contains(path);
}

/*-------------------------------------------------
		容器 - 刷新路径
*/
void S_PictureBitmapCache::refreshPath(QString path){
	int index = this->m_pathList.indexOf(path);
	if (index == -1){ return; }

	// > 刷新路径
	QImage image = QImage(path);
	QPixmap pixmap = QPixmap::fromImage(image);
	this->m_bitmapList.replace(index, pixmap);
}
/*-------------------------------------------------
		容器 - 获取资源图片
*/
QPixmap S_PictureBitmapCache::getBitmapByPath(QString path){
	int index = this->m_pathList.indexOf(path);
	if (index == -1){ return QPixmap(); }
	return this->m_bitmapList.at(index);
}