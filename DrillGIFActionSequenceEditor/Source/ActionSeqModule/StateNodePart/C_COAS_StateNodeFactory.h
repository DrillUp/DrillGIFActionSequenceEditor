#pragma once
#include "stdafx.h"

#include "C_COAS_StateNode.h"
#include "Source/Utils/WidgetFormSenior/NodeDataOfTree/C_TreeNodeFactory.h"

/*
-----==========================================================-----
		类：		状态节点工厂（图数据）.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个树节点工厂+树节点容器，用于描述/存放一棵树。
					（详细见cpp）
-----==========================================================-----
*/
CLASS_PTR(C_COAS_StateNodeFactory);
class C_COAS_StateNodeFactory : public C_TreeNodeFactory
{

	public:
							//【此类创建后，需要手动加入到容器中】
		C_COAS_StateNodeFactory();
		~C_COAS_StateNodeFactory();
		
	//-----------------------------------
	//----工厂
	public:
										//工厂 - 创建 树节点
		C_TreeNodePtr createTreeNode(QString nodeName) override;
		C_TreeNodePtr createTreeNodeEmpty() override;
										//工厂 - 创建 状态节点
		C_COAS_StateNodePtr createStateTreeNode(QString nodeName);
		C_COAS_StateNodePtr createStateTreeNodeEmpty();
		

	//-----------------------------------
	//----状态节点容器
	public:
										//类容器 - 获取 - 全部（原装）
		QList<C_COAS_StateNodePtr> getStateTreeNode_AllOrg();
										//类容器 - 获取 - 全部
		QList<C_COAS_StateNodePtr> getStateTreeNode_All();
										//类容器 - 获取 - 全部Id
		QStringList getStateTreeNode_AllId();
										//类容器 - 获取 - 全部名称
		QStringList getStateTreeNode_AllName();
										//类容器 - 获取 - 根据ID
		C_COAS_StateNodePtr getStateTreeNode_ById(QString StateTreeNode_id);
										//类容器 - 获取 - 根据名称
		C_COAS_StateNodePtr getStateTreeNode_ByName(QString StateTreeNode_name);
										//类容器 - 是否含ID
		bool hasStateTreeNode_Id(QString StateTreeNode_id);
										//类容器 - 是否含名称
		bool hasStateTreeNode_Name(QString StateTreeNode_name);
	protected:
										//私有 - 获取索引
		int getStateTreeNodeIndex_ById(QString StateTreeNode_id);
		int getStateTreeNodeIndex_ByName(QString StateTreeNode_name);
		

	//-----------------------------------
	//----类
	public:
										//类 - 获取类名
		virtual QStringList classInherited() override;
										//类 - 判断类
		bool isClass_StateTreeNodeFactory();

	//-----------------------------------
	//----类属性
	public:
										//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject() override;
										//QJsonObject -> 实体类
		virtual void setJsonObject(QJsonObject obj) override;

};

