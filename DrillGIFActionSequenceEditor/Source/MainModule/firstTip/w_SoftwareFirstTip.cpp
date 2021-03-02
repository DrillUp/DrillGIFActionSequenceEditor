#include "stdafx.h"
#include "W_SoftwareFirstTip.h"

#include "../drillgifactionsequenceeditor.h"
#include "Source/ProjectModule/storageGlobal/s_IniManager.h"
#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		��ҳ��ʾ��.cpp
		����ģ�飺	������ģ��
		���ܣ�		��ҳ��ʾ��

		ʹ�÷�����
				>�򿪴���
					W_SoftwareFirstTip d;
					d.exec();

-----==========================================================-----
*/
W_SoftwareFirstTip::W_SoftwareFirstTip(QWidget *parent)
	: QDialog(parent)
{
	W_SoftwareFirstTip::cur_instance = this;
	ui.setupUi(this);
	this->_init();
}
W_SoftwareFirstTip::~W_SoftwareFirstTip(){
}
/*-------------------------------------------------
		����
*/
W_SoftwareFirstTip* W_SoftwareFirstTip::cur_instance = NULL;
W_SoftwareFirstTip* W_SoftwareFirstTip::getInstance() {
	if (cur_instance == NULL) {
		cur_instance = new W_SoftwareFirstTip(DrillGIFActionSequenceEditor::getInstance());
	}
	return cur_instance;
}
/*-------------------------------------------------
		��ʼ��
*/
void W_SoftwareFirstTip::_init() {

	//-----------------------------------
	//----�¼���
	connect(ui.toolButton, &QToolButton::clicked, this, &W_SoftwareFirstTip::openUserManual);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(acceptData()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(rejectData()));

	//-----------------------------------
	//----ui��ʼ��
	TTool::_chinese_(ui.buttonBox);

	
}


/* --------------------------------------------------------------
		�ؼ� - �û��ֲ�
*/
void W_SoftwareFirstTip::openUserManual() {
	QString sPath = qApp->applicationDirPath();

	QString docx = sPath + "/help/����GIF�������к��ı༭��.docx";
	if (QFileInfo(docx).exists()){
		QDesktopServices::openUrl(QUrl("file:/" + docx)); 
	}else{
		QMessageBox::warning(this, "����", "�ĵ�\"����GIF�������к��ı༭��.docx\"�����ˡ�", QMessageBox::Yes);
	}

}


void W_SoftwareFirstTip::acceptData(){
	//(�޲���)

	this->accept();
}
void W_SoftwareFirstTip::rejectData(){

	if (ui.checkBox->isChecked() == true){
		S_IniManager::getInstance()->setConfig("COAS_NoMoreFirstTip", "true");
	}

	this->reject();
}
/* --------------------------------------------------------------
		�ؼ� - ���ڹر��¼�������رհ�ť��
*/
void W_SoftwareFirstTip::closeEvent(QCloseEvent *event){
	this->rejectData();
	event->accept();
}