#include "stdafx.h"
#include "s_GIFManager.h"

#include <math.h>
#include "gif.h"
#include <QImage>

/*
-----==========================================================-----
		类：		GIF转换器.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		提供GIF文件转换等功能。

		目标：		-> 拆解GIF
					-> 合成GIF

		大坑：		【透明的gif会有图片重影问题】
						1. 于是我从QMoive找到QImageReader，再找到QGifHandler最底层。
						2. 为了修改这个底层，我将源码找到，新写了个 p_DrillGIFHandler。（已被删除）
						3. 这个底层是通过decode【每帧解码】的方式读取的。【无法直接解码指定的一帧】
						4. reader中提供的 jumpToFrame 函数【没有一点用】，浪费了我大量时间研究。
						5. 在ps软件中打开gif，选择全部帧后，右键点击"处理"，导出的gif将不再出现重影问题。
						6. 至于ps软件是怎么做到不重影的，我也不知道。估计是某位神人写的解码器，能够解析。

		使用方法：
				>合成：
					QList<QFileInfo> info_list = QList<QFileInfo>();
					info_list.push_back(QFileInfo("F:/新建文件夹/0.png"));
					info_list.push_back(QFileInfo("F:/新建文件夹/1.png"));
					info_list.push_back(QFileInfo("F:/新建文件夹/2.png"));
					S_GIFManager::getInstance()->generateGIF(info_list, QFileInfo("F:/aaa.gif"),4);
				>拆解：
					S_GIFManager::getInstance()->dismantlingGIF(QFileInfo("F:/gif箱/aaa.gif"),QDir("F:/新建文件夹/"),"png");
-----==========================================================-----
*/

S_GIFManager::S_GIFManager(){
}
S_GIFManager::~S_GIFManager() {
}

/* --------------------------------------------------------------
----------GIFManager 单例
*/
S_GIFManager* S_GIFManager::cur_manager = NULL;
S_GIFManager* S_GIFManager::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_GIFManager();
	}
	return cur_manager;
}



/*-------------------------------------------------
		合成 - 执行合成（基于gif.h）
*/
bool S_GIFManager::generateGIF(QList<QFileInfo> path_list, QFileInfo gif_path, int frame_interval) {
	return this->generateGIF(path_list, gif_path, frame_interval, QList<int>());
}
bool S_GIFManager::generateGIF(QList<QFileInfo> path_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList) {

	// > 读取图片
	QList<QImage> image_list = QList<QImage>();
	for (int i = 0; i < path_list.count(); i++){
		QFileInfo info = path_list.at(i);
		QImage image = QImage();
		image.load(info.absoluteFilePath());
		image_list.push_back(image);
	}

	// > 获取高宽
	int max_width = 0;
	int max_height = 0;
	for (int i = 0; i < image_list.count(); i++){
		QImage image = image_list.at(i);
		int w = image.width();
		int h = image.height();
		if (max_width < w){
			max_width = w;
		}
		if (max_height < w){
			max_height = w;
		}
	}

	// > 开始建立
	QByteArray ba;
	ba.append(gif_path.absoluteFilePath());
	GifWriter writer = {};
	GifBegin(&writer, ba.data(), max_width, max_height, frame_interval, 8, true);

	// > 动画帧
	for (int frame = 0; frame < image_list.count(); frame++){
		QImage image = image_list.at(frame);

		uint8_t* image_p = new uint8_t[max_width * max_height * 4];
		for (int x = 0; x < max_width; x++){
			for (int y = 0; y < max_height; y++){
				QColor color = image.pixel(y,x);
				//if (color.alpha() == 0){
				//	image_p[(x*max_width + y) * 4 + 0] = 255;
				//	image_p[(x*max_width + y) * 4 + 1] = 255;
				//	image_p[(x*max_width + y) * 4 + 2] = 255;
				//	image_p[(x*max_width + y) * 4 + 3] = 255;
				//	continue;
				//}
				image_p[(x*max_width + y) * 4 + 0] = color.red();
				image_p[(x*max_width + y) * 4 + 1] = color.green();
				image_p[(x*max_width + y) * 4 + 2] = color.blue();
				image_p[(x*max_width + y) * 4 + 3] = color.alpha();
			}
		}

		// > 添加帧
		int f_i = frame_interval;
		if (frame < frame_intervalList.count()){
			f_i = frame_intervalList.at(frame);
		}
		GifWriteFrame(&writer, image_p, max_width, max_height, f_i, 8, true);
	}

	// > 结束gif绘制
	GifEnd(&writer);

	return true;
}

/*-------------------------------------------------
		拆解 - 执行拆解（qt自带）
*/
bool S_GIFManager::dismantlingGIF(QFileInfo gif_path, QDir image_dir_path, char* suffix){
	return this->dismantlingGIF(gif_path, image_dir_path, suffix, "%2_%1");
}
bool S_GIFManager::dismantlingGIF(QFileInfo gif_path, QDir image_dir_path, char* suffix, QString imageName){
	if (gif_path.exists() == false){ return false; }
	if (image_dir_path.exists() == false){ return false; }
	
	// > 普通办法
	//			【在部分透明GIF文件中，会出现重影】
	//			【是GIF文件的问题，现有的GIF解析器，无法解决重影问题】
	QImageReader* reader = new QImageReader();		//（QMovie基于QImageReader）
	reader->setFileName(gif_path.absoluteFilePath());
	this->m_lastFileList.clear();
	this->m_lastIntervalList.clear();
	
	// > 数量提示
	int image_count = reader->imageCount();
	if (image_count > 100){
		QMessageBox box(QMessageBox::Question, "提示", "GIF的帧数超过100，速度较慢，是否继续？");
		box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		box.setButtonText(QMessageBox::Ok, QString("继续"));
		box.setButtonText(QMessageBox::Cancel, QString("取消"));
		if (box.exec() == QMessageBox::Cancel){ return false; };
	}
	
	// > 读取帧
	bool has_transparentArea = false;
	for (int i = 0; i < image_count; i++){
		reader->jumpToNextImage();
		
		// > 读取
		QImage image = reader->read();
		QFileInfo fileinfo(image_dir_path.absolutePath() + QString("/" + imageName + ".").arg(i).arg(gif_path.completeBaseName()) + suffix);
		this->m_lastFileList.append(fileinfo);
		this->m_lastIntervalList.append(reader->nextImageDelay());
		
		// > 检测透明区域
		QRgb rgb = image.pixel(0, 0);
		int alpha = qAlpha(rgb);
		if (alpha < 255 && has_transparentArea == false){
			has_transparentArea = true;
		}

		// > 写入文件
		QFile file(fileinfo.absoluteFilePath());
		file.open(QFile::WriteOnly);
		image.save(&file, suffix);
		file.close();

		image.fill(QColor(0, 0, 255, 255));
	}
	delete reader;

	return true;
}

/*-------------------------------------------------
		拆解 - 获取拆解后的帧间隔数据
*/
QList<int> S_GIFManager::getLastDismantledGIFIntervalList(){
	return this->m_lastIntervalList;
}
/*-------------------------------------------------
		拆解 - 获取拆解后的文件列表
*/
QList<QFileInfo> S_GIFManager::getLastDismantledGIFFileList(){
	return this->m_lastFileList;
}