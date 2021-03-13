#ifndef P_PROJECT_CREATE_H
#define P_PROJECT_CREATE_H

#include "ui_p_ProjectCreate.h"
#include "c_ProjectData.h"

/*
-----==========================================================-----
		�ࣺ		�½���Ŀ��.h
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		���������̺󣬽��½���Ŀ�ļ��С�
					�����½���Ŀ�����Ϊ��Ŀ��
					����ϸ��cpp��
-----==========================================================-----
*/
class P_ProjectCreate : public QWidget
{
	Q_OBJECT

	public:
		P_ProjectCreate(QWidget* parent = 0);
		~P_ProjectCreate();
		
	//-----------------------------------
	//----����
	public slots:
										//���� - �����޸�
		void nameChanged(QString name);
										//���� - λ���޸�
		void pathChanged(QString path);
										//���� - ѡ���ļ���
		void chooseFiles();		
										//���� - �����ļ����޸�
		void radioChanged(bool toggled);
										//���� - ˢ������·��
		void refreshOutput();
		
	//-----------------------------------
	//----λ�ã����ｫui��װ��һ�£���ֹ·�����ҵ��ã�
	public:
		QStringList m_pathList;
	public:
										//λ�� - ���λ��
		void insertPath(QString path);
										//λ�� - ѡ��λ��
		void selectPath(QString path);
										//λ�� - ��ȡѡ���λ��
		QString getCurrentPath();

	//-----------------------------------
	//----����
	public:
		C_ProjectData local_ProjectData;
	public:
										//���� - ��������
		void setDataInModifyMode(C_ProjectData p);
										//���� - ȡ������
		C_ProjectData getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();
	private:
		Ui::P_ProjectCreate ui;
};

#endif 