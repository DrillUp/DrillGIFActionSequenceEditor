#ifndef W_FCTClassify_H
#define W_FCTClassify_H

#include "ui_W_FCTClassify.h"
#include "C_FCTClassify.h"

/*
-----==========================================================-----
		类：		类型 配置窗口.cpp
		作者：		drill_up
		所属模块：	工具模块
		功能：		类型的设置内容在该窗口设置。
-----==========================================================-----
*/

class W_FCTClassify : public QDialog
{
	Q_OBJECT

	public:
		W_FCTClassify(QWidget *parent = nullptr);
		~W_FCTClassify();


	//-----------------------------------
	//----窗口
	public:
		bool isAddMode;
		C_FCTClassify local_FCTClassify;
		QList<C_FCTClassify>* m_checkFrom;
	public:
										//窗口 - 设置数据（添加）
		void setDataInAddMode(QList<C_FCTClassify>* checkFrom);
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_FCTClassify p, QList<C_FCTClassify>* checkFrom);
										//窗口 - 取出数据
		C_FCTClassify getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_FCTClassify ui;
	//-----------------------------------
};

#endif // W_FCTClassify_H
