#pragma once
#include "stdafx.h"

#include <QKeyEvent>
#include <QListWidget>

#include "C_PiSConfig.h"

/*
-----==========================================================-----
		类：		单选/多选 图片列.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		将图片全部显示，并能单选/多选。
					（详细见cpp）
-----==========================================================-----
*/
class P_PictureSelector : public QObject
{
	Q_OBJECT

	public:
		P_PictureSelector(QListWidget *parent);		//构造函数
		~P_PictureSelector();						//析构函数
		
	//-----------------------------------
	//----控件
	protected:
		QListWidget* m_listWidget;					//图片列对象
		QString m_listWidgetStyle;					//图片列默认样式
		QList<QListWidgetItem*> m_itemTank;			//项列表
		QList<QWidget*> m_widgetTank;				//控件列表【必须要存，不然取出的Widget会自动销毁】
		int m_last_index;							//上一个选中的索引项
	public:
									//控件 - 重建图片列（如果图片多，不建议反复调用）
		void rebuildListUi();
									//控件 - 建立图片项
		virtual QListWidgetItem* createPictureItem();
									//控件 - 建立图片控件
		virtual QWidget* createPictureWidget(int i, QPixmap pixmap);
									//控件 - 清理全部
		virtual void clearAll();

	signals:
									//控件 - 选中选项变化（信号）
		void currentIndexChanged(int index);
									//控件 - 选中选项变化（多选时，信号）
		void selectedIndexChanged_Multi(QList<int> index);

									//控件 - 贴图bitmap变化（信号）
									//		【说明】：QPixmap有QPixmapCache缓存机制，其它控件可以反复读取QPixmap资源。
		void animBitmapChanged();

	//-----------------------------------
	//----事件
	protected:
		bool m_selectionSignalBlock_Root;	//圈选 锁
	protected slots:
											//事件 - 右键事件
		void itemRightClicked(QPoint point);
											//事件 - 右键事件（零个、单个、多个）
		virtual void event_itemRightClicked(QList<QListWidgetItem*> item_list);

											//事件 - 圈选变化事件
		void itemSelectionChanged();
											//事件 - 圈选变化事件（单个、多个）
		virtual void event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list);
		
	//-----------------------------------
	//----图片列设置
	protected:
		C_PiSConfig m_config;			//图片列配置项
	public:
										//图片列设置 - 设置参数
		virtual void setConfigParam(C_PiSConfig config);
										//图片列设置 - 取出参数
		virtual C_PiSConfig getConfigParam();

	//-----------------------------------
	//----资源数据
	protected:
		QList<QPixmap> m_org_bitmapList;		//资源数据
	public:
									//资源数据 - 设置数据
		virtual void setSource(QList<QFileInfo> file_list);
									//资源数据 - 设置数据
		virtual void setSource(QList<QPixmap> bitmap_list);

	//-----------------------------------
	//----选中
	public slots:
									//选中 - 设置选中
									//		【参数】：数字，可以随意越界，越界后会选择最接近的一项。
									//		【说明】：多选也可以用，将会保持只有一个被选中。
		void selectIndex(int index);
									//选中 - 设置选中（多选时）
									//		【参数】：数字，可以随意越界，越界的数字没有效果。
									//		【说明】：空数组可以取消所有选中。
		void selectIndex_Multi(QList<int> index_list);

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
		int getSelectedIndex();
									//选中 - 获取选中数据（多选时）
									//		【说明】：注意，可能出现空数组情况。
		QList<int> getSelectedIndex_Multi();

};
