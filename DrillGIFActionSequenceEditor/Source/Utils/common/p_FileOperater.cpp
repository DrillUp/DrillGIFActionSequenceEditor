#include "stdafx.h"
#include "p_FileOperater.h"


/*
-----==========================================================-----
		类：		文件操作器.cpp
		版本：		v1.21
		所属模块：	工具模块
		功能：		提供最基础的文件操作功能。

		子功能：	->复制文件/文件夹
					->删除文件/文件夹
-----==========================================================-----
*/

P_FileOperater::P_FileOperater(){
}
P_FileOperater::~P_FileOperater(){
}

/*-------------------------------------------------
		复制 - 文件
*/
bool P_FileOperater::copy_File(QFileInfo filePath_from, QFileInfo filePath_to){
	return copyFilePrivate(filePath_from.absoluteFilePath(), filePath_to.absoluteFilePath());
}
/*-------------------------------------------------
		复制 - 文件夹
*/
bool P_FileOperater::copy_Dir(QDir dirPath_from, QDir dirPath_to){
	return copyDirPrivate(dirPath_from.absolutePath(), dirPath_to.absolutePath());
}
/*-------------------------------------------------
		复制 - 文件夹（指定子文件夹深度）
*/
bool P_FileOperater::copy_DirWithDepth(QDir dirPath_from, QDir dirPath_to, int depth){
	return copyDirPrivate_recursion(dirPath_to.absolutePath(), depth, dirPath_from.absolutePath(), dirPath_from.absolutePath(), 0);
}
/*-------------------------------------------------
		复制 - 文件夹（所有子文件夹）
*/
bool P_FileOperater::copy_DirWithAllSubfolders(QDir dirPath_from, QDir dirPath_to){
	return copyDirPrivate_recursion(dirPath_to.absolutePath(), -1, dirPath_from.absolutePath(), dirPath_from.absolutePath(), 0);
}


/*-------------------------------------------------
		删除 - 文件夹
*/
bool P_FileOperater::remove_Dir(QDir dirPath){
	return dirPath.removeRecursively();
}
/*-------------------------------------------------
		删除 - 文件
*/
bool P_FileOperater::remove_File(QFileInfo filePath){
	if (filePath.isFile() == false){ return false; }
	return QFile::remove(filePath.absoluteFilePath());
}
/*-------------------------------------------------
		删除 - 文件（指定后缀）
*/
bool P_FileOperater::remove_FileBySuffix(QDir dirPath, QString suffix){
	if (dirPath.exists() == false){ return false; }

	// > 递归删除（深度0）
	return this->removeFilePrivate_recursion("*." + suffix, 0, dirPath.absolutePath(), 0);
}
/*-------------------------------------------------
		删除 - 文件（指定后缀 + 所有子文件夹）
*/
bool P_FileOperater::remove_FileBySuffix_WithAllSubfolders(QDir dirPath, QString suffix){
	if (dirPath.exists() == false){ return false; }

	// > 递归删除（深度-1）
	return this->removeFilePrivate_recursion("*." + suffix, -1, dirPath.absolutePath(), 0);
}
/*-------------------------------------------------
		删除 - 文件（指定不含后缀的名称）
*/
bool P_FileOperater::remove_FileByNameNoSuffix(QDir dirPath, QString only_basename){
	if (dirPath.exists() == false){ return false; }

	// > 递归删除（深度0）
	return this->removeFilePrivate_recursion(only_basename + ".*", 0, dirPath.absolutePath(), 0);
}
/*-------------------------------------------------
		删除 - 文件（指定不含后缀的名称 + 所有子文件夹）
*/
bool P_FileOperater::remove_FileByNameNoSuffix_WithAllSubfolders(QDir dirPath, QString only_basename){
	if (dirPath.exists() == false){ return false; }

	// > 递归删除（深度-1）
	return this->removeFilePrivate_recursion(only_basename + ".*", -1, dirPath.absolutePath(), 0);
}



/*-------------------------------------------------
		私有 - 复制文件 A -> B （非文件夹）
*/
bool P_FileOperater::copyFilePrivate(QString filePath_from, QString filePath_to) {
	QFileInfo info_from(filePath_from);
	QFileInfo info_to(filePath_to);
	QFile file_from(info_from.absoluteFilePath());
	QFile file_to(info_to.absoluteFilePath());
	if (info_from.absoluteFilePath() == info_to.absoluteFilePath()){ return true; }		//（相同路径跳过）

	if (!file_from.open(QIODevice::ReadOnly)) { return false; }
	if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return false; }

	QByteArray ba = file_from.readAll();
	file_to.write(ba);
	file_from.close();
	file_to.close();
	return true;
}
/*-------------------------------------------------
		私有 - 复制文件 A -> B （只复制子文件）
*/
bool P_FileOperater::copyDirPrivate(QString dirPath_from, QString dirPath_to){
	QFileInfo info_to(dirPath_to);
	QFileInfo info_from(dirPath_from);
	QDir dir_to(dirPath_to);
	QDir dir_from(dirPath_from);
	if (dir_to.absolutePath() == dir_from.absolutePath()){ return true; }		//（相同路径跳过）

	// > 路径生成
	if (dir_to.exists() == false){
		dir_to.mkdir(dirPath_to);
	}
	bool all_success = true;

	// > 直接复制
	QFileInfoList f_list = dir_from.entryInfoList(QDir::Files);
	for (int i = 0; i < f_list.size(); i++) {
		QFileInfo temp_info(f_list.at(i));
		QString file_to_name = info_to.absoluteFilePath() + "/" + temp_info.fileName();
		bool success = this->copyFilePrivate(temp_info.absoluteFilePath(), file_to_name);
		if ( success == false){ all_success = false; }
	}
	return all_success;
}
/*-------------------------------------------------
		私有 - 复制文件夹 A -> B （根据深度遍历复制文件夹）
*/
bool P_FileOperater::copyDirPrivate_recursion(QString dirPath_tarParent, int tar_depth, QString dirPath_curParent, QString dirPath_cur, int cur_depth) {
	QDir parent_to(dirPath_tarParent);
	QDir parent_from(dirPath_curParent);
	QDir cur_from(dirPath_cur);
	if ( parent_to.absolutePath() == parent_from.absolutePath() ){ return true; }		//（相同路径跳过）
	bool all_success = true;

	// > 复制当前层的文件（temp中的）
	QString child_path = cur_from.absolutePath();
	child_path = child_path.replace(parent_from.absolutePath(), parent_to.absolutePath());	//将复制方的文件夹路径，替换成目标方的文件夹路径
	QDir(child_path).mkpath(child_path);
	bool success_copy = this->copyDirPrivate(cur_from.absolutePath(), child_path);
	if ( success_copy == false){ all_success = false; }

	// > 深度检查
	if (tar_depth != -1){				//（-1则无限深度）
		if (tar_depth == cur_depth){	//（达到指定深度后不再复制）
			return all_success;
		}
	}

	// > 子文件夹
	QFileInfoList d_list = cur_from.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);	// NoDotAndDotDot指文件夹当前级和上一级的目录
	for (int i = 0; i < d_list.count(); i++){
		QFileInfo info = d_list.at(i);
		bool success = this->copyDirPrivate_recursion(parent_to.absolutePath(), tar_depth, parent_from.absolutePath(), info.absoluteFilePath(), cur_depth + 1);
		if ( success == false){ all_success = false; }
	}

	return all_success;
}

/*-------------------------------------------------
		私有 - 删除文件夹 （根据深度遍历删除文件夹）
*/
bool P_FileOperater::removeFilePrivate_recursion(QString nameFilter, int tar_depth, QString dirPath_cur, int cur_depth) {
	QDir cur_from(dirPath_cur);
	bool all_success = true;

	// > 删除当前层的文件
	QFileInfoList f_list = cur_from.entryInfoList(QStringList() << nameFilter, QDir::Files, QDir::Name);
	for (int i = 0; i < f_list.count(); i++) {
		bool success = QFile::remove(f_list.at(i).absoluteFilePath());
		if ( success == false){ all_success = false; }
	}

	// > 深度检查
	if (tar_depth != -1){				//（-1则无限深度）
		if (tar_depth == cur_depth){	//（达到指定深度后不再复制）
			return all_success;
		}
	}

	// > 子文件夹
	QFileInfoList d_list = cur_from.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);	// NoDotAndDotDot指文件夹当前级和上一级的目录
	for (int i = 0; i < d_list.count(); i++){
		QFileInfo info = d_list.at(i);
		bool success = this->removeFilePrivate_recursion(nameFilter, tar_depth, info.absoluteFilePath(), cur_depth + 1);
		if ( success == false){ all_success = false; }
	}

	return all_success;
}
