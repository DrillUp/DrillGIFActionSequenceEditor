#ifndef DRILLGIFACTIONSEQUENCEEDITOR_H
#define DRILLGIFACTIONSEQUENCEEDITOR_H

#include <QtWidgets/QMainWindow>
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
class DrillGIFActionSequenceEditor : public QMainWindow
{
	Q_OBJECT

	public:
		DrillGIFActionSequenceEditor(QWidget *parent = 0);
		~DrillGIFActionSequenceEditor();

	private:
		Ui::DrillGIFActionSequenceEditorClass ui;
};

#endif // DRILLGIFACTIONSEQUENCEEDITOR_H
