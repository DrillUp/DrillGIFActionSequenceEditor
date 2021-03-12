#ifndef P_PlayingPart_H
#define P_PlayingPart_H

#include <QtWidgets>
#include "ui_P_PlayingPart.h"

#include "Source/ActionSeqModule/actionPart/p_ActionPart.h"
#include "Source/ActionSeqModule/statePart/p_StatePart.h"
#include "Source/ActionSeqModule/reprintedCore/Drill_COAS_Data.h"
#include "Source/ActionSeqModule/reprintedCore/Drill_COAS_Init.h"
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
		P_PlayingPart(P_StatePart* state_part, P_ActionPart* action_part, QWidget *parent = 0);
		~P_PlayingPart();
		
	//-----------------------------------
	//----�ؼ�
	public:
		P_ActionPart* m_actionPart;
		P_StatePart* m_statePart;
		P_RadioTable* m_table_action;
		P_RadioTable* m_table_state;
		P_AnimPictureViewer* m_p_AnimPictureViewer;
	public slots:
										//�ؼ� - �༭״̬Ԫ����
		void editDefaultStateGroup();
										//�ؼ� - �����ļ�����ȡ��Դ�ļ�
		QFileInfo getSrcFileByName(QString file_name);
										//�ؼ� - ��ȡ����
		QStringList getActionNameList();
		QStringList getStateNameList();
		QStringList getStateNameListWithoutEmpty();
										//�ؼ� - ��ȡȫ�������ļ�����ȥ�أ�
		QStringList getRelatFileNameList();
										//�ؼ� - ���ű����仯
		void zoomValueChanged(double value);

	//-----------------------------------
	//----��ť
	protected:
		QString m_iconSrcPath;
	public slots:
										//��ť - ����
		void btn_play();
										//��ť - ����Ĭ��״̬Ԫ
		void btn_playDefault();
										//��ť - ����״̬Ԫ
		void btn_playState();
										//��ť - ���붯��Ԫ
		void btn_playAction();
	private:
										//��ť - ˢ�²���ͼ��
		void updateIcon();
		
	//-----------------------------------
	//----����֡
	private:
		bool m_playing;					//���ڲ���
		QTimer* m_timer;				//��ʱ��
	public:
										//����֡ - ��ʼ
		void startFrame();
										//����֡ - ��ͣ
		void stopFrame();
										//����֡ - ���ڲ���
		bool isPlaying();

	private slots:
										//����֡ - ֡ˢ��
		void updateFrame();
	signals:
										//����֡ - ��ʼ���ţ��źţ�
		void playStarted();

	//-----------------------------------
	//----��Դ�б�
	private:
		Drill_COAS_Data m_COAS_data;		//������������


	//-----------------------------------
	//----����
	public:
		QStringList local_defaultStateList;
		QList<QJsonObject> local_stateDataList;
		QList<QJsonObject> local_actionDataList;
	public:
										//���� - ˢ�»��ڵ�����
		void refreshSource();
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
