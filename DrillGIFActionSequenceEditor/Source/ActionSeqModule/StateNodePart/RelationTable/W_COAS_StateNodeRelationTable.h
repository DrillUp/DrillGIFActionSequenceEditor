#ifndef W_COAS_StateNodeRelationTable_H
#define W_COAS_StateNodeRelationTable_H

#include "ui_W_COAS_StateNodeRelationTable.h"
#include "../C_COAS_StateNode.h"
#include "../C_COAS_StateNodeFactory.h"

/*
-----==========================================================-----
		类：		状态节点关系表.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		根据输入的状态元和状态节点数据，生成一张关系表。
					【通过 状态节点_树图 的方式，即时构建，即时生成大表】
					（详细见cpp）
-----==========================================================-----
*/
class W_COAS_StateNodeRelationTable : public QDialog
{
	Q_OBJECT

	public:
		W_COAS_StateNodeRelationTable(QWidget *parent = 0 );
		~W_COAS_StateNodeRelationTable();
		
	//-----------------------------------
	//----控件
	public:
										//控件 - 刷新表格
		void refreshTable();
										//控件 - 设置表格列
		void setTableColumnCount(int count);
										//控件 - 设置状态节点
		QWidget* createItemByNodeData(C_COAS_StateNodePtr node);
		

	//-----------------------------------
	//----数据
	protected:
		QList<QJsonObject> m_availableStateDataList;			//非空状态元数据
		QList<QJsonObject> m_availableStateNodeDataList;		//非空状态节点数据
		C_COAS_StateNodeFactoryPtr m_C_COAS_StateNodeFactory;	//树图容器（此容器 在建立窗口时，临时使用，关闭窗口后销毁）
		bool m_errorBlock;										//出错时中止锁
	public:
										//数据 - 构建状态节点图
		void rebuildNodeFactory();
										//数据 - 递归添加节点
		void createNode_Recursion(C_COAS_StateNodePtr node, int layer_deep);
										//数据 - 获取状态元
		QJsonObject getStateByName(QString name);
		QList<QJsonObject> getStateByNameList(QStringList name_list);
										//数据 - 获取状态节点
		QJsonObject getStateNodeByName(QString name);
		QList<QJsonObject> getStateNodeByNameList(QStringList name_list);


	//-----------------------------------
	//----窗口
	public:
										//窗口 - 设置数据（修改）
		void setData_StateData(QList<QJsonObject> data_list);
		void setData_StateNodeData(QList<QJsonObject> data_list);
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
	private:
		Ui::W_COAS_StateNodeRelationTable ui;
	//-----------------------------------
};

#endif // W_COAS_StateNodeRelationTable_H
