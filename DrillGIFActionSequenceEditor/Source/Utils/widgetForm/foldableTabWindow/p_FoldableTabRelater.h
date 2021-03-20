#ifndef P_FoldableTabManager_H
#define P_FoldableTabManager_H

#include "private/c_FoldableTabPrivate.h"
#include "private/w_FoldableTabChildWindow.h"

/*
-----==========================================================-----
		类：		可折叠选项卡 管理器.h
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		折叠选项卡窗口的管理器。
					（详细见cpp）
-----==========================================================-----
*/

class P_FoldableTabRelater : public QObject
{
	Q_OBJECT

	public:
		P_FoldableTabRelater(QTabWidget *parent);
		~P_FoldableTabRelater();
		
	//-----------------------------------
	//----父控件
	protected:
		QTabWidget* m_tab;				//父控件
		QString m_tabStyle;				//样式
	public slots:
								//父控件 - 关闭标签
		void tabClosed(int index);
								//父控件 - 标签全归位
		void homingAllTab();

	//-----------------------------------
	//----子块
	protected:
		QList<C_FoldableTabPrivate*> m_partList;
	public slots:
								//子块 - 添加子块
		void addPart(QString name, QWidget* partWidget);	
		void addPart(QString name, QWidget* partWidget, QJsonObject param);	
		
								//子块 - 获取子块（根据名称）
		C_FoldableTabPrivate* getPartByName(QString name);
								//子块 - 获取全部子块
		QList<C_FoldableTabPrivate*> getAllPart();

								//子块 - 刷新子块窗口分布情况
		void refreshAllPart();

	//-----------------------------------
	//----子窗口
	protected:
		QList<W_FoldableTabChildWindow*> m_childWindowList;
	public slots:
								//子窗口 - 显示
		void showChildWindow(QString name);	
								//子窗口 - 隐藏
		void hideChildWindow(QString name);
								//子窗口 - 全部隐藏（全部回到标签页中）
		void hideAllChildWindow();
								//子窗口 - 获取
		W_FoldableTabChildWindow* getChildWindow(QString name);
		
};

#endif // P_FoldableTabManager_H
