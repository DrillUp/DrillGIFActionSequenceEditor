#pragma once

/*
-----==========================================================-----
		类：		GIF转换器.h
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		提供GIF文件转换等功能。
					（详细见.cpp）
-----==========================================================-----
*/

class S_GIFManager : public QObject
{
	Q_OBJECT

	public:
		S_GIFManager();
		~S_GIFManager();
		static S_GIFManager* cur_manager;		//单例
		static S_GIFManager* getInstance();		//单例，获取自己
		
	//-----------------------------------
	//----合成GIF
	public:
										//合成 - 执行合成
										//		【参数1】：合成的图片文件路径
										//		【参数2】：保存的GIF路径
										//		【参数3】：默认帧间隔（单位0.01秒）
										//		【参数4】：自定义帧间隔列表
		bool generateGIF(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval);
		bool generateGIF(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList);
		
		
	//-----------------------------------
	//----拆解GIF
	private:
		QList<int> m_lastIntervalList;		//帧间隔数据
		QList<QFileInfo> m_lastFileList;	//拆解的文件列表
	public:
										//拆解 - 执行拆解
										//		【参数1】：GIF的路径
										//		【参数2】：保存的文件夹路径
										//		【参数3】：保存的文件后缀
										//		【参数4】：保存的文件名（%1表示数字，如"小爱丽丝%1"）
		bool dismantlingGIF(QFileInfo gif_path, QDir image_dir_path, char* suffix);
		bool dismantlingGIF(QFileInfo gif_path, QDir image_dir_path, char* suffix, QString imageName);

										//拆解 - 获取拆解后的帧间隔数据
		QList<int> getLastDismantledGIFIntervalList();
		QList<int> getLastDismantledGIFIntervalList_divideTen();
										//拆解 - 获取拆解后的文件列表
		QList<QFileInfo> getLastDismantledGIFFileList();

};
