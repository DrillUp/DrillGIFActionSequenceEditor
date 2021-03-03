#ifndef W_ActionSeqLength_H
#define W_ActionSeqLength_H

#include "ui_W_ActionSeqLength.h"
#include "c_ActionSeqLength.h"

/*
-----==========================================================-----
		类：		最大值 窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		编辑 最大值 窗口的一些配置。
					（详细见cpp）
-----==========================================================-----
*/
class W_ActionSeqLength : public QDialog
{
	Q_OBJECT

	public:
		W_ActionSeqLength(QWidget *parent = 0 );
		~W_ActionSeqLength();
		
	//-----------------------------------
	//----控件
	public:
										//回车过滤
		void keyPressEvent(QKeyEvent *event);
		

	//-----------------------------------
	//----窗口
	protected:
		C_ActionSeqLength local_data;
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(C_ActionSeqLength data);
										//窗口 - 取出数据
		C_ActionSeqLength getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ActionSeqLength ui;
	//-----------------------------------
};

#endif // W_ActionSeqLength_H
