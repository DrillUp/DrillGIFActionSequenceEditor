#pragma once
#include "stdafx.h"

#include "C_TreeNode.h"
#include "../NodeData/C_NodeFactory.h"

/*
-----==========================================================-----
		类：		树节点工厂.h
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个树节点工厂+树节点容器，用于描述/存放一棵树。
					【此类包含工厂+容器，创建此类后需要 手动 加入到工厂容器中】
					（详细见cpp）
-----==========================================================-----
*/
CLASS_PTR(C_TreeNodeFactory);
class C_TreeNodeFactory : public C_NodeFactory
{

	public:
							//【此类创建后，需要手动加入到容器中】
		C_TreeNodeFactory();
		~C_TreeNodeFactory();
		
	//-----------------------------------
	//----工厂
	public:
										//工厂 - 创建 节点
		virtual C_NodePtr createNode(QString nodeName) override;
		virtual C_NodePtr createNodeEmpty() override;
										//工厂 - 创建 树节点
		virtual C_TreeNodePtr createTreeNode(QString nodeName);
		virtual C_TreeNodePtr createTreeNodeEmpty();
		
	//-----------------------------------
	//----树关系
	public:
										//树关系 - 是否为树
		bool isTree();
										//树关系 - 设置树根
										//		【说明】：必须先创建所有节点，并连接所有节点，确保 无向图为树 之后，才能设置树根。
		void setTreeRoot(C_TreeNodePtr tree_node);
										//树关系 - 是否含树根
		bool hasTreeRoot();

										//树关系 - 获取 - 树根
										//		【说明】：需要先设置树根。注意，如果是树节点子类，要记得转换为子类。
		C_TreeNodePtr getTreeRoot();
										//树关系 - 获取 - 树枝（包含树根）
										//		【说明】：需要先设置树根。注意，如果是树节点子类，要记得转换为子类。
		QList<C_TreeNodePtr> getTreeBranchAll();
		int getTreeBranchCount();
										//树关系 - 获取 - 树叶
										//		【说明】：需要先设置树根。注意，如果是树节点子类，要记得转换为子类。
		QList<C_TreeNodePtr> getTreeLeafAll();
		int getTreeLeafCount();

										//树关系 - 获取 - 树深度
										//		【说明】：需要先设置树根。
		int getTreeDeep();
										//树关系 - 获取 - 树节点（根据深度）
										//		【说明1】：树根的深度为0。
										//		【说明2】：需要先设置树根。注意，如果是树节点子类，要记得转换为子类。
		QList<C_TreeNodePtr> getTreeNode_ByDeep(int deep);

	protected:
										//树关系 - 根据树根设置层级（私有）
		void setTreeLayer_Recursion(C_TreeNodePtr tree_node, int layer);
		

	//-----------------------------------
	//----树节点容器
	public:
										//类容器 - 获取 - 全部（原装）
		QList<C_TreeNodePtr> getTreeNode_AllOrg();
										//类容器 - 获取 - 全部
		QList<C_TreeNodePtr> getTreeNode_All();
										//类容器 - 获取 - 全部Id
		QStringList getTreeNode_AllId();
										//类容器 - 获取 - 全部名称
		QStringList getTreeNode_AllName();
										//类容器 - 获取 - 根据ID
		C_TreeNodePtr getTreeNode_ById(QString TreeNode_id);
										//类容器 - 获取 - 根据名称
		C_TreeNodePtr getTreeNode_ByName(QString TreeNode_name);
										//类容器 - 是否含ID
		bool hasTreeNode_Id(QString TreeNode_id);
										//类容器 - 是否含名称
		bool hasTreeNode_Name(QString TreeNode_name);
	protected:
										//私有 - 获取索引
		int getTreeNodeIndex_ById(QString TreeNode_id);
		int getTreeNodeIndex_ByName(QString TreeNode_name);
		

	//-----------------------------------
	//----类
	public:
										//类 - 获取类名
		virtual QStringList classInherited() override;
										//类 - 判断类
		bool isClass_TreeNodeFactory();

	//-----------------------------------
	//----类属性
	public:
										//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject() override;
										//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj) override;

};

