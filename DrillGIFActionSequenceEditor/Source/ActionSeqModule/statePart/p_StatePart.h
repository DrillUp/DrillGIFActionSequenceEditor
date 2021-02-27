#ifndef P_StatePart_H
#define P_StatePart_H

#include <QtWidgets>
#include "ui_P_StatePart.h"

#include "Source/GraphModule/widget/animPictureViewer/p_AnimPictureViewer.h"

#include "Source/Utils/widgetForm/radioTable/p_RadioTable.h"
#include "Source/Utils/widgetFastForm/fastForm/p_FastForm.h"
#include "Source/Utils/widgetFormSenior/AnimationListEditor/p_AnimationListEditor.h"
#include "Source/Utils/widgetFormSenior/AnimationListPlayer/p_AnimationListPlayer.h"

/*
-----==========================================================-----
		�ࣺ		״̬Ԫ��.h
		���ߣ�		drill_up
		����ģ�飺	״̬Ԫģ��
		���ܣ�		�ò����ṩ״̬Ԫ�༭���ܡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class P_StatePart : public QWidget
{
	Q_OBJECT

	public:
		P_StatePart(QWidget *parent = 0);
		~P_StatePart();
		
	//-----------------------------------
	//----�ؼ�
	public:
		P_RadioTable* m_table;
		P_FastForm* m_FastForm;
	public slots:
										//�ؼ� - ��ȡ����Ԫ����
		QStringList getNameList();
										//�ؼ� - ����Ԫ�л�
		void currentIndexChanged(int index);
		
	//-----------------------------------
	//----����֡
	public:
		P_AnimationListEditor* m_p_AnimationListEditor;
		P_AnimationListPlayer* m_p_AnimationListPlayer;
		P_AnimPictureViewer* m_p_AnimPictureViewer;
	public:
									//����֡ - ѡ��仯
		void tableChanged_Multi(QList<int> index);
									//����֡ - ��Դ�л�
		void bitmapChanged();
									//����֡ - ���ű����л�
		void zoomValueChanged(double value);
		
	//-----------------------------------
	//----��ݼ�
	public:
									//��ݼ� - �¼�
		void keyPressEvent(QKeyEvent *event);
		
	//-----------------------------------
	//----�������ݵ�����
	protected:
		int m_last_index;
	public:
									//���� - ���汾������
		void local_saveCurIndexData();
									//���� - ��ȡ��������
		void local_loadIndexData(int index);

	//-----------------------------------
	//----����
	public:
		bool m_slotBlock_source;
		QList<QJsonObject> local_stateDataList;
	public:
										//���� - ��������
										//		��˵����������Ԫֻ�༭����Ԫ�أ����ı����鳤�ȡ�
		void setData(QList<QJsonObject> stateDataList);
										//���� - ȡ������
										//		��˵�������ⲿ���պ󣬻���Ҫ�ֶ�������ݡ�
		QList<QJsonObject> getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	private:
		Ui::P_StatePart ui;

};

#endif // P_StatePart_H
