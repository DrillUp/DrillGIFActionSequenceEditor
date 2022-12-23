#include "stdafx.h"
#include "P_FoldableTabRelater.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		可折叠选项卡 管理器.cpp
		版本：		v1.04
		作者：		drill_up
		所属模块：	工具模块
		功能：		折叠选项卡窗口的管理器。

		使用方法：
				> 准备子块
					this->m_p_FoldableTabRelater = new P_FoldableTabRelater(ui.tabWidget);
					this->m_p_FoldableTabRelater->addPart("物品", this->m_p_DatabaseItem);
					this->m_p_FoldableTabRelater->addPart("装备", this->m_p_DatabaseEquip);
					// 子块中不要有accept，因为窗口会被随时移出或者拖移，不适用于提交的对话框
					//【如果ui.tabWidget被销毁，那么子窗口会被一起销毁，不会残留。】
					
-----==========================================================-----
*/
P_FoldableTabRelater::P_FoldableTabRelater(QTabWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----初始化参数
	this->m_tab = parent;
	this->m_tabStyle = this->m_tab->styleSheet();
	this->m_slotBlock = false;

	this->m_partList.clear();
	this->m_childWindowList.clear();

	//-----------------------------------
	//----初始化ui
	this->m_tab->clear();
	this->m_tab->setMovable(true);				//标签可拖拽
	this->m_tab->setTabsClosable(true);			//标签可关闭
	this->m_tab->setStyleSheet(this->m_tabStyle + 
		"\n QTabBar::close-button { image: url(" + QRC_IconSrcPath + "/tabWidget/outFrame.png) }"+
		"\n QTabBar::close-button:hover{ image: url(" + QRC_IconSrcPath + "/tabWidget/outFrame_hover.png) }" +
		"\n QTabBar::close-button:disabled{ image: url(" + QRC_IconSrcPath + "/tabWidget/outFrame_disabled.png) }");

	//-----------------------------------
	//----事件绑定
	connect(this->m_tab, &QTabWidget::tabCloseRequested, this, &P_FoldableTabRelater::tabClosed);
	connect(this->m_tab, &QTabWidget::currentChanged, this, &P_FoldableTabRelater::tabChanged);

}

P_FoldableTabRelater::~P_FoldableTabRelater(){
}


/*-------------------------------------------------
		子块 - 添加子块
*/
void P_FoldableTabRelater::addPart(QString part_name, QWidget* partWidget){
	this->addPart(part_name, partWidget, QJsonObject());
}
void P_FoldableTabRelater::addPart(QString part_name, QWidget* partWidget, QJsonObject option){
	
	// > 子块添加
	P_FoldableTabPrivate* c_f = new P_FoldableTabPrivate();
	c_f->name = part_name;
	c_f->partWidget = partWidget;
	c_f->option = option;
	this->m_partList.append(c_f);
	
	// > 子窗口添加
	W_FoldableTabChildWindow* child_window = new W_FoldableTabChildWindow(this, c_f, this->m_tab);
	connect(child_window, &W_FoldableTabChildWindow::signal_windowActived, this, &P_FoldableTabRelater::childWindowActived);
	this->m_childWindowList.append(child_window);

	// > 刷新
	this->refreshAllPart();
}
/*-------------------------------------------------
		子块 - 获取 - 根据名称
*/
P_FoldableTabPrivate* P_FoldableTabRelater::getPart_ByName(QString part_name){
	for (int i = 0; i < this->m_partList.count();i++){
		P_FoldableTabPrivate* c_f = this->m_partList.at(i);
		if (c_f->name == part_name){
			return c_f;
		}
	}
	return nullptr;
}
/*-------------------------------------------------
		子块 - 获取 - 全部
*/
QList<P_FoldableTabPrivate*> P_FoldableTabRelater::getPart_All(){
	return this->m_partList;
}
/*-------------------------------------------------
		子块 - 是否在选项卡中
*/
bool P_FoldableTabRelater::isPart_InTab(QString part_name){
	P_FoldableTabPrivate* part = this->getPart_ByName(part_name);
	if (part == nullptr){ return false; }
	return part->isInTab();
}
/*-------------------------------------------------
		子块 - 是否在窗口中
*/
bool P_FoldableTabRelater::isPart_InWindow(QString part_name){
	P_FoldableTabPrivate* part = this->getPart_ByName(part_name);
	if (part == nullptr){ return false; }
	return part->isInWindow();
}
/*-------------------------------------------------
		子块 - 刷新子块窗口分布情况
*/
void P_FoldableTabRelater::refreshAllPart(){
	this->m_slotBlock = true;

	// > 刷新父控件
	this->m_tab->clear();
	for (int i = 0; i < this->m_partList.count(); i++){
		P_FoldableTabPrivate* part = this->m_partList.at(i);
		if (part->isInChildWindow == false){
			QWidget* w = new QWidget();
			QHBoxLayout* l = new QHBoxLayout(w);
			w->setLayout(l);
			l->setMargin(0);
			l->addWidget(part->partWidget);
			this->m_tab->addTab(w, part->name);
		}
	}

	// > 刷新子窗口
	for (int i = 0; i < this->m_childWindowList.count(); i++){
		W_FoldableTabChildWindow* window = this->m_childWindowList.at(i);
		if (window == nullptr){ continue; }
		window->refreshPart();
	}

	this->m_slotBlock = false;
}


/*-------------------------------------------------
		选项卡 - 关闭选项卡
*/
void P_FoldableTabRelater::closeTab_ByIndex(int relative_index){
	this->tabClosed(relative_index);
}
void P_FoldableTabRelater::closeTab_ByName(QString part_name){
	this->showChildWindow(part_name);
}
/*-------------------------------------------------
		选项卡 - 选择指定选项卡
*/
void P_FoldableTabRelater::selectTab_ByIndex(int relative_index){
	this->m_tab->setCurrentIndex(relative_index);
}
void P_FoldableTabRelater::selectTab_ByName(QString part_name){
	QTabBar* tabBar = this->m_tab->tabBar();
	for (int i = 0; i < tabBar->count(); i++){
		if (tabBar->tabText(i) == part_name){
			this->m_tab->setCurrentIndex(i);
			return;
		}
	}
}
/*-------------------------------------------------
		选项卡 - 子块全归位
*/
void P_FoldableTabRelater::homingAllTab(){
	this->hideAllChildWindow();		//（隐藏子窗口 == 归位）
}
/*-------------------------------------------------
		选项卡 - 关闭选项卡（私有）
*/
void P_FoldableTabRelater::tabClosed(int relative_index){
	if (relative_index >= this->m_tab->tabBar()->count()){ return; }		//（索引超出时，不操作）
	QString part_name = this->m_tab->tabText(relative_index);
	this->showChildWindow(part_name);
}
/*-------------------------------------------------
		选项卡 - 选项卡变化时（私有）
*/
void P_FoldableTabRelater::tabChanged(int relative_index){
	if (relative_index >= this->m_tab->tabBar()->count()){ return; }		//（索引超出时，不操作）
	QString part_name = this->m_tab->tabText(relative_index);
	if (part_name == ""){ return; }
	if (this->m_slotBlock == true){ return; }
	emit signal_whenPartFocusIn(part_name);
}


/*-------------------------------------------------
		子窗口 - 显示
*/
void P_FoldableTabRelater::showChildWindow(QString part_name){
	W_FoldableTabChildWindow* window = this->getChildWindow(part_name);
	if (window == nullptr){ return; }
	window->show();
}
/*-------------------------------------------------
		子窗口 - 隐藏
*/
void P_FoldableTabRelater::hideChildWindow(QString part_name){
	W_FoldableTabChildWindow* window = this->getChildWindow(part_name);
	if (window == nullptr){ return;}
	window->hide();
}
/*-------------------------------------------------
		子窗口 - 全部隐藏
*/
void P_FoldableTabRelater::hideAllChildWindow(){
	for (int i = 0; i < this->m_childWindowList.count(); i++){
		W_FoldableTabChildWindow* window = this->m_childWindowList.at(i);
		window->hide();
	}
}
/*-------------------------------------------------
		子窗口 - 获取
*/
W_FoldableTabChildWindow* P_FoldableTabRelater::getChildWindow(QString part_name){
	for (int i = 0; i < this->m_partList.count(); i++){
		P_FoldableTabPrivate* c_f = this->m_partList.at(i);
		if (c_f->name == part_name){
			return this->m_childWindowList.at(i);
		}
	}
	return nullptr;
}
/*-------------------------------------------------
		子窗口 - 被聚焦时
*/
void P_FoldableTabRelater::childWindowActived(QString part_name){
	if (part_name == ""){ return; }
	emit signal_whenPartFocusIn( part_name );
}
