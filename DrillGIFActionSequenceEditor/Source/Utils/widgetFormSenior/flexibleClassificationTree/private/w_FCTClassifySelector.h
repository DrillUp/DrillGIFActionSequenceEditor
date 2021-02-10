#ifndef W_FCTClassifySelector_H
#define W_FCTClassifySelector_H

#include "ui_W_FCTClassifySelector.h"
#include "C_FCTClassify.h"

/*
-----==========================================================-----
		类：		类型 选择窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		类型的设置内容在该窗口设置。
-----==========================================================-----
*/

class W_FCTClassifySelector : public QDialog
{
	Q_OBJECT

	public:
		W_FCTClassifySelector(QWidget *parent = nullptr);
		~W_FCTClassifySelector();
		
	//-----------------------------------
	//----控件
	public:
										//控件 - 打开窗口
		void openWindowAddClassify();
	signals:
										//添加类型（信号）
		void classifyAdded(C_FCTClassify classify);

	//-----------------------------------
	//----窗口
	protected:
		QList<C_FCTClassify>* m_classifyList;
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(QString selected_name, QList<C_FCTClassify>* dataFrom );
										//窗口 - 取出数据
		C_FCTClassify getSelectedData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_FCTClassifySelector ui;
	//-----------------------------------
};

#endif // W_FCTClassifySelector_H
