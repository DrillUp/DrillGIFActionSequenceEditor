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
					��GIFÿ֡��׼Ϊ0.01��������Ҫʱע�ⵥλ���㡿

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
bool S_GIFManager::generateGIF(QList<QFileInfo> path_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList, QColor backgroundColor) {

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
		if (max_height < h){
			max_height = h;
		}
	}

	// > ��ʼ����
	GifWriter* writer = new GifWriter();
	GifBegin(writer, gif_path.absoluteFilePath().toLocal8Bit(), max_width, max_height, frame_interval, 8, false);

	// > ����֡
	for (int frame = 0; frame < image_list.count(); frame++){
		QImage image = image_list.at(frame);
		int p_x = (max_width - image.width())/2;	 //(����֡���д���)
		int p_y = (max_height - image.height())/2;

		uint8_t* image_p = new uint8_t[max_width * max_height * 4];
		for (int y = 0; y < max_height; y++){
			for (int x = 0; x < max_width; x++){
				int r = backgroundColor.red();
				int g = backgroundColor.green();
				int b = backgroundColor.blue();
				int a = 255;
				int xx = x - p_x;
				int yy = y - p_y;
				if (xx >= 0 && xx < image.width() &&
					yy >= 0 && yy < image.height() ){
					QRgb rgb = image.pixel(xx, yy);
					r = qRed(rgb);
					g = qGreen(rgb);
					b = qBlue(rgb);
					a = qAlpha(rgb);
					if (r == 0 && g == 0 && b == 0 && a == 0){
						r = backgroundColor.red();
						g = backgroundColor.green();
						b = backgroundColor.blue();
						a = 255;
					}else if (a < 255){		// �ںϱ���
						r = (r*a + backgroundColor.red()  *(255 - a)) / 255;
						g = (g*a + backgroundColor.green()*(255 - a)) / 255;
						b = (b*a + backgroundColor.blue() *(255 - a)) / 255;
						a = 255;
					}
				}
				image_p[(x + y*max_width) * 4 + 0] = r;
				image_p[(x + y*max_width) * 4 + 1] = g;
				image_p[(x + y*max_width) * 4 + 2] = b;
				image_p[(x + y*max_width) * 4 + 3] = a;	//������͸��û�ã����ɺ�ɫ��
			}
		}

		// > ���֡
		int f_i = frame_interval;
		if (frame < frame_intervalList.count()){
			f_i = frame_intervalList.at(frame);
		}
		GifWriteFrame(writer, image_p, max_width, max_height, f_i, 8, false);
	}

	// > ����gif����
	GifEnd(writer);
	delete writer;

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
	if (image_dir_path.exists() == false){ return false; }//���������ȡ���ĵ�λ��0.001�룬������ʵֵҪ����10����
	
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
	QList<int> result_list = this->m_lastIntervalList;
	for (int i = 0; i < result_list.count(); i++){
		result_list.replace(i, result_list.at(i)*0.1);		//��ȡ����֡��λ�Ǻ��룬ֵ����10������Ҫͳһ
	}
	return result_list;
}
/*-------------------------------------------------
		��� - ��ȡ������ļ��б�
*/
QList<QFileInfo> S_GIFManager::getLastDismantledGIFFileList(){
	return this->m_lastFileList;
}