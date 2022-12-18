#pragma once
#include "stdafx.h"

#include <QObject>


/*
-----==========================================================-----
		类：		节点.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个节点，用于描述无向图。
					（详细见cpp）
-----==========================================================-----
*/
class C_NodeFactory;
CLASS_PTR(C_Node);
class C_Node{

	public:
							//【此类需要通过工厂来创建】
		C_Node(QString id, QString factory_id, QString nodeName);	//构造函数
		~C_Node();													//析构函数
		

	//-----------------------------------
	//----节点
	protected:
		QStringList m_connectedNodeIdList;		//连接的节点
	public:
										//节点 - 连接节点
										//		【说明】：此操作是双向的，连接目标节点后，目标节点也会连接此节点。
		void connectNode(C_NodePtr node);
		void connectNodeId(QString node_id);
										//节点 - 断开节点
		void disconnectNode(C_NodePtr node);
		void disconnectNodeId(QString node_id);
		void disconnectNodeAll();
		
										//节点 - 获取 - 连接的节点
		QList<C_NodePtr> getConnectedNode_All();
										//节点 - 获取 - 节点Id
		QStringList getConnectedNode_AllId();
										//节点 - 获取 - 节点数量
		int getConnectedNode_AllCount();
										//节点 - 获取 - 是否含某节点连接
		bool hasConnect(C_NodePtr node);
		bool hasConnectId(QString node_id);
		

	//-----------------------------------
	//----自连接
	protected:
		bool m_canSelfConnect;
	public:
										//自连接 - 设置
		void setCanSelfConnect(bool enabled);
		bool canSelfConnect();
										//自连接 - 是否含自连接
		bool hasSelfConnect();
										//自连接 - 自连接
		void connectSelf();
										//自连接 - 断开自连接
		void disconnectSelf();
		

	//-----------------------------------
	//----类
	protected:
		QString id;							//标识
		QString name;						//名称
		QString factory_id;					//工厂标识
	public:
										//类 - 访问器 - 标识
		void setId(QString id);
		QString getId();
										//类 - 访问器 - 名称
		void setName(QString nodeName);
		QString getName();
										//类 - 访问器 - 工厂标识
		void setFactoryId(QString factory_id);
		QString getFactoryId();
										//类 - 获取类名
		QString className();
		bool classIsInstanceOf(QString className);
		virtual QStringList classInherited();
										//类 - 判断类
		bool isClass_Node();
	protected:
										//类 - 创建自己
		virtual C_NodePtr _factory_newNode();
										//类 - 获取类
		virtual C_NodePtr _factory_getNodeById(QString node_id);
										//类 - 获取工厂
		virtual QSharedPointer<C_NodeFactory> _factory_get();

	//-----------------------------------
	//----类属性
	public:
										//运算符重载
		const bool operator== (const C_Node& a)const;
										//判断空
		bool isNull();
										//复制对象
		C_NodePtr clone();
										//复制数据 当前类 -> 目标类
		virtual void copyTo(C_NodePtr data_to);
										//复制数据 目标类 -> 当前类
		virtual void copyFrom(C_NodePtr data_from);
										//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject();
										//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj);

};