#ifndef W_ALE_CellDataGlobal_H
#define W_ALE_CellDataGlobal_H
#include "stdafx.h"

#include <QDialog>
#include "../C_ALE_DataSet.h"
#include "ui_W_ALE_CellDataGlobal.h"

/*
-----==========================================================-----
		类：		编辑数据-全局 窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑数据-全局 的配置窗口。
					（详细见cpp）
-----==========================================================-----
*/
class W_ALE_CellDataGlobal : public QDialog
{
	Q_OBJECT

	public:
		W_ALE_CellDataGlobal(QWidget *parent = 0 );
		~W_ALE_CellDataGlobal();
		
	//-----------------------------------
	//----控件
	public slots:
										//控件 - 回车过滤
		void keyPressEvent(QKeyEvent *event);
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
		int m_defaultInterval;				//默认间隔值
		int m_defaultInterval_org;			//默认间隔值（编辑前）
		C_ALE_DataSet::DATA_UNIT m_unit;	//单位
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(int defaultInterval, C_ALE_DataSet::DATA_UNIT unit);
										//窗口 - 取出数据（默认间隔值）
		int getData_DefaultInterval();
										//窗口 - 取出数据（单位）
		C_ALE_DataSet::DATA_UNIT getData_Unit();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ALE_CellDataGlobal ui;
	//-----------------------------------
};

#endif // W_ALE_CellDataGlobal_H
