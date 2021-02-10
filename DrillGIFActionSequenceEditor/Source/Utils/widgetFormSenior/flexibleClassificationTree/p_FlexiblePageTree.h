#pragma once

#include <QKeyEvent>
#include <QTreeWidget>

#include "private/c_FCTClassify.h"
#include "private/c_FCTConfig.h"
#include "private/i_FCTBranch.h"
#include "private/i_FCTLeaf.h"

/*
-----==========================================================-----
		类：		灵活分类树.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		能够显示一堆数据，并且将这些数据分类或转移到不同的树枝中，便于查询。
					注意，该树被装饰后，需要对该控件块进行交互，不要对树进行直接交互。
					【包含id分支、名称分支，不含自定义分支】
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
	//----控件
	protected:
		QTreeWidget* m_tree;						//树对象
		QString m_treeStyle;						//树默认样式
		bool m_slotBlock;							//槽阻塞
		QList<I_FCTLeaf*> m_leafItem;				//叶子列表
		QList<I_FCTBranch*> m_branchItem;			//树枝列表
	public:
									//控件 - 刷新树
		void refreshTreeUi();
									//控件 - 刷新树_分支操作
		virtual void refreshTreeUi_special();
									//控件 - 刷新树_ID分支
		void refreshTreeUi_id_inc();
									//控件 - 刷新树_名称分支
		void refreshTreeUi_name_inc();
									//控件 - 清理全部
		virtual void clearAll();

	//-----------------------------------
	//----控件（叶子）
	public:
									//叶子 - 【外部修改】叶子名称
		virtual void outerModifyLeafName(int id, QString name);
									//叶子 - 【外部修改】叶子类型
		virtual void outerModifyLeafType(int id, QString type);
									//叶子 - 获取 - 获取对象（根据ID）
		I_FCTLeaf* getLeafById(int id);
									//叶子 - 获取 - 判断叶子
		bool hasLeafName(QString name);
		I_FCTLeaf* getLeafByName(QString name);
									//叶子 - 获取 - 判断对象
		bool isLeaf(QTreeWidgetItem* item);
		bool isLeafList(QList<QTreeWidgetItem*> item_list);
		
	//-----------------------------------
	//----控件（叶子接口）
	protected:
		bool m_leafOuterControlEnabled;
	signals:
									//叶子接口 - 复制按下（信号）
		void menuCopyLeafTriggered(int id);
									//叶子接口 - 粘贴按下（信号）
		void menuPasteLeafTriggered(int id);
									//叶子接口 - 清空按下（信号）
		void menuClearLeafTriggered(int id);
	public:
									//叶子接口 - 开关（默认开启）
		void setLeafOuterControlEnabled(bool enabled);
									//叶子接口 - 复制按下
		void menuCopyLeafInAction();
									//叶子接口 - 粘贴按下
		void menuPasteLeafInAction();
									//叶子接口 - 清空按下
		void menuClearLeafInAction();

	//-----------------------------------
	//----控件（树枝）
	public:
									//树枝 - 获取树枝（名称分支专用）
		I_FCTBranch* getBranchByNameSymbol(QString symbol);
									//树枝 - 获取 - 判断对象
		bool isBranch(QTreeWidgetItem* item);
		bool isBranchList(QList<QTreeWidgetItem*> item_list);

	//-----------------------------------
	//----数据
	protected:
		QJsonObject local_treeData;			//树数据（包含 模式、 树设置 和 自定义分支）
	public:
									//数据 - 设置数据
		void setData(QJsonObject obj);
									//数据 - 取出
		QJsonObject getData();
									//数据 - 本地数据 -> ui数据
		virtual void putDataToUi();
									//数据 - ui数据 -> 本地数据
		virtual void putUiToData();

	//-----------------------------------
	//----数据（模式）
	protected:
		QString m_sortMode;				//当前模式
		QStringList m_sortModeList;		//模式类型
	public slots:
									//模式 - 切换模式
		void changeSortMode(QString sortMode);
		void changeSortModeInAction();
									//模式 - 获取
		QString getCurSortMode();
		
	//-----------------------------------
	//----数据（树设置）
	protected:
		C_FCTConfig m_config;	//树设置
	public:
										//树设置 - 设置参数
		void setConfigParam(QJsonObject config);
										//树设置 - 取出参数
		QJsonObject getConfigParam();
	protected slots:
										//树设置 - 编辑窗口
		void openConfigParamWindow();

	//-----------------------------------
	//----树事件
	public:
		bool m_selectionSignalBlock_Root;		//圈选 锁
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
		QMenu* m_mainMenu;
		QMenu* m_modeMenu;
		QString m_menuIconSrcPath;
	public:
											//右键菜单 - 一级菜单
		virtual void drawMenuMain();
		virtual void drawMenuMainLast();
											//右键菜单 - 二级菜单（模式）
		virtual void drawMenuMode();
		
	//-----------------------------------
	//----资源数据
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

};
