#ifndef W_FPT_Config_H
#define W_FPT_Config_H
#include "stdafx.h"

#include "ui_W_FPT_Config.h"
#include "C_FPT_Config.h"

/*
-----==========================================================-----
		类：		树设置 窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑 树设置 窗口的一些配置。
					（详细见cpp）
-----==========================================================-----
*/
class W_FPT_Config : public QDialog
{
	Q_OBJECT

	public:
		W_FPT_Config( QWidget *parent = 0 );
		~W_FPT_Config();
		
	//-----------------------------------
	//----控件
	public:
										//控件 - 回车过滤
		void keyPressEvent(QKeyEvent *event);
	public slots:
										//控件 - 零填充勾选变化
		void zeroFillChanged(bool enable);

	//-----------------------------------
	//----窗口
	protected:
		C_FPT_Config* local_data;
	public:
										//窗口 - 设置数据
		void setData(C_FPT_Config* data);
										//窗口 - 取出数据
		C_FPT_Config* getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	
	private:
		Ui::W_FPT_Config ui;
};

#endif // W_FPT_Config_H
