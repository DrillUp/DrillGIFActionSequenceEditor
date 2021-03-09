#ifndef CXIMAGECRT_DRILL_H
#define CXIMAGECRT_DRILL_H

#include <QtCore/qglobal.h>
#include <QFile>
#include <QFileInfo>

#ifdef CXIMAGECRT_DRILL_LIB
# define CXIMAGECRT_DRILL_EXPORT Q_DECL_EXPORT
#else
# define CXIMAGECRT_DRILL_EXPORT Q_DECL_IMPORT
#endif


/*
-----==========================================================-----
		�ࣺ		ͼ��� ����������.h
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		����ͼ���Ĳ��ֲ������ܡ�
					����ϸ��.cpp��
-----==========================================================-----
*/
class CxImage;
class CXIMAGECRT_DRILL_EXPORT S_cximageManager
{
	public:
		S_cximageManager();
		~S_cximageManager();
		static S_cximageManager* cur_manager;		//����
		static S_cximageManager* getInstance();		//��������ȡ�Լ�
		void _init();								//��ʼ��


	//-----------------------------------
	//----���GIF
	private:
		QList<int> m_lastIntervalList;		//֡�������
		QList<QFileInfo> m_lastFileList;	//�����ļ��б�
	public:
											//��� - ִ�в��
											//		������1����GIF��·��
											//		������2����������ļ���·��
											//		������3����������ļ���׺
											//		������4����������ļ�����%1��ʾ���֣���"С����˿%1"��
		bool dismantlingGIF(QFileInfo gif_path, QDir image_dir_path, char* suffix);
		bool dismantlingGIF(QFileInfo gif_path, QDir image_dir_path, char* suffix, QString imageName);
											//��� - ��ȡgif֡��
											//		�����ء���0��ʾ���ļ�ʧ�� �� ��ȡ�ļ�ʧ�ܡ�
		int getGIFFrameCount(QFileInfo gif_path);

											//��� - ��ȡ�����֡�������
		QList<int> getLastDismantledGIFIntervalList();
											//��� - ��ȡ������ļ��б�
		QList<QFileInfo> getLastDismantledGIFFileList();
		
	//-----------------------------------
	//----�ϳ�GIF
	public:
											//�ϳ� - ִ�кϳ�
											//		������1�����ϳɵ�ͼƬ�ļ�·��
											//		������2���������GIF·��
											//		������3����Ĭ��֡�������λ0.01�룩
											//		������4�����Զ���֡����б�
		bool generateGIF(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList = QList<int>());
		
	//-----------------------------------
	//----ͼ�������˽�У�
	protected:
		CxImage* m_CxImage;
	protected:
											//���� - ִ�в�⣨˽�У�
											//		��˵��1���������·�����������ġ�
											//		��˵��2��������GIFֱ���ڵ�ǰ�ļ��н�����
		bool dismantlingGIF_private(QFileInfo gif_path, char* suffix);
											//���� - ��ȡgif֡����˽�У�
											//		��˵�����������·�����������ġ�
		int getGIFFrameCount_private(QFileInfo gif_path);
											//���� - ִ�кϳɣ�˽�У�
											//		��˵��1���������·�����������ġ�
		bool generateGIF_private(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList);

};

#endif // CXIMAGECRT_DRILL_H
