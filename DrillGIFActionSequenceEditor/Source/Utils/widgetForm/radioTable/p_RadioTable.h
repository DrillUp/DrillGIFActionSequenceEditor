#pragma once

#include <QKeyEvent>
#include <QTableWidget>

#include "C_RaTConfig.h"

/*
-----==========================================================-----
		�ࣺ		��ѡ���.cpp
		�汾��		v1.00
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		������ȫ����ʾ�����ܵ�ѡ�������������ܣ�
					����ϸ��cpp��
-----==========================================================-----
*/
class P_RadioTable : public QObject
{
	Q_OBJECT

	public:
		P_RadioTable(QTableWidget *parent);		//���캯��
		~P_RadioTable();						//��������
		
	//-----------------------------------
	//----�ؼ�
	protected:
		QTableWidget* m_table;						//������
		QString m_tableStyle;						//���Ĭ����ʽ
	public:
									//�ؼ� - ˢ����
		void refreshTableUi();
									//�ؼ� - ����ȫ��
		void clearAll();
	signals:
									//�ؼ� - ѡ��ѡ��仯
		void currentIndexChanged(int index);
									//�ؼ� - ѡ���ı��仯
		void currentTextChanged(QString text);
		
	//-----------------------------------
	//----���ݣ�������ã�
	protected:
		C_RaTConfig m_config;	//�������
	public:
										//������� - ���ò���
		void setConfigParam(QJsonObject config);
										//������� - ȡ������
		QJsonObject getConfigParam();
	protected slots:
										//������� - �༭����
		void openConfigParamWindow();

	//-----------------------------------
	//----����¼�
	public:
		bool m_selectionSignalBlock_Root;	//Ȧѡ ��
	public slots:
											//�Ҽ��¼�
		void sltItemRightClicked(QPoint point);
											//Ȧѡ�仯�¼�
		void sltItemSelectionChanged();

	//-----------------------------------
	//----��Դ����
	protected:
		QStringList local_text;		//��Դ����
	public:
									//��Դ���� - ��������
		void setSource(QStringList text_list);
									//��Դ���� - ȡ������
		int getSelectedIndex();
		QString getSelectedText();

};
