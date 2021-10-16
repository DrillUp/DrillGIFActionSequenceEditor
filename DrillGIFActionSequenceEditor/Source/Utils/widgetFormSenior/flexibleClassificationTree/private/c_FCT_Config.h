#pragma once

#include "Source/Utils/widgetFormSenior/flexiblePageTree/private/c_FPT_Config.h"
#include "c_FCT_Classify.h"

/*
-----==========================================================-----
		类：		树设置 数据类（常规+种类分支）.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		灵活分类树的 树设置 数据类。
					（详细见cpp）
-----==========================================================-----
*/
class C_FCT_Config : public C_FPT_Config{

	public:
		C_FCT_Config();
		~C_FCT_Config();
		
	//-----------------------------------
	//----分支模式
	public:
									//分支模式 - 判断 种类分支_ID递增 模式
		bool is_classify_idInc_Mode();
									//分支模式 - 判断 种类分支_名称递增 模式
		bool is_classify_nameInc_Mode();
		
	//-----------------------------------
	//----种类页
	protected:
		C_FCT_Classify* m_emptyClassify;
		QList<C_FCT_Classify*> m_classifyList;
	public:
									//种类页 - 获取列表
		QList<C_FCT_Classify*> get_classify_DataList();
									//种类页 - 清理列表
		void clear_classify_DataList();

									//种类页 - 添加数据（基函数）
		void add_classify(C_FCT_Classify* data);
									//种类页 - 删除数据（基函数）
		void remove_classify(QString classify_name);
									//种类页 - 数据上移
		void set_classify_moveUp(QString classify_name);
									//种类页 - 数据下移
		void set_classify_moveDown(QString classify_name);

									//种类页 - 获取
		C_FCT_Classify* get_classify(QString classify_name);
									//种类页 - 获取 - 种类页索引
		int get_classify_Index(QString classify_name);
									//种类页 - 获取 - 数量
		int get_classify_Count();
									//种类页 - 获取 - 判断种类存在
		bool has_classify(QString classify_name);
									//种类页 - 获取 - 全部种类
		QStringList getAll_classify_NameList();
	protected:
									//私有 - 检查列表
		void checkClassifyList();
		
	//-----------------------------------
	//----类属性
	public:
									//实体类 -> QJsonObject
		virtual QJsonObject getJsonObject() override;
									//QJsonObject -> 实体类
									//		【说明】：用子类对象：P_FlexibleClassificationTree赋值parent_obj；
									//				  只有该树才有 工厂 创建种类对象。
		virtual void setJsonObject(QJsonObject obj, P_FlexiblePageTree* parent_obj) override;
};