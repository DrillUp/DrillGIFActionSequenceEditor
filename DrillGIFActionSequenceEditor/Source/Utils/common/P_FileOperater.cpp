#include "stdafx.h"
#include "P_FileOperater.h"


/*
-----==========================================================-----
		类：		文件操作器.cpp
		版本：		v1.33
		所属模块：	工具模块
		功能：		提供最基础的文件操作功能。

		子功能：	->复制文件/文件夹
					->移动文件/文件夹
					->删除文件/文件夹
					->重复文件过滤（文件名+大小+修改时间）
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
	return this->copyFilePrivate(filePath_from.absoluteFilePath(), filePath_to.absoluteFilePath());
}
/*-------------------------------------------------
		复制 - 文件（自动建立文件夹）
*/
bool P_FileOperater::copy_FileWithMkpath(QFileInfo filePath_from, QFileInfo filePath_to){
	QDir dir_to = filePath_to.absoluteDir();
	bool success_mkpath = dir_to.mkpath(dir_to.path());
	if (success_mkpath == false){ return false; }			//（父文件目录创建失败情况）
	return this->copyFilePrivate(filePath_from.absoluteFilePath(), filePath_to.absoluteFilePath());
}
/*-------------------------------------------------
		复制 - 文件夹（只含文件夹下的子文件）
*/
bool P_FileOperater::copy_Dir(QDir dirPath_from, QDir dirPath_to){
	return this->copyDirPrivate(dirPath_from.absolutePath(), dirPath_to.absolutePath());
}
/*-------------------------------------------------
		复制 - 文件夹（含指定深度的子文件夹）
*/
bool P_FileOperater::copy_DirWithDepth(QDir dirPath_from, QDir dirPath_to, int depth){
	return this->copyDirPrivate_recursion(dirPath_to.absolutePath(), depth, dirPath_from.absolutePath(), dirPath_from.absolutePath(), 0);
}
/*-------------------------------------------------
		复制 - 文件夹（含所有子文件夹）
*/
bool P_FileOperater::copy_DirWithAllSubfolders(QDir dirPath_from, QDir dirPath_to){
	//return this->copyDirPrivate_recursion(dirPath_to.absolutePath(), -1, dirPath_from.absolutePath(), dirPath_from.absolutePath(), 0);
	return this->copyDirPrivate_iterator(dirPath_to.absolutePath(), dirPath_from.absolutePath(), dirPath_from.absolutePath());
}

/*-------------------------------------------------
		复制 - 判断重复文件（私有）
*/
bool P_FileOperater::isSameFile(QFileInfo filePath_from, QFileInfo filePath_to){
	if (filePath_from.absoluteFilePath() != filePath_to.absoluteFilePath()){ return false; }	//(文件路径不重复，跳出)
	if (filePath_from.size() != filePath_to.size()){ return false; }							//(大小不同，跳出)
	if (filePath_from.lastModified() != filePath_to.lastModified()){ return false; }			//(修改日期不同，跳出)
	return true;
}
/*-------------------------------------------------
		复制 - 文件（私有）
*/
bool P_FileOperater::copyFilePrivate(QFileInfo filePath_from, QFileInfo filePath_to) {
	
	// > 判断重复文件
	if (this->isSameFile(filePath_from, filePath_to)){ return true; }

	QFile file_from(filePath_from.absoluteFilePath());
	QFile file_to(filePath_to.absoluteFilePath());

	// 【基于原生操作函数QFile::remove】
	file_to.remove();
	// 【基于原生操作函数QFile::copy】
	bool success = file_from.copy(filePath_to.absoluteFilePath());
	return success;

	// （下面写法费性能，且会改变修改日期）
	//if (!file_from.open(QIODevice::ReadOnly)) { return false; }
	//if (!file_to.open(QIODevice::WriteOnly | QIODevice::Truncate)) { return false; }
	//
	//QByteArray ba = file_from.readAll();
	//file_to.write(ba);
	//file_from.close();
	//file_to.close();
	//return true;
}
/*-------------------------------------------------
		复制 - 文件夹下的子文件（私有）
*/
bool P_FileOperater::copyDirPrivate(QString dirPath_from, QString dirPath_to){
	QDir dir_to(dirPath_to);
	QDir dir_from(dirPath_from);
	if (dir_to.absolutePath() == dir_from.absolutePath()){ return true; }		//（相同路径跳过）

	// > 路径生成
	if (dir_to.exists() == false){
		dir_to.mkdir(dirPath_to);
	}
	bool all_success = true;

	// > 子文件遍历
	QFileInfoList f_list = dir_from.entryInfoList(QDir::Files);
	for (int i = 0; i < f_list.size(); i++) {
		QFileInfo file_from_info(f_list.at(i));
		QString file_to_name = dirPath_to + "/" + file_from_info.fileName();

		bool success = this->copyFilePrivate(file_from_info.absoluteFilePath(), file_to_name);
		if ( success == false){ all_success = false; }
	}
	return all_success;
}
/*-------------------------------------------------
		复制 - 文件（递归方法，深度遍历）（私有）
*/
bool P_FileOperater::copyDirPrivate_recursion(QString dirPath_tarParent, int tar_depth, QString dirPath_curParent, QString dirPath_cur, int cur_depth) {
	QDir parent_to(dirPath_tarParent);
	QDir parent_from(dirPath_curParent);
	if ( parent_to.absolutePath() == parent_from.absolutePath() ){ return true; }		//（相同路径跳过）
	bool all_success = true;

	// > 复制当前层的子文件夹
	QDir cur_from(dirPath_cur);
	QString child_path = cur_from.absolutePath();
	child_path = child_path.replace(parent_from.absolutePath(), parent_to.absolutePath());	//将复制方的文件夹路径，替换成目标方的文件夹路径
	QDir(child_path).mkpath(child_path);

	// > 复制当前层的子文件
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
		复制 - 文件（迭代器方法，所有子文件夹）（私有）
*/
bool P_FileOperater::copyDirPrivate_iterator(QString dirPath_tarParent, QString dirPath_curParent, QString dirPath_cur) {
	QDir parent_to(dirPath_tarParent);
	QDir parent_from(dirPath_curParent);
	if (parent_to.absolutePath() == parent_from.absolutePath()) { return true; }		//（相同路径跳过）
	bool all_success = true;

	QDirIterator iter(parent_from.absolutePath(), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
	while (iter.hasNext()) {
		QString from_path = iter.next();
		QFileInfo from_info(from_path);

		// > 父文件夹替换
		QString to_path = from_path;
		to_path = to_path.replace(parent_from.absolutePath(), parent_to.absolutePath());	//将复制方的文件夹路径，替换成目标方的文件夹路径

		// > 文件夹时，创建
		if (from_info.isDir()){
			QDir(to_path).mkpath(to_path);
		}

		// > 文件时，复制
		if (from_info.isFile()){
			QDir to_dir = QFileInfo(to_path).absoluteDir();
			if (to_dir.exists() == false ){
				to_dir.mkpath(to_dir.absolutePath());	//（迭代遍历时，有时会跳文件夹，需要创建文件夹路径）
			}

			// > 移除已存在的文件
			// 【基于原生操作函数QFile::remove】
			QFile::remove(to_path);

			// > 文件复制
			// 【基于原生操作函数QFile::copy】
			bool success_move = QFile(from_path).copy(to_path);
			if (success_move == false) { all_success = false; }
		}
	}
	return all_success;
}



/*-------------------------------------------------
		移动 - 文件
*/
bool P_FileOperater::move_File(QFileInfo filePath_from, QFileInfo filePath_to){
	QDir dir_to = filePath_to.absoluteDir();
	bool success_mkpath = dir_to.mkpath(dir_to.path());
	if (success_mkpath == false){ return false; }			//（父文件目录创建失败情况）

	// 【基于原生操作函数QFile::remove】
	QFile(filePath_to.absoluteFilePath()).remove();
	// 【基于原生操作函数QFile::rename】
	bool success = QFile(filePath_from.absoluteFilePath()).rename(filePath_to.absoluteFilePath());
	return success;
}
/*-------------------------------------------------
		移动 - 文件夹（只含文件夹下的子文件）
*/
bool P_FileOperater::move_Dir(QDir dirPath_from, QDir dirPath_to){
	return this->moveDirPrivate(dirPath_from.absolutePath(), dirPath_to.absolutePath());
}
/*-------------------------------------------------
		移动 - 文件夹（含指定深度的子文件夹）
*/
bool P_FileOperater::move_DirWithDepth(QDir dirPath_from, QDir dirPath_to, int depth){
	return this->moveDirPrivate_recursion(dirPath_to.absolutePath(), depth, dirPath_from.absolutePath(), dirPath_from.absolutePath(), 0);
}
/*-------------------------------------------------
		移动 - 文件夹（含所有子文件夹）
*/
bool P_FileOperater::move_DirWithAllSubfolders(QDir dirPath_from, QDir dirPath_to){
	//bool succeed_dirMove = this->moveDirPrivate_recursion(dirPath_to.absolutePath(), -1, dirPath_from.absolutePath(), dirPath_from.absolutePath(), 0);	//（递归方法）
	bool succeed_dirMove = this->moveDirPrivate_iterator(dirPath_to.absolutePath(), dirPath_from.absolutePath(), dirPath_from.absolutePath());				//（迭代器方法）（迭代器更快，所以用这个）

	// > 移动成功后，把留下的文件夹全部删掉
	if (succeed_dirMove == true) {
		// 【基于原生操作函数QDir::removeRecursively】
		QDir(dirPath_from).removeRecursively();
	}
	return succeed_dirMove;
}
/*-------------------------------------------------
		移动 - 文件夹（完全替换）
*/
bool P_FileOperater::move_DirInReplace(QDir dirPath_from, QDir dirPath_to){

	// > 删除目标文件夹
	// 【基于原生操作函数QDir::removeRecursively】
	QDir(dirPath_to).removeRecursively();

	// > 在同一个盘时，可以改名
	// 【基于原生操作函数QDir::rename】
	bool succeed_rename = QDir(dirPath_from).rename(dirPath_from.path(),dirPath_to.path());
	if (succeed_rename == false) {

		// > 不在同一个盘时，跨盘移动
		//bool succeed_dirMove = this->moveDirPrivate_recursion(dirPath_to.absolutePath(), -1, dirPath_from.absolutePath(), dirPath_from.absolutePath(), 0);	//（递归方法）
		bool succeed_dirMove = this->moveDirPrivate_iterator(dirPath_to.absolutePath(), dirPath_from.absolutePath(), dirPath_from.absolutePath());				//（迭代器方法）（迭代器更快，所以用这个）

		// > 移动成功后，把留下的文件夹全部删掉
		if (succeed_dirMove == true) {
			// 【基于原生操作函数QDir::removeRecursively】
			QDir(dirPath_from).removeRecursively();
		}
		return succeed_dirMove;
	}
	return succeed_rename;
}
/*-------------------------------------------------
		移动 - 文件夹下的子文件（私有）
*/
bool P_FileOperater::moveDirPrivate(QString dirPath_from, QString dirPath_to){
	QDir dir_to(dirPath_to);
	QDir dir_from(dirPath_from);
	if (dir_to.absolutePath() == dir_from.absolutePath()){ return true; }		//（相同路径跳过）

	// > 路径生成
	if (dir_to.exists() == false){
		dir_to.mkdir(dirPath_to);
	}
	bool all_success = true;

	// > 子文件遍历
	QFileInfoList f_list = dir_from.entryInfoList(QDir::Files);
	for (int i = 0; i < f_list.size(); i++) {
		QFileInfo file_from_info(f_list.at(i));
		QString file_to_name = dirPath_to + "/" + file_from_info.fileName();
		
		// > 移除已存在的文件
		// 【基于原生操作函数QFile::remove】
		QFile::remove(file_to_name);

		// > 文件改名并移动
		// 【基于原生操作函数QFile::rename】
		bool success_move = QFile(file_from_info.absoluteFilePath()).rename(file_to_name);
		if ( success_move == false){ all_success = false; }
	}
	return all_success;
}
/*-------------------------------------------------
		移动 - 文件（递归方法，深度遍历）（私有）
*/
bool P_FileOperater::moveDirPrivate_recursion(QString dirPath_tarParent, int tar_depth, QString dirPath_curParent, QString dirPath_cur, int cur_depth) {
	QDir parent_to(dirPath_tarParent);
	QDir parent_from(dirPath_curParent);
	if (parent_to.absolutePath() == parent_from.absolutePath()){ return true; }		//（相同路径跳过）
	bool all_success = true;

	// > 复制当前层的子文件夹
	QDir cur_from(dirPath_cur);
	QString child_path = cur_from.absolutePath();
	child_path = child_path.replace(parent_from.absolutePath(), parent_to.absolutePath());	//将复制方的文件夹路径，替换成目标方的文件夹路径
	QDir(child_path).mkpath(child_path);

	// > 移动当前层的子文件
	bool success_move = this->moveDirPrivate(cur_from.absolutePath(), child_path);
	if (success_move == false){ all_success = false; }

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
		bool success = this->moveDirPrivate_recursion(parent_to.absolutePath(), tar_depth, parent_from.absolutePath(), info.absoluteFilePath(), cur_depth + 1);
		if (success == false){ all_success = false; }
	}

	return all_success;
}
/*-------------------------------------------------
		移动 - 文件（迭代器方法，所有子文件夹）（私有）
*/
bool P_FileOperater::moveDirPrivate_iterator(QString dirPath_tarParent, QString dirPath_curParent, QString dirPath_cur) {
	QDir parent_to(dirPath_tarParent);
	QDir parent_from(dirPath_curParent);
	if (parent_to.absolutePath() == parent_from.absolutePath()) { return true; }		//（相同路径跳过）
	bool all_success = true;

	QDirIterator iter(parent_from.absolutePath(), QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
	while (iter.hasNext()) {
		QString from_path = iter.next();
		QFileInfo from_info(from_path);

		// > 父文件夹替换
		QString to_path = from_path;
		to_path = to_path.replace(parent_from.absolutePath(), parent_to.absolutePath());	//将复制方的文件夹路径，替换成目标方的文件夹路径

		// > 文件夹时，创建
		if (from_info.isDir()){
			QDir(to_path).mkpath(to_path);
		}

		// > 文件时，移动
		if (from_info.isFile()){
			QDir to_dir = QFileInfo(to_path).absoluteDir();
			if (to_dir.exists() == false){
				to_dir.mkpath(to_dir.absolutePath());	//（迭代遍历时，有时会跳文件夹，需要创建文件夹路径）
			}

			// > 移除已存在的文件
			// 【基于原生操作函数QFile::remove】
			QFile::remove(to_path);

			// > 文件改名并移动
			// 【基于原生操作函数QFile::rename】
			bool success_move = QFile(from_path).rename(to_path);
			if (success_move == false) { all_success = false; }
		}
	}
	return all_success;
}




/*-------------------------------------------------
		删除 - 文件夹（含所有子文件夹）
*/
bool P_FileOperater::remove_Dir(QDir dirPath){
	// 【基于原生操作函数QDir::removeRecursively】
	return dirPath.removeRecursively();
}
/*-------------------------------------------------
		删除 - 文件
*/
bool P_FileOperater::remove_File(QFileInfo filePath){
	if (filePath.isFile() == false){ return false; }
	// 【基于原生操作函数QFile::remove】
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

	// > 后缀去掉"."
	if (suffix.mid(0, 1) == "."){ suffix = suffix.mid(1); }

	//return this->removeFilePrivate_recursion("*." + suffix, -1, dirPath.absolutePath(), 0);	//（递归方法）
	return this->removeFilePrivate_iterator("*." + suffix, dirPath.absolutePath());				//（迭代器方法）（迭代器更快，所以用这个）
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

	//return this->removeFilePrivate_recursion(only_basename + ".*", -1, dirPath.absolutePath(), 0);	//（递归方法）
	return this->removeFilePrivate_iterator(only_basename + ".*", dirPath.absolutePath());				//（迭代器方法）（迭代器更快，所以用这个）
}
/*-------------------------------------------------
		删除 - 文件（递归方法，深度遍历）（私有）
*/
bool P_FileOperater::removeFilePrivate_recursion(QString nameFilter, int tar_depth, QString dirPath_cur, int cur_depth) {
	QDir cur_from(dirPath_cur);
	bool all_success = true;

	// > 删除当前层的文件
	//		（删整个文件夹不需要递归，这里递归就是为了删指定后缀的文件）
	QFileInfoList f_list = cur_from.entryInfoList(QStringList() << nameFilter, QDir::Files, QDir::Name);
	for (int i = 0; i < f_list.count(); i++) {
		// 【基于原生操作函数QFile::remove】
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
/*-------------------------------------------------
		删除 - 文件（迭代器方法，所有子文件夹）（私有）
*/
bool P_FileOperater::removeFilePrivate_iterator(QString nameFilter, QString dirPath) {
	bool all_success = true;
	QDirIterator iter(dirPath, QStringList() << nameFilter, QDir::Files, QDirIterator::Subdirectories);
	while (iter.hasNext()) {
		QString file_path = iter.next();
		bool success = QFile::remove(file_path);
		if (success == false) { all_success = false; }
	}
	return all_success;
}




/*-------------------------------------------------
		查找 - 文件夹下的文件名（只含文件夹下的子文件）
*/
QList<QFileInfo> P_FileOperater::search_FileInDir(QDir dirPath, QRegExp exp){
	QList<QFileInfo> result_list;
	QList<QFileInfo> file_list = this->searchFilePrivate_recursion(0, dirPath.absolutePath(), 0);
	if (exp.isEmpty()){ return file_list; }
	
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo info = file_list.at(i);
		if ( exp.exactMatch(info.fileName()) ){		//（必须全词匹配）
			result_list.append(info);
		}
	}
	return result_list;
}
/*-------------------------------------------------
		查找 - 文件夹下的文件名（含指定深度的子文件夹）
*/
QList<QFileInfo> P_FileOperater::search_FileInWithDepth(QDir dirPath, int depth, QRegExp exp){
	QList<QFileInfo> result_list;
	QList<QFileInfo> file_list = this->searchFilePrivate_recursion(depth, dirPath.absolutePath(), 0);
	if (exp.isEmpty()){ return file_list; }
	
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo info = file_list.at(i);
		if (exp.exactMatch(info.fileName())){		//（必须全词匹配）
			result_list.append(info);
		}
	}
	return result_list;
}
/*-------------------------------------------------
		查找 - 文件夹下的文件名（含所有子文件夹）
*/
QList<QFileInfo> P_FileOperater::search_FileInWithAllSubfolders(QDir dirPath, QRegExp exp){
	QList<QFileInfo> result_list;

	//QList<QFileInfo> file_list = this->searchFilePrivate_recursion(-1, dirPath.absolutePath(), 0);	//（递归方法）
	QList<QFileInfo> file_list = this->searchFilePrivate_iterator(dirPath.absolutePath());				//（迭代器方法）（迭代器更快，所以用这个）

	if (exp.isEmpty()){ return file_list; }
	for (int i = 0; i < file_list.count(); i++){
		QFileInfo info = file_list.at(i);
		if (exp.exactMatch(info.fileName())){		//（必须全词匹配）
			result_list.append(info);
		}
	}
	return result_list;
}

/*-------------------------------------------------
		查找 - 文件夹下的文件名（递归方法，深度遍历）（私有）
*/
QList<QFileInfo> P_FileOperater::searchFilePrivate_recursion(int tar_depth, QString dirPath_cur, int cur_depth) {
	QDir cur_from(dirPath_cur);
	bool all_success = true;

	// > 子文件
	QList<QFileInfo> f_list = cur_from.entryInfoList(QDir::Files);

	// > 深度检查
	if (tar_depth != -1){				//（-1则无限深度）
		if (tar_depth == cur_depth){	//（达到指定深度后不再复制）
			return f_list;
		}
	}

	// > 子文件夹
	QFileInfoList d_list = cur_from.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);	// NoDotAndDotDot指文件夹当前级和上一级的目录
	for (int i = 0; i < d_list.count(); i++){
		QFileInfo info = d_list.at(i);
		f_list.append( this->searchFilePrivate_recursion(tar_depth, info.absoluteFilePath(), cur_depth + 1) );
	}

	return f_list;
}
/*-------------------------------------------------
		查找 - 文件夹下的文件名（迭代器方法，所有子文件夹）（私有）
*/
QList<QFileInfo> P_FileOperater::searchFilePrivate_iterator(QString dirPath) {
	QList<QFileInfo> result_list;
	QDirIterator iter(dirPath, QDir::Files, QDirIterator::Subdirectories);
	while (iter.hasNext()) {
		result_list.append(QFileInfo(iter.next()));
	}
	return result_list;
}


