#pragma once
#include "stdafx.h"

#include <QListWidget>
#include "Private/I_PiS_Cell.h"
#include "C_PiS_Config.h"

/*
-----==========================================================-----
		类：		图片选择块.h
		版本：		v1.03
		作者：		drill_up
		所属模块：	工具模块
		功能：		能显示图片列表，并能单选/多选。（PiS全称：Picture_Selector）
					【块中以 refresh刷新 为主，少用 rebuild重建 】
					（详细见cpp）
-----==========================================================-----
*/

class P_PiS_Selector : public QObject
{
	Q_OBJECT

	public:
		P_PiS_Selector(QListWidget *parent);	//构造函数
		~P_PiS_Selector();						//析构函数
		
	//-----------------------------------
	//----工厂
	protected:
										//工厂 - 建立一个元胞数据
		virtual C_PictureBlockDataPtr createPictureCellData();
										//工厂 - 建立一个元胞
		virtual I_PiS_Cell* createPictureCell(C_PictureBlockDataPtr data);
										//工厂 - 刷新元胞
		virtual void refreshPictureCell(int index);
		
	//-----------------------------------
	//----父控件
	protected:
		QListWidget* m_parent;					//父控件
		QString m_parentStyle;					//父控件样式
		
	//-----------------------------------
	//----控件
	protected:
		QList<I_PiS_Cell*> m_cellTank;			//元胞列表
	public:
										//控件 - 重建UI
		virtual void rebuildUI(QStringList bitmapPath_list);
										//控件 - 刷新UI
										//		【说明】：此刷新包括将所有元胞重新分配编号、重新上色。
		virtual void refreshUI();
										//控件 - 获取全部资源路径
		QStringList getAllBitmapPath();
										//控件 - 清理项
		virtual void clearCells();
										//控件 - 清理全部
		virtual void clearAll();
	protected:
										//控件 - 获取元胞索引
		int getCellIndexByItem(QListWidgetItem* item);
		int getCellIndexByWidget(P_PictureBlock* widget);

	//-----------------------------------
	//----鼠标事件
	protected:
		bool m_slotBlock_selection;				//鼠标事件 锁
		int m_selection_lastIndex;				//上一个选中项
	protected slots:
										//鼠标事件 - 圈选变化事件槽
										//		【说明】：此处直接连接了 QListWidget::itemSelectionChanged，并非用了QEvent。
		void itemSelectionChanged();
										//鼠标事件 - 圈选变化事件（单个、多个）
										//		【说明】：此处由子类继承。
		virtual void event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list);

										//鼠标事件 - 右键事件槽
										//		【说明】：此处直接连接了 QListWidget::customContextMenuRequested，并非用了QEvent。
		void itemRightClicked(QPoint point);
										//鼠标事件 - 右键事件（零个、单个、多个）
										//		【说明】：此处由子类继承。
		virtual void event_itemRightClicked(QList<QListWidgetItem*> item_list);

										//右键菜单 - 零个（点击空白处）
										//		【说明】：此处由子类继承，默认返回nullptr。
		virtual QMenu* createMenu_selectNone();
										//右键菜单 - 单个
										//		【说明】：此处由子类继承，默认返回nullptr。
		virtual QMenu* createMenu_selectSingle(int pos);
										//右键菜单 - 多个
										//		【说明】：此处由子类继承，默认返回nullptr。
		virtual QMenu* createMenu_selectMultiple(QList<int> pos_list);

										//右键菜单 - menu - 修改选中
										//		【说明】：此处提供了 选中 的功能菜单。
		virtual QMenu* createMenu_changeSelection();

		
	//-----------------------------------
	//----配置类
	protected:
		C_PiS_Config m_config;					//配置类
	public:
										//配置类 - 设置配置
		virtual void setConfigParam(C_PiS_Config config);
										//配置类 - 取出配置
		virtual C_PiS_Config getConfigParam();


	//-----------------------------------
	//----选中
	public slots:
										//选中 - 设置选中（单选）
										//		【参数】：数字，可以随意越界，越界后会选择最接近的一项。
										//		【说明】：多选也可以用，将会保持只有一个被选中。
		void selectIndex_Single(int index);
										//选中 - 设置选中（多选）
										//		【参数】：数字，可以随意越界，越界的数字没有效果。
										//		【说明】：空数组可以取消所有选中。
		void selectIndex_Multiple(QList<int> index_list);
										//选中 - 设置选中+其他取消选中（多选）
										//		【参数】：数字，可以随意越界，越界的数字没有效果。
										//		【说明】：空数组可以取消所有选中。
		void selectIndex_MultipleWithOtherUnselect(QList<int> index_list);

										//选中 - 选中上一项
		void selectLast();
										//选中 - 选中下一项
		void selectNext();
										//选中 - 选中首项
		void selectStart();
										//选中 - 选中尾项
		void selectEnd();

	public:
										//选中 - 获取选中数据
										//		【说明】：-1，表示全部未选；多选时，表示第一个选中的索引。
		int getSelectedIndex_Single();
										//选中 - 获取选中数据（多选时）
										//		【说明】：注意，可能出现空数组情况。
		QList<int> getSelectedIndex_Multiple();

	public:
	signals:
										//选中 - 选中选项变化（信号）
		void signal_currentIndexChanged_Single(int index);
	public:
	signals:
										//选中 - 选中选项变化（多选时，信号）
		void signal_selectedIndexChanged_Multiple(QList<int> index);
		
	public:
										//选中（多选） - action - 全选
		void action_selectAll();
										//选中（多选） - action - 选择左边全部
		void action_selectLeftAll();
										//选中（多选） - action - 选择右边全部
		void action_selectRightAll();
										//选中（多选） - action - 选奇数
		void action_selectOdd();
										//选中（多选） - action - 选偶数
		void action_selectEven();


};
