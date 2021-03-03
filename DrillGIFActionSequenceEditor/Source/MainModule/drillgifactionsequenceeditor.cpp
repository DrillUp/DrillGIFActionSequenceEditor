#include "stdafx.h"
#include "drillgifactionsequenceeditor.h"

#include "about/w_SoftwareAbout.h"
#include "Source/RmmvInteractiveModule/operateBoard/w_RmmvOperateBoard.h"
#include "Source/RmmvInteractiveModule/custom/s_RmmvDataContainer.h"
#include "Source/ActionSeqModule/actionSeqPart/p_ActionSeqPart.h"
#include "Source/ActionSeqModule/actionSeqData/s_ActionSeqDataContainer.h"
#include "Source/ActionSeqModule/actionSeqData/lengthData/w_ActionSeqLength.h"
#include "Source/ProjectModule/s_ProjectManager.h"
#include "Source/ProjectModule/file/s_TempFileManager.h"
#include "Source/ProjectModule/storageGlobal/s_IniManager.h"

/*
-----==========================================================-----
		类：		主窗体.cpp
		作者：		drill_up
		所属模块：	主窗体模块
		功能：		程序进入后的主控制窗口。
					
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
		单例
*/
DrillGIFActionSequenceEditor* DrillGIFActionSequenceEditor::cur_instance = NULL;
DrillGIFActionSequenceEditor* DrillGIFActionSequenceEditor::getInstance() {
	if (cur_instance == NULL) {
		cur_instance = new DrillGIFActionSequenceEditor(NULL);
	}
	return cur_instance;
}
/*-------------------------------------------------
		初始化
*/
void DrillGIFActionSequenceEditor::_init() {

	//-----------------------------------
	//----最大化/最小化
	Qt::WindowFlags flags = Qt::Dialog;
	flags |= Qt::WindowCloseButtonHint;
	flags |= Qt::WindowMaximizeButtonHint;
	flags |= Qt::WindowMinimizeButtonHint;
	this->setWindowFlags(flags);
	this->m_last_uiSize = this->size();

	//-----------------------------------
	//----初始化ui
	QHBoxLayout* l = new QHBoxLayout();
	ui.widget_actionSeq->setLayout(l);
	this->m_p_ActionSeqPart = new P_ActionSeqPart(ui.widget_actionSeq);
	l->setMargin(0);
	l->addWidget(this->m_p_ActionSeqPart);

	// > UI读取
	this->ui_loadConfig();
	
	//-----------------------------------
	//----初始化参数
	this->m_w_RmmvOperateBoard = nullptr;
	S_ProjectManager::getInstance();
	S_RmmvDataContainer::getInstance();
	S_ActionSeqDataContainer::getInstance();

	//-----------------------------------
	//----事件绑定
	connect(ui.toolButton_new, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::newProject);
	connect(ui.toolButton_open, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openProject);
	connect(ui.toolButton_save, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::saveProject);
	connect(ui.toolButton_saveAs, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::saveAsProject);
	connect(ui.toolButton_rmmv, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openWindowRmmvInteractive);
	connect(ui.toolButton_userManual, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openUserManual);
	connect(ui.toolButton_about, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openAbout);
	connect(S_ProjectManager::getInstance(), &S_ProjectManager::changeWindowTitle, this, &DrillGIFActionSequenceEditor::changeWindowTitle);
	// （注意rmmv交互的数据要最先连接，这样在存档读取时不会乱序）
	connect(S_RmmvDataContainer::getInstance(), &S_RmmvDataContainer::dataAllReloaded, this, &DrillGIFActionSequenceEditor::rmmvInteractiveDataLoaded);
	connect(S_ActionSeqDataContainer::getInstance(), &S_ActionSeqDataContainer::dataAllReloaded, this, &DrillGIFActionSequenceEditor::actionSeqDataLoaded);

}

/*-------------------------------------------------
		控件 - 打开rmmv交互窗口
*/
void DrillGIFActionSequenceEditor::openWindowRmmvInteractive(){
	if (this->m_w_RmmvOperateBoard == nullptr){
		this->m_w_RmmvOperateBoard = new W_RmmvOperateBoard(this);
		this->m_w_RmmvOperateBoard->setData(S_RmmvDataContainer::getInstance()->getRmmvProjectData());
	}
	this->m_w_RmmvOperateBoard->show();

	//（RmmvProjectData在操作"打开rmmv"获取到数据后，立即存入容器）
}
/*-------------------------------------------------
		控件 - rmmv交互数据已读取
*/
void DrillGIFActionSequenceEditor::rmmvInteractiveDataLoaded(){
	if (this->m_w_RmmvOperateBoard != nullptr){		//（刷新窗口数据）
		this->m_w_RmmvOperateBoard->setData(S_RmmvDataContainer::getInstance()->getRmmvProjectData());
	}
}


/*-------------------------------------------------
		控件 - 动作序列已读取
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
		控件 - 动作序列数据重建
*/
void DrillGIFActionSequenceEditor::rebuildActionSeqData(){
	W_ActionSeqLength d(this);
	d.setDataInModifyMode(S_ActionSeqDataContainer::getInstance()->getActionSeqLength());
	if (d.exec() == QDialog::Accepted){
		C_ActionSeqLength result = d.getData();
		QJsonObject obj = S_ActionSeqDataContainer::getInstance()->buildEmptyActionSeqData(result);

		S_ActionSeqDataContainer::getInstance()->setActionSeqLength(result);
		S_ActionSeqDataContainer::getInstance()->setActionSeqData(obj);

		// > 读取数据
		this->actionSeqDataLoaded();
		this->m_p_ActionSeqPart->setPartGray();
	}
}
/*-------------------------------------------------
		控件 - 新建项目
*/
void DrillGIFActionSequenceEditor::newProject(){
	S_ProjectManager::getInstance()->newProject();

	// （新建项目时，数据要重建）
	this->rebuildActionSeqData();
}
/*-------------------------------------------------
		控件 - 打开项目
*/
void DrillGIFActionSequenceEditor::openProject(){
	S_ProjectManager::getInstance()->openProject();
}
/*-------------------------------------------------
		控件 - 保存项目
*/
void DrillGIFActionSequenceEditor::saveProject(){
	
	// > 点击保存前，将页面数据全部导出
	this->m_p_ActionSeqPart->putUiToData();
	
	// > 保存
	S_ProjectManager::getInstance()->saveProject();
}
/*-------------------------------------------------
		控件 - 另存为项目
*/
void DrillGIFActionSequenceEditor::saveAsProject(){
	
	// > 点击保存前，将页面数据全部导出
	this->m_p_ActionSeqPart->putUiToData();
	
	// > 另存为
	S_ProjectManager::getInstance()->saveAs();
}
/*-------------------------------------------------
		控件 - 修改文件名称
*/
void DrillGIFActionSequenceEditor::changeWindowTitle(QString title){
	this->setWindowTitle(title);
}
/* --------------------------------------------------------------
		控件 - 用户手册
*/
void DrillGIFActionSequenceEditor::openUserManual() {
	QString sPath = qApp->applicationDirPath();

	QString docx = sPath + "/help/关于GIF动作序列核心编辑器.docx";
	if (QFileInfo(docx).exists()){
		QDesktopServices::openUrl(QUrl("file:/" + docx)); 
	}else{
		QMessageBox::warning(this, "错误", "文档\"关于GIF动作序列核心编辑器.docx\"不见了。", QMessageBox::Yes);
	}

}
/* --------------------------------------------------------------
		控件 - 关于...
*/
void DrillGIFActionSequenceEditor::openAbout() {
	W_SoftwareAbout d(this);
	d.exec();
}


/* --------------------------------------------------------------
		控件 - 窗口切换大小事件
*/
void DrillGIFActionSequenceEditor::resizeEvent(QResizeEvent *event){

	QSize size = event->size();
	if (size.width() + 100 < QApplication::desktop()->width() &&
		size.height() + 80 < QApplication::desktop()->height()){
		this->m_last_uiSize = size;		//（最大化时不存值，也不存过大的值）
	}

	event->accept();
}
/* --------------------------------------------------------------
		控件 - 窗口关闭事件（点击关闭按钮）
*/
void DrillGIFActionSequenceEditor::closeEvent(QCloseEvent *event){

	if (S_ProjectManager::getInstance()->dirtyTip() == false) {
		event->ignore();
		return;
	}

	// > UI保存
	this->ui_saveConfig();

	// > temp文件销毁
	S_TempFileManager::getInstance()->destroyInstance();

	event->accept();
}
/* --------------------------------------------------------------
		窗口 - 拖入文件判断
*/
void DrillGIFActionSequenceEditor::dragEnterEvent(QDragEnterEvent *event) {
	
	qDebug() << event;		//（拖动的文件只要经过窗口，就会触发）
	QString file_name = event->mimeData()->urls()[0].fileName();
	if (QFileInfo(file_name).suffix() == "drillas" ) {
		event->acceptProposedAction();	//（接受文件）
	}else {
		event->ignore();	//（不接受文件，显示不可拖入）
	}
}
/* --------------------------------------------------------------
		窗口 - 放置文件动作
*/
void DrillGIFActionSequenceEditor::dropEvent(QDropEvent *event) {

	//（要记得在ui里面添加acceptDrops）
	S_ProjectManager::getInstance()->openProjectByDropFile(event);

}

/* --------------------------------------------------------------
		窗口 - 用户自定义UI读取
*/
void DrillGIFActionSequenceEditor::ui_loadConfig(){

	// > 窗口高宽
	QString ww = S_IniManager::getInstance()->getConfig("COAS_MainWindowWidth");
	QString hh = S_IniManager::getInstance()->getConfig("COAS_MainWindowHeight");
	if (ww != "" && hh != "" && ww.toInt() >0 && hh.toInt() >0){
		this->resize(ww.toInt(), hh.toInt());
	}

	// > 窗口位置（暂不用）
	//QString xx = S_IniManager::getInstance()->getConfig("COAS_MainWindowX");
	//QString yy = S_IniManager::getInstance()->getConfig("COAS_MainWindowY");
	//if (xx != "" && yy != ""){
	//	this->move(xx.toInt(), yy.toInt());
	//}

	// > 最大化
	QString config = S_IniManager::getInstance()->getConfig("COAS_MainWindowMaximized");
	if (config == "true"){
		this->showMaximized();
	}

	// > 子控件
	this->m_p_ActionSeqPart->ui_loadConfig();
	
}
/* --------------------------------------------------------------
		窗口 - 用户自定义UI存储
*/
void DrillGIFActionSequenceEditor::ui_saveConfig(){

	// > 窗口高宽
	S_IniManager::getInstance()->setConfig("COAS_MainWindowWidth",QString::number(this->m_last_uiSize.width()));
	S_IniManager::getInstance()->setConfig("COAS_MainWindowHeight", QString::number(this->m_last_uiSize.height()));

	// > 窗口位置（暂不用）
	//S_IniManager::getInstance()->setConfig("COAS_MainWindowX", QString::number(this->x()));
	//S_IniManager::getInstance()->setConfig("COAS_MainWindowY", QString::number(this->y()));

	// > 最大化
	if (this->isMaximized() == true){
		S_IniManager::getInstance()->setConfig("COAS_MainWindowMaximized", "true");
	}else{
		S_IniManager::getInstance()->setConfig("COAS_MainWindowMaximized", "false");
	}

	// > 子控件
	this->m_p_ActionSeqPart->ui_saveConfig();

}