#pragma once
#include "stdafx.h"
#include "Source/Utils/WidgetForm/PictureListEditor/P_PLE_Editor.h"
#include "Source/Utils/WidgetFormSenior/AnimationBlock/C_AnimationBlockData.h"
#include "C_ALE_Config.h"
#include "C_ALE_DataSet.h"

#include "Private/W_ALE_GIFReader_TypeChoose.h"
#include "Private/W_ALE_GIFWriter_TypeChoose.h"
#include <QKeyEvent>

/*
-----==========================================================-----
		类：		动画帧编辑块.cpp
		版本：		v1.10
		作者：		drill_up
		所属模块：	工具模块
		功能：		将图片全部显示，能多选、编辑图片。（PLE全称：Picture_List_Editor）
					【底层基于QListWidget的item和widget，与QGraphics无关】
					【编辑块固定为 多选 】
					（详细见cpp）
-----==========================================================-----
*/

class P_ALE_Editor : public P_PLE_Editor
{
	Q_OBJECT

	public:
		P_ALE_Editor(QListWidget *parent);		//构造函数
		~P_ALE_Editor();						//析构函数
		

	//-----------------------------------
	//----工厂
	protected:
										//工厂 - 建立一个元胞数据（覆写）
		virtual C_PictureBlockDataPtr createPictureCellData() override;
										//工厂 - 建立一个元胞（覆写）
		virtual I_PiS_Cell* createPictureCell(C_PictureBlockDataPtr data) override;
										//工厂 - 刷新元胞（覆写）
		virtual void refreshPictureCell(int index) override;


	//-----------------------------------
	//----控件
	protected:
										//控件 - 重建UI（不开放）
										//		【说明】：此功能包含rebuildUI原操作。
		virtual void rebuildUI_ALE(QStringList bitmapPath_list, QList<int> interval_list);
		virtual void rebuildUI(QStringList bitmapPath_list) override;
	public:
										//控件 - 刷新UI
		virtual void refreshUI() override;
										//控件 - 清理控件
		virtual void clearCells() override;
										//控件 - 清理全部
		virtual void clearAll() override;


	//-----------------------------------
	//----块数据
	protected:
		//QString m_dataSet_parentDir;				//资源父路径（全局数据，来自父类）
		C_ALE_DataSet::DATA_UNIT m_dataSet_unit;	//单位（全局数据）
		int m_dataSet_defaultInterval;				//默认帧间隔（全局数据）
													//（列表数据都放在cell里面，全局数据才放这里）
	public:
											//块数据 - 设置数据
		virtual void setDataSet_ALE(C_ALE_DataSet data);
											//块数据 - 获取数据
											//		【说明】：该函数指在 完成动画帧后/点击提交后，返回的打包好的数据。
		virtual C_ALE_DataSet getDataSet_ALE();
											//块数据 - 获取当前数据 - 单位
											//		【说明】：该函数指在 编辑动画帧时，获取到的临时数据。
		C_ALE_DataSet::DATA_UNIT getCurrentData_Unit();
											//块数据 - 获取当前数据 - 帧列表
											//		【说明】：该函数指在 编辑动画帧时，获取到的临时数据。
		QList<int> getCurrentData_IntervalList();
	protected:
											//块数据 - 设置参数（不开放）
		virtual void setDataSet_PLE(C_PLE_DataSet data) override;
											//块数据 - 取出参数（不开放）
		virtual C_PLE_DataSet getDataSet_PLE() override;


	//-----------------------------------
	//----块配置
	protected:
		C_ALE_Config m_config_ALE;
	public:
											//块配置 - 设置参数
		virtual void setConfigParam_ALE(C_ALE_Config config);
											//块配置 - 取出参数
		virtual C_ALE_Config getConfigParam_ALE();
	protected:
											//块配置 - 设置参数（不开放）
		virtual void setConfigParam_PLE(C_PLE_Config config) override;
											//块配置 - 取出参数（不开放）
		virtual C_PLE_Config getConfigParam_PLE() override;
											//块配置 - 设置参数（不开放）
		virtual void setConfigParam(C_PiS_Config config) override;
											//块配置 - 取出参数（不开放）
		virtual C_PiS_Config getConfigParam() override;
	public slots:
											//块配置 - 窗口编辑ui设置
		void openWindow_setConfigParam_ALE();
		

	//-----------------------------------
	//----鼠标事件
	protected:
										//鼠标事件 - 圈选变化事件（单个、多个）
		virtual void event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list) override;
										//鼠标事件 - 右键事件（零个、单个、多个）
		virtual void event_itemRightClicked(QList<QListWidgetItem*> item_list) override;

										//右键菜单 - 零个（覆写）（点击空白处）
		virtual QMenu* createMenu_selectNone() override;
										//右键菜单 - 单个（覆写）
		virtual QMenu* createMenu_selectSingle(int pos) override;
										//右键菜单 - 多个（覆写）
		virtual QMenu* createMenu_selectMultiple(QList<int> pos_list) override;
		
										//右键菜单 - action - 编辑数据-单选
		void action_openWindow_CellDataSingle_ALE();
										//右键菜单 - action - 编辑数据-多选
		void action_openWindow_CellDataMultiple_ALE();
										//右键菜单 - action - 编辑数据-全局
		void action_openWindow_CellDataGlobal_ALE();

										//右键菜单 - 操作 - 修改默认帧间隔
										//		【说明】：与原默认帧间隔一样的值，会被统一改变。
										//		【说明】：此处为 改Cell 中的默认帧间隔。改数据的见C_ALE_DataSet。
		void setIntervalDefaultInAll(int gif_interval);

		
	//-----------------------------------
	//----基函数（添加/删除）
	//
	//		（暂无子类函数）


	//-----------------------------------
	//----导入图片
	protected:
									//导入图片 - 复制文件到文件夹
									//		【说明】：此过程会去掉一些已选择的文件，注意返回值。
		virtual QStringList importPic_addToParentDir(QStringList file_list) override;
		
	//-----------------------------------
	//----导出图片
	//
	//		（暂无子类函数）


	//-----------------------------------
	//----基函数（复制/粘贴）
	protected:
									//基函数（复制/粘贴） - 复制（覆写）
		virtual void base_copy(QList<int> index_list) override;
									//基函数（复制/粘贴） - 粘贴（覆写）
		virtual void base_paste(int index) override;
	protected:
									//action - 是否可以粘贴（不开放）
		virtual bool action_isEnablePaste_PLE() override;
	public:
									//action - 是否可以粘贴
		virtual bool action_isEnablePaste_ALE();
		

	//-----------------------------------
	//----快捷键
	//
	//		（暂无子类函数）



	//-----------------------------------
	//----导入GIF
	public slots:
									//action - 添加GIF
		void action_appendGIF_ALE();
		void action_insertGIF_ALE();
	protected:
									//导入GIF - 选择文件
		QString importGIF_openQFileDialog();
									//导入GIF - 文件 转 图片单块
									//		【参数1】：文件列表
									//		【参数2】：帧间隔列表
		QList<C_PictureBlockDataPtr> importGIF_getCellListByFileList(QList<QFileInfo> file_list, QList<int> interval_list);
		
	//-----------------------------------
	//----导出GIF
	public:
									//action - 导出GIF（单张）
		void action_exportGIF_Single();
									//action - 导出GIF（多张）
		void action_exportGIF_Multiple();
									//action - 导出GIF（全部）
		void action_exportGIF_All();
	protected:
									//导出GIF - 选择文件
		QString exportGIF_openQFileDialog_File(QString defaultFile_name = "动图");
									//导出GIF - 执行导出
		void exportGIF_exportToFile(QFileInfo tar_file, int writerMethod, QList<C_AnimationBlockDataPtr> cellData_list);


		
	//-----------------------------------
	//----导入序列大图
	public slots:
									//action - 添加序列大图
		void action_appendSeq_ALE();
		void action_insertSeq_ALE(); 
	protected:
									//导入序列大图 - 选择文件
		QString importSeq_openQFileDialog();
									//导入序列大图 - 文件 转 图片单块
									//		【参数】：文件列表
		QList<C_PictureBlockDataPtr> importSeq_getCellListByFileList(QStringList file_list);
		
	//-----------------------------------
	//----导出序列大图
	public:
									//action - 导出序列大图（单张）
		void action_exportSeq_Single();
									//action - 导出序列大图（多张）
		void action_exportSeq_Multiple();
									//action - 导出序列大图（全部）
		void action_exportSeq_All();
	protected:
									//导出序列大图 - 选择文件
		QString exportSeq_openQFileDialog_File(QString defaultFile_name = "序列大图");

};
