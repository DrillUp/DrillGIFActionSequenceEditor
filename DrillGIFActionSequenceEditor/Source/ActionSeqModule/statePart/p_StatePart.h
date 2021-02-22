#ifndef P_StatePart_H
#define P_StatePart_H

#include <QtWidgets>
#include "ui_P_StatePart.h"
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
	//----����֡
	public:
		P_AnimationListEditor* m_p_AnimationListEditor;
		P_AnimationListPlayer* m_p_AnimationListPlayer;
	public:
									//����֡ - ѡ��仯
		void tableChanged_Multi(QList<int> index);
		

	//-----------------------------------
	//----����
	public:
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
