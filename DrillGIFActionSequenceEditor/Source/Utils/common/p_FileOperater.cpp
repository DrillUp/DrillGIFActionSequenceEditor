#include "stdafx.h"
#include "p_FileOperater.h"


/*
-----==========================================================-----
		�ࣺ		�ļ�������.cpp
		�汾��		v1.22
		����ģ�飺	����ģ��
		���ܣ�		�ṩ��������ļ��������ܡ�

		�ӹ��ܣ�	->�����ļ�/�ļ���
					->ɾ���ļ�/�ļ���
					->�ظ��ļ����ˣ��ļ���+��С+�޸�ʱ�䣩
-----==========================================================-----
*/

P_FileOperater::P_FileOperater(){
}
P_FileOperater::~P_FileOperater(){
}

/*-------------------------------------------------
		���� - �ļ�
*/
bool P_FileOperater::copy_File(QFileInfo filePath_from, QFileInfo filePath_to){
	return copyFilePrivate(filePath_from.absoluteFilePath(), filePath_to.absoluteFilePath());
}
/*-------------------------------------------------
		���� - �ļ���
*/
bool P_FileOperater::copy_Dir(QDir dirPath_from, QDir dirPath_to){
	return copyDirPrivate(dirPath_from.absolutePath(), dirPath_to.absolutePath());
}
/*-------------------------------------------------
		���� - �ļ��У�ָ�����ļ�����ȣ�
*/
bool P_FileOperater::copy_DirWithDepth(QDir dirPath_from, QDir dirPath_to, int depth){
	return copyDirPrivate_recursion(dirPath_to.absolutePath(), depth, dirPath_from.absolutePath(), dirPath_from.absolutePath(), 0);
}
/*-------------------------------------------------
		���� - �ļ��У��������ļ��У�
*/
bool P_FileOperater::copy_DirWithAllSubfolders(QDir dirPath_from, QDir dirPath_to){
	return copyDirPrivate_recursion(dirPath_to.absolutePath(), -1, dirPath_from.absolutePath(), dirPath_from.absolutePath(), 0);
}


/*-------------------------------------------------
		ɾ�� - �ļ���
*/
bool P_FileOperater::remove_Dir(QDir dirPath){
	return dirPath.removeRecursively();
}
/*-------------------------------------------------
		ɾ�� - �ļ�
*/
bool P_FileOperater::remove_File(QFileInfo filePath){
	if (filePath.isFile() == false){ return false; }
	return QFile::remove(filePath.absoluteFilePath());
}
/*-------------------------------------------------
		ɾ�� - �ļ���ָ����׺��
*/
bool P_FileOperater::remove_FileBySuffix(QDir dirPath, QString suffix){
	if (dirPath.exists() == false){ return false; }

	// > �ݹ�ɾ�������0��
	return this->removeFilePrivate_recursion("*." + suffix, 0, dirPath.absolutePath(), 0);
}
/*-------------------------------------------------
		ɾ�� - �ļ���ָ����׺ + �������ļ��У�
*/
bool P_FileOperater::remove_FileBySuffix_WithAllSubfolders(QDir dirPath, QString suffix){
	if (dirPath.exists() == false){ return false; }

	// > �ݹ�ɾ�������-1��
	return this->removeFilePrivate_recursion("*." + suffix, -1, dirPath.absolutePath(), 0);
}
/*-------------------------------------------------
		ɾ�� - �ļ���ָ��������׺�����ƣ�
*/
bool P_FileOperater::remove_FileByNameNoSuffix(QDir dirPath, QString only_basename){
	if (dirPath.exists() == false){ return false; }

	// > �ݹ�ɾ�������0��
	return this->removeFilePrivate_recursion(only_basename + ".*", 0, dirPath.absolutePath(), 0);
}
/*-------------------------------------------------
		ɾ�� - �ļ���ָ��������׺������ + �������ļ��У�
*/
bool P_FileOperater::remove_FileByNameNoSuffix_WithAllSubfolders(QDir dirPath, QString only_basename){
	if (dirPath.exists() == false){ return false; }

	// > �ݹ�ɾ�������-1��
	return this->removeFilePrivate_recursion(only_basename + ".*", -1, dirPath.absolutePath(), 0);
}



/*-------------------------------------------------
		˽�� - �ж��ظ��ļ�
*/
bool P_FileOperater::isSameFile(QFileInfo filePath_from, QFileInfo filePath_to){
	if (filePath_from.fileName() != filePath_to.fileName()){ return false; }			//(����������)
	if (filePath_from.size() != filePath_to.size()){ return false; }					//(��С��ͬ����)
	if (filePath_from.lastModified() != filePath_to.lastModified()){ return false; }	//(�޸����ڲ�ͬ����)
	return true;
}
/*-------------------------------------------------
		˽�� - �����ļ� A -> B �����ļ��У�
*/
bool P_FileOperater::copyFilePrivate(QString filePath_from, QString filePath_to) {
	QFileInfo info_from(filePath_from);
	QFileInfo info_to(filePath_to);
	QFile file_from(info_from.absoluteFilePath());
	QFile file_to(info_to.absoluteFilePath());
	if (info_from.absoluteFilePath() == info_to.absoluteFilePath()){ return true; }		//����ͬ·��������
	if (this->isSameFile(info_from, info_to)){ return true; }							//���ļ��ظ���������

	file_to.remove();
	bool success = file_from.copy(info_to.absoluteFilePath());
	return success;

	// ����д����ı��޸����ڣ�
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
		˽�� - �����ļ� A -> B ��ֻ�������ļ���
*/
bool P_FileOperater::copyDirPrivate(QString dirPath_from, QString dirPath_to){
	QFileInfo info_to(dirPath_to);
	QFileInfo info_from(dirPath_from);
	QDir dir_to(dirPath_to);
	QDir dir_from(dirPath_from);
	if (dir_to.absolutePath() == dir_from.absolutePath()){ return true; }		//����ͬ·��������

	// > ·������
	if (dir_to.exists() == false){
		dir_to.mkdir(dirPath_to);
	}
	bool all_success = true;

	// > ֱ�Ӹ���
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
		˽�� - �����ļ��� A -> B ��������ȱ��������ļ��У�
*/
bool P_FileOperater::copyDirPrivate_recursion(QString dirPath_tarParent, int tar_depth, QString dirPath_curParent, QString dirPath_cur, int cur_depth) {
	QDir parent_to(dirPath_tarParent);
	QDir parent_from(dirPath_curParent);
	QDir cur_from(dirPath_cur);
	if ( parent_to.absolutePath() == parent_from.absolutePath() ){ return true; }		//����ͬ·��������
	bool all_success = true;

	// > ���Ƶ�ǰ����ļ���temp�еģ�
	QString child_path = cur_from.absolutePath();
	child_path = child_path.replace(parent_from.absolutePath(), parent_to.absolutePath());	//�����Ʒ����ļ���·�����滻��Ŀ�귽���ļ���·��
	QDir(child_path).mkpath(child_path);
	bool success_copy = this->copyDirPrivate(cur_from.absolutePath(), child_path);
	if ( success_copy == false){ all_success = false; }

	// > ��ȼ��
	if (tar_depth != -1){				//��-1��������ȣ�
		if (tar_depth == cur_depth){	//���ﵽָ����Ⱥ��ٸ��ƣ�
			return all_success;
		}
	}

	// > ���ļ���
	QFileInfoList d_list = cur_from.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);	// NoDotAndDotDotָ�ļ��е�ǰ������һ����Ŀ¼
	for (int i = 0; i < d_list.count(); i++){
		QFileInfo info = d_list.at(i);
		bool success = this->copyDirPrivate_recursion(parent_to.absolutePath(), tar_depth, parent_from.absolutePath(), info.absoluteFilePath(), cur_depth + 1);
		if ( success == false){ all_success = false; }
	}

	return all_success;
}

/*-------------------------------------------------
		˽�� - ɾ���ļ��� ��������ȱ���ɾ���ļ��У�
*/
bool P_FileOperater::removeFilePrivate_recursion(QString nameFilter, int tar_depth, QString dirPath_cur, int cur_depth) {
	QDir cur_from(dirPath_cur);
	bool all_success = true;

	// > ɾ����ǰ����ļ�
	QFileInfoList f_list = cur_from.entryInfoList(QStringList() << nameFilter, QDir::Files, QDir::Name);
	for (int i = 0; i < f_list.count(); i++) {
		bool success = QFile::remove(f_list.at(i).absoluteFilePath());
		if ( success == false){ all_success = false; }
	}

	// > ��ȼ��
	if (tar_depth != -1){				//��-1��������ȣ�
		if (tar_depth == cur_depth){	//���ﵽָ����Ⱥ��ٸ��ƣ�
			return all_success;
		}
	}

	// > ���ļ���
	QFileInfoList d_list = cur_from.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);	// NoDotAndDotDotָ�ļ��е�ǰ������һ����Ŀ¼
	for (int i = 0; i < d_list.count(); i++){
		QFileInfo info = d_list.at(i);
		bool success = this->removeFilePrivate_recursion(nameFilter, tar_depth, info.absoluteFilePath(), cur_depth + 1);
		if ( success == false){ all_success = false; }
	}

	return all_success;
}
