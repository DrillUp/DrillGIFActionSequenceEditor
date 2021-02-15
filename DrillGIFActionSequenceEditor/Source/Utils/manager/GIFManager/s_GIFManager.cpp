#include "stdafx.h"
#include "s_GIFManager.h"

#include <math.h>
#include "gif.h"

/*
-----==========================================================-----
		类：		GIF转换器.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		提供GIF文件转换等功能。

		目标：		-> 拆解GIF
					-> 合成GIF
					
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
	return this->dismantlingGIF(gif_path, image_dir_path, suffix, "%1");
}
bool S_GIFManager::dismantlingGIF(QFileInfo gif_path, QDir image_dir_path, char* suffix, QString imageName){
	if (gif_path.exists() == false){ return false; }
	if (image_dir_path.exists() == false){ return false; }
		
	QMovie* movie = new QMovie();
	movie->setFileName(gif_path.absoluteFilePath());
	movie->setCacheMode(QMovie::CacheAll);

	for( int i = 0; i < movie->frameCount(); i++ ){
		movie->jumpToFrame(i);
		QImage image = movie->currentImage();
		
		QFile file(image_dir_path.absolutePath() + QString("/" + imageName + ".").arg(i) + suffix);
		file.open(QFile::WriteOnly);
		image.save(&file, suffix );
		file.close();
	}
	delete movie;
	return true;
}