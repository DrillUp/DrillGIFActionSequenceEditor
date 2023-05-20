#pragma once
#include "stdafx.h"

#include "C_COAS_StateNodeRelation.h"
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
CLASS_PTR(C_COAS_StateNodeRelationFactory);
class C_COAS_StateNodeRelationFactory : public C_TreeNodeFactory
{

	public:
							//【此类创建后，需要手动加入到容器中】
		C_COAS_StateNodeRelationFactory();
		~C_COAS_StateNodeRelationFactory();
		
	//-----------------------------------
	//----工厂
	public:
										//工厂 - 创建 树节点
		C_TreeNodePtr createTreeNode(QString nodeName) override;
		C_TreeNodePtr createTreeNodeEmpty() override;
										//工厂 - 创建 状态节点
		C_COAS_StateNodeRelationPtr createStateTreeNode(QString nodeName);
		C_COAS_StateNodeRelationPtr createStateTreeNodeEmpty();
		

	//-----------------------------------
	//----状态节点容器
	public:
										//类容器 - 获取 - 全部（原装）
		QList<C_COAS_StateNodeRelationPtr> getStateTreeNode_AllOrg();
										//类容器 - 获取 - 全部
		QList<C_COAS_StateNodeRelationPtr> getStateTreeNode_All();
										//类容器 - 获取 - 全部Id
		QStringList getStateTreeNode_AllId();
										//类容器 - 获取 - 全部名称
		QStringList getStateTreeNode_AllName();
										//类容器 - 获取 - 根据ID
		C_COAS_StateNodeRelationPtr getStateTreeNode_ById(QString StateTreeNode_id);
										//类容器 - 获取 - 根据名称
		C_COAS_StateNodeRelationPtr getStateTreeNode_ByName(QString StateTreeNode_name);
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
										//访问器 - 获取子类类名
		virtual QString get__CLASS_NAME__() override;
										//访问器 - 获取子类类名（中文名）
		virtual QString get__CLASS_NAME__Chinese() override;
										//访问器 - 获取所有继承的类名
		virtual QStringList get__CLASS_INHERITED_LIST__() override;
										//访问器 - 是否继承了某类
		virtual bool get__CLASS_IS_INSTANCE_OF__(QString class_name) override;
										//类 - 判断类
		bool isClass_StateTreeNodeFactory();

	//-----------------------------------
	//----类属性
	public:
										//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject_childData() override;
										//QJsonObject -> 实体类
		virtual void setJsonObject_childData(QJsonObject obj) override;

};

