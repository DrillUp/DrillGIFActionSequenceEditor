#include "stdafx.h"
#include "S_ALE_FileManager.h"

#include "Source/DllModule/Cximagecrt_drill/Src_header/S_cximageManager.h"
#include "Source/Utils/Manager/GIFManager/S_GIFManager.h"
#include "Source/Utils/Common/P_FileOperater.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 文件夹管理器.cpp
		所属模块：	示例模块
		功能：		动画帧编辑块 的文件夹管理器。
					【父路径直接为一个文件夹，通过判断文件夹内是否存在文件，来进行覆盖提示。所有 添加文件 的操作都会用到该管理器。】
-----==========================================================-----
*/

S_ALE_FileManager::S_ALE_FileManager() : S_PLE_FileManager(){
	this->m_lastGIF_FileList.clear();
	this->m_lastGIF_IntervalList.clear();
}
S_ALE_FileManager::~S_ALE_FileManager() {
}

/* --------------------------------------------------------------
----------单例
*/
S_ALE_FileManager* S_ALE_FileManager::cur_manager = nullptr;
S_ALE_FileManager* S_ALE_FileManager::getInstance() {
	if (cur_manager == nullptr) {
		cur_manager = new S_ALE_FileManager();
	}
	return cur_manager;
}


/*-------------------------------------------------
		管理器 - 添加 GIF文件 到文件夹
*/
bool S_ALE_FileManager::addGIFFile_ToDir(QString fileFullPath, int readerMethod, QString parentDir){
	QFileInfo cur_file_info(fileFullPath);
	if (cur_file_info.exists() == false){ return false; }
	QDir tar_dir(parentDir);
	if (tar_dir.exists() == false){ return false; }

	// > 重复文件检查
	bool is_cover = false;
	QFileInfo repeat_file = QFileInfo(tar_dir.absoluteFilePath(cur_file_info.completeBaseName() + "_0" + ".png"));
	if (repeat_file.exists() == true){
		is_cover = true;

		QString context;
		context.append("要导入的GIF\"");
		context.append(cur_file_info.completeBaseName());
		context.append("\"在配置中已存在，是否覆盖原文件？");
		context.append("\n");
		context.append("（覆盖后，其他关联该名称的块都会使用覆盖的文件）");
		context.append("\n");
		context.append("（建议在添加资源前，规划整理一下文件名）");

		if (QMessageBox::question(nullptr, "提示", context, "覆盖", "取消导入", 0) == 1) {
			return false;
		}
	}

	// > QImageReader读取
	if (readerMethod == 0){

		// > 拆解GIF到目录
		bool successed = S_GIFManager::getInstance()->dismantlingGIF(cur_file_info, tar_dir, "png", "%2_%1");
		if ( successed == false){
			QMessageBox::warning(nullptr, "错误", "GIF解析失败。", QMessageBox::Yes);
			return false;
		}

		// > 获取GIF信息
		this->m_lastGIF_IntervalList = S_GIFManager::getInstance()->getLastDismantledGIFIntervalList();
		this->m_lastGIF_FileList = S_GIFManager::getInstance()->getLastDismantledGIFFileList();

		// > 发出信号
		for (int i = 0; i < this->m_lastGIF_FileList.count(); i++){
			QFileInfo file_info = this->m_lastGIF_FileList.at(i);
			if (is_cover == true){
				emit signal_picCovered(file_info.completeBaseName());
			}else{
				emit signal_picAdded(file_info.completeBaseName());
			}
		}
		return true;

	// > cximage读取
	}else if (readerMethod == 1){
		
		// > 拆解GIF到目录
		bool successed = S_cximageManager::getInstance()->dismantlingGIF(cur_file_info, tar_dir, "png", "%2_%1");
		if (successed == false){
			QMessageBox::warning(nullptr, "错误", "GIF解析失败。", QMessageBox::Yes);
			return false;
		}

		// > 获取GIF信息
		this->m_lastGIF_IntervalList = S_cximageManager::getInstance()->getLastDismantledGIFIntervalList();
		this->m_lastGIF_FileList = S_cximageManager::getInstance()->getLastDismantledGIFFileList();
		return true;
	}
	return false;
}
/*-------------------------------------------------
		管理器 - 获取上一个GIF的分解文件列表
*/
QList<QFileInfo> S_ALE_FileManager::getGIF_LastFileList(){
	return this->m_lastGIF_FileList;
}
/*-------------------------------------------------
		管理器 - 获取上一个GIF的帧间隔列表
*/
QList<int> S_ALE_FileManager::getGIF_LastIntervalList(){
	return this->m_lastGIF_IntervalList;
}


/*-------------------------------------------------
		管理器 - 添加 序列大图文件 到文件夹
*/
QStringList S_ALE_FileManager::addSeqFile_ToDir(QString fileFullPath, QList<QPixmap> bitmap_list, QString parentDir){
	QFileInfo cur_file_info(fileFullPath);
	if (cur_file_info.exists() == false){ return QStringList(); }
	QDir tar_dir(parentDir);
	if (tar_dir.exists() == false){ return QStringList(); }
	if (bitmap_list.count() == 0){ return QStringList(); }
	

	// > 重复文件处理
	bool is_cover = false;
	QFileInfo repeat_file = QFileInfo(tar_dir.absoluteFilePath(cur_file_info.completeBaseName() + "_切片_0" + ".png"));
	if (repeat_file.exists() == true){
		is_cover = true;

		QString context;
		context.append("要导入的序列大图\"");
		context.append(cur_file_info.completeBaseName());
		context.append("\"在配置中已存在，是否覆盖原文件？");
		context.append("\n");
		context.append("（覆盖后，其他关联该名称的块都会使用覆盖的文件）");
		context.append("\n");
		context.append("（建议在添加资源前，规划整理一下文件名）");

		if (QMessageBox::question(nullptr, "提示", context, "覆盖", "取消导入", 0) == 1) {
			return QStringList();
		}
	}
	
	// > 保存切割的图片
	QStringList result_list;
	for (int i = 0; i < bitmap_list.count(); i++){
		QPixmap bitmap = bitmap_list.at(i);
		QString fileBaseName = cur_file_info.completeBaseName() + "_切片_" + QString::number(i);
		QString path = tar_dir.absoluteFilePath(fileBaseName + ".png");
		bitmap.toImage().save(path, "png");
		result_list.append(path);

		// > 发出信号
		if (is_cover == true){
			emit signal_picCovered(fileBaseName);
		}else{
			emit signal_picAdded(fileBaseName);
		}
	}

	return result_list;
}
