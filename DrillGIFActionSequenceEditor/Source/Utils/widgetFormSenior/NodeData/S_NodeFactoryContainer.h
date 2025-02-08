#pragma once
#include "stdafx.h"

#include "C_NodeFactory.h"


/*
-----==========================================================-----
		类：		节点工厂 容器.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		存放工厂对象的容器，用于 节点获取创建自己的工厂对象 用。
					【此容器存放所有 工厂，包括子类工厂。】
					（详细见cpp）
-----==========================================================-----
*/

class S_NodeFactoryContainer : public QObject
{
	Q_OBJECT

	public:
		S_NodeFactoryContainer();
		~S_NodeFactoryContainer();
		static S_NodeFactoryContainer* cur_manager;		//单例
		static S_NodeFactoryContainer* getInstance();	//单例，获取自己
		
	//-----------------------------------
	//----工厂容器
	protected:
		QList<C_NodeFactoryPtr> m_NodeFactoryTank;
	public:
										//工厂容器 - 添加
		void addFactory(C_NodeFactoryPtr factory);
										//工厂容器 - 删除
		void removeFactory(QString factory_id);
		
										//工厂容器 - 获取 - 全部
		QList<C_NodeFactoryPtr> getFactory_All();
										//工厂容器 - 获取 - 全部Id
		QStringList getFactory_AllId();
										//工厂容器 - 获取 - 根据Id
		C_NodeFactoryPtr getFactory_ById(QString factory_id);
										//工厂容器 - 获取 - 是否含工厂
		bool hasFactory(QString factory_id);
	protected:
										//私有 - 获取索引
		int getFactoryIndex_ById(QString id);

};

