#pragma once
#include "stdafx.h"

#include <QKeyEvent>
#include <QTableWidget>

#include "C_RaTConfig.h"

/*
-----==========================================================-----
		类：		单选表格.cpp
		版本：		v1.02
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
		QList<QTableWidgetItem*> m_itemTank;		//表格项列表
	public:
									//控件 - 刷新表格
		void refreshTableUi();
									//控件 - 清理全部
		void clearAll();
									//控件 - 表格项数量
		int count();
									//控件 - 修改指定位置文本（不发textchange信号）
		void modifyText(int index, QString text);
									//控件 - 修改选中项文本（不发textchange信号）
		void modifyText_Selected( QString text );
									//控件 - 获取名称
		QString getTextByIndex(int index);
		QStringList getTextByIndex(QList<int> index_list);

	protected:
									//控件 - 获取文本
		QString getRealText(QTableWidgetItem* item);
									//控件 - 刷新 项 自身文本
		void refreshItem(QTableWidgetItem* item);
		
	//-----------------------------------
	//----数据（表格设置）
	protected:
		C_RaTConfig m_config;	//表格设置
	public:
										//表格设置 - 设置参数
		void setConfigParam(C_RaTConfig config);
										//表格设置 - 取出参数
		C_RaTConfig setConfigParam();
										//表格设置 - 设置参数
		void setConfigParam_obj(QJsonObject config);
										//表格设置 - 取出参数
		QJsonObject getConfigParam_obj();
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
									//资源数据 - 取出数据（多选）
		QList<int> getSelectedIndex_Multi();
		QList<QString> getSelectedText_Multi();

		
	//-----------------------------------
	//----选中
	protected:
		int m_last_index;			//上一个选中项
	public slots:
									//选中 - 设置选中
									//		【参数】：数字，可以随意越界，越界后会选择最接近的一项。
		void selectIndex(int index);
									//选中 - 设置选中
									//		【参数】：字符串，多个相同字符串，只选第一个。没有匹配的则无效。
		void selectText(QString text);
									//选中 - 设置选中（多选）
		void selectIndex_Multi(QList<int> index);
		void selectText_Multi(QList<QString> text);
									//选中 - 选中上一项
		void selectLast();
									//选中 - 选中下一项
		void selectNext();
									//选中 - 选中首项
		void selectStart();
									//选中 - 选中尾项
		void selectEnd();
	signals:
									//选中 - 选中选项变化
		void currentIndexChanged(int index);
									//选中 - 选中文本变化
		void currentTextChanged(QString text);
									//选中 - 选中选项变化（多选）
		void currentIndexChanged_Multi(QList<int> index_list);

		
	//-----------------------------------
	//----右键菜单
	protected:
		bool m_itemOuterControlEnabled;			//菜单编辑 - 开关
		bool m_itemOuterControl_CopyActive;		//菜单编辑 - 复制激活
		bool m_itemOuterControl_PasteActive;	//菜单编辑 - 粘贴激活
		bool m_itemOuterControl_ClearActive;	//菜单编辑 - 清空激活
	signals:
									//接口 - 复制按下（信号）
		void menuCopyItemTriggered(int index);
									//接口 - 粘贴按下（信号）
		void menuPasteItemTriggered(int index);
									//接口 - 清空按下（信号）
		void menuClearItemTriggered(int index);
	public slots:
									//接口 - 开关（默认开启）
		void setItemOuterControlEnabled(bool enabled);
									//接口 - 激活控制
		void setItemOuterControl_CopyActive(bool enabled);
		void setItemOuterControl_PasteActive(bool enabled);
		void setItemOuterControl_ClearActive(bool enabled);
									//接口 - 复制按下
		void menuCopyItemInAction();
									//接口 - 粘贴按下
		void menuPasteItemInAction();
									//接口 - 清空按下
		void menuClearItemInAction();


};
