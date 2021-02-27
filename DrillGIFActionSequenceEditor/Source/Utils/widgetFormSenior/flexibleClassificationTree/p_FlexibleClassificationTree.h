#pragma once

#include "P_FlexiblePageTree.h"

/*
-----==========================================================-----
		类：		灵活分类树（含自定义分支）.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		能够显示一堆数据，并且将这些数据分类或转移到不同的树枝中，便于查询。
					注意，该树被装饰后，需要对该控件块进行交互，不要对树进行直接交互。
					（详细见cpp）
-----==========================================================-----
*/
class P_FlexibleClassificationTree : public P_FlexiblePageTree
{
	Q_OBJECT

	public:
		P_FlexibleClassificationTree(QTreeWidget *parent);		//构造函数
		~P_FlexibleClassificationTree();						//析构函数
		
	//-----------------------------------
	//----控件
	public:
									//控件 - 刷新树
		virtual void refreshTreeUi_special();
									//控件 - 刷新树_自定义分支_id递增
		void refreshTreeUi_classify_idInc();
									//控件 - 刷新树_自定义分支_名称递增
		void refreshTreeUi_classify_nameInc();
									//控件 - 清理全部
		virtual void clearAll();
		
	//-----------------------------------
	//----控件（叶子）
	public:
									//叶子 - 获取 - 根据类型获取
		QList<I_FCTLeaf*> getLeafByType(QString type);
									//叶子 - 获取 - 类型
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
	//----控件（树枝）
	public:
									//树枝 - 获取树枝（自定义分支专用）
		I_FCTBranch* getBranchByTypeName(QString classify_name);
		
	//-----------------------------------
	//----数据
	public:
									//数据 - 本地数据 -> ui数据
		virtual void putDataToUi();
									//数据 - ui数据 -> 本地数据
		virtual void putUiToData();

	//-----------------------------------
	//----数据（自定义分支）
	protected:
		QList<C_FCTClassify> m_classifyData;		//自定义分支（只在putDataToUi时初始化）
	public slots:
									//类型 - 添加
		void addClassify(C_FCTClassify classify);
		void addClassifyInAction();
									//类型 - 修改
		void modifyClassify(QString classify_name, C_FCTClassify classify);
		void modifyClassifyInAction();
									//类型 - 去除
		void removeClassify(QString classify_name);
		void removeClassifyInAction();
		void removeClassifyListInAction();
									//类型 - 上移
		void moveUpInAction();
									//类型 - 下移
		void moveDownInAction();
									//类型 - 获取 - 全部类型
		QStringList getAllClassify();
									//类型 - 获取 - 获取类型索引
		int indexOfClassify(QString classify_name);
									//类型 - 获取 - 判断类型存在
		bool hasClassify(QString classify_name);
									//类型 - 当前为自定义分支模式
		bool isClassifyMode();
	protected:
									//类型 - 从本地数据中，读取类型列表
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
		virtual void drawMenuMain();
		virtual void drawMenuMainLast();
											//右键菜单 - 二级菜单（模式）
		virtual void drawMenuMode();
		
	//-----------------------------------
	//----资源数据
	protected slots:
										//资源数据 - 重建数据（私有）
		virtual void rebuildTreeData();
										//资源数据 - 重建数据_自定义分支_id递增（私有）
		void rebuildTreeData_classify_idInc();
										//资源数据 - 重建数据_自定义分支_名称递增（私有）
		void rebuildTreeData_classify_nameInc();

};
