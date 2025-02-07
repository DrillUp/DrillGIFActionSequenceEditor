#ifndef W_FCT_ClassifySelector_H
#define W_FCT_ClassifySelector_H
#include "stdafx.h"

#include "ui_W_FCT_ClassifySelector.h"
#include "C_FCT_Classify.h"
#include "../P_FlexibleClassificationTree.h"

/*
-----==========================================================-----
		类：		种类 选择窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		种类的设置内容在该窗口设置。
-----==========================================================-----
*/

class W_FCT_ClassifySelector : public QDialog
{
	Q_OBJECT

	public:
		W_FCT_ClassifySelector(P_FlexibleClassificationTree* p_obj);
		~W_FCT_ClassifySelector();
		
	//-----------------------------------
	//----控件
	protected:
		P_FlexibleClassificationTree* m_parentObj;
	public:
										//控件 - 添加类型
		void addClassifyInAction();
										//控件 - 刷新列表
		void refreshTable();

	//-----------------------------------
	//----窗口
	protected:
		C_FCT_Config* m_configPtr = nullptr;
	public:
										//窗口 - 设置数据（修改）
		void setData(C_FCT_Config* config, QString last_selectedName = "");
										//窗口 - 取出数据
		QString getSelectedData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_FCT_ClassifySelector ui;
	//-----------------------------------
};

#endif // W_FCT_ClassifySelector_H
