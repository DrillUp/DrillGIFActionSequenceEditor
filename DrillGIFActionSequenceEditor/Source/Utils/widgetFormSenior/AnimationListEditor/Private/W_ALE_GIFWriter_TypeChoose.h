#ifndef W_ALE_GIFWriter_TypeChoose_H
#define W_ALE_GIFWriter_TypeChoose_H
#include "stdafx.h"

#include <QDialog>
#include "ui_W_ALE_GIFWriter_TypeChoose.h"

/*
-----==========================================================-----
		类：		GIF写入器 类型选择窗口.h
		作者：		drill_up
		所属模块：	工具模块
		功能：		GIF导出 时，选择导出方式的窗口。
					（详细见cpp）
-----==========================================================-----
*/

class W_ALE_GIFWriter_TypeChoose : public QDialog
{
	Q_OBJECT

	public:
		W_ALE_GIFWriter_TypeChoose(QWidget *parent = 0 );
		~W_ALE_GIFWriter_TypeChoose();

	//-----------------------------------
	//----窗口
	protected:
		int m_typeIndex;
	public:
										//窗口 - 设置数据（修改）
		void setDataInModifyMode(int typeIndex);
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
		Ui::W_ALE_GIFWriter_TypeChoose ui;
	//-----------------------------------
};

#endif // W_ALE_GIFWriter_TypeChoose_H
