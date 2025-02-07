#ifndef W_FCT_Classify_H
#define W_FCT_Classify_H
#include "stdafx.h"

#include "ui_W_FCT_Classify.h"
#include "C_FCT_Classify.h"
#include "../P_FlexibleClassificationTree.h"

/*
-----==========================================================-----
		类：		种类 配置窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		种类的设置内容在该窗口设置。
-----==========================================================-----
*/

class W_FCT_Classify : public QDialog
{
	Q_OBJECT

	public:
		W_FCT_Classify(P_FlexibleClassificationTree* p_tree);
		~W_FCT_Classify();
		
	//-----------------------------------
	//----父控件
	protected:
		P_FlexibleClassificationTree* m_parentObj;

	//-----------------------------------
	//----窗口
	public:
		bool isAddMode;
		C_FCT_Classify* m_dataPtr;
	public:
										//窗口 - 设置数据（添加）
		void setDataInAddMode();
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_FCT_Classify* p);
										//窗口 - 取出数据
		C_FCT_Classify* getData();
										//窗口 - 本地数据 -> ui数据
		virtual void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		virtual void putUiToData();

	public slots:
										//窗口 - 校验数据
		virtual bool checkData();
										//窗口 - 提交数据
		void acceptData();	

	private:
		Ui::W_FCT_Classify ui;
};

#endif // W_FCT_Classify_H
