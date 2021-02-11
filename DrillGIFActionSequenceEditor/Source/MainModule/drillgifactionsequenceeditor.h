#ifndef DRILLGIFACTIONSEQUENCEEDITOR_H
#define DRILLGIFACTIONSEQUENCEEDITOR_H

#include <QtWidgets/QDialog>
#include "ui_drillgifactionsequenceeditor.h"

/*
-----==========================================================-----
		�ࣺ		������.h
		���ߣ�		drill_up
		����ģ�飺	������ģ��
		���ܣ�		��������������ƴ��ڡ�
					����ϸ��cpp��
-----==========================================================-----
*/
class DrillGIFActionSequenceEditor : public QDialog
{
	Q_OBJECT

	public:
		DrillGIFActionSequenceEditor(QWidget *parent = 0);
		~DrillGIFActionSequenceEditor();
		static DrillGIFActionSequenceEditor* cur_instance;		//����
		static DrillGIFActionSequenceEditor* getInstance();		//��������ȡ�Լ�
		void _init();											//��ʼ��
		
	//-----------------------------------
	//----�ؼ�
	public:
								//�ؼ� - ��rmmv��������
		void openWindowRmmvInteractive();

	//-----------------------------------
	//----����
	private:
		Ui::DrillGIFActionSequenceEditorClass ui;
};

#endif // DRILLGIFACTIONSEQUENCEEDITOR_H
