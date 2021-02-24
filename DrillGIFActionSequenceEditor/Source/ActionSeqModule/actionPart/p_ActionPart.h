#ifndef P_ActionPart_H
#define P_ActionPart_H

#include <QtWidgets>
#include "ui_P_ActionPart.h"

#include "Source/GraphModule/widget/animPictureViewer/p_AnimPictureViewer.h"

#include "Source/Utils/widgetForm/radioTable/p_RadioTable.h"
#include "Source/Utils/widgetFormSenior/AnimationListEditor/p_AnimationListEditor.h"
#include "Source/Utils/widgetFormSenior/AnimationListPlayer/p_AnimationListPlayer.h"

/*
-----==========================================================-----
		�ࣺ		����Ԫ��.h
		���ߣ�		drill_up
		����ģ�飺	����Ԫģ��
		���ܣ�		�ò����ṩ����Ԫ�ı༭��
					����ϸ��cpp��
-----==========================================================-----
*/
class P_ActionPart : public QWidget
{
	Q_OBJECT

	public:
		P_ActionPart(QWidget *parent = 0);
		~P_ActionPart();
		
	//-----------------------------------
	//----�ؼ�
	public:
		P_RadioTable* m_table;
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
	//----����
	public:
		QList<QJsonObject> local_actionDataList;
	public:
										//���� - ��������
										//		��˵����������Ԫֻ�༭����Ԫ�أ����ı����鳤�ȡ�
		void setData(QList<QJsonObject> actionDataList);
										//���� - ȡ������
										//		��˵�������ⲿ���պ󣬻���Ҫ�ֶ�������ݡ�
		QList<QJsonObject> getData();
										//���� - �������� -> ui����
		void putDataToUi();							
										//���� - ui���� -> ��������
		void putUiToData();

	private:
		Ui::P_ActionPart ui;

};

#endif // P_ActionPart_H
