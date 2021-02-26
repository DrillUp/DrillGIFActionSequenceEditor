#pragma once
#include <QObject>

/*
-----==========================================================-----
		类：		文件操作器.h
		版本：		v1.21
		所属模块：	工具模块
		功能：		提供最基础的文件操作功能。
					（详细内容见.cpp）
-----==========================================================-----
*/
class P_FileOperater : public QObject
{
	Q_OBJECT

	public:
		P_FileOperater();
		~P_FileOperater();

	//-----------------------------------
	//----复制
	public:
									//复制 - 文件
		bool copy_File(QFileInfo filePath_from, QFileInfo filePath_to);
									//复制 - 文件夹
		bool copy_Dir(QDir dirPath_from, QDir dirPath_to);
									//复制 - 文件夹（指定子文件夹深度）
		bool copy_DirWithDepth(QDir dirPath_from, QDir dirPath_to, int depth);
									//复制 - 文件夹（所有子文件夹）
		bool copy_DirWithAllSubfolders(QDir dirPath_from, QDir dirPath_to);

	//-----------------------------------
	//----删除
	public:
									//删除 - 文件夹
		bool remove_Dir(QDir dirPath);
									//删除 - 文件
		bool remove_File(QFileInfo filePath);
									//删除 - 文件（指定后缀）
		bool remove_FileBySuffix(QDir dirPath, QString suffix);
									//删除 - 文件（指定后缀 + 所有子文件夹）
		bool remove_FileBySuffix_WithAllSubfolders(QDir dirPath, QString suffix);
									//删除 - 文件（指定不含后缀的名称）
		bool remove_FileByNameNoSuffix(QDir dirPath, QString only_basename);
									//删除 - 文件（指定不含后缀的名称 + 所有子文件夹）
		bool remove_FileByNameNoSuffix_WithAllSubfolders(QDir dirPath, QString only_basename);
		
	//-----------------------------------
	//----私有
	protected:
		virtual bool copyFilePrivate(QString filePath_from, QString filePath_to);	//复制文件（私有）
		virtual bool copyDirPrivate(QString dirPath_from, QString dirPath_to);		//复制文件夹（私有，只复制子文件）
		virtual bool copyDirPrivate_recursion(QString dirPath_tarParent, int tar_depth, QString dirPath_curParent, QString dirPath_cur, int cur_depth);	//递归复制文件夹（私有）
		virtual bool removeFilePrivate_recursion(QString nameFilter, int tar_depth, QString dirPath_cur, int cur_depth);		//递归删除文件夹（私有）

};

