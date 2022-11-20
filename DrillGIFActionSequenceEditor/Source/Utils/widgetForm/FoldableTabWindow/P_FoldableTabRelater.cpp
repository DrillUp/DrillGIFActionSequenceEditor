#include "stdafx.h"
#include "P_FoldableTabRelater.h"

#include "Source/Utils/Common/TTool.h"

/*
-----==========================================================-----
		类：		可折叠选项卡 管理器.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		折叠选项卡窗口的管理器。

		使用方法：
				>准备子块
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

	this->m_partList = QList<C_FoldableTabPrivate*>();
	this->m_childWindowList = QList<W_FoldableTabChildWindow*>();

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

}

P_FoldableTabRelater::~P_FoldableTabRelater(){
}


/*-------------------------------------------------
		父控件 - 关闭标签
*/
void P_FoldableTabRelater::tabClosed(int index){
	if (index >= this->m_tab->tabBar()->count()){ return; }		//（索引超出时，不操作）
	QString text = this->m_tab->tabText(index);
	this->showChildWindow(text);
}
/*-------------------------------------------------
		父控件 - 切换到指定标签
*/
void P_FoldableTabRelater::selectTab(QString tab_name){
	QTabBar* tabBar = this->m_tab->tabBar();
	for (int i = 0; i < tabBar->count(); i++){
		if (tabBar->tabText(i) == tab_name){
			this->m_tab->setCurrentIndex(i);
			return;
		}
	}
}
/*-------------------------------------------------
		父控件 - 标签全归位
*/
void P_FoldableTabRelater::homingAllTab(){
	this->hideAllChildWindow();		//（隐藏子窗口 == 归位）
}

/*-------------------------------------------------
		子块 - 添加子块
*/
void P_FoldableTabRelater::addPart(QString name, QWidget* partWidget){
	this->addPart(name, partWidget, QJsonObject());
}
void P_FoldableTabRelater::addPart(QString name, QWidget* partWidget, QJsonObject param){
	
	// > 子块添加
	C_FoldableTabPrivate* c_f = new C_FoldableTabPrivate();
	c_f->name = name;
	c_f->partWidget = partWidget;
	c_f->param = param;
	this->m_partList.append(c_f);
	
	// > 子窗口添加
	W_FoldableTabChildWindow* child_window = new W_FoldableTabChildWindow(this, c_f, this->m_tab);
	this->m_childWindowList.append(child_window);

	// > 刷新
	this->refreshAllPart();
}
/*-------------------------------------------------
		子块 - 获取子块（根据名称）
*/
C_FoldableTabPrivate* P_FoldableTabRelater::getPartByName(QString name){
	for (int i = 0; i < this->m_partList.count();i++){
		C_FoldableTabPrivate* c_f = this->m_partList.at(i);
		if (c_f->name == name){
			return c_f;
		}
	}
	return nullptr;
}
/*-------------------------------------------------
		子块 - 获取全部子块
*/
QList<C_FoldableTabPrivate*> P_FoldableTabRelater::getAllPart(){
	return this->m_partList;
}
/*-------------------------------------------------
		子块 - 刷新子块窗口分布情况
*/
void P_FoldableTabRelater::refreshAllPart(){

	// > 刷新父控件
	this->m_tab->clear();
	for (int i = 0; i < this->m_partList.count(); i++){
		C_FoldableTabPrivate* part = this->m_partList.at(i);
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
}


/*-------------------------------------------------
		子窗口 - 显示
*/
void P_FoldableTabRelater::showChildWindow(QString name){
	W_FoldableTabChildWindow* window = this->getChildWindow(name);
	if (window == nullptr){ return; }
	window->show();
}
/*-------------------------------------------------
		子窗口 - 隐藏
*/
void P_FoldableTabRelater::hideChildWindow(QString name){
	W_FoldableTabChildWindow* window = this->getChildWindow(name);
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
W_FoldableTabChildWindow* P_FoldableTabRelater::getChildWindow(QString name){
	for (int i = 0; i < this->m_partList.count(); i++){
		C_FoldableTabPrivate* c_f = this->m_partList.at(i);
		if (c_f->name == name){
			return this->m_childWindowList.at(i);
		}
	}
	return nullptr;
}
