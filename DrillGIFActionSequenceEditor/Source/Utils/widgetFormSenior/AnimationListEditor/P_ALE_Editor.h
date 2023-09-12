﻿#pragma once
#include "stdafx.h"
#include "Source/Utils/WidgetForm/PictureSelector/P_PiS_Selector.h"
#include "C_ALE_Config.h"
#include "C_ALE_DataSet.h"
#include <QKeyEvent>

/*
-----==========================================================-----
		类：		动画帧编辑块.cpp
		版本：		v1.04
		作者：		drill_up
		所属模块：	工具模块
		功能：		将图片全部显示，并能单选/多选。
					（详细见cpp）
-----==========================================================-----
*/
class P_ALE_Editor : public P_PiS_Selector
{
	Q_OBJECT

	public:
		P_ALE_Editor(QListWidget *parent);		//构造函数
		~P_ALE_Editor();						//析构函数
		
	//-----------------------------------
	//----控件
	protected:
		QString m_iconSrcPath;
	public:
										//控件 - 每项建立控件（覆写）
		virtual QWidget* createPictureWidget(int i, QPixmap pixmap);
										//控件 - 清理控件
		virtual void clearItems();
	protected:
										//控件 - 清理全部
		virtual void clearAll();
		
	public:
	signals:
										//控件 - 全部帧被删除（信号）
		void signal_allFrameDeleted();

	//-----------------------------------
	//----动画帧设置
	protected:
		C_ALE_Config m_config_ALE;
	public:
		void setConfigParam_ALE(C_ALE_Config config);
											//动画帧设置 - 取出参数
		C_ALE_Config getConfigParam_ALE();
	protected:
											//动画帧设置 - 设置参数（不开放）
		virtual void setConfigParam(C_PiS_Config config);
											//动画帧设置 - 取出参数（不开放）
		virtual C_PiS_Config getConfigParam();
	public slots:
											//动画帧设置 - 窗口编辑ui设置
		void openWindow_setConfigParam();
		
	//-----------------------------------
	//----鼠标事件
	protected:
										//鼠标事件 - 右键事件（零个、单个、多个）
		virtual void event_itemRightClicked(QList<QListWidgetItem*> item_list);
										//鼠标事件 - 圈选变化事件（单个、多个）
		virtual void event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list);


	//-----------------------------------
	//----资源数据
	protected:
		C_ALE_DataSet m_data;						//配置
		C_ALE_DataSet::DATA_UNIT m_unit;			//单位
	protected:
										//资源数据 - 设置贴图（继承）（不开放）
		virtual void setSourceBitmap(QList<QPixmap> bitmap_list);
	public:
										//资源数据 - 设置数据（注意，数据中的id要赋值）
		void setSource(C_ALE_DataSet data);
										//资源数据 - 获取数据
		C_ALE_DataSet getSource();
										//资源数据 - 设置单位
										//			【说明】：数据的单位以编辑块的为准。
		void setUnit(C_ALE_DataSet::DATA_UNIT unit);
										//资源数据 - 获取单位
										//			【说明】：数据的单位以编辑块的为准。
		C_ALE_DataSet::DATA_UNIT getUnit();
		
	//-----------------------------------
	//----资源数据（操作）
	protected:
		QList<QFileInfo> m_copyedList;
	protected:
									//操作 - 添加【此处 绕开了 insertItem 的bug，代码结构会比较复杂】
		void op_append(QString gif_src);
		void op_insert(int index, QStringList gif_src_list, QList<int> interval_list = QList<int>() );
									//操作 - 移除
		void op_remove(QList<int> index_list);
									//操作 - 交换位置
		void op_swap(int index_a, int index_b);
									//操作 - 刷新贴图内容
		void op_refresh(int index);
		void op_refreshAll(int startAt = 0);
		
	public slots:
									//action - 添加帧（点击空白处）
		void op_appendInAction();
									//action - 添加帧（点击帧时）
		void op_insertInAction();
									//action - 添加帧（GIF）
		void op_insertGIFInAction();
									//action - 添加帧（序列大图）
		void op_insertSeqPicInAction();
									//action - 删除帧（单个和多个）
		void op_removeInAction();
									//action - 复制（单个和多个）
		void op_copyInAction();
									//action - 粘贴（单个和多个）
		void op_pasteInAction();
									//action - 左移
		void op_moveLeftInAction();
									//action - 右移
		void op_moveRightInAction();
									//action - 编辑帧
		void op_editOneInAction();
									//action - 编辑帧时间
		void op_editMultiInAction();
									//action - 全选
		void op_selectAllInAction();
									//action - 选奇数
		void op_selectOddInAction();
									//action - 选偶数
		void op_selectEvenInAction();
		
	//-----------------------------------
	//----资源数据（导出）
	protected:
		QString m_exportName;
	public:
									//导出 - 导出单图
		void op_exportPic(int index, QFileInfo target_file);
									//导出 - 导出多图
		void op_exportPic_Multi(QList<int> index_list, QDir target_dir);
									//导出 - 导出GIF
		void op_exportGIF_Multi(QList<int> index_list, QFileInfo target_file, int writerMethod);
	public slots:
									//导出 - 设置导出名称
		void setExportName(QString name);
									//action - 导出单图
		void op_exportSingle_PicInAction();
									//action - 导出多图
		void op_exportSelected_PicInAction();
									//action - 导出全部图
		void op_exportAll_PicInAction();
									//action - 导出GIF - 选中项
		void op_exportSelected_GIFInAction();
									//action - 导出GIF - 全部项
		void op_exportAll_GIFInAction();
									//action - 导出序列大图 - 选中项
		void op_exportSelected_SeqPicInAction();
									//action - 导出序列大图 - 全部项
		void op_exportAll_SeqPicInAction();

		
	//-----------------------------------
	//----文件选择窗口
	protected:
									//文件选择窗口 - 选择多张图片（导入）
		QStringList openQFileDialog_getPicFileList();
									//文件选择窗口 - 选择GIF（导入）
		QString openQFileDialog_getGIFFile();
									//文件选择窗口 - 选择序列大图（导入）
		QString openQFileDialog_getSeqPicFile();
									//文件选择窗口 - 选择GIF（导出）
		QString openQFileDialog_exportGIFFile();
									//文件选择窗口 - 选择png（导出）
		QString openQFileDialog_exportPNGFile(QString name_suffix = "动画帧");
									//文件选择窗口 - 选择文件夹（导出）
		QString openQFileDialog_exportDir();
	
	//-----------------------------------
	//----快捷键
	public:
									//快捷键 - 全选
		void shortcut_selectAll();
									//快捷键 - 复制
		void shortcut_copy();
									//快捷键 - 粘贴
		void shortcut_paste();
									//快捷键 - 删除
		void shortcut_delete();

};
