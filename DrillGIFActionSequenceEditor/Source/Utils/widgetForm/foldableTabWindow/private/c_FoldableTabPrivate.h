#pragma once

#include <QWidget>

/*
-----==========================================================-----
		�ࣺ		�۵�ѡ� �ؼ��ࣨ˽�У�.h
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�۵�����ʹ�õ�˽���ࡣ
-----==========================================================-----
*/
class C_FoldableTabPrivate{

	public:
		C_FoldableTabPrivate();
		~C_FoldableTabPrivate();

	public:
		QString name;				//�ؼ��� ����
		QWidget* partWidget;		//�ؼ��� ����
		bool isInChildWindow;		//���Ӵ��ڱ��

		QJsonObject param;			//������������ʱ���ã�
		
	public:
									//���ж�
		bool isEmpty();
};