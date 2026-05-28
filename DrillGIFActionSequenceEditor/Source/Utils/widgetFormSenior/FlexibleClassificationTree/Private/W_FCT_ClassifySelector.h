#ifndef W_FCT_ClassifySelector_H
#define W_FCT_ClassifySelector_H
#include "stdafx.h"

#include "ui_W_FCT_ClassifySelector.h"
#include "C_FCT_Config.h"

/*
-----==========================================================-----
		类：		种类 选择窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		种类的设置内容在该窗口设置。
-----==========================================================-----
*/
class P_FlexibleClassificationTree;
class W_FCT_ClassifySelector : public QDialog
{
	Q_OBJECT

	public:
		W_FCT_ClassifySelector(QWidget *parent = 0);
		~W_FCT_ClassifySelector();
		
	//-----------------------------------
	//----控件
	protected:
		P_FlexibleClassificationTree* m_parentObj = nullptr;
		C_FCT_Config* m_configPtr = nullptr;
	public:
										//控件 - 控件初始化
										//		【说明】：窗口不能直接嵌套 分类树 本体，所以只能把需要的参数都通过这个函数进行传递。
		void initWidget(C_FCT_Config* config, P_FlexibleClassificationTree* parentObj);
	public slots:
										//控件 - 添加类型
		void addClassifyInAction();
										//控件 - 刷新列表
		void refreshTable();
										//控件 - 刷新描述
		void refreshDescription();

	//-----------------------------------
	//----窗口
	public:
										//窗口 - 设置数据
		void setSelectedName(QString selectedName = "");
										//窗口 - 取出数据
		QString getSelectedName();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_FCT_ClassifySelector ui;
	//-----------------------------------
};

#endif // W_FCT_ClassifySelector_H
