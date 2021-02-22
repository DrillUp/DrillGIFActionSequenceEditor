#ifndef W_ALEConfigEdit_H
#define W_ALEConfigEdit_H

#include <QDialog>
#include "../c_ALEConfig.h"
#include "ui_W_ALEConfigEdit.h"

/*
-----==========================================================-----
		类：		动画帧设置 窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑动画帧的整体ui设置。
					（详细见cpp）
-----==========================================================-----
*/
class W_ALEConfigEdit : public QDialog
{
	Q_OBJECT

	public:
		W_ALEConfigEdit(QWidget *parent = 0 );
		~W_ALEConfigEdit();
		
	//-----------------------------------
	//----控件		
	public:
		bool m_slotBlock;
	public:
										//回车过滤
		void keyPressEvent(QKeyEvent *event);
	public slots:
										//控件 - 勾选切换
		void checkBoxChanged(bool checked);
										//控件 - 选择数字
		void timeSelected(QString text);
										//控件 - 编辑数字
		void timeEdited(double value);

	//-----------------------------------
	//----窗口
	protected:
		C_ALEConfig local_config;		//
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_ALEConfig config);
										//窗口 - 取出数据
		C_ALEConfig getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ALEConfigEdit ui;
	//-----------------------------------
};

#endif // W_ALEConfigEdit_H
