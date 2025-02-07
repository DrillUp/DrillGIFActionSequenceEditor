#pragma once
#include "stdafx.h"

#include <QKeyEvent>
#include <QTreeWidget>

#include "Private/I_FPT_Branch.h"
#include "Private/I_FPT_Leaf.h"
#include "Private/C_FPT_Config.h"
#include "Private/W_FPT_Config.h"

/*
-----==========================================================-----
		类：		灵活分类树.cpp
		版本：		v1.05
		作者：		drill_up
		所属模块：	工具模块
		功能：		能够显示一堆数据，并且将这些数据分类或转移到不同的树枝中，便于查询。
					注意，该树被装饰后，需要对该控件块进行交互，不要对树进行直接交互。
					【包含id分支、名称分支，不含种类分支】
					（详细见cpp）
-----==========================================================-----
*/
class P_ObjectSortController;
class C_ObjectSortData;
class P_FlexiblePageTree : public QObject
{
	Q_OBJECT

	public:
		P_FlexiblePageTree(QTreeWidget *parent );	//构造函数
		~P_FlexiblePageTree();						//析构函数
		

	//-----------------------------------
	//----工厂（子类继承覆写用接口）
	public:
											//工厂 - 创建 树设置 数据（可覆写）
		virtual C_FPT_Config* createConfigData();
											//工厂 - 创建 树设置 编辑窗口（可覆写）
		virtual W_FPT_Config* createConfigWindow();
											//工厂 - 创建 叶子控件（可覆写）
		virtual I_FPT_Leaf* createFCTLeaf();
											//工厂 - 创建 树枝控件（可覆写）
		virtual I_FPT_Branch* createFCTBranch();

	//-----------------------------------
	//----树对象
	protected:
		QTreeWidget* m_tree;				//树对象
		QString m_treeStyle;				//树默认样式
	public:
									//树对象 - 获取树
		QTreeWidget* getTree();
									//树对象 - 刷新树
		void refreshTreeUi();
									//树对象 - 清理全部（不含树配置）
		virtual void clearAll();
	protected:
									//树对象 - 刷新树 - 分支
		virtual void refreshTreeUi_special();
									//树对象 - 刷新树 - 分支 - ID分支
		virtual void refreshTreeUi_id_inc();
									//树对象 - 刷新树 - 分支 - 名称分支
		virtual void refreshTreeUi_name_inc();

	//-----------------------------------
	//----叶子
	protected:
		QList<I_FPT_Leaf*> m_leafItem;		//叶子列表
	public:
									//叶子 - 获取 - 获取对象（根据ID）
		I_FPT_Leaf* getLeafById(int id);
		I_FPT_Leaf* getLeafByName(QString name);
									//叶子 - 获取 - 名称
		QString getLeafName(int id);
									//叶子 - 获取 - 判断叶子
		bool hasLeafName(QString name);
									//叶子 - 获取 - 判断对象
		bool isLeaf(QTreeWidgetItem* item);
		bool isLeafList(QList<QTreeWidgetItem*> item_list);
	public slots:
									//叶子 - 【外部修改】叶子名称
		virtual void outerModifyLeafName(int id, QString name);
									//叶子 - 【外部修改】叶子类型
		virtual void outerModifyLeafType(int id, QString type);
									//叶子 - 【外部修改】选中的叶子名称（选树枝不影响，只记录叶子）
		virtual void outerModifySelectedLeafName( QString name);
									//叶子 - 【外部修改】选中的叶子类型（选树枝不影响，只记录叶子）
		virtual void outerModifySelectedLeafType( QString type);

	//-----------------------------------
	//----树枝
	protected:
		QList<I_FPT_Branch*> m_branchItem;	//树枝列表
	public:
									//树枝 - 获取树枝（名称分支专用）
		I_FPT_Branch* getBranchByNameSymbol(QString symbol);
									//树枝 - 获取 - 判断对象
		bool isBranch(QTreeWidgetItem* item);
		bool isBranchList(QList<QTreeWidgetItem*> item_list);
		
	//-----------------------------------
	//----选中
	protected:
		QTreeWidgetItem* m_last_selectedItem;
		I_FPT_Leaf* m_last_selectedLeaf;
	public:
	signals:
									//选中 - 树节点变化（信号）
									//		【说明】：包括叶子和树枝。
		void signal_currentItemChanged(QTreeWidgetItem* item);
	public:
	signals:
									//选中 - 叶子变化（信号）
		void signal_currentLeafChanged(QTreeWidgetItem* item, int id, QString name);


	//-----------------------------------
	//----树事件
	public:
		bool m_slotBlock;					//槽阻塞
	public slots:
											//单击事件
		void sltItemClicked(QTreeWidgetItem *item, int index);
											//双击事件
		void sltItemDoubleClicked(QTreeWidgetItem *item, int index);
											//右键事件
		void sltItemRightClicked(QPoint point);
											//圈选变化事件
		void sltItemSelectionChanged();
		
	//-----------------------------------
	//----右键菜单
	protected:
		QMenu* m_mainMenu;						//右键菜单
		QMenu* m_modeMenu;						//分支菜单
		bool m_leafOuterControlEnabled;			//菜单 - 开关
		bool m_leafOuterControl_CopyActive;		//菜单 - 复制激活
		bool m_leafOuterControl_PasteActive;	//菜单 - 粘贴激活
		bool m_leafOuterControl_ClearActive;	//菜单 - 清空激活
	public:
											//右键菜单 - 一级菜单
		virtual void drawMenuMain();
		virtual void drawMenuMainLast();
											//右键菜单 - 二级菜单（模式）
		virtual void drawMenuMode();
	public slots:
											//右键菜单 - 开关（默认开启）
		void setLeafOuterControlEnabled(bool enabled);
											//右键菜单 - 激活控制
		void setLeafOuterControl_CopyActive(bool enabled);
		void setLeafOuterControl_PasteActive(bool enabled);
		void setLeafOuterControl_ClearActive(bool enabled);
											//右键菜单 - 复制按下
		void menuCopyLeafInAction();
											//右键菜单 - 粘贴按下
		void menuPasteLeafInAction();
											//右键菜单 - 清空按下
		void menuClearLeafInAction();
	public:
	signals:
											//右键菜单 - 复制按下（信号）
		void signal_menuCopyLeafTriggered(int id);
	public:
	signals:
											//右键菜单 - 粘贴按下（信号）
		void signal_menuPasteLeafTriggered(int id);
	public:
	signals:
											//右键菜单 - 清空按下（信号）
		void signal_menuClearLeafTriggered(int id);

		
	//-----------------------------------
	//----数据（树设置）
	protected:
		C_FPT_Config* m_config;			//树设置
	public:
										//树设置 - 设置参数
		virtual void setConfig(C_FPT_Config* config);
										//树设置 - 取出参数
		virtual C_FPT_Config* getConfig();
										//树设置 - 切换模式
		void changeSortMode(QString sortMode);
		void changeSortModeInAction();
										//树设置 - 获取当前模式
		QString getCurrentSortMode();
	protected slots:
										//树设置 - 编辑窗口
		void openConfigParamWindow();
		
	//-----------------------------------
	//----数据（资源数据）
	protected:
		P_ObjectSortController* m_source_ObjectSortController;		//数据排序管理器
		QList<C_ObjectSortData> m_source_list;						//解析的数据（该数据会在本地被修改）
	public:
										//资源数据 - 载入
										//			（必须放在setData初始化函数后面）
										//			（由于必须要叠加修改排序内容，所以这里不是指针。）
		void loadSource(QList<QJsonObject> obj_list);
		void loadSource(QList<QJsonObject> obj_list, QString id_symbol, QString name_symbol, QString type_symbol);
										//资源数据 - 获取选中的数据（若选的是树枝，返回-1或""）
		int getSelectedSourceId();
		QString getSelectedSourceName();
		QString getSelectedSourceType();
	protected:
										//资源数据 - 获取选中的对象（私有）
		C_ObjectSortData* getSelectedSource();
	protected slots:
										//资源数据 - 重建数据（私有）
		virtual void rebuildTreeData();
										//资源数据 - 重建数据_ID分支（私有）
		void rebuildTreeData_id_inc();
										//资源数据 - 重建数据_名称分支（私有）
		void rebuildTreeData_name_inc();

	//-----------------------------------
	//----窗口锁
	protected:
		int m_config_windowLockNum;
	protected:
										//窗口锁 - 窗口数量+1
		void windowLock_incOne();
										//窗口锁 - 窗口数量-1
		void windowLock_decOne();
	public:
	signals:
										//窗口锁 - 是否弹出任何窗口（信号）
										//		【说明】：该树弹出一个窗口会发送true，继续弹出多个不会发送，只有全部窗口关闭后才发false。
		void signal_anyWindowLocked(bool locked);

};
