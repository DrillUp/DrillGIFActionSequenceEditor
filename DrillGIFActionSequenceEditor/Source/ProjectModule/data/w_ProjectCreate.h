#ifndef W_PROJECT_CREATE_H
#define W_PROJECT_CREATE_H

#include "ui_w_ProjectCreate.h"
#include "c_ProjectData.h"

/*
-----==========================================================-----
		�ࣺ		�½���Ŀ����.cpp
		����ģ�飺	��Ŀ����ģ��
		���ܣ�		���������̺󣬽��½���Ŀ�ļ��С�
					�����½���Ŀ�����Ϊ��Ŀ��
-----==========================================================-----
*/
class W_ProjectCreate : public QDialog
{
	Q_OBJECT

	public:
		W_ProjectCreate(QWidget* parent = 0);
		~W_ProjectCreate();
		
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
		void addPath(QString path);
										//λ�� - ѡ��λ��
		void selectPath(QString path);
										//λ�� - ��ȡѡ���λ��
		QString getCurrentPath();


	//-----------------------------------
	//----����
	public:
		//bool isAddMode;
		C_ProjectData local_ProjectData;
	public:
										//���� - �������ݣ�ֻ�޸ģ�
		void setDataInModifyMode(C_ProjectData p);
										//���� - ȡ������
		C_ProjectData getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	public slots:
										//���� - �ύ���ݣ�У�飩
		void acceptData();	

	private:
		Ui::W_ProjectCreate ui;
	//-----------------------------------

};

#endif 