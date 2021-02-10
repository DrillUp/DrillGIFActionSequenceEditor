#pragma once

#include <QKeyEvent>
#include <QTableWidget>

#include "C_RaTConfig.h"

/*
-----==========================================================-----
		类：		单选表格.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		将数据全部显示，并能单选。（不含排序功能）
					（详细见cpp）
-----==========================================================-----
*/
class P_RadioTable : public QObject
{
	Q_OBJECT

	public:
		P_RadioTable(QTableWidget *parent);		//构造函数
		~P_RadioTable();						//析构函数
		
	//-----------------------------------
	//----控件
	protected:
		QTableWidget* m_table;						//表格对象
		QString m_tableStyle;						//表格默认样式
	public:
									//控件 - 刷新树
		void refreshTableUi();
									//控件 - 清理全部
		void clearAll();
	signals:
									//控件 - 选中选项变化
		void currentIndexChanged(int index);
									//控件 - 选中文本变化
		void currentTextChanged(QString text);
		
	//-----------------------------------
	//----数据（表格设置）
	protected:
		C_RaTConfig m_config;	//表格设置
	public:
										//表格设置 - 设置参数
		void setConfigParam(QJsonObject config);
										//表格设置 - 取出参数
		QJsonObject getConfigParam();
	protected slots:
										//表格设置 - 编辑窗口
		void openConfigParamWindow();

	//-----------------------------------
	//----表格事件
	public:
		bool m_selectionSignalBlock_Root;	//圈选 锁
	public slots:
											//右键事件
		void sltItemRightClicked(QPoint point);
											//圈选变化事件
		void sltItemSelectionChanged();

	//-----------------------------------
	//----资源数据
	protected:
		QStringList local_text;		//资源数据
	public:
									//资源数据 - 设置数据
		void setSource(QStringList text_list);
									//资源数据 - 取出数据
		int getSelectedIndex();
		QString getSelectedText();

};
