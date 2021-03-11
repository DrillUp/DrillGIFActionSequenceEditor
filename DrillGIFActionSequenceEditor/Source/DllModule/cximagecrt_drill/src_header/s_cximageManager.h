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

		子功能：	-> 拆解GIF
					-> 合成GIF
					-> 获取GIF数据及信息

		使用方法：
				> 合成：
					QList<QFileInfo> info_list = QList<QFileInfo>();
					info_list.push_back(QFileInfo("F:/新建文件夹/0.png"));
					info_list.push_back(QFileInfo("F:/新建文件夹/1.png"));
					info_list.push_back(QFileInfo("F:/新建文件夹/2.png"));
					S_GIFManager::getInstance()->generateGIF(info_list, QFileInfo("F:/aaa.gif"),4);
				> 拆解：
					S_GIFManager::getInstance()->dismantlingGIF(QFileInfo("F:/gif箱/aaa.gif"),QDir("F:/新建文件夹/"),"png");

		说明：		【该接口来自cximagecrt_drill工程生成的dll】
					图片分为8位索引、24位、32位等类型，gif只能支持8位索引色，并且所有图片用的同一张索引色表。
					所以生成图片是巨坑，用了我整整7天的时间。【如果你要深入，必须先了解图片格式与压缩原理】
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
		QString m_lastDismantledError;		//错误信息
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
											//拆解 - 获取拆解时错误信息
		QString getLastDismantledError();
		
	//-----------------------------------
	//----合成GIF
	private:
		QString m_lastGenerateError;		//错误信息
	public:
											//合成 - 执行合成
											//		【参数1】：合成的图片文件路径
											//		【参数2】：保存的GIF路径
											//		【参数3】：默认帧间隔（单位0.01秒）
											//		【参数4】：自定义帧间隔列表
		bool generateGIF(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList = QList<int>());
											//合成 - 获取合成时错误信息
		QString getLastGenerateError();


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
											//		【说明2】：输入的图片如果高于8位深度，会被降低为8位图片。
		bool generateGIF_private(QList<QFileInfo> file_list, QFileInfo gif_path, int frame_interval, QList<int> frame_intervalList);

};

#endif // CXIMAGECRT_DRILL_H
