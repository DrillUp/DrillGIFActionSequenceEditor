#pragma once
#include "stdafx.h"
#include <QObject>

#include "Source/Utils/WidgetForm/PictureListEditor/Private/S_PLE_FileManager.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 文件夹管理器.cpp
		所属模块：	示例模块
		功能：		动画帧编辑块 的文件夹管理器。
					【父路径直接为一个文件夹，通过判断文件夹内是否存在文件，来进行覆盖提示。所有 添加文件 的操作都会用到该管理器。】
-----==========================================================-----
*/

class S_ALE_FileManager : public S_PLE_FileManager{

	protected:
		S_ALE_FileManager();
		~S_ALE_FileManager();
	public:
		static S_ALE_FileManager* cur_manager;			//单例
		static S_ALE_FileManager* getInstance();		//单例，获取自己
	

	//-----------------------------------
	//----文件夹管理器（GIF文件）
	//		【注意，这里不含导出的函数，因为不涉及文件夹变化，导出函数见 P_ALE_Editor 】
	protected:
		QList<QFileInfo> m_lastGIF_FileList;
		QList<int> m_lastGIF_IntervalList;
	public:
										//管理器 - 添加 GIF文件 到文件夹
										//		【参数1】：GIF文件全路径
										//		【参数2】：GIF读取方式（0:QImageReader读取，1:cximage读取）
										//		【参数3】：目标文件夹的全路径
										//		【返回】：成功情况，执行后有多个输出数据
		bool addGIFFile_ToDir(QString fileFullPath, int readerMethod, QString parentDir);
										//管理器 - 获取上一个GIF的分解文件列表
		QList<QFileInfo> getGIF_LastFileList();
										//管理器 - 获取上一个GIF的帧间隔列表
		QList<int> getGIF_LastIntervalList();
		

	//-----------------------------------
	//----文件夹管理器（序列大图文件）
	//		【注意，这里不含导出的函数，因为不涉及文件夹变化，导出函数见 P_ALE_Editor 】
	public:
										//管理器 - 添加 序列大图文件 到文件夹
										//		【参数1】：序列大图文件全路径
										//		【参数2】：拆分的贴图列表
										//		【参数3】：目标文件夹的全路径
										//		【返回】：新加文件的全路径列表，如果没成功则返回空列表
		QStringList addSeqFile_ToDir(QString fileFullPath, QList<QPixmap> bitmap_list, QString parentDir);

};

