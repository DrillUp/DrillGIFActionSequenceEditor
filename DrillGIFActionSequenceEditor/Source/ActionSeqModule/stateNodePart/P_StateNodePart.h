#ifndef P_StateNodePart_H
#define P_StateNodePart_H

#include <QtWidgets>
#include "ui_P_StateNodePart.h"

#include "Source/Utils/widgetForm/radioTable/p_RadioTable.h"

/*
-----==========================================================-----
		类：		状态元块.h
		作者：		drill_up
		所属模块：	状态元模块
		功能：		该部分提供状态元编辑功能。
					（详细见cpp）
-----==========================================================-----
*/
class P_StateNodePart : public QWidget
{
	Q_OBJECT

	public:
		P_StateNodePart(QWidget *parent = 0);
		~P_StateNodePart();
		
	//-----------------------------------
	//----控件
	protected:
		QStringList m_curTagTank;
	public slots:
									//控件 - 修改标签列表
		void btn_editTagTank();
									//控件 - 刷新
		void refreshTagTank();
		
	//-----------------------------------
	//----快捷键
	protected:
		QJsonObject m_copyed_data;
	protected:
									//操作 - 替换
		void op_replace(int index, QJsonObject state);
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
									//本地索引 - 获取全部状态元名称
		QStringList getNameList();
									//本地索引 - 状态元切换
		void currentIndexChanged(int index);
									//本地索引 - 保存本地数据
		void local_saveCurIndexData();
									//本地索引 - 读取本地数据
		void local_loadIndexData(int index);

	//-----------------------------------
	//----窗口
	public:
		bool m_slotBlock_source;
		QList<QJsonObject> m_stateNodeDataList;
	public:
										//窗口 - 设置数据
										//		【说明】：动作元只编辑数组元素，不改变数组长度。
		void setData(QList<QJsonObject> stateNodeDataList);
										//窗口 - 取出数据
										//		【说明】：外部接收后，还需要手动打包内容。
		QList<QJsonObject> getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	private:
		Ui::P_StateNodePart ui;

};

#endif // P_StateNodePart_H
