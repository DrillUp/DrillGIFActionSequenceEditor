#pragma once
#include "stdafx.h"

#include <QObject>
#include "Source/Utils/WidgetFormSenior/NodeDataOfTree/C_TreeNode.h"

/*
-----==========================================================-----
		类：		状态节点（图数据）.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个状态节点，用于存放状态节点、状态元数据。
					功能基于 树节点，能进行树功能的解析。
					（详细见cpp）
-----==========================================================-----
*/
class C_COAS_StateNodeFactory;
CLASS_PTR(C_COAS_StateNode);
class C_COAS_StateNode : public C_TreeNode{

	public:
							//【此类需要通过工厂来创建】
		C_COAS_StateNode(QString id, QString factory_id, QString nodeName);	//构造函数
		~C_COAS_StateNode();													//析构函数
		

	//-----------------------------------
	//----状态节点
	protected:
		bool m_isObjData_State;			//状态元数据
		bool m_isObjData_StateNode;		//状态节点数据
		QJsonObject m_curObjData;		//当前数据
	public:
										//状态节点 - 设置状态元数据
		void setObjData_State(QJsonObject data);
										//状态节点 - 设置状态节点数据
		void setObjData_StateNode(QJsonObject data);
		
										//状态节点 - 是否为状态元数据
		bool isObjData_State();
										//状态节点 - 是否为状态节点数据
		bool isObjData_StateNode();
										//状态节点 - 播放类型 - 状态元
		bool isPlayType_State();
										//状态节点 - 播放类型 - 状态节点
		bool isPlayType_StateNode();
										//状态节点 - 获取子节点 - 状态元
		QStringList getChildList_State();
										//状态节点 - 获取子节点 - 状态节点
		QStringList getChildList_StateNode();
										//状态节点 - 获取节点描述
		QString getNodeDescription();

	//-----------------------------------
	//----类
	public:
										//类 - 获取类名
		virtual QStringList classInherited() override;
										//类 - 判断类
		bool isClass_StateTreeNode();
	protected:
										//类 - 创建自己
		virtual C_TreeNodePtr _factoryTree_newNode() override;
										//类 - 获取类
		virtual C_TreeNodePtr _factoryTree_getNodeById(QString node_id) override;
										//类 - 获取工厂
		virtual QSharedPointer<C_TreeNodeFactory> _factoryTree_get() override;

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