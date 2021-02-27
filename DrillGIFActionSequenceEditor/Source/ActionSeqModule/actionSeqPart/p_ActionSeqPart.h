#ifndef P_ActionSeqPart_H
#define P_ActionSeqPart_H

#include <QtWidgets>
#include "ui_P_ActionSeqPart.h"

#include "Source/ActionSeqModule/actionPart/p_ActionPart.h"
#include "Source/ActionSeqModule/statePart/p_StatePart.h"
#include "Source/ActionSeqModule/playingPart/p_PlayingPart.h"

#include "Source/Utils/widgetForm/foldableTabWindow/p_FoldableTabRelater.h"
#include "Source/Utils/widgetFormSenior/flexibleClassificationTree/p_FlexibleClassificationTree.h"

/*
-----==========================================================-----
		�ࣺ		�������п�.h
		���ߣ�		drill_up
		����ģ�飺	��������ģ��
		���ܣ�		�ò����ṩ�������з�ӳ�����ܣ���Ƕ��״̬Ԫ������Ԫ�Ŀ顣
					����ϸ��cpp��
-----==========================================================-----
*/
class P_ActionSeqPart : public QWidget
{
	Q_OBJECT

	public:
		P_ActionSeqPart(QWidget *parent = 0);
		~P_ActionSeqPart();
		
	//-----------------------------------
	//----�������� ȫ����
	protected:
		P_FlexibleClassificationTree* m_p_tree;		//����������ʾ��

		int m_realLen_actionSeq;					//���� - ��������
		int m_realLen_action;						//���� - ����Ԫ
		int m_realLen_state;						//���� - ״̬Ԫ

	public slots:
										//ȫ���� - ��ѡ��仯
		void currentActionSeqChanged(QTreeWidgetItem* item, int id, QString name);
		
	//-----------------------------------
	//----�������ݵ�����
	protected:
		int m_cur_actionSeqIndex;				//��ǰ������������
		QJsonObject m_cur_actionSeq;			//��ǰ��������
		QList<QJsonObject> m_cur_actionTank;	//��ǰ����Ԫ����
		QList<QJsonObject> m_cur_stateTank;		//��ǰ״̬Ԫ����
	public:
									//���� - ���汾������
		void local_saveCurIndexData();
									//���� - ��ȡ��������
		void local_loadIndexData(int index);

	//-----------------------------------
	//----��ؼ�
	protected:
		P_FoldableTabRelater* m_p_FoldableTabRelater;	//���۵�ѡ�
		P_ActionPart* m_actionPart;						//����Ԫ��
		P_StatePart* m_statePart;						//״̬Ԫ��
		P_PlayingPart* m_playingPart;					//��ӳ��

	//-----------------------------------
	//----����
	public:
		bool m_slotBlock_source;
		QJsonObject local_actionSeq;				//������������
		QList<QJsonObject> m_actionSeq_list;		//���������б���ʱ��
	public:
										//���� - ��������
		void setData(QJsonObject actionSeq);
										//���� - ȡ������
		QJsonObject getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	private:
		Ui::P_ActionSeqPart ui;

};

#endif // P_ActionSeqPart_H
