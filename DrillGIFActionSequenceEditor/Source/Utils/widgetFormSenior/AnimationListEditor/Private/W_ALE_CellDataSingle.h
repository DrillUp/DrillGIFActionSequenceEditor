#ifndef W_ALE_CellDataSingle_H
#define W_ALE_CellDataSingle_H
#include "stdafx.h"

#include <QDialog>
#include "Source/Utils/WidgetFormSenior/AnimationBlock/C_AnimationBlockData.h"
#include "Source/Utils/WidgetFormSenior/AnimationListEditor/C_ALE_DataSet.h"
#include "ui_W_ALE_CellDataSingle.h"

/*
-----==========================================================-----
		类：		编辑数据-单选 窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑数据-单选 的配置窗口。
					（详细见cpp）
-----==========================================================-----
*/
class W_ALE_CellDataSingle : public QDialog
{
	Q_OBJECT

	public:
		W_ALE_CellDataSingle(QWidget *parent = 0 );
		~W_ALE_CellDataSingle();
		
	//-----------------------------------
	//----控件
	public slots:
										//控件 - 回车过滤
		void keyPressEvent(QKeyEvent *event);
										//控件 - 选择文件（修改当前帧资源）
		void chooseFile();
										//控件 - 打开动画帧文件夹
		void openParentDir();

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
		C_AnimationBlockDataPtr m_data;
		C_ALE_DataSet::DATA_UNIT m_unit;	//单位（只读）
		QString m_parentDir;				//资源父路径（只读）
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_AnimationBlockDataPtr data, C_ALE_DataSet::DATA_UNIT unit, QString parent_dir);
										//窗口 - 取出数据
		C_AnimationBlockDataPtr getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ALE_CellDataSingle ui;
	//-----------------------------------
};

#endif // W_ALE_CellDataSingle_H
