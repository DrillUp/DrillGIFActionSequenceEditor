#pragma once
#include "stdafx.h"
#include "Source/Utils/WidgetForm/PictureSelector/P_PiS_Selector.h"
#include "C_PLE_Config.h"
#include "C_PLE_DataSet.h"
#include <QKeyEvent>

/*
-----==========================================================-----
		类：		图片编辑块.cpp
		版本：		v1.10
		作者：		drill_up
		所属模块：	工具模块
		功能：		将图片全部显示，能多选、编辑图片。（PLE全称：Picture_List_Editor）
					【底层基于QListWidget的item和widget，与QGraphics无关】
					【编辑块固定为 多选 】
					（详细见cpp）
-----==========================================================-----
*/

class P_PLE_Editor : public P_PiS_Selector
{
	Q_OBJECT

	public:
		P_PLE_Editor(QListWidget *parent);		//构造函数
		~P_PLE_Editor();						//析构函数
		

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
										//		【说明】：此功能转移到 setData_PLE 中执行。
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
		QString m_dataSet_parentDir;		//资源父路径（全局数据）
	public:
											//块数据 - 设置数据
		virtual void setDataSet_PLE(C_PLE_DataSet data);
											//块数据 - 获取数据
		virtual C_PLE_DataSet getDataSet_PLE();
											//块数据 - 获取当前数据 - 图片列表
											//		【说明】：该函数指在 编辑图片时，获取到的临时数据。
		QStringList getCurrentData_PicList();


	//-----------------------------------
	//----块配置
	protected:
		C_PLE_Config m_config_PLE;
	public:
											//块配置 - 设置参数
		virtual void setConfigParam_PLE(C_PLE_Config config);
											//块配置 - 取出参数
		virtual C_PLE_Config getConfigParam_PLE();
	protected:
											//块配置 - 设置参数（不开放）
		virtual void setConfigParam(C_PiS_Config config) override;
											//块配置 - 取出参数（不开放）
		virtual C_PiS_Config getConfigParam() override;
	public slots:
											//块配置 - 窗口编辑ui设置
		void openWindow_setConfigParam_PLE();
		

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
		
		
	//-----------------------------------
	//----基函数（数据交换）
	protected:
									//基函数（数据交换） - 一对一交换
		void base_swap(int index_a, int index_b);
									//基函数（数据交换） - 取出多个然后放在指定位置
		void base_swapList(int org_index, QList<int> org_index_list);
	public slots:
									//action - 左移
		void action_moveLeft();
									//action - 右移
		void action_moveRight();
									//action - 刷新编号
		void action_refreshCellTitle();
									//action - 是否需要刷新编号
		bool action_isCellTitleNeedRefresh();


	//-----------------------------------
	//----基函数（添加/删除）
	protected:
									//基函数（添加/删除） - 添加（append）
		void base_append(QList<C_PictureBlockDataPtr> cellData_list);
									//基函数（添加/删除） - 添加（insert）
									//		【说明】：由于insertItem函数有问题，这里采用 append + swapList 的方法实现。
		void base_insert(int index, QList<C_PictureBlockDataPtr> cellData_list);
									//基函数（添加/删除） - 移除
		void base_remove(QList<int> index_list);
	public:
	signals:
									//信号 - 元胞变化
		void signal_cellChanged();
	public:
	signals:
									//信号 - 元胞全部被删除
		void signal_allCellDeleted();

		   
	//-----------------------------------
	//----导入图片
	public slots:
									//action - 添加图片
		void action_append();
		void action_insert();
									//action - 删除图片
		void action_remove();
	protected:
									//导入图片 - 选择文件（可选多张）
		QStringList importPic_openQFileDialog();
									//导入图片 - 复制文件到文件夹
									//		【说明】：此过程会去掉一些已选择的文件，注意返回值。
		virtual QStringList importPic_addToParentDir(QStringList file_list);
									//导入图片 - 文件 转 图片单块
									//		【说明】：该函数调用了 工厂函数 来创建图片单块，子类可以不用继承。
		QList<C_PictureBlockDataPtr> importPic_getCellListByFileList(QStringList file_list);
	public:
	signals:
									//导入图片 - 文件夹变化（信号）
									//		【说明】：导入图片后，文件夹变化的信号。如果要更精确的文件名变化，可以去使用S_PLE_FileManager的信号。
		void signal_picListChanged();
		
	//-----------------------------------
	//----导出图片
	//		（导出图片的功能本质上只是复制图片）
	protected:
		QString m_customExportName;
	public:
									//导出图片 - 设置自定义名称
		void setCustomExportName(QString name);
		QString getCustomExportName();
	public slots:
									//action - 导出图片（单张）
		void action_exportPic_Single();
									//action - 导出图片（多张）
		void action_exportPic_Multiple();
									//action - 导出图片（全部）
		void action_exportPic_All();
	protected:
									//导出图片 - 选择文件（单张导出）
		QString exportPic_openQFileDialog_File(QString defaultFile_name = "图片");
									//导出图片 - 选择文件夹（多张导出）
		QString exportPic_openQFileDialog_Dir();
		
		
	//-----------------------------------
	//----基函数（复制/粘贴）
	protected:
									//基函数（复制/粘贴） - 复制
		virtual void base_copy(QList<int> index_list);
									//基函数（复制/粘贴） - 粘贴
		virtual void base_paste(int index);
	public:
									//action - 复制
		void action_copy();
									//action - 粘贴
		void action_paste();
									//action - 是否可以粘贴
		virtual bool action_isEnablePaste_PLE();

		
		
	//-----------------------------------
	//----快捷键
	public:
									//快捷键 - 事件绑定
									//		【说明】：快捷键无法绑定到父控件，此函数需要通过 父类执行 绑定事件。触发快捷键成功则返回true。
		bool event_shortcut_keyPress(QKeyEvent *event);
	protected:
									//快捷键 - 全选
		void shortcut_selectAll();
									//快捷键 - 复制
		void shortcut_copy();
									//快捷键 - 粘贴
		void shortcut_paste();
									//快捷键 - 删除
		void shortcut_delete();

	
};
