#ifndef W_ALE_Config_H
#define W_ALE_Config_H
#include "stdafx.h"

#include <QDialog>
#include "../C_ALE_Config.h"
#include "ui_W_ALE_Config.h"

/*
-----==========================================================-----
		类：		动画帧编辑块 配置类 窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		动画帧编辑块 配置类 的编辑窗口。
					（详细见cpp）
-----==========================================================-----
*/

class W_ALE_Config : public QDialog
{
	Q_OBJECT

	public:
		W_ALE_Config(QWidget *parent = 0 );
		~W_ALE_Config();
		
	//-----------------------------------
	//----控件
	public slots:
										//控件 - 回车过滤
		void keyPressEvent(QKeyEvent *event);
										//控件 - 勾选切换
		void checkBoxChanged_MaskEnabled(bool checked);
		void checkBoxChanged_MaskBorderEnabled(bool checked);

	//-----------------------------------
	//----窗口
	protected:
		bool m_slotBlock;
		C_ALE_Config local_config;		//
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_ALE_Config config);
										//窗口 - 取出数据
		C_ALE_Config getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();
	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ALE_Config ui;
	//-----------------------------------
};

#endif // W_ALE_Config_H
