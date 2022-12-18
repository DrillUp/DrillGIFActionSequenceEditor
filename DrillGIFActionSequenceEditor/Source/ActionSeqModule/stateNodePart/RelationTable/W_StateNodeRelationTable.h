#ifndef W_StateNodeRelationTable_H
#define W_StateNodeRelationTable_H

#include "ui_W_StateNodeRelationTable.h"
#include "../StateTreeNode/C_StateTreeNode.h"
#include "../StateTreeNode/C_StateTreeNodeFactory.h"

/*
-----==========================================================-----
		类：		状态节点关系表.h
		作者：		drill_up
		所属模块：	动画序列模块
		功能：		编辑 最大值 窗口的一些配置。
					（详细见cpp）
-----==========================================================-----
*/
class W_StateNodeRelationTable : public QDialog
{
	Q_OBJECT

	public:
		W_StateNodeRelationTable(QWidget *parent = 0 );
		~W_StateNodeRelationTable();
		
	//-----------------------------------
	//----控件
	public:
										//控件 - 刷新表格
		void refreshTable();
										//控件 - 设置表格列
		void setTableColumnCount(int count);
										//控件 - 设置状态节点
		QWidget* createItemByNodeData(C_StateTreeNodePtr node);
		

	//-----------------------------------
	//----数据
	protected:
		QList<QJsonObject> m_availableStateDataList;
		QList<QJsonObject> m_availableStateNodeDataList;
		C_StateTreeNodeFactoryPtr m_C_StateTreeNodeFactory;	//此树容器 在建立窗口时，临时使用，关闭窗口后销毁。
	public:
										//数据 - 构建状态节点图
		void rebuildNodeFactory();
										//数据 - 递归添加节点
		void createNode_Recursion(C_StateTreeNodePtr node, int layer_deep);
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
		Ui::W_StateNodeRelationTable ui;
	//-----------------------------------
};

#endif // W_StateNodeRelationTable_H
