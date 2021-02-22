#ifndef P_PlayingPart_H
#define P_PlayingPart_H

#include <QtWidgets>
#include "ui_P_PlayingPart.h"

#include "Source/Utils/widgetForm/radioTable/p_RadioTable.h"

/*
-----==========================================================-----
		�ࣺ		��ӳ��.h
		���ߣ�		drill_up
		����ģ�飺	��������ģ��
		���ܣ�		�ò����ṩ�������еķ�ӳ��
					����ϸ��cpp��
-----==========================================================-----
*/
class P_PlayingPart : public QWidget
{
	Q_OBJECT

	public:
		P_PlayingPart(QWidget *parent = 0);
		~P_PlayingPart();
		
	//-----------------------------------
	//----�ؼ�
	public:
		P_RadioTable* m_table_action;
		P_RadioTable* m_table_state;
	public:
										//�ؼ� - ��ʼ����
		void openPlay();
										//�ؼ� - ��ȡ����
		QStringList getStateNameList();
		QStringList getActionNameList();

	//-----------------------------------
	//----����
	public:
		QStringList local_defaultStateList;
		QList<QJsonObject> local_stateDataList;
		QList<QJsonObject> local_actionDataList;
	public:
										//���� - ��������
		void setSource(QList<QJsonObject> stateDataList, QList<QJsonObject> actionDataList);
										//���� - ��������
		void setDefaultStateData(QStringList defaultStateList);
										//���� - ȡ������
		QStringList getDefaultStateData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	private:
		Ui::P_PlayingPart ui;

};

#endif // P_PlayingPart_H
