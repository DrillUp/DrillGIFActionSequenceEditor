#pragma once

/*
-----==========================================================-----
		类：		GIF转换器.h
		版本：		v1.04
		作者：		drill_up
		所属模块：	工具模块
		功能：		提供GIF文件转换等功能。
					【GIF每帧标准为0.01秒间隔，必要时注意单位换算】
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
										//		【参数5】：强制填涂的背景
		bool generateGIF(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList = QList<int>(), QColor backgroundColor = QColor(255,255,255));
		
		
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
										//拆解 - 获取gif帧数
										//		【返回】：0表示打开文件失败 或 读取文件失败。
		int getGIFFrameCount(QFileInfo gif_path);

										//拆解 - 获取拆解后的帧间隔数据
		QList<int> getLastDismantledGIFIntervalList();
										//拆解 - 获取拆解后的文件列表
		QList<QFileInfo> getLastDismantledGIFFileList();

};
