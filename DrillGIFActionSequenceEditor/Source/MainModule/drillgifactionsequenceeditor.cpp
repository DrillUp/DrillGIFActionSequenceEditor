#include "stdafx.h"
#include "drillgifactionsequenceeditor.h"

#include "about/w_SoftwareAbout.h"
#include "birthDayTip/w_SoftwareBirthDayTip.h"
#include "Source/RmmvInteractiveModule/OperateBoard/W_RmmvOperateBoard.h"
#include "Source/RmmvInteractiveModule/OperateBoard/S_RmmvOperateBoardConfig.h"
#include "Source/RmmvUtilsProjectModule/ProjectData/S_RmmvProjectDataContainer.h"
#include "Source/ActionSeqModule/Data/S_ActionSeqDataContainer.h"
#include "Source/ActionSeqModule/Data/LengthData/W_COAS_Length.h"
#include "Source/ActionSeqModule/DataPart/P_COAS_DataPart.h"
#include "Source/ProjectModule/S_ProjectManager.h"
#include "Source/ProjectModule/File/S_TempFileManager.h"
#include "Source/ProjectModule/StorageGlobal/S_IniManager.h"

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
	this->m_P_COAS_DataPart = new P_COAS_DataPart(ui.widget_actionSeq);
	l->setMargin(0);
	l->addWidget(this->m_P_COAS_DataPart);

	// > UI读取
	this->ui_loadConfig();
	
	//-----------------------------------
	//----初始化参数
	this->m_w_RmmvOperateBoard = nullptr;
	S_ProjectManager::getInstance();
	S_RmmvProjectDataContainer::getInstance();
	S_ActionSeqDataContainer::getInstance();
	S_RmmvOperateBoardConfig::getInstance();

	//-----------------------------------
	//----事件绑定
	connect(ui.toolButton_new, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::newProject);
	connect(ui.toolButton_open, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openProject);
	connect(ui.toolButton_save, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::saveProject);
	connect(ui.toolButton_saveAs, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::saveAsProject);
	connect(ui.toolButton_rmmv, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openWindowRmmvInteractive);
	connect(ui.toolButton_userManual, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openUserManual_menu);
	connect(ui.toolButton_about, &QToolButton::clicked, this, &DrillGIFActionSequenceEditor::openAbout);
	connect(S_ProjectManager::getInstance(), &S_ProjectManager::signal_titleChanged, this, &DrillGIFActionSequenceEditor::changeWindowTitle);
	// （注意rmmv交互的数据要最先连接，这样在存档读取时不会乱序）
	connect(S_RmmvProjectDataContainer::getInstance(), &S_RmmvProjectDataContainer::signal_dataAllReloaded, this, &DrillGIFActionSequenceEditor::rmmvInteractiveDataLoaded);
	connect(S_ActionSeqDataContainer::getInstance(), &S_ActionSeqDataContainer::dataAllReloaded, this, &DrillGIFActionSequenceEditor::actionSeqDataLoaded);


	// > 生日彩蛋
	W_SoftwareBirthDayTip d(this);
	if (d.isInBirthDay()){
		d.exec();
	}
}

/*-------------------------------------------------
		控件 - 打开rmmv交互窗口
*/
void DrillGIFActionSequenceEditor::openWindowRmmvInteractive(){
	if (this->m_w_RmmvOperateBoard == nullptr){
		this->m_w_RmmvOperateBoard = new W_RmmvOperateBoard(this);
		this->m_w_RmmvOperateBoard->setData(S_RmmvProjectDataContainer::getInstance()->getData());
	}
	this->m_w_RmmvOperateBoard->show();

	//（RmmvProjectData在操作"打开rmmv"获取到数据后，立即存入容器）
}
/*-------------------------------------------------
		控件 - rmmv交互数据已读取
*/
void DrillGIFActionSequenceEditor::rmmvInteractiveDataLoaded(){
	if (this->m_w_RmmvOperateBoard != nullptr){		//（刷新窗口数据）
		this->m_w_RmmvOperateBoard->setData(S_RmmvProjectDataContainer::getInstance()->getData());
	}
}


/*-------------------------------------------------
		控件 - 动画序列已读取
*/
void DrillGIFActionSequenceEditor::actionSeqDataLoaded(){

	QList<C_COAS_DataPtr> data_list = S_ActionSeqDataContainer::getInstance()->getActionSeqData();
	C_COAS_Length data_length = S_ActionSeqDataContainer::getInstance()->getActionSeqLength();
	if (data_list.isEmpty()){
		ui.main_widget->setEnabled(false);
		ui.toolButton_save->setEnabled(false);
		ui.toolButton_saveAs->setEnabled(false);
	}else{
		ui.main_widget->setEnabled(true);
		ui.toolButton_save->setEnabled(true);
		ui.toolButton_saveAs->setEnabled(true);

		this->m_P_COAS_DataPart->setData(data_list, data_length);
	}
}
/*-------------------------------------------------
		控件 - 动画序列数据重建
*/
void DrillGIFActionSequenceEditor::rebuildActionSeqData(){
	ui.main_widget->setEnabled(false);	 //（重要配置锁定控件）

	W_COAS_Length d(this);
	d.setDataInModifyMode(S_ActionSeqDataContainer::getInstance()->getActionSeqLength());
	if (d.exec() == QDialog::Accepted){
		C_COAS_Length result = d.getData();
		S_ActionSeqDataContainer::getInstance()->setActionSeqLength(result);

		// > 读取数据
		this->actionSeqDataLoaded();
		this->m_P_COAS_DataPart->setPartGray();
	}

	ui.main_widget->setEnabled(true);	 //（重要配置解锁控件）
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
	ui.main_widget->setEnabled(false);	 //（重要配置锁定控件）

	S_ProjectManager::getInstance()->openProject();

	ui.main_widget->setEnabled(true);	 //（重要配置解锁控件）
}
/*-------------------------------------------------
		控件 - 保存项目
*/
void DrillGIFActionSequenceEditor::saveProject(){
	
	// > 点击保存前，将页面数据全部导出
	this->m_P_COAS_DataPart->putUiToData();

	// > rmmv配置数据存储
	if (this->m_w_RmmvOperateBoard != nullptr){
		S_RmmvProjectDataContainer::getInstance()->modifyData(this->m_w_RmmvOperateBoard->getData());
	}
	
	// > 保存
	S_ProjectManager::getInstance()->saveProject();
}
/*-------------------------------------------------
		控件 - 另存为项目
*/
void DrillGIFActionSequenceEditor::saveAsProject(){
	
	// > 点击保存前，将页面数据全部导出
	this->m_P_COAS_DataPart->putUiToData();
	
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
		控件 - 关于...
*/
void DrillGIFActionSequenceEditor::openAbout() {
	W_SoftwareAbout d(this);
	d.exec();
}
/* --------------------------------------------------------------
		控件 - 帮助文档
*/
void DrillGIFActionSequenceEditor::openUserManual_menu() {
	QToolButton* cur_btn = qobject_cast<QToolButton*>(this->sender());		//从action里面取出数据指针
	QMenu *menu = new QMenu();
	QAction *action;

	action = new QAction("关于GIF动画序列核心编辑器（入门篇）.docx", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/nav/Help.png"));
	connect(action, &QAction::triggered, this, &DrillGIFActionSequenceEditor::openUserManual_1);
	menu->addAction(action);

	action = new QAction("关于GIF动画序列核心编辑器（高级篇）.docx", this);
	action->setIcon(QIcon(QRC_IconSrcPath + "/nav/Help.png"));
	connect(action, &QAction::triggered, this, &DrillGIFActionSequenceEditor::openUserManual_2);
	menu->addAction(action);

	QPoint p = cur_btn->mapToGlobal(QPoint(0, 0));
	p.setY(p.y() + cur_btn->height());
	menu->exec(p);
}
void DrillGIFActionSequenceEditor::openUserManual_1() {
	QString sPath = qApp->applicationDirPath();
	QString docx_name = "关于GIF动画序列核心编辑器（入门篇）.docx";

	QString docx = sPath + "/help/" + docx_name;
	if (QFileInfo(docx).exists()){
		QDesktopServices::openUrl(QUrl("file:/" + docx)); 
	}else{
		QMessageBox::warning(this, "错误", "文档\"" + docx_name + "\"不见了。", QMessageBox::Yes);
	}
}
void DrillGIFActionSequenceEditor::openUserManual_2() {
	QString sPath = qApp->applicationDirPath();
	QString docx_name = "关于GIF动画序列核心编辑器（高级篇）.docx";

	QString docx = sPath + "/help/" + docx_name;
	if (QFileInfo(docx).exists()){
		QDesktopServices::openUrl(QUrl("file:/" + docx));
	}else{
		QMessageBox::warning(this, "错误", "文档\"" + docx_name + "\"不见了。", QMessageBox::Yes);
	}
}


/* --------------------------------------------------------------
		事件 - 窗口切换大小事件
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
		事件 - 窗口关闭事件（点击关闭按钮）
*/
void DrillGIFActionSequenceEditor::closeEvent(QCloseEvent *event){

	// > 停止播放
	this->m_P_COAS_DataPart->stopPlaying();

	// > 工程
	if (S_ProjectManager::getInstance()->dirtyTip() == false) {
		event->ignore();
		return;
	}

	// > temp文件销毁
	S_TempFileManager::getInstance()->destroyInstance();

	// > UI保存
	this->ui_saveConfig();

	event->accept();
}
/* --------------------------------------------------------------
		事件 - 拖入文件判断
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
		事件 - 放置文件动作
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
	QString ww = S_IniManager::getInstance()->getConfig("COAS_MainWindow_Width");
	QString hh = S_IniManager::getInstance()->getConfig("COAS_MainWindow_Height");
	if (ww != "" && hh != "" && ww.toInt() >0 && hh.toInt() >0){
		this->resize(ww.toInt(), hh.toInt());
	}

	// > 窗口位置（暂不用）
	//QString xx = S_IniManager::getInstance()->getConfig("COAS_MainWindow_X");
	//QString yy = S_IniManager::getInstance()->getConfig("COAS_MainWindow_Y");
	//if (xx != "" && yy != ""){
	//	this->move(xx.toInt(), yy.toInt());
	//}

	// > 最大化
	QString config = S_IniManager::getInstance()->getConfig("COAS_MainWindow_Maximized");
	if (config == "true"){
		this->showMaximized();
	}

	// > 子控件
	this->m_P_COAS_DataPart->ui_loadConfig();
	
}
/* --------------------------------------------------------------
		窗口 - 用户自定义UI存储
*/
void DrillGIFActionSequenceEditor::ui_saveConfig(){

	// > 窗口高宽
	S_IniManager::getInstance()->setConfig("COAS_MainWindow_Width",QString::number(this->m_last_uiSize.width()));
	S_IniManager::getInstance()->setConfig("COAS_MainWindow_Height", QString::number(this->m_last_uiSize.height()));

	// > 窗口位置（暂不用）
	//S_IniManager::getInstance()->setConfig("COAS_MainWindow_X", QString::number(this->x()));
	//S_IniManager::getInstance()->setConfig("COAS_MainWindow_Y", QString::number(this->y()));

	// > 最大化
	if (this->isMaximized() == true){
		S_IniManager::getInstance()->setConfig("COAS_MainWindow_Maximized", "true");
	}else{
		S_IniManager::getInstance()->setConfig("COAS_MainWindow_Maximized", "false");
	}

	// > 子控件
	this->m_P_COAS_DataPart->ui_saveConfig();

}