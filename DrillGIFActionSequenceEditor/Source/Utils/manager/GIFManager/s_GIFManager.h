#pragma once

/*
-----==========================================================-----
		�ࣺ		GIFת����.h
		�汾��		v1.02
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�ṩGIF�ļ�ת���ȹ��ܡ�
					��GIFÿ֡��׼Ϊ0.01��������Ҫʱע�ⵥλ���㡿
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
										//		������5����ǿ����Ϳ�ı���
		bool generateGIF(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList = QList<int>(), QColor backgroundColor = QColor(255,255,255));
		
		
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

										//��� - ��ȡ�����֡�������
		QList<int> getLastDismantledGIFIntervalList();
										//��� - ��ȡ������ļ��б�
		QList<QFileInfo> getLastDismantledGIFFileList();

};
