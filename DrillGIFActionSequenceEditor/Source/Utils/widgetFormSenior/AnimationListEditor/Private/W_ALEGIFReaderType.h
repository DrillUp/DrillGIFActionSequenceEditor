#ifndef W_ALEGIFReaderType_H
#define W_ALEGIFReaderType_H
#include "stdafx.h"

#include <QDialog>
#include "ui_W_ALEGIFReaderType.h"

/*
-----==========================================================-----
		类：		选择GIF读取器 窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		单独选择项的窗口。
					（详细见cpp）
-----==========================================================-----
*/
class W_ALEGIFReaderType : public QDialog
{
	Q_OBJECT

	public:
		W_ALEGIFReaderType(QWidget *parent = 0 );
		~W_ALEGIFReaderType();

	//-----------------------------------
	//----窗口
	protected:
		int local_index;
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(int index);
										//窗口 - 取出数据
		int getData();
										//窗口 - 本地数据 -> ui数据
		void putDataToUi();							
										//窗口 - ui数据 -> 本地数据
		void putUiToData();

	public slots:
										//窗口 - 提交数据（校验）
		void acceptData();	

	private:
		Ui::W_ALEGIFReaderType ui;
	//-----------------------------------
};

#endif // W_ALEGIFReaderType_H
