#pragma once

#include <QKeyEvent>
#include <QTableWidget>

#include "C_RaTConfig.h"

/*
-----==========================================================-----
		�ࣺ		��ѡ���.cpp
		�汾��		v1.02
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
		QList<QTableWidgetItem*> m_itemTank;		//������б�
	public:
									//�ؼ� - ˢ����
		void refreshTableUi();
									//�ؼ� - �޸�ָ��λ���ı�������textchange�źţ�
		void modifyText(int index, QString text);
									//�ؼ� - �޸�ѡ�����ı�������textchange�źţ�
		void modifyText_Selected( QString text );
									//�ؼ� - ����ȫ��
		void clearAll();
		
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

		
	//-----------------------------------
	//----ѡ��
	public slots:
									//ѡ�� - ����ѡ��
									//		�������������֣���������Խ�磬Խ����ѡ����ӽ���һ�
		void selectIndex(int index);
									//ѡ�� - ����ѡ��
									//		�����������ַ����������ͬ�ַ�����ֻѡ��һ����û��ƥ�������Ч��
		void selectText(QString text);
									//ѡ�� - ѡ����һ��
		void selectLast();
									//ѡ�� - ѡ����һ��
		void selectNext();
									//ѡ�� - ѡ������
		void selectStart();
									//ѡ�� - ѡ��β��
		void selectEnd();
	signals:
									//ѡ�� - ѡ��ѡ��仯
		void currentIndexChanged(int index);
									//ѡ�� - ѡ���ı��仯
		void currentTextChanged(QString text);

};
