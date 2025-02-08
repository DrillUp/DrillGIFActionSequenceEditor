#include "stdafx.h"
#include <QtWidgets/QApplication>

#include "Source/MainModule/drillgifactionsequenceeditor.h"
#include "Source/MainModule/firstTip/w_SoftwareFirstTip.h"
#include "Source/ProjectModule/StorageGlobal/S_IniManager.h"
#include <QDebug>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	// > 主窗体
	DrillGIFActionSequenceEditor::getInstance()->show();
	
	// > 首页提示
	if (S_IniManager::getInstance()->getConfig("COAS_NoMoreFirstTip") != "true"){
		W_SoftwareFirstTip::getInstance()->show();
	}
	
	return a.exec();
}
