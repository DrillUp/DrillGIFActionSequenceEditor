#include "stdafx.h"
#include "Source/MainModule/drillgifactionsequenceeditor.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	DrillGIFActionSequenceEditor::getInstance()->show();
	
	return a.exec();
}
