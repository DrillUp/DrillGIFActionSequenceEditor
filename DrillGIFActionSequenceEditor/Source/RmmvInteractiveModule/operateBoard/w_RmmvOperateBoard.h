#ifndef W_RmmvOperateBoard_H
#define W_RmmvOperateBoard_H

#include <QtWidgets/QDialog>
#include "ui_W_RmmvOperateBoard.h"
#include "p_RmmvOperateBoard.h"

/*
-----==========================================================-----
		类：		操作面板（窗口）.h
		版本：		v1.01
		所属模块：	交互模块
		功能：		操作面板的窗口。
					（详细见cpp）
-----==========================================================-----
*/
class W_RmmvOperateBoard : public QDialog
{
	Q_OBJECT

	public:
		W_RmmvOperateBoard(QWidget *parent = 0);		//构造函数
		~W_RmmvOperateBoard();							//析构函数

	//-----------------------------------
	//----控件
	public:
		P_RmmvOperateBoard* m_P_RmmvOperateBoard;

	//-----------------------------------
	//----窗口
	public:
										//窗口 - 设置数据
		void setData(C_RmmvProjectData rmmvData);
										//窗口 - 取出数据
		C_RmmvProjectData getData();
										//窗口 - 本地数据 -> ui数据
										//窗口 - ui数据 -> 本地数据
										//窗口 - 提交数据（校验）
	private:
		Ui::W_RmmvOperateBoard ui;
		
};

#endif // W_RmmvOperateBoard_H
