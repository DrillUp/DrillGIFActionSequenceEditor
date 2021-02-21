#ifndef P_ActionPart_H
#define P_ActionPart_H

#include <QtWidgets>
#include "ui_P_ActionPart.h"

#include "Source/Utils/widgetForm/radioTable/p_RadioTable.h"
#include "Source/Utils/widgetFormSenior/AnimationListEditor/p_AnimationListEditor.h"

/*
-----==========================================================-----
		类：		动作元块.h
		作者：		drill_up
		所属模块：	动作元模块
		功能：		该部分提供动作元的编辑。
					（详细见cpp）
-----==========================================================-----
*/
class P_ActionPart : public QWidget
{
	Q_OBJECT

	public:
		P_ActionPart(QWidget *parent = 0);
		~P_ActionPart();
		
	//-----------------------------------
	//----控件
	public:
		P_RadioTable* m_table;
	public slots:
										//控件 - 获取动作元名称
		QStringList getNameList();
										//控件 - 动作元切换
		void currentIndexChanged(int index);

	//-----------------------------------
	//----动画帧
	public:
		P_AnimationListEditor* m_p_AnimationListEditor;
	public:
									//动画帧 - 选项变化
		void tableChanged_Multi(QList<int> index);

	//-----------------------------------
	//----窗口
	public:
		QList<QJsonObject> local_actionDataList;
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
		Ui::P_ActionPart ui;

};

#endif // P_ActionPart_H
