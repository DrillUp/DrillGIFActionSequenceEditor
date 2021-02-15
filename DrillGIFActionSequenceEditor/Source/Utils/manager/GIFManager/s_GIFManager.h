#pragma once

/*
-----==========================================================-----
		�ࣺ		GIFת����.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ṩGIF�ļ�ת���ȹ��ܡ�
					����ϸ��.cpp��
-----==========================================================-----
*/

class S_GIFManager : public QObject
{
	Q_OBJECT

	public:
		S_GIFManager();
		~S_GIFManager();
		static S_GIFManager* cur_manager;		//����
		static S_GIFManager* getInstance();		//��������ȡ�Լ�
		
	//-----------------------------------
	//----�ϳ�GIF
	public:
										//�ϳ� - ִ�кϳ�
										//		������1�����ϳɵ�ͼƬ�ļ�·��
										//		������2���������GIF·��
										//		������3����Ĭ��֡�������λ0.01�룩
										//		������4�����Զ���֡����б�
		bool generateGIF(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval);
		bool generateGIF(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList);
		
		
	//-----------------------------------
	//----���GIF
	public:
										//��� - ִ�в��
										//		������1����GIF��·��
										//		������2����������ļ���·��
										//		������3����������ļ���׺
										//		������4����������ļ�����%1��ʾ���֣���"С����˿%1"��
		bool dismantlingGIF(QFileInfo gif_path, QDir image_dir_path, char* suffix);
		bool dismantlingGIF(QFileInfo gif_path, QDir image_dir_path, char* suffix, QString imageName);
		
};
