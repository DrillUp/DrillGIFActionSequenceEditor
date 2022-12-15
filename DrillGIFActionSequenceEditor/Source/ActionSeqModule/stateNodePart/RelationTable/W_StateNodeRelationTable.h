#ifndef W_StateNodeRelationTable_H
#define W_StateNodeRelationTable_H

#include "ui_W_StateNodeRelationTable.h"

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
		QWidget* createItemByNodeData(QJsonObject node_data);
		
	//-----------------------------------
	//----数据
	protected:
		QList<QJsonObject> m_availableStateDataList;
		QList<QJsonObject> m_availableStateNodeDataList;
	public:
										//数据 - 获取最大层数
		int getMaxLayerCount();

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
