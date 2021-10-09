#pragma once
#include <QList>
#include <QMap>

#define PROJECT_SUFFIX		"drillas"										//��Ŀ�ļ���׺
#define PROJECT_INSTANCE	DrillGIFActionSequenceEditor::getInstance()		//�����ڹ���

/*
-----==========================================================-----
		�ࣺ		��Ŀ���� ʵ����.h
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		�洢��Ŀ�Ļ������ݵ������ࡣ
-----==========================================================-----
*/
class C_ProjectData{

	public:
		C_ProjectData();
		~C_ProjectData();
		
	//-----------------------------------
	//----��Ŀ����
	private:
		QString name;					//��Ŀ����ǿ��˽�У���Ŀ����
		QString path;					//��Ŀ·����ǿ��˽�У�D:/aaa/vvv/��Ŀ��/��
	public:
										//��ȡ��Ŀ��
		QString getName();
										//��ȡ��Ŀ��׺
		QString getSuffix();
										//������Ŀ��
		void setName(QString name);
										//������Ŀ·��
		void setPath(QString path);
										//��ȡ��Ŀ��Ŀ¼��D:/aaa/vvv/��
		QString getParentPath();
										//��ȡ��Ŀ·����D:/aaa/vvv/��Ŀ��/��
		QString getProjectRootPath();
										//��ȡ��Ŀ�ļ��У�D:/aaa/vvv/��Ŀ��/��Ŀ��Files/��
		QString getProjectFilePath();
										//��ȡ��Ŀ�ļ���D:/aaa/vvv/��Ŀ��/��Ŀ��.xxx��
		QString getProjectFile();
		
	//-----------------------------------
	//----�������
	public:
		static QString softname;				//�����
		static QString version;					//����汾
		static QString version_serial;			//����汾��
		QString note;							//��Ŀ�ʼǣ���ע��
		QDateTime createDate;					//����ʱ��
		QDateTime lastSaveDate;					//��һ���޸�ʱ��
	public:
										//��ȡ���ȫ��
		QString getSoftname();
		
	//-----------------------------------
	//----������
	public:
										//���ж�
		bool isNull();
										//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
										//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);

};
