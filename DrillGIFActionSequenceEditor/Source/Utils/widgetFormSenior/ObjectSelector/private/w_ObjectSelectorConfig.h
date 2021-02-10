#ifndef W_ObjectSelectorConfig_H
#define W_ObjectSelectorConfig_H

#include "ui_W_ObjectSelectorConfig.h"
#include "c_ObjectSelectorConfig.h"

/*
-----==========================================================-----
		类：		对象选择 窗口.h
		版本：		v1.00
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑对象选择窗口的一些配置。
					（详细见cpp）
-----==========================================================-----
*/
class W_ObjectSelectorConfig : public QDialog
{
	Q_OBJECT

	public:
		W_ObjectSelectorConfig(QWidget *parent = 0 );
		~W_ObjectSelectorConfig();
		
	public:
										//回车过滤
		void keyPressEvent(QKeyEvent *event);
		
	public slots:
										//零填充勾选变化
		void zeroFillChanged(bool enable);							

	//-----------------------------------
	//----窗口
	protected:
		C_ObjectSelectorConfig local_data;
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_ObjectSelectorConfig data);
										//窗口 - 取出数据
		C_ObjectSelectorConfig getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ObjectSelectorConfig ui;
	//-----------------------------------
};

#endif // W_ObjectSelectorConfig_H
