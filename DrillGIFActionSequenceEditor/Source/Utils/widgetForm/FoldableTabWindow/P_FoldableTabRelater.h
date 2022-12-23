#ifndef P_FoldableTabManager_H
#define P_FoldableTabManager_H
#include "stdafx.h"

#include "Private/P_FoldableTabPrivate.h"
#include "Private/W_FoldableTabChildWindow.h"

/*
-----==========================================================-----
		类：		可折叠选项卡 管理器.h
		版本：		v1.04
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
	//----子块
	protected:
		QList<P_FoldableTabPrivate*> m_partList;
		bool m_slotBlock;		//槽阻塞
	public slots:
								//子块 - 添加子块
		void addPart(QString part_name, QWidget* partWidget);	
		void addPart(QString part_name, QWidget* partWidget, QJsonObject option);
		
								//子块 - 获取 - 根据名称
		P_FoldableTabPrivate* getPart_ByName(QString part_name);
								//子块 - 获取 - 全部
		QList<P_FoldableTabPrivate*> getPart_All();
								//子块 - 是否在选项卡中
		bool isPart_InTab(QString part_name);
								//子块 - 是否在窗口中
		bool isPart_InWindow(QString part_name);

								//子块 - 刷新全部子块
		void refreshAllPart();
	signals:
								//信号 - 子块被聚焦时
		void signal_whenPartFocusIn(QString part_name);
		
	//-----------------------------------
	//----选项卡
	protected:
		QTabWidget* m_tab;				//选项卡控件
		QString m_tabStyle;				//样式
	public slots:
								//选项卡 - 关闭选项卡
								//		【说明】：index为当前选项卡的相对位置，且可以越界。
		void closeTab_ByIndex(int relative_index);
		void closeTab_ByName(QString part_name);
								//选项卡 - 选择指定选项卡
								//		【说明】：如果标签为窗口模式，则无效。
		void selectTab_ByIndex(int relative_index);
		void selectTab_ByName(QString part_name);
								//选项卡 - 子块全归位
		void homingAllTab();
	protected slots:
								//选项卡 - 关闭选项卡（私有）
		void tabClosed(int relative_index);
								//选项卡 - 选项卡变化时（私有）
		void tabChanged(int relative_index);

	//-----------------------------------
	//----子窗口
	protected:
		QList<W_FoldableTabChildWindow*> m_childWindowList;
	public slots:
								//子窗口 - 显示
		void showChildWindow(QString part_name);	
								//子窗口 - 隐藏
		void hideChildWindow(QString part_name);
								//子窗口 - 全部隐藏（全部回到标签页中）
		void hideAllChildWindow();
								//子窗口 - 获取
		W_FoldableTabChildWindow* getChildWindow(QString part_name);
	protected slots:
								//子窗口 - 被聚焦时
		void childWindowActived(QString part_name);
		
};

#endif // P_FoldableTabManager_H
