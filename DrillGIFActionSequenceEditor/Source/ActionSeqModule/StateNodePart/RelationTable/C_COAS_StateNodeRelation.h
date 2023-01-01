#pragma once
#include "stdafx.h"

#include <QObject>
#include "../../StatePart/C_COAS_State.h"
#include "../C_COAS_StateNode.h"
#include "Source/Utils/WidgetFormSenior/NodeDataOfTree/C_TreeNode.h"

/*
-----==========================================================-----
		类：		状态节点（图数据）.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个状态节点，用于存放状态节点、状态元数据（临时的数据）。
					功能基于 树节点，能进行树功能的解析。
					（详细见cpp）
-----==========================================================-----
*/
class C_COAS_StateNodeRelationFactory;
CLASS_PTR(C_COAS_StateNodeRelation);
class C_COAS_StateNodeRelation : public C_TreeNode{

	public:
							//【此类需要通过工厂来创建】
		C_COAS_StateNodeRelation(QString id, QString factory_id, QString nodeName);	//构造函数
		~C_COAS_StateNodeRelation();												//析构函数
		

	//-----------------------------------
	//----状态节点
	protected:
		bool m_isPtrData_State;							//状态元数据
		bool m_isPtrData_StateNode;						//状态节点数据
		C_COAS_StatePtr m_curPtrData_State;				//状态元数据
		C_COAS_StateNodePtr m_curPtrData_StateNode;		//状态节点数据
	public:
										//状态节点 - 设置状态元数据
		void setPtrData_State(C_COAS_StatePtr data);
										//状态节点 - 设置状态节点数据
		void setPtrData_StateNode(C_COAS_StateNodePtr data);
		
										//状态节点 - 是否为状态元数据
		bool isPtrData_State();
										//状态节点 - 是否为状态节点数据
		bool isPtrData_StateNode();
										//状态节点 - 播放类型 - 播放状态元
		bool isPlayType_State();
										//状态节点 - 播放类型 - 播放状态节点
		bool isPlayType_StateNode();
										//状态节点 - 播放类型 - 随机播放
		bool isPlayType_RandomPlay();
										//状态节点 - 播放类型 - 顺序播放
		bool isPlayType_PlainPlay();
										//状态节点 - 获取子节点 - 状态元
		QStringList getChildList_State();
										//状态节点 - 获取子节点 - 状态节点
		QStringList getChildList_StateNode();
										//状态节点 - 获取节点描述（前缀）
		QString getNodeDescriptionPrefix();
										//状态节点 - 获取节点描述（后缀）
		QString getNodeDescriptionSuffix();

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