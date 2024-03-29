﻿#pragma once
#include "stdafx.h"

#include "C_Node.h"

/*
-----==========================================================-----
		类：		节点工厂.h
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个节点工厂+节点容器，用于描述/存放无向图。
					【此类包含工厂+容器，创建此类后需要 手动 加入到工厂容器中】
					（详细见cpp）
-----==========================================================-----
*/

CLASS_PTR(C_NodeFactory);
class C_NodeFactory : public C_ContainerDataBase{

	public:
							//【此类创建后，需要手动加入到容器中】
		C_NodeFactory();
		~C_NodeFactory();
		
	//-----------------------------------
	//----工厂
	public:
										//工厂 - 创建 节点
		virtual C_NodePtr createNode(QString nodeName);
		virtual C_NodePtr createNodeEmpty();
		

	//-----------------------------------
	//----图关系
	protected:
		bool m_temp_hasLoop;
		bool m_temp_hasIsolated;
	public:
										//图关系 - 是否含自连接
		bool hasSelfConnect();
										//图关系 - 是否含回路
		bool hasLoop();
										//图关系 - 是否含孤岛
		bool hasIsolated();
	protected:
										//图关系 - 执行遍历
		void ergodicNode(C_NodePtr root);

										//算法 - 深度优先遍历（DFS）
		void demo_DFS(C_NodePtr root);
										//算法 - 广度优先遍历（BFS）
		void demo_BFS(C_NodePtr root);

		
	//-----------------------------------
	//----节点容器
	protected:
		QList<C_NodePtr> m_NodeTank;
	public:
										//类容器 - 删除 - 空类
		void removeNode_AllEmpty();
										//类容器 - 删除 - 根据ID
		bool removeNode_ById(QString Node_id);
										//类容器 - 删除 - 根据名称
		bool removeNode_ByName(QString Node_name);

										//类容器 - 获取 - 全部（原装）
		QList<C_NodePtr> getNode_AllOrg();
										//类容器 - 获取 - 全部
		QList<C_NodePtr> getNode_All();
										//类容器 - 获取 - 全部Id
		QStringList getNode_AllId();
										//类容器 - 获取 - 全部名称
		QStringList getNode_AllName();
										//类容器 - 获取 - 根据ID
		C_NodePtr getNode_ById(QString Node_id);
										//类容器 - 获取 - 根据名称
		C_NodePtr getNode_ByName(QString Node_name);
										//类容器 - 是否含ID
		bool hasNode_Id(QString Node_id);
										//类容器 - 是否含名称
		bool hasNode_Name(QString Node_name);
	protected:
										//私有 - 获取索引
		int getNodeIndex_ById(QString Node_id);
		int getNodeIndex_ByName(QString Node_name);
		

	//-----------------------------------
	//----类
	public:
										//访问器 - 获取子类类名
		virtual QString get__CLASS_NAME__() override;
										//访问器 - 获取子类类名（中文名）
		virtual QString get__CLASS_NAME__Chinese() override;
										//访问器 - 获取所有继承的类名
		virtual QStringList get__CLASS_INHERITED_LIST__() override;
										//访问器 - 是否继承了某类
		virtual bool get__CLASS_IS_INSTANCE_OF__(QString class_name) override;
										//类 - 判断类
		bool isClass_NodeFactory();
		
	//-----------------------------------
	//----类属性
	public:
										//运算符重载
		const bool operator== (const C_NodeFactory& a)const;
										//判断空
		bool isNull();
										//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject_childData();
										//QJsonObject -> 实体类
		virtual void setJsonObject_childData(QJsonObject obj);
};

