#pragma once
#include "stdafx.h"

#include <QKeyEvent>
#include <QListWidget>

#include "C_PiS_Config.h"

/*
-----==========================================================-----
		类：		图片选择块.h
		版本：		v1.03
		作者：		drill_up
		所属模块：	工具模块
		功能：		能显示图片列表，并能单选/多选。（PiS全称：Picture_Selector）
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
										//工厂 - 建立一个图片控件
		virtual QWidget* createPictureWidget(int i, QPixmap pixmap);
		
	//-----------------------------------
	//----父控件
	protected:
		QListWidget* m_parent;					//父控件
		QString m_parentStyle;					//父控件样式
		
	//-----------------------------------
	//----控件
	protected:
		QList<QListWidgetItem*> m_itemTank;		//项列表
		QList<QWidget*> m_widgetTank;			//控件列表【必须要存，不然取出的Widget会自动销毁】
		int m_last_index;						//上一个选中的索引项
	public:
										//控件 - 重建UI（如果图片多，不建议反复调用）
		void rebuildUI();
										//控件 - 建立项
		virtual QListWidgetItem* createPictureItem();
										//控件 - 清理项
		virtual void clearItems();
										//控件 - 清理全部
		virtual void clearAll();

	//-----------------------------------
	//----鼠标事件
	protected:
		bool m_slotBlock_selection;
	protected slots:
										//鼠标事件 - 右键事件
										//		【说明】：此处直接连接了 QListWidget::customContextMenuRequested，并非用了QEvent。
		void itemRightClicked(QPoint point);
										//鼠标事件 - 右键事件（零个、单个、多个）
										//		【说明】：此处由子类继承。
		virtual void event_itemRightClicked(QList<QListWidgetItem*> item_list);

										//鼠标事件 - 圈选变化事件
										//		【说明】：此处直接连接了 QListWidget::itemSelectionChanged，并非用了QEvent。
		void itemSelectionChanged();
										//鼠标事件 - 圈选变化事件（单个、多个）
										//		【说明】：此处由子类继承。
		virtual void event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list);
		
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
	//----资源数据
	protected:
		QList<QPixmap> m_org_bitmapList;		//资源贴图
	public:
										//资源数据 - 设置贴图
										//		【说明】：此处只提供单一的入口，因为该控件只绘制bitmap到图片单块，不对资源作多余操作。
		virtual void setSourceBitmap(QList<QPixmap> bitmap_list);
										//资源数据 - 获取贴图
		QList<QPixmap> getSourceBitmap();
	public:
	signals:
										//资源数据 - 贴图变化（信号）
										//		【说明】：QPixmap有QPixmapCache缓存机制，其它控件可以反复读取QPixmap资源。
		void signal_SourceBitmapChanged();

	//-----------------------------------
	//----选中
	public slots:
										//选中 - 设置选中
										//		【参数】：数字，可以随意越界，越界后会选择最接近的一项。
										//		【说明】：多选也可以用，将会保持只有一个被选中。
		void selectIndex_Single(int index);
										//选中 - 设置选中（多选时）
										//		【参数】：数字，可以随意越界，越界的数字没有效果。
										//		【说明】：空数组可以取消所有选中。
		void selectIndex_Multiple(QList<int> index_list);

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


};
