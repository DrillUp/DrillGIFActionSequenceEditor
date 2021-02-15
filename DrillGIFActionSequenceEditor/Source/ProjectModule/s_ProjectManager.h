#pragma once

#include "storage/s_StorageManagerTemplate.h"
#include "data/c_ProjectData.h"

/*
-----==========================================================-----
		�ࣺ		��Ŀ����.h
		�汾��		v1.12
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		��Ŀ����������ݡ���ز�������������Ҫ���ơ�
					����ϸ��cpp��
-----==========================================================-----
*/

class S_ProjectManager : public QObject, public S_StorageManagerTemplate
{
	Q_OBJECT

	public:
		S_ProjectManager();
		~S_ProjectManager();
		static S_ProjectManager* project_manager;		//����
		static S_ProjectManager* getInstance();			//��������ȡ�Լ�������Ҫ�õ�ȫ�����ò��ܽ��м��㣩
		
	//-----------------------------------
	//----ȫ��
	private:
		bool m_isDirty;							//�޸ı��
		QList<QWidget*> m_lockingWidgets;		//�������
	private:
										//#ȫ�� - �޸�����
		void changeTitle();
	public:
										//ȫ�� - �����������޸�
										//		��˵���� �κ�һ������accepted����Ӧ�õ��ô˷�����ȷ�����ݱ��޸Ĺ���
		void setDirty();
										//ȫ�� - �����޸Ĳ�����ʾ
										//		�����ء� true���ɹ�ִ�У�false����;��ȡ��������false��������ҲӦ��ȡ����
										//		��˵���� �൱��һ���м����ڵ㣬��ʾ���߱������̡�
		bool dirtyTip();
										//ȫ�� - ������Ŀ
										//		�������� �����ķ�ģ̬�Ի��򡣣�ģֱ̬�Ӿ������ˣ�����Ҫ���Ǹ÷�������
										//		��˵���� ���κ�һ�����ڣ���Ӧ��������Ŀ��ȷ������������������
		void addLock(QWidget* widget);
										//ȫ�� - ������Ŀ
										//		��˵���� �رմ��ں���Ҫ���øú�������������ݷ�ģ̬�Ի������������ƽ�����
		void removeLock(QWidget* widget);
										//ȫ�� - �ж�����
		bool isLocked();
										//ȫ�� - �ж��޸�
		bool isDirty();

	signals:							//ȫ�� - �޸�����
		void changeWindowTitle(QString title);
	signals:							//ȫ�� - �����仯
		void lockChanged(bool isLock);
	
	//-----------------------------------
	//----��Ŀ
	public slots:
										//��Ŀ - �½����Ի���
										//		�����ء� true���ɹ�ִ�У�false����;��ȡ����
										//		��˵���� �½���ʵ��Ч�����������Ŀ��������Ŀ��ҲΪ�ա�
		bool newProject();
										//��Ŀ - �򿪣��Ի���
										//		�����ء� true���ɹ�ִ�У�false����;��ȡ����
		bool openProject();	
										//��Ŀ - �򿪣�����ֵ��
										//		�����ء� true���ɹ�ִ�У�false��������Ч��
		bool openProjectByArgs(int argc, char *argv[]);
										//��Ŀ - �򿪣���ק�ļ���
										//		�����ء� true���ɹ�ִ�У�false����;��ȡ����
		bool openProjectByDropFile(QDropEvent *event);
										//��Ŀ - ���棨�Ի���
										//		�����ء� true���ɹ�ִ�У�false����;��ȡ����
		bool saveProject();	
										//��Ŀ - ���Ϊ���Ի���
										//		�����ء� true���ɹ�ִ�У�false����;��ȡ����
		bool saveAs();
										//��Ŀ - ǿ�Ʊ���
										//		�����ء� true���ɹ�ִ�У�false����;��ȡ����
		bool saveInForce();

	private:
										//#��Ŀ - �����Ŀ����
		void clearProject();	
										//#��Ŀ - �򿪣������ļ�����
		void openProjectDirectly(QString open_path);
										//#��Ŀ - ���棨Ŀ���ļ��У�
		void saveAll(QString url);
										//#��Ŀ - ���棨�ļ���
		void createSaveFile();
										//#��Ŀ - ��ȡ���ļ���
		void readSaveFile();
										//#��Ŀ - ��ȡ�ļ������ݺ�׺��F:/aaa/bbb.suf����·����
		QString getOneFileBySuffix(QString url,QString suffix);	
		

	//-----------------------------------
	//----�洢���ݣ������s_StorageManager��
	public:
		C_ProjectData data_ProjectData;
	public:
										//���� - ��ȡ�洢������
		QString getSaveName();
										//���� - �����ǰ����������
		void clearAllData();
										//���� - ȫ����Ŀ�������� -> QJsonObject
		QJsonObject getAllDataOfJsonObject();
										//���� - QJsonObject -> ȫ����Ŀ��������
		void setAllDataFromJsonObject(QJsonObject obj);

		
	//-----------------------------------
	//----����
	public slots:
									//���� - ����Ŀ·���ļ���
									//		�����ء� ���ɹ�����false
		bool openDesktopProjectUrl();
									//���� - ����Ŀ·����ָ���ļ�
									//		�������� "name.xxx"���ļ���������·����
									//		�����ء� ���ɹ�����false
		bool openDesktopProjectFile(QString filename);
};

