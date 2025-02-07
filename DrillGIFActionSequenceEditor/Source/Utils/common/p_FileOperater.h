#pragma once
#include "stdafx.h"
#include <QObject>

/*
-----==========================================================-----
		类：		文件操作器.h
		版本：		v1.32
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
									//		【说明】：若目标文件已存在，则会强制覆盖文件。
		bool copy_File(QFileInfo filePath_from, QFileInfo filePath_to);
									//复制 - 文件（自动建立文件夹）
									//		【说明】：若目标文件已存在，则会强制覆盖文件。
		bool copy_FileWithMkpath(QFileInfo filePath_from, QFileInfo filePath_to);
									//复制 - 文件夹（只含文件夹下的子文件）
									//		【说明】：若目标文件夹的文件已存在，则会强制覆盖目标文件夹内文件。目标文件夹未涉及的文件会保留。
		bool copy_Dir(QDir dirPath_from, QDir dirPath_to);
									//复制 - 文件夹（含指定深度的子文件夹）
									//		【说明】：若目标文件夹的文件已存在，则会强制覆盖目标文件夹内文件。目标文件夹未涉及的文件会保留。
		bool copy_DirWithDepth(QDir dirPath_from, QDir dirPath_to, int depth);
									//复制 - 文件夹（含所有子文件夹）
									//		【说明】：若目标文件夹的文件已存在，则会强制覆盖目标文件夹内文件。目标文件夹未涉及的文件会保留。
		bool copy_DirWithAllSubfolders(QDir dirPath_from, QDir dirPath_to);
	protected:
									//复制 - 判断重复文件（私有）
		virtual bool isSameFile(QFileInfo filePath_from, QFileInfo filePath_to);
									//复制 - 文件（私有）
		virtual bool copyFilePrivate(QFileInfo filePath_from, QFileInfo filePath_to);
									//复制 - 文件夹下的子文件（私有）
		virtual bool copyDirPrivate(QString dirPath_from, QString dirPath_to);
									//复制 - 文件（递归方法，深度遍历）（私有）
		virtual bool copyDirPrivate_recursion(QString dirPath_tarParent, int tar_depth, QString dirPath_curParent, QString dirPath_cur, int cur_depth);
									//复制 - 文件（迭代器方法，所有子文件夹）（私有）
		virtual bool copyDirPrivate_iterator(QString dirPath_tarParent, QString dirPath_curParent, QString dirPath_cur);
		

	//-----------------------------------
	//----移动
	public:
									//移动 - 文件
									//		【说明】：若目标文件已存在，则会强制覆盖文件。
									//		【说明】：移动操作的速度非常快，性能消耗也低，能跨盘移动。
		bool move_File(QFileInfo filePath_from, QFileInfo filePath_to);
									//移动 - 文件夹（只含文件夹下的子文件）
									//		【说明】：若目标文件夹已存在，则会强制覆盖目标文件夹内文件。目标文件夹未涉及的文件会保留。
									//		【说明】：移动操作的速度非常快，性能消耗也低，能跨盘移动。
		bool move_Dir(QDir dirPath_from, QDir dirPath_to);
									//移动 - 文件夹（含指定深度的子文件夹）
									//		【说明】：若目标文件夹已存在，则会强制覆盖目标文件夹内文件。目标文件夹未涉及的文件会保留。
									//		【说明】：移动操作的速度非常快，性能消耗也低，能跨盘移动。
		bool move_DirWithDepth(QDir dirPath_from, QDir dirPath_to, int depth);
									//移动 - 文件夹（含所有子文件夹）
									//		【说明】：若目标文件夹已存在，则会强制覆盖目标文件夹内文件。目标文件夹未涉及的文件会保留。
									//		【说明】：移动操作的速度非常快，性能消耗也低，能跨盘移动。
		bool move_DirWithAllSubfolders(QDir dirPath_from, QDir dirPath_to);
									//移动 - 文件夹（完全替换）
									//		【说明】：若目标文件夹已存在，则会强制替换文件夹，注意是整个文件夹。目标文件夹所有文件都没了。
									//		【说明】：移动操作的速度非常快，性能消耗也低，能跨盘移动。
		bool move_DirInReplace(QDir dirPath_from, QDir dirPath_to);
	protected:
									//移动 - 文件夹下的子文件（私有）
		virtual bool moveDirPrivate(QString dirPath_from, QString dirPath_to);
									//移动 - 文件（递归方法，深度遍历）（私有）
		virtual bool moveDirPrivate_recursion(QString dirPath_tarParent, int tar_depth, QString dirPath_curParent, QString dirPath_cur, int cur_depth);
									//移动 - 文件（迭代器方法，所有子文件夹）（私有）
		virtual bool moveDirPrivate_iterator(QString dirPath_tarParent, QString dirPath_curParent, QString dirPath_cur);


	//-----------------------------------
	//----删除
	public:
									//删除 - 文件夹（含所有子文件夹）
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
	protected:
									//删除 - 文件（递归方法，深度遍历）（私有）
		virtual bool removeFilePrivate_recursion(QString nameFilter, int tar_depth, QString dirPath_cur, int cur_depth);
									//删除 - 文件（迭代器方法，所有子文件夹）（私有）
		virtual bool removeFilePrivate_iterator(QString nameFilter, QString dirPath_cur);

		
	//-----------------------------------
	//----查找
	public:
									//查找 - 文件夹下的文件名（只含文件夹下的子文件）
									//		【说明】：如果要找后缀为".facet"的文件，正则为：QRegExp(".*\\.facet")
		QList<QFileInfo> search_FileInDir(QDir dirPath, QRegExp exp = QRegExp());
									//查找 - 文件夹下的文件名（含指定深度的子文件夹）
									//		【说明】：如果要找后缀为".facet"的文件，正则为：QRegExp(".*\\.facet")
		QList<QFileInfo> search_FileInWithDepth(QDir dirPath, int depth, QRegExp exp = QRegExp());
									//查找 - 文件夹下的文件名（含所有子文件夹）
									//		【说明】：如果要找后缀为".facet"的文件，正则为：QRegExp(".*\\.facet")
		QList<QFileInfo> search_FileInWithAllSubfolders(QDir dirPath, QRegExp exp = QRegExp());
	protected:
									//查找 - 文件夹下的文件名（递归方法，深度遍历）（私有）
		virtual QList<QFileInfo> searchFilePrivate_recursion(int tar_depth, QString dirPath_cur, int cur_depth);
									//查找 - 文件夹下的文件名（迭代器方法，所有子文件夹）（私有）
		virtual QList<QFileInfo> searchFilePrivate_iterator(QString dirPath_cur);
		

};

