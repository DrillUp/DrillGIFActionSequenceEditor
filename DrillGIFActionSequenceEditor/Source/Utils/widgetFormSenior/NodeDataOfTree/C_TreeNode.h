#pragma once
#include "stdafx.h"

#include <QObject>
#include "../NodeData/C_Node.h"

/*
-----==========================================================-----
		类：		树节点.h
		版本：		v1.01
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个树节点，用于建立一棵树。
					（详细见cpp）
-----==========================================================-----
*/
class C_TreeNodeFactory;
CLASS_PTR(C_TreeNode);
class C_TreeNode : public C_Node{

	public:
							//【此类需要通过工厂来创建】
		C_TreeNode(QString id, QString factory_id, QString nodeName);	//构造函数
		~C_TreeNode();													//析构函数
		

	//-----------------------------------
	//----树节点
	//		（这里的树节点是单向的结构，从 无向图 中指定的一个树根触发，逐渐向下分配层级和子节点。）
	protected:
		int m_tree_curLayer;			//当前层级
		QStringList m_tree_childIdList;	//子节点
	public:
										//树节点 - 设置当前层级
										//		【说明】：此操作 只被 工厂类 调用。
		void setTree_CurLayer_FactoryOnly(int layer);
										//树节点 - 设置子节点
										//		【说明】：此操作 只被 工厂类 调用。
		void setTree_ChildIdList_FactoryOnly(QStringList id_list);

										//树节点 - 获取当前层级
										//		【说明】：如果 结构不为树 或 未设置树根，则返回-1。
		int getTree_CurLayer();
										//树节点 - 获取父节点
										//		【说明】：获取前需要先判断 结构是否为树 。
		C_TreeNodePtr getTree_Parent();
		QString getTree_ParentId();
		
										//树节点 - 是否为树根
		bool isTreeRoot();
										//树节点 - 是否为树枝（包含树根）
		bool isTreeBranch();
										//树节点 - 是否为树叶
		bool isTreeLeaf();
										//树节点 - 获取 - 树节点
		QList<C_TreeNodePtr> getChildNode_Current();
		QList<C_TreeNodePtr> getChildNode_Recursion(int layer_deep = 0);
										//树节点 - 获取 - 树节点Id
		QStringList getChildNodeId_Current();
		QStringList getChildNodeId_Recursion(int layer_deep = 0);
										//树节点 - 获取 - 树节点数量
		int getChildNodeCount_Current();
		int getChildNodeCount_Recursion(int layer_deep = 0);
										//树节点 - 获取 - 树枝
		QList<C_TreeNodePtr> getChildBranch_Current();
		QList<C_TreeNodePtr> getChildBranch_Recursion(int layer_deep = 0);
										//树节点 - 获取 - 叶子
		QList<C_TreeNodePtr> getChildLeaf_Current();
		QList<C_TreeNodePtr> getChildLeaf_Recursion(int layer_deep = 0);
		
	//-----------------------------------
	//----类
	public:
										//类 - 获取类名
		virtual QStringList classInherited() override;
										//类 - 判断类
		bool isClass_TreeNode();
	protected:
										//类 - 创建自己
		virtual C_NodePtr _factory_newNode() override;
										//类 - 获取类
		virtual C_NodePtr _factory_getNodeById(QString node_id) override;
										//类 - 获取工厂
		virtual QSharedPointer<C_NodeFactory> _factory_get() override;
										//类 - 创建自己
		virtual C_TreeNodePtr _factoryTree_newNode();
										//类 - 获取类
		virtual C_TreeNodePtr _factoryTree_getNodeById(QString node_id);
										//类 - 获取工厂
		virtual QSharedPointer<C_TreeNodeFactory> _factoryTree_get();

	//-----------------------------------
	//----类属性
	public:
										//复制数据 当前类 -> 目标类
		virtual void copyTo(C_NodePtr data_to) override;
										//复制数据 目标类 -> 当前类
		virtual void copyFrom(C_NodePtr data_from) override;
										//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject() override;
										//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj) override;

};