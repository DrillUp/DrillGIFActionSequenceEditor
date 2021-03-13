#include "stdafx.h"
#include "W_SoftwareAbout.h"

#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/Utils/common/TTool.h"


/*
-----==========================================================-----
		�ࣺ		���ڴ���.cpp
		����ģ�飺	������ģ��
		���ܣ�		���ڴ��ڡ�

		ʹ�÷�����
				>�򿪴���
					W_SoftwareAbout d;
					d.exec();

-----==========================================================-----
*/
W_SoftwareAbout::W_SoftwareAbout(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	//-----------------------------------
	//----�¼���
	connect(ui.toolButton, &QToolButton::clicked, this, &W_SoftwareAbout::openUserManual);

	//-----------------------------------
	//----ui��ʼ��
	TTool::_chinese_(ui.buttonBox);

	ui.label_title->setText("GIF�������б༭����" + S_ProjectManager::getInstance()->data_ProjectData.version + "��");

}

W_SoftwareAbout::~W_SoftwareAbout(){
}


/* --------------------------------------------------------------
		�ؼ� - �û��ֲ�
*/
void W_SoftwareAbout::openUserManual() {
	QString sPath = qApp->applicationDirPath();

	QString docx = sPath + "/help/����GIF�������к��ı༭��.docx";
	if (QFileInfo(docx).exists()){
		QDesktopServices::openUrl(QUrl("file:/" + docx)); 
	}else{
		QMessageBox::warning(this, "����", "�ĵ�\"����GIF�������к��ı༭��.docx\"�����ˡ�", QMessageBox::Yes);
	}

}