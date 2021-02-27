#pragma once
#include "Source/Utils/widgetForm/pictureSelector/p_PictureSelector.h"
#include "c_ALEConfig.h"
#include "c_ALEData.h"
#include <QKeyEvent>

/*
-----==========================================================-----
		类：		动画帧编辑块.cpp
		版本：		v1.02
		作者：		drill_up
		所属模块：	工具模块
		功能：		将图片全部显示，并能单选/多选。
					（详细见cpp）
-----==========================================================-----
*/
class P_AnimationListEditor : public P_PictureSelector
{
	Q_OBJECT

	public:
		P_AnimationListEditor(QListWidget *parent);		//构造函数
		~P_AnimationListEditor();						//析构函数
		
	//-----------------------------------
	//----控件
	protected:
		QString m_iconSrcPath;
	public:
									//控件 - 每项建立控件（覆写）
		virtual QWidget* createPictureWidget(int i, QPixmap pixmap);
									//控件 - 清理全部
		virtual void clearAll();

	signals:
									//控件 - 全部帧被删除（信号）
		void allFrameDeleted();
		
	//-----------------------------------
	//----动画帧设置
	protected:
		C_ALEConfig m_config_ALE;
	public:
		void setConfigParam_ALE(C_ALEConfig config);
											//动画帧设置 - 取出参数
		C_ALEConfig getConfigParam_ALE();
	protected:
											//动画帧设置 - 设置参数（不开放）
		virtual void setConfigParam(C_PiSConfig config);
											//动画帧设置 - 取出参数（不开放）
		virtual C_PiSConfig getConfigParam();
	public slots:
											//动画帧设置 - 窗口编辑ui设置
		void openWindow_setConfigParam();
		
	//-----------------------------------
	//----事件
	protected:
											//事件 - 右键事件（零个、单个、多个）
		virtual void event_itemRightClicked(QList<QListWidgetItem*> item_list);
											//事件 - 圈选变化事件（单个、多个）
		virtual void event_itemSelectionChanged(QList<QListWidgetItem*> selected_item_list);


	//-----------------------------------
	//----资源数据
	protected:
		C_ALEData m_data;						//配置
	protected:
									//资源数据 - 设置数据（不开放）
		virtual void setSource(QList<QFileInfo> file_list);
									//资源数据 - 设置数据（不开放）
		virtual void setSource(QList<QPixmap> bitmap_list);
	public:
									//资源数据 - 设置数据（注意，数据中的id要赋值）
		void setSource(C_ALEData data);
									//资源数据 - 获取数据
		C_ALEData getSource();
		
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
									//action - 删除帧（单个和多个）
		void op_removeInAction();
									//action - 全选
		void op_selectAllInAction();
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

		
	//-----------------------------------
	//----编辑窗口
	public:
									//编辑窗口 - 选择多张图片
		QStringList openWindow_getPicFileList();
									//编辑窗口 - 选择GIF
		QString openWindow_getGIFFile();
	
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
