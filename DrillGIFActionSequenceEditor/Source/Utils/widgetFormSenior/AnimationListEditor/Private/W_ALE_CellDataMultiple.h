#ifndef W_ALE_CellDataMultiple_H
#define W_ALE_CellDataMultiple_H
#include "stdafx.h"

#include <QDialog>
#include "Source/Utils/WidgetFormSenior/AnimationBlock/C_AnimationBlockData.h"
#include "Source/Utils/WidgetFormSenior/AnimationListEditor/C_ALE_DataSet.h"
#include "ui_W_ALE_CellDataMultiple.h"

/*
-----==========================================================-----
		类：		编辑数据-多选 窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑数据-多选 的配置窗口。
					（详细见cpp）
-----==========================================================-----
*/
class W_ALE_CellDataMultiple : public QDialog
{
	Q_OBJECT

	public:
		W_ALE_CellDataMultiple(QWidget *parent = 0 );
		~W_ALE_CellDataMultiple();
		
	//-----------------------------------
	//----控件		
	public slots:
										//控件 - 回车过滤
		void keyPressEvent(QKeyEvent *event);
										//控件 - 打开动画帧文件夹
		void openParentDir();
										//控件 - 刷新表格
		void refreshTable();

										//控件 - 选择数字（帧单位）
		void timeSelected_frameUnit(QString text);
										//控件 - 编辑数字（帧单位）
		void timeEdited_frameUnit(int value);
										//控件 - 选择数字（秒单位）
		void timeSelected_secondUnit(QString text);
										//控件 - 编辑数字（秒单位）
		void timeEdited_secondUnit(double value);

	//-----------------------------------
	//----窗口
	protected:
		bool m_slotBlock;
		QList<C_AnimationBlockDataPtr> m_dataList;
		C_ALE_DataSet::DATA_UNIT m_unit;	//单位（只读）
		QString m_parentDir;				//资源父路径（只读）
		int m_defaultInterval;				//默认间隔值（只读）
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(QList<C_AnimationBlockDataPtr> data_list, C_ALE_DataSet::DATA_UNIT unit, QString parent_dir, int defaultInterval);
										//窗口 - 取出数据
		QList<C_AnimationBlockDataPtr> getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ALE_CellDataMultiple ui;
	//-----------------------------------
};

#endif // W_ALE_CellDataMultiple_H
