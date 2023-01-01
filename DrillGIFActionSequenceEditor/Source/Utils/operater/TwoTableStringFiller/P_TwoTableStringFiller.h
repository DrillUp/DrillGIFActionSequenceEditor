#pragma once
#include "stdafx.h"
#include <QTableWidget>

/*
-----==========================================================-----
		类：		双表格字符串填写器.cpp
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		可以将左表格的内容填到右表格。
					【只操作字符串，左表格字符串可以重复，右表格可以选重复或不重复】
					（详细见cpp）
-----==========================================================-----
*/
class P_TwoTableStringFiller : public QObject
{
	Q_OBJECT

	public:
		P_TwoTableStringFiller(QTableWidget *parent_left, QTableWidget *parent_right);		//构造函数
		~P_TwoTableStringFiller();															//析构函数
		
	//-----------------------------------
	//----控件
	protected:
		QTableWidget* m_leftTable;			//左表格
		QTableWidget* m_rightTable;			//右表格
	public slots:
									//控件 - 刷新表格
		void refreshTable();
		void refreshTable_Left();
		void refreshTable_Right();
									//控件 - 设置左表格标题
		void setLeftTitle(QString title);
									//控件 - 设置右表格标题
		void setRightTitle(QString title);
									//控件 - 显示左表格 首行序号
		void setLeftRowHeaderVisible(bool visible);
									//控件 - 显示右表格 首行序号
		void setRightRowHeaderVisible(bool visible);


	//-----------------------------------
	//----选中
	protected:
		bool m_slotBlock;
	public slots:
									//选中 - 移动到左表格
		void moveSelectedToLeft();
									//选中 - 移动到右表格
		void moveSelectedToRight();
									//选中 - 左表格 清除选中
		void clearSelectionLeft();
									//选中 - 右表格 清除选中
		void clearSelectionRight();

		
	//-----------------------------------
	//----数据
	protected:
		QStringList m_leftTank;			//左边字符串
		QStringList m_rightTank;		//右边字符串
	public:
									//数据 - 设置左边数据
		void setLeftString(QStringList str_list);
									//数据 - 设置右边数据
		void setRightString(QStringList str_list);
									//数据 - 获取左边数据
		QStringList getLeftString();
									//数据 - 获取右边数据
		QStringList getRightString();
									//数据 - 清理全部
		void clearAll();
									//数据 - 右边数据上移
		void moveUpRightString();
									//数据 - 右边数据下移
		void moveDownRightString();
		

	//-----------------------------------
	//----快捷键
	protected:
		QString m_copyed_data;
	protected:
									//快捷键 - 事件
		bool eventFilter(QObject *target, QEvent *event);
									//快捷键 - 复制
		virtual void shortcut_copyData();
									//快捷键 - 粘贴
		virtual void shortcut_pasteData();
									//快捷键 - 删除
		void shortcut_deleteData();

};
