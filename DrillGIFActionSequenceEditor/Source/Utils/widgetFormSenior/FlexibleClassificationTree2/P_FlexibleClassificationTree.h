#pragma once
#include "stdafx.h"

#include "../FlexiblePageTree/P_FlexiblePageTree.h"

/*
-----==========================================================-----
		类：		灵活分类树（含种类分支）.cpp
		版本：		v1.05
		作者：		drill_up
		所属模块：	工具模块
		功能：		能够显示一堆数据，并且将这些数据分类或转移到不同的树枝中，便于查询。
					注意，该树被装饰后，需要对该控件块进行交互，不要对树进行直接交互。
					（详细见cpp）
-----==========================================================-----
*/
class C_FCT_Config;
class C_FCT_Classify;
class W_FCT_Classify;
class P_FlexibleClassificationTree : public P_FlexiblePageTree
{
	Q_OBJECT

	public:
		P_FlexibleClassificationTree(QTreeWidget *parent);		//构造函数
		~P_FlexibleClassificationTree();						//析构函数
		

	//-----------------------------------
	//----工厂（子类继承覆写用接口）
	public:
											//工厂 - 创建 树设置 数据（覆写父类）
		virtual C_FPT_Config* createConfigData();
											//工厂 - 创建 种类 数据（可覆写）
		virtual C_FCT_Classify* createClassifyData();
											//工厂 - 创建 种类 编辑窗口（可覆写）
		virtual W_FCT_Classify* createClassifyWindow();
		
	//-----------------------------------
	//----树对象
	public:
									//树对象 - 清理全部
		virtual void clearAll() override;
	protected:
									//树对象 - 刷新树 - 分支
		virtual void refreshTreeUi_special() override;
									//树对象 - 刷新树 - 分支 - 种类分支_id递增
		virtual void refreshTreeUi_classify_idInc();
									//树对象 - 刷新树 - 分支 - 种类分支_名称递增
		virtual void refreshTreeUi_classify_nameInc();
		

	//-----------------------------------
	//----叶子
	public:
									//叶子 - 获取 - 根据种类获取
		QList<I_FPT_Leaf*> getLeafByType(QString type);
									//叶子 - 获取 - 种类
		QString getLeafType(int id);
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
	public:
									//树枝 - 获取树枝（种类分支专用）
		I_FPT_Branch* getBranchByTypeName(QString classify_name);


	//-----------------------------------
	//----数据（种类分支）
	public slots:
									//种类 - 添加
		void addClassify(C_FCT_Classify* classify);
									//种类 - 添加（action）
		void addClassifyInAction();
									//种类 - 添加 - 验证新种类
		void addClassifyDistinguishedList(QStringList new_classify_nameList);
									//种类 - 修改
		void modifyClassify(QString classify_name, C_FCT_Classify* classify);
									//种类 - 修改（action）
		void modifyClassifyInAction();
									//种类 - 去除
		void removeClassify(QString classify_name);
									//种类 - 去除（action）
		void removeClassifyInAction();
		void removeClassifyListInAction();
									//种类 - 上移
		void moveUpInAction();
									//种类 - 下移
		void moveDownInAction();
									//种类 - 获取 - 是否为种类分支
		bool isClassifyMode();
									//种类 - 获取 - 全部种类名
		QStringList getAllClassifyName();
	protected:
									//种类 - 从本地数据中，读取种类列表
		void appendClassifyData_FromLocalObj();
	
	//-----------------------------------
	//----数据（修改的数据）
	protected:
		QList<C_ObjectSortData> m_changedSource;
	public:
										//修改的数据 - 【内部修改】叶子的类型
		void innerModifyLeafType(int id, QString type);
		void innerModifyLeafTypeInAction();
										//修改的数据 - 获取叶子的类型
		QString getLeafTypeById(int id);
										//修改的数据 - 记录修改（只改type参数）
		void appendChangedSource(C_ObjectSortData data);
										//修改的数据 - 修改的数据（只改type参数）
		QList<C_ObjectSortData> getChangedSource();


	//-----------------------------------
	//----右键菜单
	public:
											//右键菜单 - 一级菜单
		virtual void drawMenuMain() override;
		virtual void drawMenuMainLast() override;
											//右键菜单 - 二级菜单（模式）
		virtual void drawMenuMode() override;
		
		
	//-----------------------------------
	//----数据（树设置）
	protected:
										//树设置 - 设置参数
		virtual void setConfig(C_FPT_Config* config) override;
										//树设置 - 取出参数
		virtual C_FPT_Config* getConfig() override;
	public:
										//树设置 - 设置参数Ex
		virtual void setConfigEx(C_FCT_Config* config);
										//树设置 - 取出参数Ex
		virtual C_FCT_Config* getConfigEx();


	//-----------------------------------
	//----数据（资源数据）
	protected slots:
										//资源数据 - 重建数据（私有）
		virtual void rebuildTreeData() override;
										//资源数据 - 重建数据_种类分支_id递增（私有）
		void rebuildTreeData_classify_idInc();
										//资源数据 - 重建数据_种类分支_名称递增（私有）
		void rebuildTreeData_classify_nameInc();

};
