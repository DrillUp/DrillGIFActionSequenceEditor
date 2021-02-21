#include "stdafx.h"
#include "s_GIFManager.h"

#include <math.h>
#include "gif.h"
#include <QImage>

/*
-----==========================================================-----
		�ࣺ		GIFת����.cpp
		�汾��		v1.02
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ṩGIF�ļ�ת���ȹ��ܡ�

		Ŀ�꣺		-> ���GIF
					-> �ϳ�GIF

		��ӣ�		��͸����gif����ͼƬ��Ӱ���⡿
						1. �����Ҵ�QMoive�ҵ�QImageReader�����ҵ�QGifHandler��ײ㡣
						2. Ϊ���޸�����ײ㣬�ҽ�Դ���ҵ�����д�˸� p_DrillGIFHandler�����ѱ�ɾ����
						3. ����ײ���ͨ��decode��ÿ֡���롿�ķ�ʽ��ȡ�ġ����޷�ֱ�ӽ���ָ����һ֡��
						4. reader���ṩ�� jumpToFrame ������û��һ���á����˷����Ҵ���ʱ���о���
						5. ��ps����д�gif��ѡ��ȫ��֡���Ҽ����"����"��������gif�����ٳ�����Ӱ���⡣
						6. ����ps�������ô��������Ӱ�ģ���Ҳ��֪����������ĳλ����д�Ľ��������ܹ�������

		ʹ�÷�����
				>�ϳɣ�
					QList<QFileInfo> info_list = QList<QFileInfo>();
					info_list.push_back(QFileInfo("F:/�½��ļ���/0.png"));
					info_list.push_back(QFileInfo("F:/�½��ļ���/1.png"));
					info_list.push_back(QFileInfo("F:/�½��ļ���/2.png"));
					S_GIFManager::getInstance()->generateGIF(info_list, QFileInfo("F:/aaa.gif"),4);
				>��⣺
					S_GIFManager::getInstance()->dismantlingGIF(QFileInfo("F:/gif��/aaa.gif"),QDir("F:/�½��ļ���/"),"png");
-----==========================================================-----
*/

S_GIFManager::S_GIFManager(){
}
S_GIFManager::~S_GIFManager() {
}

/* --------------------------------------------------------------
----------GIFManager ����
*/
S_GIFManager* S_GIFManager::cur_manager = NULL;
S_GIFManager* S_GIFManager::getInstance() {
	if (cur_manager == NULL) {
		cur_manager = new S_GIFManager();
	}
	return cur_manager;
}



/*-------------------------------------------------
		�ϳ� - ִ�кϳɣ�����gif.h��
*/
bool S_GIFManager::generateGIF(QList<QFileInfo> path_list, QFileInfo gif_path, int frame_interval) {
	return this->generateGIF(path_list, gif_path, frame_interval, QList<int>());
}
bool S_GIFManager::generateGIF(QList<QFileInfo> path_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList) {

	// > ��ȡͼƬ
	QList<QImage> image_list = QList<QImage>();
	for (int i = 0; i < path_list.count(); i++){
		QFileInfo info = path_list.at(i);
		QImage image = QImage();
		image.load(info.absoluteFilePath());
		image_list.push_back(image);
	}

	// > ��ȡ�߿�
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

	// > ��ʼ����
	QByteArray ba;
	ba.append(gif_path.absoluteFilePath());
	GifWriter writer = {};
	GifBegin(&writer, ba.data(), max_width, max_height, frame_interval, 8, true);

	// > ����֡
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

		// > ���֡
		int f_i = frame_interval;
		if (frame < frame_intervalList.count()){
			f_i = frame_intervalList.at(frame);
		}
		GifWriteFrame(&writer, image_p, max_width, max_height, f_i, 8, true);
	}

	// > ����gif����
	GifEnd(&writer);

	return true;
}

/*-------------------------------------------------
		��� - ִ�в�⣨qt�Դ���
*/
bool S_GIFManager::dismantlingGIF(QFileInfo gif_path, QDir image_dir_path, char* suffix){
	return this->dismantlingGIF(gif_path, image_dir_path, suffix, "%2_%1");
}
bool S_GIFManager::dismantlingGIF(QFileInfo gif_path, QDir image_dir_path, char* suffix, QString imageName){
	if (gif_path.exists() == false){ return false; }
	if (image_dir_path.exists() == false){ return false; }
	
	// > ��ͨ�취
	//			���ڲ���͸��GIF�ļ��У��������Ӱ��
	//			����GIF�ļ������⣬���е�GIF���������޷������Ӱ���⡿
	QImageReader* reader = new QImageReader();		//��QMovie����QImageReader��
	reader->setFileName(gif_path.absoluteFilePath());
	this->m_lastFileList.clear();
	this->m_lastIntervalList.clear();
	
	// > ������ʾ
	int image_count = reader->imageCount();
	if (image_count > 100){
		QMessageBox box(QMessageBox::Question, "��ʾ", "GIF��֡������100���ٶȽ������Ƿ������");
		box.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		box.setButtonText(QMessageBox::Ok, QString("����"));
		box.setButtonText(QMessageBox::Cancel, QString("ȡ��"));
		if (box.exec() == QMessageBox::Cancel){ return false; };
	}
	
	// > ��ȡ֡
	bool has_transparentArea = false;
	for (int i = 0; i < image_count; i++){
		reader->jumpToNextImage();
		
		// > ��ȡ
		QImage image = reader->read();
		QFileInfo fileinfo(image_dir_path.absolutePath() + QString("/" + imageName + ".").arg(i).arg(gif_path.completeBaseName()) + suffix);
		this->m_lastFileList.append(fileinfo);
		this->m_lastIntervalList.append(reader->nextImageDelay());
		
		// > ���͸������
		QRgb rgb = image.pixel(0, 0);
		int alpha = qAlpha(rgb);
		if (alpha < 255 && has_transparentArea == false){
			has_transparentArea = true;
		}

		// > д���ļ�
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
		��� - ��ȡ�����֡�������
*/
QList<int> S_GIFManager::getLastDismantledGIFIntervalList(){
	return this->m_lastIntervalList;
}
/*-------------------------------------------------
		��� - ��ȡ������ļ��б�
*/
QList<QFileInfo> S_GIFManager::getLastDismantledGIFFileList(){
	return this->m_lastFileList;
}