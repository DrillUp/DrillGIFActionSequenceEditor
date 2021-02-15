#pragma once

#include <QWidget>

/*
-----==========================================================-----
		�ࣺ		temp�ļ��й���.h
		�汾��		v1.19
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		��temp�ļ��е��κβ�������������С�
					ֱ����exe�ļ�Ŀ¼�½���temp�ļ��У����ļ���ת�ơ���ʱ����
					����ϸ���ݼ�.cpp��
-----==========================================================-----
*/

class S_TempFileManager : public QObject
{
	Q_OBJECT

	public:
		S_TempFileManager();
		~S_TempFileManager();
		static S_TempFileManager* cur_instance;				//����
		static S_TempFileManager* getInstance();			//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩
		void init();										//��ʼ��
		void destroyInstance();								//���ٵ���
		
	//-----------------------------------
	//----˽������
	private:
		QString temp_file_url;		//��ǰ�����ļ���·��
		QString workspace_url;		//��ǰ������·��
	private:
		bool copyFile(QString filePath_from, QString filePath_to);	//�����ļ���˽�У�
		bool copyDir(QString dirPath_from, QString dirPath_to);		//�����ļ��У�˽�У�ֻ�������ļ���
		bool copyDir_recursion(QString tar_parent_url, int tar_depth, QString cur_parent_url, QString cur_url, int cur_depth);	//�ݹ鸴���ļ��У�˽�У�
		void removeFile_recursion(QString nameFilter, int tar_depth, QString cur_url, int cur_depth);		//�ݹ�ɾ���ļ��У�˽�У�

	//-----------------------------------
	//----log�ļ�
	public:
									//���� - ����log�ļ�����д
									//		������1�� "name.txt"���ļ���������·����
									//		������2�� log���ݣ�utf8�洢��
									//		�����ء� ��
		void replaceDebugLog(QString filename, QString logdata);
									//���� - ����log�ļ���׷��
									//		������1�� "name.txt"���ļ���������·����
									//		������2�� log���ݣ�utf8�洢��
									//		�����ء� ��
		void addDebugLog(QString filename, QString logdata);
									//���� - ����log�ļ�����д
									//		������1�� "name.txt"���ļ���������·����
									//		������2�� �ֽ�����utf8��
									//		�����ء� ��
		void replaceDebugLog(QString filename, QByteArray logdata);

	//-----------------------------------
	//----����
	public:
									//���� - ָ���ļ�/�ļ��� -> temp�ļ��У�ȫ����
									//		�����ء� �ɹ��������ֹ��׺����ֹ����Ϊ��
		bool copyResourceToTemp_File(QString src_url);
		bool copyResourceToTemp_Dir(QString src_url);
		bool copyResourceToTemp_DirWithDepth(QString src_url, int depth);
		bool copyResourceToTemp_DirWithAllSubfolders(QString src_url);
									//���� - temp�ļ��� -> ָ���ļ��� ��ȫ����
									//		�����ء� �ɹ��������ֹ��׺����ֹ����Ϊ��
		bool copyTempToTarget_Dir(QString tar_url);
		bool copyTempToTarget_DirWithDepth(QString tar_url, int depth);
		bool copyTempToTarget_DirWithAllSubfolders(QString tar_url);

									//���� - ����ļ� -> temp�ļ���
									//		������1�� "F:/name"���ļ��У���Ҫ����Ŀ¼ ��F:��
									//		������2�� ["aa.xds","bb.vcut"��]���б�������·����
									//		�����ء� ��
		void copyResourceToTemp_SeveralFile(QString src_url, QStringList file_names);
									//���� - ���temp�ļ� -> ָ���ļ��� 
									//		������1�� "F:/name"���ļ��У���Ҫ����Ŀ¼ ��F:��
									//		������2�� ["aa.xds","bb.vcut"��]���б�������·����
									//		�����ء� ��
		void copyTempToTarget_SeveralFile(QString tar_url, QStringList file_names);
									//���� - temp�ļ��� ���ļ����������µ�����
									//		������1�� "name.xxx"���ļ���������·����
									//		������2�� "new_name.xxx"���ļ���������·����
									//		�����ء� ��
		void copyTempToTemp_FileWithNewName(QString filename, QString file_new_name);
									//���� - temp�ļ��� ���ļ��У��������µ��ļ�����
									//		������1�� "xxxx"���ļ��У�������·����
									//		������2�� "xxxx/xx"���ļ��У�������ȫ·����
									//		�����ء� ��
		bool copyTempToTemp_Dir(QString dir_name, QString dir_new_name);
		bool copyTempToTemp_DirWithDepth(QString dir_name, QString dir_new_name, int depth);
		bool copyTempToTemp_DirWithAllSubfolders(QString dir_name, QString dir_new_name);
		

	//-----------------------------------
	//----����
	public:
									//���� - ����һ���ļ���д�����ݣ��ļ��Ѵ��ڣ��򲻲�����
									//		������1�� "name.xxx"���ļ���������·����
									//		������2�� �ַ������ݣ�utf8�洢��
									//		�����ء� ��
		void generateTempFile(QString filename, QString filedata);
									//���� - ����һ���ļ���д�����ݣ��ļ��Ѵ��ڣ��򸲸ǣ�
									//		������1�� "name.xxx"���ļ���������·����
									//		������2�� �ַ�������
									//		������3�� �������ã�Ĭ��utf8��
									//		�����ء� ��
		void generateTempFileStrict(QString filename, QString filedata, QString code);
		
	//-----------------------------------
	//----��
	/*	���ļ���ֱ���ã�bool ok = QFile::rename(path1, path2);	*/

	//-----------------------------------
	//----ɾ��
	public:
									//ɾ�� - ɾ��temp�ļ�
		void removeInTemp_Dir(QString dirname);
		void removeInTemp_File(QString filename);
		void removeInTemp_FileBySuffix(QString suffix);
		void removeInTemp_FileBySuffix_WithAllSubfolders(QString suffix);
		void removeInTemp_FileByNameNoSuffix(QString onlyname);
		void removeInTemp_FileByNameNoSuffix_WithAllSubfolders(QString onlyname);
									//ɾ�� - ��������ļ��������ļ��У�
		void clearAllTempFile();
		
	//-----------------------------------
	//----��ȡ
	public:
									//��ȡ - �����ļ�����·��
									//		�����ء� "F:/.../temp"
		QString getMainUrl();
									//��ȡ - ��ǰ���湤����·��
									//		�����ء� "F:/.../temp/workspace"
		QString getTempFileUrl();
									//��ȡ - �Ƿ����ָ���ļ�
									//		�������� "name.xxx"���ļ���������·����
									//		�����ء� ture/false
		bool hasTempFile(QString filename);
									//��ȡ - ָ���ļ��Ƿ���temp�ļ�����
									//		�������� "F:/.../..."���ļ���
									//		�����ء� ture/false
		bool isInCurTempFile(QString filename);

		
	//-----------------------------------
	//----��׺
	private:
		QStringList skip_suffix;					//���Ժ�׺
		QStringList forbidden_suffix;				//��ֹ��׺
		QStringList default_skip_suffix;			//Ĭ�Ϻ��Ժ�׺
		QStringList default_forbidden_suffix;		//Ĭ�Ͻ�ֹ��׺
	public:
									//��׺ - ���˺��Եĺ�׺���ļ��и���ʱ��
									//		�����ء� ["xds","vcut"]���б�������·����
		QStringList getSkipSuffix();
									//��׺ - ���ù��˺�׺���ļ��и���ʱ��
									//		������1�� ["xds","vcut"]���б�������·����
		void setSkipSuffix(QStringList suffix_list);
									//��׺ - ������˺�׺
		void resetSkipSuffix();
									//��׺ - ��ֹ��׺���ļ��н�ֹ���ƣ�
									//		�����ء� ["exe"]���б�������·����
		QStringList getForbiddenSuffix();
									//��׺ - ���ý�ֹ��׺���ļ��н�ֹ���ƣ�
									//		������1�� ["exe"]���б�������·����
		void setForbiddenSuffix(QStringList suffix_list);
									//��׺ - �����ֹ��׺
		void resetForbiddenSuffix();

	private:
									//��׺ - �����ļ�
		bool isSkipFile(QFileInfo info);
									//��׺ - ��ֹ�ļ�
		bool isForbiddenFile(QFileInfo info);

		
	//-----------------------------------
	//----����
	public slots:
									//���� - �򿪻���·���ļ���
									//		�����ء� ���ɹ�����false
		bool openDesktopTempUrl();
									//���� - �򿪻���·����ָ���ļ�
									//		�������� "name.xxx"���ļ���������·����
									//		�����ء� ���ɹ�����false
		bool openDesktopTempFile(QString filename);
};

