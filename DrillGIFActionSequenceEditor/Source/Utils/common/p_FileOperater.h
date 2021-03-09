#pragma once
#include <QObject>

/*
-----==========================================================-----
		�ࣺ		�ļ�������.h
		�汾��		v1.22
		����ģ�飺	����ģ��
		���ܣ�		�ṩ��������ļ��������ܡ�
					����ϸ���ݼ�.cpp��
-----==========================================================-----
*/
class P_FileOperater : public QObject
{
	Q_OBJECT

	public:
		P_FileOperater();
		~P_FileOperater();

	//-----------------------------------
	//----����
	public:
									//���� - �ļ�
		bool copy_File(QFileInfo filePath_from, QFileInfo filePath_to);
									//���� - �ļ���
		bool copy_Dir(QDir dirPath_from, QDir dirPath_to);
									//���� - �ļ��У�ָ�����ļ�����ȣ�
		bool copy_DirWithDepth(QDir dirPath_from, QDir dirPath_to, int depth);
									//���� - �ļ��У��������ļ��У�
		bool copy_DirWithAllSubfolders(QDir dirPath_from, QDir dirPath_to);

	//-----------------------------------
	//----ɾ��
	public:
									//ɾ�� - �ļ���
		bool remove_Dir(QDir dirPath);
									//ɾ�� - �ļ�
		bool remove_File(QFileInfo filePath);
									//ɾ�� - �ļ���ָ����׺��
		bool remove_FileBySuffix(QDir dirPath, QString suffix);
									//ɾ�� - �ļ���ָ����׺ + �������ļ��У�
		bool remove_FileBySuffix_WithAllSubfolders(QDir dirPath, QString suffix);
									//ɾ�� - �ļ���ָ��������׺�����ƣ�
		bool remove_FileByNameNoSuffix(QDir dirPath, QString only_basename);
									//ɾ�� - �ļ���ָ��������׺������ + �������ļ��У�
		bool remove_FileByNameNoSuffix_WithAllSubfolders(QDir dirPath, QString only_basename);
		
	//-----------------------------------
	//----˽��
	protected:
		virtual bool isSameFile(QFileInfo filePath_from, QFileInfo filePath_to);	//�ж��ظ��ļ�
		virtual bool copyFilePrivate(QString filePath_from, QString filePath_to);	//�����ļ���˽�У�
		virtual bool copyDirPrivate(QString dirPath_from, QString dirPath_to);		//�����ļ��У�˽�У�ֻ�������ļ���
		virtual bool copyDirPrivate_recursion(QString dirPath_tarParent, int tar_depth, QString dirPath_curParent, QString dirPath_cur, int cur_depth);	//�ݹ鸴���ļ��У�˽�У�
		virtual bool removeFilePrivate_recursion(QString nameFilter, int tar_depth, QString dirPath_cur, int cur_depth);		//�ݹ�ɾ���ļ��У�˽�У�

};

