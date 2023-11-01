#include "stdafx.h"
#include "S_PLE_FileManager.h"

#include "Source/Utils/Common/P_FileOperater.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 文件夹管理器.cpp
		所属模块：	示例模块
		功能：		动画帧编辑块 的文件夹管理器。
					【父路径直接为一个文件夹，通过判断文件夹内是否存在文件，来进行覆盖提示。所有 添加文件 的操作都会用到该管理器。】
-----==========================================================-----
*/

S_PLE_FileManager::S_PLE_FileManager(){
}
S_PLE_FileManager::~S_PLE_FileManager() {
}

/* --------------------------------------------------------------
----------单例
*/
S_PLE_FileManager* S_PLE_FileManager::cur_manager = nullptr;
S_PLE_FileManager* S_PLE_FileManager::getInstance() {
	if (cur_manager == nullptr) {
		cur_manager = new S_PLE_FileManager();
	}
	return cur_manager;
}


/*-------------------------------------------------
		管理器 - 添加 单个图片文件 到文件夹
*/
QString S_PLE_FileManager::addPicFile_ToDir(QString fileFullPath, QString parentDir){
	QFileInfo cur_file_info(fileFullPath);
	if (cur_file_info.exists() == false){ return ""; }
	QDir tar_dir(parentDir);
	if (tar_dir.exists() == false){ return ""; }
	
	// > 图片文件路径就是父路径时，直接关联名称
	if (tar_dir.absolutePath() == cur_file_info.absolutePath()){
		return fileFullPath;
	}

	// > 重复文件时
	//			【父路径直接为一个文件夹，通过判断文件夹内是否存在文件，来进行覆盖提示。所有添加文件操作都会调用到 S_PLE_FileManager 文件夹管理器。】
	QString curFileBaseName = cur_file_info.completeBaseName();
	QString new_filePath = tar_dir.absoluteFilePath(curFileBaseName + ".png");			//（所有文件都以png为准）
	QString old_filePath = this->getFile_InDir(curFileBaseName, parentDir);				//（文件夹下可以放txt，但是不能和png重名）
	qDebug() << parentDir;
	qDebug() << new_filePath;
	qDebug() << old_filePath;
	P_FileOperater op;
	if (old_filePath != ""){

		QString context;
		context.append("要导入的图片\"");
		context.append(curFileBaseName);
		context.append("\"在配置中已存在，是否覆盖原文件？");
		context.append("\n");
		context.append("（覆盖后，其他关联该名称的块都会使用覆盖的文件）");
		context.append("\n");
		context.append("（建议在添加资源前，规划整理一下文件名）");

		switch (QMessageBox::question(nullptr, "提示", context, "覆盖新资源", "使用已存在的资源", "取消", 0)) {
			case 0:
				op.remove_File(new_filePath);
				op.copy_File(cur_file_info.absoluteFilePath(), new_filePath);
				emit signal_picCovered(curFileBaseName);	//（覆盖信号）
				return new_filePath;
				break;
			case 1:
				return old_filePath;
				break;
			case 2:
			default:
				return "";
				break;
		}
	}

	// > 不重复文件
	op.copy_File(cur_file_info.absoluteFilePath(), new_filePath);
	emit signal_picAdded(curFileBaseName);	//（添加信号）

	return new_filePath;
}

/*-------------------------------------------------
		管理器 - 资源父路径下是否有文件
*/
bool S_PLE_FileManager::hasFile_InDir(QString fileBaseName, QString parentDir){
	return this->getFile_InDir(fileBaseName, parentDir) != "";
}
/*-------------------------------------------------
		管理器 - 获取资源父路径的文件
*/
QString S_PLE_FileManager::getFile_InDir(QString fileBaseName, QString parentDir){
	QDir dir(parentDir);
	QFileInfoList file_info_list = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < file_info_list.count(); i++){
		QFileInfo file_info = file_info_list.at(i);
		if (file_info.completeBaseName() == fileBaseName){
			return file_info.absoluteFilePath();
		}
	}
	return "";
}

