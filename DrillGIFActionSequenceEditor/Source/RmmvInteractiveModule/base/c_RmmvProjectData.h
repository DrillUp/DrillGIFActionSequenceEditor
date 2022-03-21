#pragma once
#include <QJsonObject>

/*
-----==========================================================-----
		�ࣺ		rmmv�������� ʵ����.h
		�汾��		v1.01	�����಻�ٸ��¡�
		����ģ�飺	����ģ��
		���ܣ�		���̻�����Ϣ���ݡ�
					����ϸ��.cpp��
-----==========================================================-----
*/
class C_RmmvProjectData{

	public:
		C_RmmvProjectData();
		~C_RmmvProjectData();
		
	//-----------------------------------
	//----����
	protected:
		QString name;				//������
		QString version;			//���̰汾
		QString path;				//���̸�·��
	public:
		bool optional_backup;
	public:
												//���� - ���������ļ�
		void initFromFile(QString full_filePath);
												//���� - ��ȡ��������
												//		��˵������index.html�е������������ơ�
		QString getName();
												//���� - ��ȡ���̰汾
		QString getVersion();
												//���� - ��ȡ���̸�·��
												//		��˵���������̸�·��������"C:/aaa/�������ʾ��"��
		QString getRootPath();
												//���� - �жϹ����Ƿ����
		bool isProjectExist();
	
	//-----------------------------------
	//----������
	public:
												//���ж�
		bool isNull();
												//ʵ���� -> QJsonObject
		QJsonObject getJsonObject();
												//QJsonObject -> ʵ����
		void setJsonObject(QJsonObject obj);
												//���������
		const bool operator== (const C_RmmvProjectData& a)const;
};
