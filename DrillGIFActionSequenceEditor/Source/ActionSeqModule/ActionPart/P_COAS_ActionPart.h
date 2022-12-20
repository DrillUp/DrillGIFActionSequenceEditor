#ifndef P_COAS_ActionPart_H
#define P_COAS_ActionPart_H

#include <QtWidgets>
#include "ui_P_COAS_ActionPart.h"

#include "Source/GraphModule/Widget/AnimPictureViewer/P_AnimPictureViewer.h"

#include "Source/Utils/WidgetForm/RadioTable/P_RadioTable.h"
#include "Source/Utils/WidgetFormSenior/AnimationListEditor/p_AnimationListEditor.h"
#include "Source/Utils/WidgetFormSenior/AnimationListPlayer/p_AnimationListPlayer.h"

/*
-----==========================================================-----
		类：		动作元块.h
		作者：		drill_up
		所属模块：	动作元模块
		功能：		该部分提供动作元的编辑。
					（详细见cpp）
-----==========================================================-----
*/
class P_COAS_ActionPart : public QWidget
{
	Q_OBJECT

	public:
		P_COAS_ActionPart(QWidget *parent = 0);
		~P_COAS_ActionPart();
		
	//-----------------------------------
	//----控件
	protected:
		QStringList m_curTagTank;
	public slots:
									//控件 - 修改标签列表
		void btn_editTagTank();
									//控件 - 刷新标签显示
		void refreshTagTank();

	//-----------------------------------
	//----动画帧
	public:
		P_AnimationListEditor* m_p_AnimationListEditor;
		P_AnimationListPlayer* m_p_AnimationListPlayer;
		P_AnimPictureViewer* m_p_AnimPictureViewer;
	public:
									//动画帧 - 选项变化
		void tableChanged_Multi(QList<int> index);
									//动画帧 - 资源切换
		void bitmapChanged();
									//动画帧 - 缩放比例切换
		void zoomValueChanged(double value);
		
	//-----------------------------------
	//----快捷键
	protected:
		QJsonObject m_copyed_data;
	protected:
									//操作 - 替换
		void op_replace(int index, QJsonObject actiong_seq);
									//操作 - 清空
		void op_clear(int index);
	public:
									//快捷键 - 事件
		void keyPressEvent(QKeyEvent *event);
									//快捷键 - 复制
		void shortcut_copyData();
									//快捷键 - 粘贴
		void shortcut_pasteData();
									//快捷键 - 清空
		void shortcut_clearData();
		
	//-----------------------------------
	//----本地索引
	protected:
		P_RadioTable* m_table;
		int m_last_index;
	public:
									//本地索引 - 获取全部动作元名称
		QStringList getNameList();
									//本地索引 - 动作元切换
		void currentIndexChanged(int index);
									//本地索引 - 保存本地数据
		void local_saveCurIndexData();
									//本地索引 - 读取本地数据
		void local_loadIndexData(int index);

		
	//-----------------------------------
	//----数据检查
	protected:
		QStringList m_errorMessage;
	public:
										//数据检查 - 执行检查
										//		【说明】：此函数被父控件调用，当前控件不检查。
		void checkData_ActionDataList(QList<QJsonObject> actionDataList);
										//数据检查 - 获取检查信息
		QStringList checkData_getErrorMessage();

	//-----------------------------------
	//----窗口
	public:
		bool m_slotBlock_source;
		QList<QJsonObject> m_actionDataList;
	public:
										//窗口 - 设置数据
										//		【说明】：动作元只编辑数组元素，不改变数组长度。
		void setData(QList<QJsonObject> actionDataList);
										//窗口 - 取出数据
										//		【说明】：外部接收后，还需要手动打包内容。
		QList<QJsonObject> getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	private:
		Ui::P_COAS_ActionPart ui;

};

#endif // P_COAS_ActionPart_H
