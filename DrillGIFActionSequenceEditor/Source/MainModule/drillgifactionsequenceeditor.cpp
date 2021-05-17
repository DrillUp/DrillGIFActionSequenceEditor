#include "stdafx.h"
#include "drillgifactionsequenceeditor.h"

#include "about/w_SoftwareAbout.h"
#include "Source/RmmvInteractiveModule/operateBoard/w_RmmvOperateBoard.h"
#include "Source/RmmvInteractiveModule/base/s_RmmvDataContainer.h"
#include "Source/ActionSeqModule/actionSeqPart/p_ActionSeqPart.h"
#include "Source/ActionSeqModule/actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/ActionSeqModule/actionSeqData/lengthData/w_ActionSeqLength.h"
#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/ProjectModule/file/s_TempFileManager.h"
#include "Source/ProjectModule/storageGlobal/s_IniManager.h"

/*
-----==========================================================-----
		�ࣺ		������.cpp
		���ߣ�		drill_up
		����ģ�飺	������ģ��
		���ܣ�		��������������ƴ��ڡ�
					
-----==========================================================-----
*/
DrillGIFActionSequenceEditor::DrillGIFActionSequenceEditor(QWidget *parent)
	: QDialog(parent)
{
	DrillGIFActionSequenceEditor::cur_instance = this;
	ui.setupUi(this);
	this->_init();
}
DrillGIFActionSequenceEditor::~DrillGIFActionSequenceEditor(){
}

/*-------------------------------------------------
		����
*/
DrillGIFActionSequenceEditor* DrillGIFActionSequenceEditor::cur_instance = NULL;
DrillGIFActionSequenceEditor* DrillGIFActionSequenceEditor::getInstance() {
	if (cur_instance == NULL) {
		cur_instance = new DrillGIFActionSequenceEditor(NULL);
	}
	return cur_instance;
}
/*-------------------------------------------------
		��ʼ��
*/
void DrillGIFActionSequenceEditor::_init() {

	//-----------------------------------
	//----���/��С��
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowMinimizeButtonHint;
	this->setWindowFlags(flags);
	this->m_last_uiSize = this->size();

	//-----------------------------------
	//----��ʼ��ui
	QHBoxLayout* l = new QHBoxLayout();
	ui.widget_actionSeq->setLayout(l);
	this->m_p_ActionSeqPart = new P_ActionSeqPart(ui.widget_actionSeq);
	l->setMargin(0);
	l->addWidget(this->m_p_ActionSeqPart);

	// > UI��ȡ
	this->ui_loadConfig();
	
	//-----------------------------------
	//----��ʼ������
	this->m_w_RmmvOperateBoard = nullptr;
	S_ProjectManager::getInstance();
	S_RmmvDataContainer::getInstance();
	S_ActionSeqDataContainer::getInstance();

	//-----------------------------------
	//----�¼���
	connect(ui.toolButton_new, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::newProject);
	connect(ui.toolButton_open, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openProject);
	connect(ui.toolButton_save, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::saveProject);
	connect(ui.toolButton_saveAs, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::saveAsProject);
	connect(ui.toolButton_rmmv, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openWindowRmmvInteractive);
	connect(ui.toolButton_userManual, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openUserManual);
	connect(ui.toolButton_about, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openAbout);
	connect(S_ProjectManager::getInstance(), &S_ProjectManager::changeWindowTitle, this, &DrillGIFActionSequenceEditor::changeWindowTitle);
	// ��ע��rmmv����������Ҫ�������ӣ������ڴ浵��ȡʱ��������
	connect(S_RmmvDataContainer::getInstance(), &S_RmmvDataContainer::dataAllReloaded, this, &DrillGIFActionSequenceEditor::rmmvInteractiveDataLoaded);
	connect(S_ActionSeqDataContainer::getInstance(), &S_ActionSeqDataContainer::dataAllReloaded, this, &DrillGIFActionSequenceEditor::actionSeqDataLoaded);

}

/*-------------------------------------------------
		�ؼ� - ��rmmv��������
*/
void DrillGIFActionSequenceEditor::openWindowRmmvInteractive(){
	if (this->m_w_RmmvOperateBoard == nullptr){
		this->m_w_RmmvOperateBoard = new W_RmmvOperateBoard(this);
		this->m_w_RmmvOperateBoard->setData(S_RmmvDataContainer::getInstance()->getRmmvProjectData());
	}
	this->m_w_RmmvOperateBoard->show();

	//��RmmvProjectData�ڲ���"��rmmv"��ȡ�����ݺ���������������
}
/*-------------------------------------------------
		�ؼ� - rmmv���������Ѷ�ȡ
*/
void DrillGIFActionSequenceEditor::rmmvInteractiveDataLoaded(){
	if (this->m_w_RmmvOperateBoard != nullptr){		//��ˢ�´������ݣ�
		this->m_w_RmmvOperateBoard->setData(S_RmmvDataContainer::getInstance()->getRmmvProjectData());
	}
}


/*-------------------------------------------------
		�ؼ� - ���������Ѷ�ȡ
*/
void DrillGIFActionSequenceEditor::actionSeqDataLoaded(){

	QJsonObject data_obj = S_ActionSeqDataContainer::getInstance()->getActionSeqData();
	C_ActionSeqLength data_length = S_ActionSeqDataContainer::getInstance()->getActionSeqLength();
	if (data_obj.isEmpty()){
		ui.main_widget->setEnabled(false);
		ui.toolButton_save->setEnabled(false);
		ui.toolButton_saveAs->setEnabled(false);
	}else{
		ui.main_widget->setEnabled(true);
		ui.toolButton_save->setEnabled(true);
		ui.toolButton_saveAs->setEnabled(true);

		this->m_p_ActionSeqPart->setData(data_obj, data_length);
	}
}
/*-------------------------------------------------
		�ؼ� - �������������ؽ�
*/
void DrillGIFActionSequenceEditor::rebuildActionSeqData(){
	W_ActionSeqLength d(this);
	d.setDataInModifyMode(S_ActionSeqDataContainer::getInstance()->getActionSeqLength());
	if (d.exec() == QDialog::Accepted){
		C_ActionSeqLength result = d.getData();
		QJsonObject obj = S_ActionSeqDataContainer::getInstance()->buildEmptyActionSeqData(result);

		S_ActionSeqDataContainer::getInstance()->setActionSeqLength(result);
		S_ActionSeqDataContainer::getInstance()->setActionSeqData(obj);

		// > ��ȡ����
		this->actionSeqDataLoaded();
		this->m_p_ActionSeqPart->setPartGray();
	}
}
/*-------------------------------------------------
		�ؼ� - �½���Ŀ
*/
void DrillGIFActionSequenceEditor::newProject(){
	S_ProjectManager::getInstance()->newProject();

	// ���½���Ŀʱ������Ҫ�ؽ���
	this->rebuildActionSeqData();
}
/*-------------------------------------------------
		�ؼ� - ����Ŀ
*/
void DrillGIFActionSequenceEditor::openProject(){
	S_ProjectManager::getInstance()->openProject();
}
/*-------------------------------------------------
		�ؼ� - ������Ŀ
*/
void DrillGIFActionSequenceEditor::saveProject(){
	
	// > �������ǰ����ҳ������ȫ������
	this->m_p_ActionSeqPart->putUiToData();

	// > rmmv�������ݴ洢
	if (this->m_w_RmmvOperateBoard != nullptr){
		S_RmmvDataContainer::getInstance()->modify(this->m_w_RmmvOperateBoard->getData());
	}
	
	// > ����
	S_ProjectManager::getInstance()->saveProject();
}
/*-------------------------------------------------
		�ؼ� - ���Ϊ��Ŀ
*/
void DrillGIFActionSequenceEditor::saveAsProject(){
	
	// > �������ǰ����ҳ������ȫ������
	this->m_p_ActionSeqPart->putUiToData();
	
	// > ���Ϊ
	S_ProjectManager::getInstance()->saveAs();
}
/*-------------------------------------------------
		�ؼ� - �޸��ļ�����
*/
void DrillGIFActionSequenceEditor::changeWindowTitle(QString title){
	this->setWindowTitle(title);
}
/* --------------------------------------------------------------
		�ؼ� - �û��ֲ�
*/
void DrillGIFActionSequenceEditor::openUserManual() {
	QString sPath = qApp->applicationDirPath();

	QString docx = sPath + "/help/����GIF�������к��ı༭��.docx";
	if (QFileInfo(docx).exists()){
		QDesktopServices::openUrl(QUrl("file:/" + docx)); 
	}else{
		QMessageBox::warning(this, "����", "�ĵ�\"����GIF�������к��ı༭��.docx\"�����ˡ�", QMessageBox::Yes);
	}

}
/* --------------------------------------------------------------
		�ؼ� - ����...
*/
void DrillGIFActionSequenceEditor::openAbout() {
	W_SoftwareAbout d(this);
	d.exec();
}


/* --------------------------------------------------------------
		�ؼ� - �����л���С�¼�
*/
void DrillGIFActionSequenceEditor::resizeEvent(QResizeEvent *event){

	QSize size = event->size();
	if (size.width() + 100 < QApplication::desktop()->width() &&
		size.height() + 80 < QApplication::desktop()->height()){
		this->m_last_uiSize = size;		//�����ʱ����ֵ��Ҳ��������ֵ��
	}

	event->accept();
}
/* --------------------------------------------------------------
		�ؼ� - ���ڹر��¼�������رհ�ť��
*/
void DrillGIFActionSequenceEditor::closeEvent(QCloseEvent *event){

	// > ֹͣ����
	this->m_p_ActionSeqPart->stopPlaying();

	// > ����
	if (S_ProjectManager::getInstance()->dirtyTip() == false) {
		event->ignore();
		return;
	}

	// > temp�ļ�����
	S_TempFileManager::getInstance()->destroyInstance();

	// > UI����
	this->ui_saveConfig();

	event->accept();
}
/* --------------------------------------------------------------
		���� - �����ļ��ж�
*/
void DrillGIFActionSequenceEditor::dragEnterEvent(QDragEnterEvent *event) {
	
	qDebug() << event;		//���϶����ļ�ֻҪ�������ڣ��ͻᴥ����
	QString file_name = event->mimeData()->urls()[0].fileName();
	if (QFileInfo(file_name).suffix() == "drillas" ) {
		event->acceptProposedAction();	//�������ļ���
	}else {
		event->ignore();	//���������ļ�����ʾ�������룩
	}
}
/* --------------------------------------------------------------
		���� - �����ļ�����
*/
void DrillGIFActionSequenceEditor::dropEvent(QDropEvent *event) {

	//��Ҫ�ǵ���ui�������acceptDrops��
	S_ProjectManager::getInstance()->openProjectByDropFile(event);

}

/* --------------------------------------------------------------
		���� - �û��Զ���UI��ȡ
*/
void DrillGIFActionSequenceEditor::ui_loadConfig(){

	// > ���ڸ߿�
	QString ww = S_IniManager::getInstance()->getConfig("COAS_MainWindow_Width");
	QString hh = S_IniManager::getInstance()->getConfig("COAS_MainWindow_Height");
	if (ww != "" && hh != "" && ww.toInt() >0 && hh.toInt() >0){
		this->resize(ww.toInt(), hh.toInt());
	}

	// > ����λ�ã��ݲ��ã�
	//QString xx = S_IniManager::getInstance()->getConfig("COAS_MainWindow_X");
	//QString yy = S_IniManager::getInstance()->getConfig("COAS_MainWindow_Y");
	//if (xx != "" && yy != ""){
	//	this->move(xx.toInt(), yy.toInt());
	//}

	// > ���
	QString config = S_IniManager::getInstance()->getConfig("COAS_MainWindow_Maximized");
	if (config == "true"){
		this->showMaximized();
	}

	// > �ӿؼ�
	this->m_p_ActionSeqPart->ui_loadConfig();
	
}
/* --------------------------------------------------------------
		���� - �û��Զ���UI�洢
*/
void DrillGIFActionSequenceEditor::ui_saveConfig(){

	// > ���ڸ߿�
	S_IniManager::getInstance()->setConfig("COAS_MainWindow_Width",QString::number(this->m_last_uiSize.width()));
	S_IniManager::getInstance()->setConfig("COAS_MainWindow_Height", QString::number(this->m_last_uiSize.height()));

	// > ����λ�ã��ݲ��ã�
	//S_IniManager::getInstance()->setConfig("COAS_MainWindow_X", QString::number(this->x()));
	//S_IniManager::getInstance()->setConfig("COAS_MainWindow_Y", QString::number(this->y()));

	// > ���
	if (this->isMaximized() == true){
		S_IniManager::getInstance()->setConfig("COAS_MainWindow_Maximized", "true");
	}else{
		S_IniManager::getInstance()->setConfig("COAS_MainWindow_Maximized", "false");
	}

	// > �ӿؼ�
	this->m_p_ActionSeqPart->ui_saveConfig();

}