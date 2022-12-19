#pragma once
#include "stdafx.h"

#include "C_StateTreeNode.h"
#include "Source/Utils/widgetFormSenior/NodeDataOfTree/C_TreeNodeFactory.h"

/*
-----==========================================================-----
		类：		状态节点工厂（图数据）.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		定义一个树节点工厂+树节点容器，用于描述/存放一棵树。
					（详细见cpp）
-----==========================================================-----
*/
CLASS_PTR(C_StateTreeNodeFactory);
class C_StateTreeNodeFactory : public C_TreeNodeFactory
{

	public:
							//【此类创建后，需要手动加入到容器中】
		C_StateTreeNodeFactory();
		~C_StateTreeNodeFactory();
		
	//-----------------------------------
	//----工厂
	public:
										//工厂 - 创建 树节点
		C_TreeNodePtr createTreeNode(QString nodeName) override;
		C_TreeNodePtr createTreeNodeEmpty() override;
										//工厂 - 创建 状态节点
		C_StateTreeNodePtr createStateTreeNode(QString nodeName);
		C_StateTreeNodePtr createStateTreeNodeEmpty();
		

	//-----------------------------------
	//----状态节点容器
	public:
										//类容器 - 获取 - 全部（原装）
		QList<C_StateTreeNodePtr> getStateTreeNode_AllOrg();
										//类容器 - 获取 - 全部
		QList<C_StateTreeNodePtr> getStateTreeNode_All();
										//类容器 - 获取 - 全部Id
		QStringList getStateTreeNode_AllId();
										//类容器 - 获取 - 全部名称
		QStringList getStateTreeNode_AllName();
										//类容器 - 获取 - 根据ID
		C_StateTreeNodePtr getStateTreeNode_ById(QString StateTreeNode_id);
										//类容器 - 获取 - 根据名称
		C_StateTreeNodePtr getStateTreeNode_ByName(QString StateTreeNode_name);
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

