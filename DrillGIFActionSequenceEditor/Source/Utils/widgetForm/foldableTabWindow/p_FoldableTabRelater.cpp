#include "stdafx.h"
#include "p_FoldableTabRelater.h"

#include "Source/Utils/common/TTool.h"

/*
-----==========================================================-----
		�ࣺ		���۵�ѡ� ������.cpp
		�汾��		v1.02
		���ߣ�		drill_up
		����ģ�飺	����ģ��
		���ܣ�		�۵�ѡ����ڵĹ�������

		ʹ�÷�����
				>׼���ӿ�
					this->m_p_FoldableTabRelater = new P_FoldableTabRelater(ui.tabWidget);
					this->m_p_FoldableTabRelater->addPart("��Ʒ", this->m_p_DatabaseItem);
					this->m_p_FoldableTabRelater->addPart("װ��", this->m_p_DatabaseEquip);
					// �ӿ��в�Ҫ��accept����Ϊ���ڻᱻ��ʱ�Ƴ��������ƣ����������ύ�ĶԻ���
					//�����ui.tabWidget�����٣���ô�Ӵ��ڻᱻһ�����٣������������
					
-----==========================================================-----
*/
P_FoldableTabRelater::P_FoldableTabRelater(QTabWidget *parent)
	: QObject(parent)
{

	//-----------------------------------
	//----��ʼ������
	this->m_tab = parent;
	this->m_tabStyle = this->m_tab->styleSheet();
	this->m_iconSrcPath = ":/DrillGIFActionSequenceEditor/Resources/icons";

	this->m_partList = QList<C_FoldableTabPrivate*>();
	this->m_childWindowList = QList<W_FoldableTabChildWindow*>();

	//-----------------------------------
	//----��ʼ��ui
	this->m_tab->clear();
	this->m_tab->setMovable(true);				//��ǩ����ק
	this->m_tab->setTabsClosable(true);			//��ǩ�ɹر�
	this->m_tab->setStyleSheet(this->m_tabStyle + 
		"\n QTabBar::close-button { image: url(" + this->m_iconSrcPath + "/tabWidget/outFrame.png) }"+
		"\n QTabBar::close-button:hover{ image: url(" + this->m_iconSrcPath + "/tabWidget/outFrame_hover.png) }" +
		"\n QTabBar::close-button:disabled{ image: url(" + this->m_iconSrcPath + "/tabWidget/outFrame_disabled.png) }");

	//-----------------------------------
	//----�¼���
	connect(this->m_tab, &QTabWidget::tabCloseRequested, this, &P_FoldableTabRelater::tabClosed);

}

P_FoldableTabRelater::~P_FoldableTabRelater(){
}


/*-------------------------------------------------
		���ؼ� - �رձ�ǩ
*/
void P_FoldableTabRelater::tabClosed(int index){
	QString text = this->m_tab->tabText(index);
	this->showChildWindow(text);
}
/*-------------------------------------------------
		���ؼ� - ��ǩȫ��λ
*/
void P_FoldableTabRelater::homingAllTab(){
	this->hideAllChildWindow();		//�������Ӵ��� == ��λ��
}

/*-------------------------------------------------
		�ӿ� - ����ӿ�
*/
void P_FoldableTabRelater::addPart(QString name, QWidget* partWidget){
	this->addPart(name, partWidget, QJsonObject());
}
void P_FoldableTabRelater::addPart(QString name, QWidget* partWidget, QJsonObject param){
	
	// > �ӿ����
	C_FoldableTabPrivate* c_f = new C_FoldableTabPrivate();
	c_f->name = name;
	c_f->partWidget = partWidget;
	c_f->param = param;
	this->m_partList.append(c_f);
	
	// > �Ӵ������
	W_FoldableTabChildWindow* child_window = new W_FoldableTabChildWindow(this, c_f, this->m_tab);
	this->m_childWindowList.append(child_window);

	// > ˢ��
	this->refreshAllPart();
}
/*-------------------------------------------------
		�ӿ� - ��ȡ�ӿ飨�������ƣ�
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
		�ӿ� - ��ȡȫ���ӿ�
*/
QList<C_FoldableTabPrivate*> P_FoldableTabRelater::getAllPart(){
	return this->m_partList;
}
/*-------------------------------------------------
		�ӿ� - ˢ���ӿ鴰�ڷֲ����
*/
void P_FoldableTabRelater::refreshAllPart(){

	// > ˢ�¸��ؼ�
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

	// > ˢ���Ӵ���
	for (int i = 0; i < this->m_childWindowList.count(); i++){
		W_FoldableTabChildWindow* window = this->m_childWindowList.at(i);
		if (window == nullptr){ continue; }
		window->refreshPart();
	}
}


/*-------------------------------------------------
		�Ӵ��� - ��ʾ
*/
void P_FoldableTabRelater::showChildWindow(QString name){
	W_FoldableTabChildWindow* window = this->getChildWindow(name);
	if (window == nullptr){ return; }
	window->show();
}
/*-------------------------------------------------
		�Ӵ��� - ����
*/
void P_FoldableTabRelater::hideChildWindow(QString name){
	W_FoldableTabChildWindow* window = this->getChildWindow(name);
	if (window == nullptr){ return;}
	window->hide();
}
/*-------------------------------------------------
		�Ӵ��� - ȫ������
*/
void P_FoldableTabRelater::hideAllChildWindow(){
	for (int i = 0; i < this->m_childWindowList.count(); i++){
		W_FoldableTabChildWindow* window = this->m_childWindowList.at(i);
		window->hide();
	}
}
/*-------------------------------------------------
		�Ӵ��� - ��ȡ
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
