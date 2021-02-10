#ifndef W_RaTConfig_H
#define W_RaTConfig_H

#include "ui_W_RaTConfig.h"
#include "../c_RaTConfig.h"

/*
-----==========================================================-----
		类：		单选表格配置 窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑 单选表格配置 窗口的一些配置。
					（详细见cpp）
-----==========================================================-----
*/
class W_RaTConfig : public QDialog
{
	Q_OBJECT

	public:
		W_RaTConfig(QWidget *parent = 0 );
		~W_RaTConfig();
		
	//-----------------------------------
	//----控件
	public:
										//回车过滤
		void keyPressEvent(QKeyEvent *event);
		
	public slots:
										//零填充勾选变化
		void zeroFillChanged(bool enable);	

	//-----------------------------------
	//----窗口
	protected:
		C_RaTConfig local_data;
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_RaTConfig data);
										//窗口 - 取出数据
		C_RaTConfig getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_RaTConfig ui;
	//-----------------------------------
};

#endif // W_RaTConfig_H
