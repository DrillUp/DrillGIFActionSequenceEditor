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
		类：		图像库 操作管理器.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		导出图像库的部分操作功能。
					（详细见.cpp）
-----==========================================================-----
*/
class CxImage;
class CXIMAGECRT_DRILL_EXPORT S_cximageManager
{
	public:
		S_cximageManager();
		~S_cximageManager();
		static S_cximageManager* cur_manager;		//单例
		static S_cximageManager* getInstance();		//单例，获取自己
		void _init();								//初始化


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
		
	//-----------------------------------
	//----合成GIF
	public:
											//合成 - 执行合成
											//		【参数1】：合成的图片文件路径
											//		【参数2】：保存的GIF路径
											//		【参数3】：默认帧间隔（单位0.01秒）
											//		【参数4】：自定义帧间隔列表
		bool generateGIF(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList = QList<int>());
		
	//-----------------------------------
	//----图像操作（私有）
	protected:
		CxImage* m_CxImage;
	protected:
											//操作 - 执行拆解（私有）
											//		【说明1】：输入的路径不能有中文。
											//		【说明2】：拆解的GIF直接在当前文件夹建立。
		bool dismantlingGIF_private(QFileInfo gif_path, char* suffix);
											//操作 - 获取gif帧数（私有）
											//		【说明】：输入的路径不能有中文。
		int getGIFFrameCount_private(QFileInfo gif_path);
											//操作 - 执行合成（私有）
											//		【说明1】：输入的路径不能有中文。
		bool generateGIF_private(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList);

};

#endif // CXIMAGECRT_DRILL_H
