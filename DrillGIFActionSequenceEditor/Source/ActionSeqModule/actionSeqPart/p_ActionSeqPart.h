#ifndef P_ActionSeqPart_H
#define P_ActionSeqPart_H

#include <QtWidgets>
#include "ui_P_ActionSeqPart.h"

#include "Source/ActionSeqModule/actionPart/p_ActionPart.h"
#include "Source/ActionSeqModule/statePart/p_StatePart.h"
#include "Source/ActionSeqModule/playingPart/p_PlayingPart.h"
#include "Source/ActionSeqModule/actionSeqData/lengthData/c_ActionSeqLength.h"

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
	//----��ݼ�
	protected:
		QJsonObject m_copyed_actionSeq;
	protected:
									//���� - �滻
		void op_replace(int index, QJsonObject actiong_seq);
									//���� - ���
		void op_clear(int index);
	public:
									//��ݼ� - �¼�
		void keyPressEvent(QKeyEvent *event);
									//��ݼ� - ����
		void shortcut_copyData();
									//��ݼ� - ճ��
		void shortcut_pasteData();
									//��ݼ� - ���
		void shortcut_clearData();

	//-----------------------------------
	//----��ؼ�
	protected:
		P_FoldableTabRelater* m_p_FoldableTabRelater;	//���۵�ѡ�
		P_ActionPart* m_actionPart;						//����Ԫ��
		P_StatePart* m_statePart;						//״̬Ԫ��
		P_PlayingPart* m_playingPart;					//��ӳ��
	public:
										//��ؼ� - �û�
										//		��˵������ѡ��һ���������к��ûҲŻᱻȡ����
		void setPartGray();
										//��ؼ� - ֹͣ����
		void stopPlaying();

	//-----------------------------------
	//----����
	public:
		bool m_slotBlock_source;					//����ʱ����
		QList<QJsonObject> m_actionSeq_list;		//���������б���ʱ��
	public:
										//���� - ��������
		void setData(QJsonObject actionSeq, C_ActionSeqLength length);
										//���� - ȡ������
		QJsonObject getDataActionSeqData();
		C_ActionSeqLength getDataActionSeqLength();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();
	public:
										//���� - �޸ĳ���
		void modifyDataLengthInAction();
		
	public:
										//���� - �û��Զ���UI��ȡ
		void ui_loadConfig();
										//���� - �û��Զ���UI�洢
		void ui_saveConfig();
	private:
		Ui::P_ActionSeqPart ui;

};

#endif // P_ActionSeqPart_H
